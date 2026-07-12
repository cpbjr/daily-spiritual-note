import json
import os
import re
import sys
from typing import Dict, Optional, Tuple

import requests
from jinja2 import Template

from config import settings

sys.path.insert(0, os.path.expanduser("~/cost-logs"))
try:
    from cost_logger import log_api_call
except ImportError:
    # cost_logger lives on the production server (~/cost-logs). When it is not
    # available (local dev, dry-run, fresh server) fall back to a no-op so the
    # whole application doesn't fail to import.
    def log_api_call(*args, **kwargs):
        pass


# Hermes OAuth helpers live outside this app's venv; cron wrapper adds PYTHONPATH.
_HERMES_AGENT_ROOT = os.path.expanduser("~/.hermes/hermes-agent")
if _HERMES_AGENT_ROOT not in sys.path and os.path.isdir(_HERMES_AGENT_ROOT):
    sys.path.insert(0, _HERMES_AGENT_ROOT)


class AIService:
    def __init__(self):
        # Load prompts
        prompt_dir = os.path.join(os.path.dirname(__file__), "..", "prompts")
        with open(os.path.join(prompt_dir, "system_prompt.txt"), "r") as f:
            self.system_prompt = f.read()

        with open(os.path.join(prompt_dir, "user_prompt.txt"), "r") as f:
            self.user_prompt_template = f.read()

    def generate_reflection(self, context: Dict) -> Dict:
        """
        Generates a reflection using the primary provider, falling back to the
        fallback provider if the primary fails or has no credentials configured.
        """
        template = Template(self.user_prompt_template)
        user_prompt = template.render(**context)

        # Build ordered provider list
        provider_order = [settings.AI_PROVIDER]
        if settings.AI_FALLBACK_PROVIDER and settings.AI_FALLBACK_PROVIDER not in provider_order:
            provider_order.append(settings.AI_FALLBACK_PROVIDER)

        last_error = None
        for provider in provider_order:
            try:
                result = self._try_provider(provider, user_prompt)
                print(f"✅ AI response served by: {provider}")
                return result
            except Exception as e:
                print(f"⚠️  Provider '{provider}' failed: {e}")
                last_error = e

        raise Exception(f"All AI providers failed. Last error: {last_error}")

    def _try_provider(self, provider: str, user_prompt: str) -> Dict:
        if provider in ("xai-oauth", "xai_oauth", "grok-oauth"):
            return self._generate_xai_oauth(user_prompt)
        if provider == "nous":
            if not settings.NOUS_API_KEY:
                raise Exception("NOUS_RESEARCH_API_KEY not set")
            return self._generate_openai_compatible(
                url=settings.NOUS_URL,
                key=settings.NOUS_API_KEY,
                model=settings.NOUS_MODEL,
                user_prompt=user_prompt,
                provider_name="nous",
            )
        if provider == "xai":
            if not settings.XAI_API_KEY:
                raise Exception("XAI_API_KEY not set")
            return self._generate_openai_compatible(
                url=settings.XAI_URL,
                key=settings.XAI_API_KEY,
                model=settings.XAI_MODEL,
                user_prompt=user_prompt,
                provider_name="xai",
            )
        if provider == "google":
            if not settings.GEMINI_API_KEY:
                raise Exception("GEMINI_API_KEY not set")
            return self._generate_gemini(user_prompt)
        raise Exception(f"Unsupported AI provider: {provider}")

    def _resolve_xai_oauth(self, force_refresh: bool = False) -> Tuple[str, str]:
        """
        Return (api_key, base_url) from Hermes xAI OAuth (SuperGrok/Premium+).

        Uses the Hermes agent venv via subprocess so this app's venv does not
        need the full Hermes dependency tree (httpx, etc.).
        """
        last_err: Optional[Exception] = None

        # 1) In-process import (works if hermes venv/deps are available)
        try:
            from hermes_cli.auth import resolve_xai_oauth_runtime_credentials

            creds = resolve_xai_oauth_runtime_credentials(force_refresh=force_refresh)
            api_key = str(creds.get("api_key") or "").strip()
            base_url = str(creds.get("base_url") or "https://api.x.ai/v1").strip().rstrip("/")
            if api_key:
                return api_key, base_url or "https://api.x.ai/v1"
        except Exception as e:
            last_err = e

        # 2) Subprocess with Hermes' own Python (reliable for cron)
        hermes_py = os.path.expanduser("~/.hermes/hermes-agent/venv/bin/python3")
        if os.path.isfile(hermes_py):
            import subprocess

            helper = r"""
import json, os, sys
sys.path.insert(0, os.path.expanduser("~/.hermes/hermes-agent"))
os.environ.setdefault("HERMES_HOME", os.path.expanduser("~/.hermes"))
from hermes_cli.auth import resolve_xai_oauth_runtime_credentials
force = sys.argv[1] == "1"
creds = resolve_xai_oauth_runtime_credentials(force_refresh=force)
print(json.dumps({
    "api_key": creds.get("api_key") or "",
    "base_url": creds.get("base_url") or "https://api.x.ai/v1",
}))
"""
            try:
                proc = subprocess.run(
                    [hermes_py, "-c", helper, "1" if force_refresh else "0"],
                    capture_output=True,
                    text=True,
                    timeout=45,
                    env={**os.environ, "HERMES_HOME": os.path.expanduser("~/.hermes")},
                )
                if proc.returncode != 0:
                    raise Exception((proc.stderr or proc.stdout or "oauth helper failed").strip()[:300])
                # last non-empty line is JSON (in case of library noise)
                out_lines = [ln for ln in (proc.stdout or "").splitlines() if ln.strip()]
                payload = json.loads(out_lines[-1])
                api_key = str(payload.get("api_key") or "").strip()
                base_url = str(payload.get("base_url") or "https://api.x.ai/v1").strip().rstrip("/")
                if api_key:
                    return api_key, base_url or "https://api.x.ai/v1"
                raise Exception("oauth helper returned empty api_key")
            except Exception as e:
                last_err = e

        raise Exception(
            "xAI OAuth credentials unavailable. Run `hermes auth add xai-oauth` "
            f"or `hermes model` and select xAI Grok OAuth. Last error: {last_err}"
        )

    def _generate_xai_oauth(self, user_prompt: str) -> Dict:
        model = settings.XAI_OAUTH_MODEL or settings.XAI_MODEL or "grok-4.3"
        api_key, base_url = self._resolve_xai_oauth(force_refresh=False)
        url = f"{base_url.rstrip('/')}/chat/completions"

        try:
            return self._generate_openai_compatible(
                url=url,
                key=api_key,
                model=model,
                user_prompt=user_prompt,
                provider_name="xai-oauth",
            )
        except requests.HTTPError as e:
            status = e.response.status_code if e.response is not None else None
            if status == 401:
                print("🔄 xAI OAuth 401 — refreshing token and retrying once...")
                api_key, base_url = self._resolve_xai_oauth(force_refresh=True)
                url = f"{base_url.rstrip('/')}/chat/completions"
                return self._generate_openai_compatible(
                    url=url,
                    key=api_key,
                    model=model,
                    user_prompt=user_prompt,
                    provider_name="xai-oauth",
                )
            raise

    def _generate_openai_compatible(
        self, url: str, key: str, model: str, user_prompt: str, provider_name: str
    ) -> Dict:
        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {key}",
        }
        payload = {
            "model": model,
            "messages": [
                {"role": "system", "content": self.system_prompt},
                {"role": "user", "content": user_prompt},
            ],
            "temperature": 0.7,
            "response_format": {"type": "json_object"},
        }

        response = requests.post(url, headers=headers, json=payload, timeout=120)
        try:
            response.raise_for_status()
        except requests.HTTPError:
            # Attach response body for debugging without raising a bare status
            raise

        data = response.json()
        if "usage" in data:
            log_api_call(
                "daily-summary",
                provider_name,
                model,
                input_tokens=data["usage"].get("prompt_tokens", 0),
                output_tokens=data["usage"].get("completion_tokens", 0),
            )

        content = data["choices"][0]["message"]["content"]
        return self._parse_json(content, provider_name)

    def _generate_gemini(self, user_prompt: str) -> Dict:
        import google.generativeai as genai

        genai.configure(api_key=settings.GEMINI_API_KEY)
        model = genai.GenerativeModel(settings.GEMINI_MODEL)
        response = model.generate_content(
            f"{self.system_prompt}\n\n{user_prompt}",
            generation_config=genai.types.GenerationConfig(
                temperature=0.7,
                response_mime_type="application/json",
            ),
        )
        if hasattr(response, "usage_metadata") and response.usage_metadata:
            log_api_call(
                "daily-summary",
                "google",
                settings.GEMINI_MODEL,
                input_tokens=getattr(response.usage_metadata, "prompt_token_count", 0),
                output_tokens=getattr(response.usage_metadata, "candidates_token_count", 0),
            )
        return self._parse_json(response.text, "google")

    def _parse_json(self, content: str, provider_name: str) -> Dict:
        """
        Parse JSON from model output, stripping markdown code fences if present.
        """
        # Strip ```json ... ``` or ``` ... ``` wrappers
        stripped = re.sub(r"^```(?:json)?\s*", "", content.strip(), flags=re.IGNORECASE)
        stripped = re.sub(r"\s*```$", "", stripped.strip())
        try:
            return json.loads(stripped)
        except json.JSONDecodeError as e:
            raise Exception(
                f"Failed to parse {provider_name} response as JSON: {e}\nContent: {content[:200]}"
            )

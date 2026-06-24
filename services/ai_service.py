import json
import os
import re
import sys
import requests
from typing import Dict
from jinja2 import Template
from config import settings

sys.path.insert(0, os.path.expanduser("~/cost-logs"))
from cost_logger import log_api_call

class AIService:
    def __init__(self):
        # Load prompts
        prompt_dir = os.path.join(os.path.dirname(__file__), '..', 'prompts')
        with open(os.path.join(prompt_dir, 'system_prompt.txt'), 'r') as f:
            self.system_prompt = f.read()

        with open(os.path.join(prompt_dir, 'user_prompt.txt'), 'r') as f:
            self.user_prompt_template = f.read()

    def generate_reflection(self, context: Dict) -> Dict:
        """
        Generates a reflection using the primary provider, falling back to the
        fallback provider if the primary fails or has no API key configured.
        """
        template = Template(self.user_prompt_template)
        user_prompt = template.render(**context)

        # Build ordered provider list, skipping providers with no key
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
        elif provider == "xai":
            if not settings.XAI_API_KEY:
                raise Exception("XAI_API_KEY not set")
            return self._generate_openai_compatible(
                url=settings.XAI_URL,
                key=settings.XAI_API_KEY,
                model=settings.XAI_MODEL,
                user_prompt=user_prompt,
                provider_name="xai",
            )
        elif provider == "google":
            if not settings.GEMINI_API_KEY:
                raise Exception("GEMINI_API_KEY not set")
            return self._generate_gemini(user_prompt)
        else:
            raise Exception(f"Unsupported AI provider: {provider}")

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
        response.raise_for_status()

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
            raise Exception(f"Failed to parse {provider_name} response as JSON: {e}\nContent: {content[:200]}")

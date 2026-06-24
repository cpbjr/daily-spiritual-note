import google.generativeai as genai
import json
import os
import sys
from typing import Dict
from jinja2 import Template
from config import settings

sys.path.insert(0, os.path.expanduser("~/cost-logs"))
from cost_logger import log_api_call

class AIService:
    def __init__(self):
        self.provider = settings.AI_PROVIDER
        
        # Load prompts
        prompt_dir = os.path.join(os.path.dirname(__file__), '..', 'prompts')
        with open(os.path.join(prompt_dir, 'system_prompt.txt'), 'r') as f:
            self.system_prompt = f.read()
        
        with open(os.path.join(prompt_dir, 'user_prompt.txt'), 'r') as f:
            self.user_prompt_template = f.read()

        if self.provider == "google":
            genai.configure(api_key=settings.GEMINI_API_KEY)
            self.model = genai.GenerativeModel(settings.AI_MODEL)
        elif self.provider == "xai":
            # xAI uses OpenAI compatible API
            import requests # already imported in other services, but keeping it clean here
            self.xai_url = "https://api.x.ai/v1/chat/completions"

    def generate_reflection(self, context: Dict) -> Dict:
        """
        Generates a reflection using the specified AI provider.
        """
        template = Template(self.user_prompt_template)
        user_prompt = template.render(**context)
        
        if self.provider == "google":
            return self._generate_gemini(user_prompt)
        elif self.provider == "xai":
            return self._generate_xai(user_prompt)
        else:
            raise Exception(f"Unsupported AI provider: {self.provider}")

    def _generate_gemini(self, user_prompt: str) -> Dict:
        response = self.model.generate_content(
            f"{self.system_prompt}\n\n{user_prompt}",
            generation_config=genai.types.GenerationConfig(
                temperature=0.7,
                response_mime_type="application/json"
            )
        )
        if hasattr(response, "usage_metadata") and response.usage_metadata:
            log_api_call("daily-summary", "google", settings.AI_MODEL,
                         input_tokens=getattr(response.usage_metadata, "prompt_token_count", 0),
                         output_tokens=getattr(response.usage_metadata, "candidates_token_count", 0))
        try:
            return json.loads(response.text)
        except json.JSONDecodeError:
            raise Exception("Failed to parse Gemini response as JSON")

    def _generate_xai(self, user_prompt: str) -> Dict:
        import requests
        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {settings.XAI_API_KEY}"
        }
        payload = {
            "model": settings.AI_MODEL,
            "messages": [
                {"role": "system", "content": self.system_prompt},
                {"role": "user", "content": user_prompt}
            ],
            "temperature": 0.7,
            "response_format": {"type": "json_object"}
        }
        
        response = requests.post(self.xai_url, headers=headers, json=payload)
        response.raise_for_status()

        try:
            data = response.json()
            if "usage" in data:
                log_api_call("daily-summary", "xai", settings.AI_MODEL,
                             input_tokens=data["usage"].get("prompt_tokens", 0),
                             output_tokens=data["usage"].get("completion_tokens", 0))
            content = data["choices"][0]["message"]["content"]
            return json.loads(content)
        except (KeyError, json.JSONDecodeError) as e:
            raise Exception(f"Failed to parse xAI response as JSON: {e}")

from pydantic import Field
from pydantic_settings import BaseSettings, SettingsConfigDict

class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", extra="ignore", populate_by_name=True)

    # Environment
    ENV: str = "production"

    # AI Provider Settings
    AI_PROVIDER: str = "nous"           # primary provider
    AI_FALLBACK_PROVIDER: str = "xai"  # "" to disable fallback

    # Nous Research
    NOUS_API_KEY: str = Field(default="", alias="NOUS_RESEARCH_API_KEY")
    NOUS_MODEL: str = "Hermes-4-405B"
    NOUS_URL: str = "https://inference-api.nousresearch.com/v1/chat/completions"

    # xAI
    XAI_API_KEY: str = ""
    XAI_MODEL: str = "grok-4.20-0309-reasoning"
    XAI_URL: str = "https://api.x.ai/v1/chat/completions"

    # Google Gemini
    GEMINI_API_KEY: str = ""
    GEMINI_MODEL: str = "gemini-2.0-flash"

    # SMTP Configurations
    SMTP_SERVER: str = "smtp.mail.me.com"
    SMTP_PORT: int = 587
    SMTP_USER: str = "cpbjr@mac.com"
    SMTP_PASSWORD: str = ""

    # Email Settings
    FROM_EMAIL: str = "cpbjr@mac.com"
    TO_EMAIL: str = "cpbjr@mac.com"

    # Readings API
    READINGS_API_BASE_URL: str = "https://cpbjr.github.io/catholic-readings-api"

    # YouVersion Configuration
    BIBLE_VERSION_ID: str = "2015"     # NRSVCI
    BIBLE_VERSION_ABBR: str = "NRSVCI"

settings = Settings()

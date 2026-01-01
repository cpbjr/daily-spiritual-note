import os
from dotenv import load_dotenv
from pydantic_settings import BaseSettings

load_dotenv()

class Settings(BaseSettings):
    # API Configurations
    GEMINI_API_KEY: str = os.getenv("GEMINI_API_KEY", "")
    ENV: str = "production"
    
    # AI Provider Settings
    AI_PROVIDER: str = "google" # "google" or "xai"
    AI_MODEL: str = "gemini-2.0-flash"
    
    # xAI Configurations
    XAI_API_KEY: str = os.getenv("XAI_API_KEY", "")
    
    # SMTP Configurations
    SMTP_SERVER: str = "smtp.mail.me.com"
    SMTP_PORT: int = 587
    SMTP_USER: str = os.getenv("SMTP_USER", "cpbjr@mac.com")
    SMTP_PASSWORD: str = os.getenv("SMTP_PASSWORD", "")
    
    # Email Settings
    FROM_EMAIL: str = "cpbjr@mac.com"
    TO_EMAIL: str = "cpbjr@mac.com"
    
    # Readings API
    READINGS_API_BASE_URL: str = "https://cpbjr.github.io/catholic-readings-api"
    
    # YouVersion Configuration
    BIBLE_VERSION_ID: str = "3548"  # RSVCI
    
    class Config:
        env_file = ".env"

settings = Settings()

# Daily Spiritual Note

Daily Catholic readings delivered by email — AI-generated reflections, saint of the day, and historical events.

**Repo:** `https://github.com/cpbjr/daily-spiritual-note`
**Status:** IN PRODUCTION — running daily at 4:00 AM MDT (10:00 UTC)

---

## What It Does

Each morning the script pulls the day's Catholic readings from the GitHub Pages readings API, generates a spiritual reflection (xAI Grok via Hermes OAuth primary, Nous fallback), and emails the result to cpbjr@mac.com. Content includes:

- Daily Mass readings (Gospel, Epistle, Psalm)
- Saint of the day
- AI-generated reflection
- Spiritual challenge and prayer

## Tech Stack

| Layer | Detail |
|-------|--------|
| Language | Python 3.12 |
| AI | xAI `grok-4.3` via Hermes `xai-oauth` (primary), Nous `Hermes-4-405B` (fallback), Google Gemini (optional) |
| Email | Apple iCloud SMTP (`cpbjr@mac.com`) |
| Readings API | `https://cpbjr.github.io/catholic-readings-api/` |
| Server | Hetzner beefy (`5.78.152.85`), user `buduser` |
| Deployment | GitHub Actions → `git pull` on beefy |
| Schedule | Hermes cron at 4:00 AM Mountain (`daily-spiritual-note.sh`) |

## Deployment

Push to `main` → GitHub Action SSHes into beefy → `git pull` → live in ~30s.

See [`.agent/SOPs/deployment-procedure.md`](.agent/SOPs/deployment-procedure.md) for full details.

## Local Development

```bash
cp .env.example .env       # fill in API keys
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 main.py
```

## Project Structure

```
main.py                  # entry point
config.py                # settings loaded from .env
run.sh                   # cron wrapper (loads .env, runs main.py)
services/
  ai_service.py          # Nous / xAI / Gemini generation
  readings_service.py    # USCCB API client
  email_service.py       # SMTP delivery
  theme_service.py       # liturgical theme helpers
prompts/                 # system and user prompt templates
templates/               # email HTML template
.agent/                  # project documentation and SOPs
```

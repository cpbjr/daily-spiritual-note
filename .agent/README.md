# Daily Spiritual Note — Project Documentation

**Status:** ✅ IN PRODUCTION (Python)
**Last Updated:** 2026-06-24
**Repo:** https://github.com/cpbjr/daily-spiritual-note

## Overview

Daily Catholic readings delivery with AI-generated spiritual content. Python cron job on Hetzner (whitepine), deployed automatically via GitHub Actions on push to main.

## Quick Reference

| Item | Value |
|------|-------|
| Production path | `/home/deploy/daily-summary` |
| Schedule | 4:00 AM MDT (10:00 UTC) daily |
| AI model | Nous `Hermes-4-405B` (primary), xAI `grok-4.20-0309-reasoning` (fallback) |
| Email | cpbjr@mac.com → cpbjr@mac.com |
| Readings API | https://cpbjr.github.io/catholic-readings-api/ |

## Tech Stack

- **Python 3.12** — core application
- **Nous Research Hermes** — AI generation (xAI Grok as fallback)
- **SMTP** — Apple iCloud Mail
- **Deployment** — GitHub Actions → SSH → git pull on whitepine

## Documentation

### SOPs/
- **[deployment-procedure.md](SOPs/deployment-procedure.md)** — GitHub Actions deploy flow, manual fallback, cron config
- **[switch-node-configuration.md](SOPs/switch-node-configuration.md)** — legacy n8n reference (archived)

### Tasks/
- **[active.md](Tasks/active.md)** — current work
- **[planned.md](Tasks/planned.md)** — upcoming work
- **[completed/](Tasks/completed/)** — historical record

## History

| Date | Event |
|------|-------|
| Oct 2025 | Built as n8n workflow on AWS |
| Jan 2026 | Migrated to Python, deployed to Hetzner |
| Jun 2026 | Extracted from AI_Automation monorepo into own repo |

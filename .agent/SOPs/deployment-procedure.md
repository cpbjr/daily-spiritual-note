# Deployment Procedure: Daily Spiritual Note

**Last Updated:** 2026-07-12  
**Purpose:** How changes get from development to production

---

## Overview

This is a Python job scheduled by **Hermes cron** on the **beefy** Hetzner server. Deployment is automated via GitHub Actions: pushing to `main` triggers an SSH-based `git pull` on beefy within ~30 seconds.

| Item | Value |
|------|--------|
| **Production path** | `/home/buduser/apps/daily-spiritual-note` |
| **Server** | beefy (Hetzner, `5.78.152.85`), user `buduser` |
| **GitHub repo** | `https://github.com/cpbjr/daily-spiritual-note` |
| **Schedule** | Hermes cron `daily-spiritual-note` — `0 4 * * *` America/Denver (4:00 AM MT) |
| **Wrapper** | `~/.hermes/scripts/daily-spiritual-note.sh` → `main.py` |

**AI path (production defaults):** primary `xai-oauth` / `grok-4.3` (Hermes SuperGrok OAuth on beefy), fallback `nous` / `Hermes-4-405B`.

---

## Normal Deployment Flow

```
Edit (laptop or beefy) → commit → git push origin main → GitHub Action deploys → beefy updated
```

1. Make changes and push to `main` (or merge a PR).
2. GitHub Action (`.github/workflows/deploy.yml`) SSHes into beefy as `buduser` and runs `git pull origin main`.
3. Changes are live within ~30 seconds.
4. Next Hermes cron run picks up the new code (or re-run the job to verify).

**The `.env` file on the server is NOT in git** — it stays untouched by deployments.  
`venv/`, `logs/`, and local secrets are ignored by git.

---

## GitHub Actions Setup

Workflow: `.github/workflows/deploy.yml`.

### Secrets (repo Settings → Secrets and variables → Actions)

| Secret | Purpose |
|--------|---------|
| `DEPLOY_SSH_KEY` | Private key authorized for `buduser@5.78.152.85` (beefy deploy key) |

Suggested key path on beefy (private key never committed):

```bash
~/.ssh/github_actions_deploy_beefy
# public key must be in buduser's ~/.ssh/authorized_keys
```

### If the Action fails

1. Check the Actions tab on GitHub for error output.
2. Common causes: SSH key rotated/removed, server unreachable, git conflict on beefy.
3. Manual fallback:

```bash
ssh buduser@5.78.152.85 'cd /home/buduser/apps/daily-spiritual-note && git pull origin main'
```

---

## Server Git Setup

```bash
cd /home/buduser/apps/daily-spiritual-note
git remote -v
git log --oneline -3
git status
```

Keep the working tree clean of uncommitted production-only edits (or commit them).  
`.env` is safe because it is gitignored.

---

## Cron Configuration

**Scheduler:** Hermes (`daily-spiritual-note`, job id may vary).  
**Not** system crontab on whitepine.

```bash
# Cron wrapper
~/.hermes/scripts/daily-spiritual-note.sh

# App logs
/home/buduser/apps/daily-spiritual-note/logs/cron-YYYYMMDD.log
```

To check recent execution:

```bash
tail -50 /home/buduser/apps/daily-spiritual-note/logs/cron-$(date +%Y%m%d).log
```

---

## Whitepine (legacy)

Earlier production lived on whitepine (`5.78.128.255`, user `deploy`, path `/home/deploy/daily-summary`).  
After cutover, whitepine crontab for this app should stay **disabled** to avoid duplicate emails. The deploy workflow no longer targets whitepine.

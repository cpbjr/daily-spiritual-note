# Deployment Procedure: Daily Spiritual Note

**Last Updated:** 2026-06-24
**Purpose:** How changes get from local development to the production server

---

## Overview

This is a Python cron job running on the **whitepine** Hetzner server. Deployment is
fully automated via GitHub Actions — pushing to `main` triggers an SSH-based `git pull`
on the server within ~30 seconds.

**Production path:** `/home/deploy/daily-summary`
**Server:** whitepine (Hetzner, 5.78.128.255), `deploy` user
**GitHub repo:** `https://github.com/cpbjr/daily-spiritual-note`
**Schedule:** `0 10 * * *` (10:00 UTC = 4:00 AM MDT) via crontab

---

## Normal Deployment Flow

```
Edit locally → commit → git push origin main → GitHub Action deploys → server updated
```

1. Make changes locally in `/home/cpbjr/WhitePineTech/Projects/Daily-Summary`
2. Commit and push to `main`
3. GitHub Action (`deploy.yml`) SSHes into whitepine and runs `git pull origin main`
4. Changes are live within ~30 seconds
5. Next cron execution picks up the new code

**The `.env` file on the server is NOT in git** — it stays untouched by deployments.
The `venv/` and `cron.log` are also ignored by git.

---

## GitHub Actions Setup

The workflow is at `.github/workflows/deploy.yml`.

### Secrets required (set in repo Settings → Secrets):
- `DEPLOY_SSH_KEY` — private key for `deploy@whitepine` (`~/.ssh/github_actions_deploy`)
- `DEPLOY_KNOWN_HOSTS` — whitepine's host keys (from `ssh-keyscan -H 5.78.128.255`)

The deploy key (`~/.ssh/github_actions_deploy`) is already in `authorized_keys` on the server.

### If the Action fails:
1. Check Actions tab on GitHub for error output
2. Common causes: SSH key expired/removed, server unreachable, git conflict on server
3. Manual fallback: `ssh whitepine "cd /home/deploy/daily-summary && git pull origin main"`

---

## Server Git Setup

The server's `/home/deploy/daily-summary` is a git repo tracking `origin/main`:
```bash
# Verify:
ssh whitepine "cd /home/deploy/daily-summary && git remote -v && git log --oneline -3"
```

**Important:** The server may have local file differences (e.g., older `.gitignore`, `run.sh`).
These will be overwritten on `git pull` if they differ from `main`. The `.env` file is safe
because it's in `.gitignore`.

---

## Cron Configuration

On the server, view/edit via `crontab -e` as `deploy` user:
```
0 10 * * * /home/deploy/daily-summary/run.sh
```

`run.sh` loads `.env`, then runs `./venv/bin/python3 main.py`, appending output to `cron.log`.

To check recent execution output:
```bash
ssh whitepine "tail -50 /home/deploy/daily-summary/cron.log"
```

---

## Manual Trigger (Testing)

To run the email immediately without waiting for cron:
```bash
ssh whitepine "cd /home/deploy/daily-summary && ./run.sh"
```

---

## Environment Variables

The `.env` file lives only on the server at `/home/deploy/daily-summary/.env`.
See `.env.example` in the repo for required variables. To update a secret:
```bash
ssh whitepine "nano /home/deploy/daily-summary/.env"
```

---

## History Note

This project was previously an n8n workflow (October 2025), then migrated to Python
(January 2026). The old n8n deployment SOP (via REST API to AWS) is no longer relevant.
The project was also extracted from the `AI_Automation` monorepo into its own repo
(`cpbjr/daily-spiritual-note`) in June 2026.

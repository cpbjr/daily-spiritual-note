## Active Tasks

**Last Updated:** 2026-06-29

- [/] Monitor cron execution and log reliability on production server.
  - Deployment now automated via GitHub Actions (push to main → git pull on whitepine)
  - Check cron.log periodically: `ssh whitepine "tail -50 /home/deploy/daily-summary/cron.log"`
  - **2026-06-29:** Cron missed today — `run.sh` had wrong `cd` path (`/home/cpbjr/...` instead of `/home/deploy/daily-summary`). Fixed and deployed. PR #2 merged. Email fired manually.
- [ ] Add support for attachments or liturgical images (optional).

---

> [!TIP]
> Completed tasks have been moved to `.agent/Tasks/completed/2026-01/` for historical record.

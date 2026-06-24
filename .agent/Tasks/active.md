## Active Tasks

**Last Updated:** 2026-06-24

- [/] Monitor cron execution and log reliability on production server.
  - Deployment now automated via GitHub Actions (push to main → git pull on whitepine)
  - Check cron.log periodically: `ssh whitepine "tail -50 /home/deploy/daily-summary/cron.log"`
- [ ] Add support for attachments or liturgical images (optional).

---

> [!TIP]
> Completed tasks have been moved to `.agent/Tasks/completed/2026-01/` for historical record.

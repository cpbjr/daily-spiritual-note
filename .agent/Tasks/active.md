## Active Tasks

**Last Updated:** 2026-08-15

- [x] 2026-08-15: 4 AM cron crashed — template required `readings.firstReading`; Assumption API JSON omitted it. Guarded firstReading like psalm/secondReading. Manual resend delivered.
- [ ] catholic-readings-api `readings/2026/08-15.json` still missing firstReading (USCCB Day: Revelation 11:19A; 12:1-6A, 10AB).
- [/] Monitor cron execution and log reliability on production server.
  - Deployment now automated via GitHub Actions (push to main → git pull on whitepine)
  - Check cron.log periodically: `ssh whitepine "tail -50 /home/deploy/daily-summary/cron.log"`
  - **2026-06-29:** Cron missed today — `run.sh` had wrong `cd` path (`/home/cpbjr/...` instead of `/home/deploy/daily-summary`). Fixed and deployed. PR #2 merged. Email fired manually.
- [ ] Add support for attachments or liturgical images (optional).

---

> [!TIP]
> Completed tasks have been moved to `.agent/Tasks/completed/2026-01/` for historical record.

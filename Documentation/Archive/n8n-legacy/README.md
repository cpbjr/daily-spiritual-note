# n8n Legacy Artifacts (archived)

These files are from the original **n8n** implementation of the Daily Spiritual
Email workflow, before it was rewritten as the standalone Python application.
They are kept for historical reference only — **nothing in the live app uses
them**.

| File | What it is |
|------|------------|
| `Daily Spiritual Email-root.json` | The exported n8n workflow (was at the repo root). |
| `liturgical-readings.js` | n8n Code node — hard-coded USCCB readings database. |
| `Saints and Feast Days.js` | n8n Code node — saints / feast-day database. |
| `dodgers-schedule.js` | n8n Code node — 2025 Dodgers schedule lookup. |

> The `.js` files were originally committed with a `.c` extension; they are
> n8n-flavored JavaScript (`const`, `$json`) and were renamed on archival to
> reflect their real language.

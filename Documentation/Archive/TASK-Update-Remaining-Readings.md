# Task: Update Remaining 2025 Catholic Readings Files

## Status
- ✅ **October 2025:** All 30 files corrected with USCCB data (committed: 18c2766)
- ⏳ **November 2025:** 28 files need correction (11-03 through 11-30)
- ⏳ **December 2025:** 27 files need correction (various dates)

**Total remaining:** 55 files

## Problem Background

When the GitHub API was initially created, 85 readings files were generated using **placeholder/fallback data** based on day-of-week patterns instead of fetching actual USCCB readings. This resulted in incorrect liturgical data.

**Example error (Oct 3 before fix):**
- ❌ Placeholder: Hebrews 12:18-19, Psalm 48, Mark 6:7-13 (generic Thursday readings)
- ✅ Actual USCCB: Baruch 1:15-22, Psalm 79, Luke 10:13-16 (Friday of 26th Week)

## How to Update Files

### Step 1: Fetch Readings from USCCB

Use WebFetch to retrieve actual readings for each date:

```javascript
// November dates to fetch: 11-03 through 11-30
// December dates to fetch: 12-02, 12-03, 12-04, 12-05, 12-06, 12-07, 12-09,
//                          12-10, 12-11, 12-12, 12-13, 12-14, 12-15, 12-16,
//                          12-17, 12-18, 12-19, 12-20, 12-21, 12-22, 12-23,
//                          12-24, 12-26, 12-27, 12-28, 12-29, 12-30

// USCCB URL pattern: https://bible.usccb.org/bible/readings/MMDDYY.cfm
// Example for Nov 3: https://bible.usccb.org/bible/readings/110325.cfm
```

**WebFetch prompt to use:**
```
Extract the exact readings. Return ONLY a JSON object with this structure:
{"firstReading": "citation", "psalm": "citation", "secondReading": "citation or null", "gospel": "citation", "season": "liturgical description"}
```

### Step 2: Create JSON Files

For each date, create/update the JSON file in this format:

```json
{
  "date": "2025-MM-DD",
  "monthDay": "M/D",
  "season": "Ordinary Time",
  "readings": {
    "firstReading": "Book Chapter:Verses",
    "psalm": "Psalm Number:Verses",
    "gospel": "Gospel Chapter:Verses"
  },
  "usccbLink": "https://bible.usccb.org/bible/readings/MMDDYY.cfm",
  "apiEndpoint": "https://cpbjr.github.io/catholic-readings-api/readings/2025/MM-DD.json"
}
```

**Note for Sundays:** Include `"secondReading"` field between psalm and gospel.

**Example (November 3, 2025):**
```json
{
  "date": "2025-11-03",
  "monthDay": "11/3",
  "season": "Ordinary Time",
  "readings": {
    "firstReading": "Romans 9:1-5",
    "psalm": "Psalm 147:12-13, 14-15, 19-20",
    "gospel": "Luke 14:1-6"
  },
  "usccbLink": "https://bible.usccb.org/bible/readings/110325.cfm",
  "apiEndpoint": "https://cpbjr.github.io/catholic-readings-api/readings/2025/11-03.json"
}
```

### Step 3: Efficient Batch Processing

**Option A: Use bash script (fastest)**

Create `/tmp/update_november_readings.sh` with heredoc blocks for each file (see October script as template).

**Option B: Use WebFetch in batches**

Fetch 6-10 dates at a time using parallel WebFetch calls:

```javascript
// Batch 1: Nov 3-10
WebFetch(url1), WebFetch(url2), WebFetch(url3)...

// Batch 2: Nov 11-20
WebFetch(url1), WebFetch(url2)...

// etc.
```

### Step 4: Commit and Push

```bash
cd /home/cpbjr/Documents/AI_Automation/Projects/Daily Summary/catholic-readings-api

# Stage files
git add readings/2025/11-*.json
# or for December:
git add readings/2025/12-*.json

# Commit
git commit -m "Fix November 2025 readings with correct USCCB data

- Replaced placeholder readings with actual USCCB data
- Updated all 28 November files (11-03 through 11-30)
- Fetched correct readings from bible.usccb.org

All November readings now match official USCCB liturgical calendar.

🤖 Generated with Claude Code
Co-Authored-By: Claude <noreply@anthropic.com>"

# Push to GitHub Pages
git push origin main
```

## Dates Requiring Updates

### November 2025 (28 files)
```
11-03, 11-04, 11-05, 11-06, 11-07, 11-08, 11-09, 11-10,
11-11, 11-12, 11-13, 11-14, 11-15, 11-16, 11-17, 11-18,
11-19, 11-20, 11-21, 11-22, 11-23, 11-24, 11-25, 11-26,
11-27, 11-28, 11-29, 11-30
```

**Special liturgical dates in November:**
- Nov 1: All Saints (may already be correct)
- Nov 2: All Souls (may already be correct)
- Nov 9: Dedication of the Lateran Basilica
- Nov 30: Feast of Saint Andrew, Apostle (First Sunday of Advent may also fall here)

### December 2025 (27 files)
```
12-02, 12-03, 12-04, 12-05, 12-06, 12-07, 12-09, 12-10,
12-11, 12-12, 12-13, 12-14, 12-15, 12-16, 12-17, 12-18,
12-19, 12-20, 12-21, 12-22, 12-23, 12-24, 12-26, 12-27,
12-28, 12-29, 12-30
```

**Special liturgical dates in December:**
- Dec 1, 8, 15, 22: Advent Sundays (already correct - not in fix list)
- Dec 8: Immaculate Conception (already correct - not in fix list)
- Dec 25: Christmas (already correct - not in fix list)
- Dec 24: Christmas Eve vigil readings
- Dec 26: Feast of Saint Stephen
- Dec 27: Feast of Saint John, Apostle and Evangelist
- Dec 28: Feast of the Holy Innocents

## Verification

After updating files, verify a sample date:

```bash
# Check the corrected file
cat readings/2025/11-03.json

# Visit USCCB to confirm
# https://bible.usccb.org/bible/readings/110325.cfm

# Check GitHub Pages API
curl https://cpbjr.github.io/catholic-readings-api/readings/2025/11-03.json
```

## Priority

**Recommended order:**
1. **November first** - sooner chronologically, gives workflow a full month of correct data
2. **December second** - completes the year
3. **Test workflow** - Run a manual execution to verify readings display correctly in the daily email

## Success Criteria

- ✅ All 55 files updated with actual USCCB data
- ✅ Files committed and pushed to GitHub Pages
- ✅ GitHub API returns correct readings for each date
- ✅ Daily Email workflow displays accurate liturgical readings
- ✅ No more placeholder/fallback data in any 2025 readings file

## Notes

- The WebFetch tool has full access to bible.usccb.org for this project
- USCCB URL format is consistent: `MMDDYY.cfm` where YY=25 for 2025
- Sundays and major feasts include a Second Reading
- Season names vary (Ordinary Time, Advent, Christmas Time, etc.)
- The October correction commit (18c2766) serves as a reference for the pattern

## Related Files

- **October script template:** `/tmp/update_october_readings.sh`
- **Readings directory:** `catholic-readings-api/readings/2025/`
- **GitHub repository:** https://github.com/cpbjr/catholic-readings-api
- **Live API:** https://cpbjr.github.io/catholic-readings-api/

---

**Created:** 2025-10-03
**Last Updated:** 2025-10-03
**Assignee:** To be completed by user or Claude Code
**Estimated Time:** 45-60 minutes for all 55 files

# Workflow Error Handling Configuration

**Last Updated:** 2025-10-13
**Status:** Fixed and Deployed
**Production Workflow ID:** XOsIARJl8r1E7SWJ

---

## Critical Bug Fixed: October 13, 2025

### Problem Identified

The "Daily Spiritual Email" workflow was **stopping at "Get Saint" node** when saint data was unavailable (404 error from GitHub Pages API).

**Impact:**
- No email sent on days without special liturgical celebrations (~44% of days)
- Silent failures in production
- User not receiving daily spiritual content

### Root Cause

**Incorrect Error Handling Configuration:**

```json
{
  "name": "Get Saint",
  "onError": "continueErrorOutput"  // ❌ WRONG
}
```

**Why this failed:**
- `onError: "continueErrorOutput"` creates TWO output paths: main + error
- Error data routes to ERROR output (separate, disconnected path)
- Switch node is connected to MAIN output
- Switch receives no data → n8n stops workflow with "no input" error

### Solution Applied

**Correct Configuration:**

```json
{
  "name": "Get Saint",
  "continueOnFail": true  // ✅ CORRECT
}
```

**Why this works:**
- `continueOnFail: true` keeps ONE output path (main)
- Error data is treated as valid output on main path
- Switch node receives error data and can process it
- Switch routes to "Continue without Saint" path
- Workflow completes successfully

---

## Current Error Handling Strategy

### Nodes with Error Handling Enabled

#### 1. Get Readings Node (Line 49)
```json
{
  "name": "Get Readings",
  "type": "n8n-nodes-base.httpRequest",
  "continueOnFail": true,
  "parameters": {
    "url": "https://cpbjr.github.io/catholic-readings-api/readings/2025/{{ $json.apiDateFormat }}.json"
  }
}
```

**Purpose:** Continue workflow even if readings API is down

#### 2. Get Saint Node (Line 65)
```json
{
  "name": "Get Saint",
  "type": "n8n-nodes-base.httpRequest",
  "continueOnFail": true,
  "parameters": {
    "url": "https://cpbjr.github.io/catholic-readings-api/saints/2025/{{ $json.apiDateFormat }}.json"
  }
}
```

**Purpose:** Continue workflow when saint data unavailable (ordinary days)

### Graceful Degradation Logic

**Switch Node Routing:**

```
Get Saint → Switch Node
  ├─ Path 1: "Saint Found" (if $json.saint.name exists)
  │   └→ Merge with Saint → AI Agent (full content)
  │
  └─ Path 2: "No Saint" (if $json.data contains "Page not found" OR empty)
      └→ Continue without Saint → AI Agent (readings only)
```

---

## Expected Behavior by Scenario

### Scenario A: Normal Day with Saint (56% of year)
**Example:** January 1 (Mary, Mother of God)

```
✅ Get Readings → 200 OK
✅ Get Saint → 200 OK
✅ Switch → "Saint Found" path
✅ Email includes: readings + saint celebration
```

### Scenario B: Ordinary Day without Saint (44% of year)
**Example:** October 13 (Monday, Ordinary Time)

```
✅ Get Readings → 200 OK
⚠️  Get Saint → 404 (continues with warning)
✅ Switch → "No Saint" path
✅ Email includes: readings only
```

### Scenario C: Complete API Failure (rare)
**Example:** GitHub Pages outage

```
⚠️  Get Readings → 404 (continues with warning)
⚠️  Get Saint → 404 (continues with warning)
✅ Switch → "No Saint" path
✅ Email includes: fallback content + USCCB link
```

---

## n8n Error Handling Patterns

### Pattern 1: continueOnFail (Recommended)
```json
{
  "continueOnFail": true
}
```

**Use when:**
- You want downstream nodes to handle errors
- Errors should be treated as valid data
- Workflow should continue regardless of HTTP status

**Behavior:**
- Errors go to main output
- Downstream nodes receive error data as `$json`
- Can be processed by Switch/IF nodes

### Pattern 2: onError (Advanced, Use Carefully)
```json
{
  "onError": "continueErrorOutput"
}
```

**Use when:**
- You need separate error handling paths
- Error workflow is explicitly connected to error output
- Complex branching based on success vs failure

**⚠️ Warning:**
- Creates split output paths
- Downstream nodes MUST be connected to error output
- Easy to create disconnected paths (workflow stops)

### Pattern 3: alwaysOutputData
```json
{
  "alwaysOutputData": true
}
```

**Use when:**
- Node might return empty results
- Empty results should trigger downstream nodes
- Prevents workflow from stopping on "no data"

---

## Production Configuration Details

**Deployment Location:** AWS EC2
**Server URL:** http://3.16.157.215:5678
**Workflow ID:** XOsIARJl8r1E7SWJ
**File Location:** `/home/cpbjr/Documents/AI_Automation/Projects/Daily Summary/Daily Spiritual Email.json`

**Schedule:**
- Trigger: 4:00 AM EST daily (Schedule Trigger node)
- Timezone: America/New_York

**API Dependencies:**
- Primary: https://cpbjr.github.io/catholic-readings-api/
- Fallback: https://bible.usccb.org/bible/readings/

**Email Configuration:**
- Service: Gmail SMTP
- From: christopher.bisgaard@gmail.com
- To: cpbjr@mac.com
- Credentials: Gmail SMTP (credential ID: 02p2se41k0UMRSBZ)

---

## Monitoring & Validation

### Daily Checks
- Email arrives at ~4:05 AM EST
- No execution failures in n8n logs
- Content accuracy matches USCCB.org

### Weekly Validation
- Check execution history: http://3.16.157.215:5678/workflow/XOsIARJl8r1E7SWJ/executions
- Verify saint data appears on feast days
- Confirm readings data is current

### Monthly Review
- Review error patterns in execution logs
- Verify API endpoint availability
- Check for n8n version updates

---

## Troubleshooting Guide

### Issue: Workflow stops unexpectedly

**Check:**
1. Verify `continueOnFail: true` is set on HTTP Request nodes
2. Check Switch node connections (main output, not error output)
3. Review execution logs for node with red icon

**Fix:**
- Edit workflow JSON file
- Add `"continueOnFail": true` to failing node
- Re-import to n8n

### Issue: Email not received

**Check:**
1. Execution log shows "success" status?
2. Gmail SMTP credentials valid?
3. Spam folder in email client?

**Fix:**
- Test "Send Email" node manually
- Verify SMTP credentials in n8n settings
- Check recipient email address is correct

### Issue: Wrong readings displayed

**Check:**
1. API endpoint returning correct data?
2. Date formatting in "Format Date" node correct?
3. Year hardcoded somewhere in URL?

**Fix:**
- Test API endpoint manually: `curl https://cpbjr.github.io/catholic-readings-api/readings/2025/10-13.json`
- Verify date variables in "Format Date" output
- Update API URL if year is hardcoded

---

## Future Improvements

### Planned Enhancements
1. **Error Notification System** - Email admin on workflow failures
2. **Data Quality Validation** - Check readings match USCCB before sending
3. **Fallback Content** - Pre-generated emails for complete API failures
4. **Monitoring Dashboard** - Track execution success rate over time

### 2026 Preparation
- Generate 2026 readings data before December 31, 2025
- Update API URL year parameter (or make dynamic)
- Test full workflow with 2026 data in staging
- Deploy updated workflow to production

---

## Related Documentation

- **Project Overview:** `../.agent/README.md`
- **Product Requirements:** `../PRD.md`
- **Import Instructions:** `../IMPORT-INSTRUCTIONS.md`
- **API Repository:** `../catholic-readings-api/`

---

## Change History

| Date | Change | Impact |
|------|--------|--------|
| 2025-10-13 | Fixed "Get Saint" error handling | Workflow now completes on ordinary days |
| 2025-10-13 | Added "Get Readings" error handling | Protection against API failures |
| 2025-09-21 | Initial deployment | Production launch |

---

**Maintained by:** Claude Code
**Last Verified:** October 13, 2025
**Next Review:** November 13, 2025

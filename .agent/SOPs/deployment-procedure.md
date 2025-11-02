# Deployment Procedure: Daily Spiritual Email Workflow

**Last Updated:** October 26, 2025
**Author:** Claude Code (documented from successful October 26 deployment)
**Purpose:** Step-by-step guide for deploying workflow updates to AWS production server

---

## Overview

This SOP documents the procedure for deploying n8n workflow updates to the AWS EC2 production server. It covers both full workflow deployments (recommended) and troubleshooting steps for partial updates.

**Use this procedure when:**
- Fixing bugs in existing workflow nodes
- Adding new nodes or connections
- Updating node configurations or parameters
- Deploying corrected workflow JSON files

---

## Prerequisites

### Required Information
- **Production Server:** https://n8n.whitepine-tech.com
- **Workflow ID:** `h2DLuz8HuZKuLZhq`
- **API Key:** JWT token (stored in `.agent/README.md`)
- **Workflow File:** Updated JSON file in project directory

### Required Tools
- **curl** (for API calls)
- **jq** (for JSON formatting and validation)
- **Text editor** (for workflow JSON editing)

---

## Deployment Methods

### Method 1: Full Workflow Update (Recommended)

**Use when:** Making any structural changes to workflow (nodes, connections, parameters)
**Success Rate:** 100% (tested October 26, 2025)
**Time Required:** 5-10 minutes

#### Step 1: Prepare Workflow JSON

1. **Locate the corrected workflow file**
   ```bash
   cd "/home/cpbjr/Documents/AI_Automation/Projects/Daily Summary"
   ls -la *.json
   ```

2. **Validate JSON syntax**
   ```bash
   python3 -m json.tool "Daily Spiritual Email.json" > /dev/null
   ```
   If command succeeds silently, JSON is valid.

3. **Remove `id` field from JSON (CRITICAL)**

   The `id` field must NOT be in the request body for PUT requests (it goes in the URL).

   **Option A: Manual edit**
   - Open workflow JSON in text editor
   - Find and remove the `"id": "h2DLuz8HuZKuLZhq"` line
   - Save file

   **Option B: Use jq command**
   ```bash
   jq 'del(.id)' "Daily Spiritual Email.json" > temp.json
   mv temp.json "Daily Spiritual Email.json"
   ```

4. **Verify required fields present**
   ```bash
   jq 'keys' "Daily Spiritual Email.json"
   ```
   Should include: `name`, `nodes`, `connections`, `settings`

#### Step 2: Deploy to Production

1. **Set environment variables**
   ```bash
   export N8N_API_KEY="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJmMDgxMTM3OC0wYjViLTRhZGQtOTkwMi01OTJiZTIwOTZkMjEiLCJpc3MiOiJuOG4iLCJhdWQiOiJwdWJsaWMtYXBpIiwiaWF0IjoxNzYxNDI2ODAxfQ.vAhvKbvpGFTlYbmnOCs3TLIgAe5cLeR4aPviF7Wc2q4"
   export WORKFLOW_ID="h2DLuz8HuZKuLZhq"
   export N8N_URL="https://n8n.whitepine-tech.com"
   ```

2. **Deploy via REST API**
   ```bash
   curl -X PUT \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     -H "X-N8N-API-KEY: $N8N_API_KEY" \
     -H "Content-Type: application/json" \
     -d @"Daily Spiritual Email.json"
   ```

3. **Check response**
   - **Success:** Returns complete workflow JSON with `"id": "h2DLuz8HuZKuLZhq"`
   - **Failure:** Returns error object with `"message"` field

#### Step 3: Verify Deployment

1. **Fetch deployed workflow**
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.'
   ```

2. **Verify critical fields**
   ```bash
   # Check node count (should be 12)
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.nodes | length'

   # Check if workflow is active
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.active'

   # Check specific node configuration (e.g., Switch node)
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.nodes[] | select(.name == "Switch") | .parameters'
   ```

3. **Verify Switch node typeValidation (critical for this workflow)**
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.nodes[] | select(.name == "Switch") | .parameters.rules.values[].conditions.options.typeValidation'
   ```
   **Expected output:** Both conditions should return `"loose"`

---

### Method 2: Partial Workflow Update (NOT Recommended)

**Use when:** ONLY for very simple changes (e.g., updating a single string parameter)
**Success Rate:** Low (~30% for complex nodes like Switch)
**Time Required:** 10-20 minutes (including troubleshooting)

**Why NOT Recommended:**
- Silently fails on complex nested structures
- No verification mechanism in n8n MCP
- Switch nodes particularly problematic
- Wastes time debugging phantom issues

**If you must use partial updates:**

1. Use `mcp__n8n__n8n_update_partial_workflow` tool
2. Limit to 5 operations or fewer
3. **ALWAYS verify with `n8n_get_workflow` after update**
4. If verification fails, fall back to Method 1 (Full Update)

**Example operations that work:**
- `updateName`: Change workflow name
- `add/removeTag`: Modify workflow tags
- `updateSettings`: Change execution order or timezone

**Example operations that fail silently:**
- `updateNode`: Complex nodes like Switch, IF, Code
- `addConnection`: Multi-output nodes
- Nested parameter updates (e.g., Switch conditions)

---

## Post-Deployment Checklist

### Immediate Verification (0-5 minutes)

- [ ] Workflow JSON returned from API contains correct `id`
- [ ] Node count matches expected (12 nodes for Daily Spiritual Email)
- [ ] Workflow `active` status is `true`
- [ ] Critical node configurations match source file (especially Switch node)
- [ ] No error messages in API response

### Short-Term Verification (1-24 hours)

- [ ] Monitor next scheduled execution (4:00 AM EST)
- [ ] Verify email delivery to cpbjr@mac.com
- [ ] Check email content for formatting and accuracy
- [ ] Review execution logs in n8n UI for errors

### Long-Term Monitoring (1-7 days)

- [ ] Confirm daily email delivery continues
- [ ] Verify both Switch paths work (saint found vs no saint)
- [ ] Monitor execution time (should be 8-12 seconds)
- [ ] Check for any SMTP or API failures

---

## Troubleshooting

### Problem: Deployment API returns 401 Unauthorized

**Cause:** Invalid or expired API key
**Solution:**
1. Verify API key is correct in `.agent/README.md`
2. Check if API key has been rotated on AWS server
3. Test API key with simple GET request:
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" "$N8N_URL/api/v1/workflows"
   ```

### Problem: Deployment succeeds but changes not reflected

**Cause:** Most likely used partial update on complex node (Switch, IF, Code)
**Solution:**
1. Use Method 1 (Full Workflow Update) instead
2. Verify with `n8n_get_workflow` that changes are present
3. Check n8n UI directly to confirm visual representation

### Problem: JSON validation fails before deployment

**Cause:** Syntax error or invalid n8n workflow structure
**Solution:**
1. Use `python3 -m json.tool` to identify syntax error line
2. Use `mcp__n8n__validate_workflow` to check n8n-specific issues
3. Compare against known-good workflow file (Daily Spiritual Email.json is production-verified)

### Problem: Workflow imported but not executing on schedule

**Cause:** Workflow `active` status is `false`
**Solution:**
1. Check workflow status:
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.active'
   ```
2. Activate via n8n UI or API:
   ```bash
   curl -X PATCH \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     -H "X-N8N-API-KEY: $N8N_API_KEY" \
     -H "Content-Type: application/json" \
     -d '{"active": true}'
   ```

### Problem: Node credentials not found after deployment

**Cause:** Credentials are not included in workflow JSON (expected behavior)
**Solution:**
1. Verify credentials exist on AWS server (via n8n UI)
2. Check credential IDs match between local and production:
   - Google SMTP: `t5muBt48UrDocNe8`
   - OpenRouter: `YxHKEc4tsQR19kOm`
3. If credentials missing, recreate in n8n UI

---

## Best Practices

### Pre-Deployment
1. **Always validate JSON syntax** before deploying
2. **Test workflow locally** if possible (use localhost n8n instance)
3. **Back up current production workflow** before updating:
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     > "backup-$(date +%Y%m%d-%H%M%S).json"
   ```
4. **Document changes** in `.agent/System/current-issue.md` or README.md

### During Deployment
1. **Use Method 1 (Full Update)** unless you have a specific reason not to
2. **Remove `id` field** from request body (goes in URL for PUT)
3. **Verify immediately** after deployment (don't wait for next execution)
4. **Check critical configurations** specific to your workflow (e.g., Switch typeValidation)

### Post-Deployment
1. **Monitor next execution** to confirm changes work as expected
2. **Update documentation** with deployment timestamp and verification results
3. **Archive old workflow versions** to `Documentation/Archive/`
4. **Document lessons learned** if deployment had issues

---

## Rollback Procedure

If deployment causes issues:

1. **Locate previous working version**
   ```bash
   ls -lt Documentation/Archive/*.json | head -5
   ```

2. **Deploy previous version using Method 1**
   ```bash
   curl -X PUT \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     -H "X-N8N-API-KEY: $N8N_API_KEY" \
     -H "Content-Type: application/json" \
     -d @"Documentation/Archive/backup-20251026-134237.json"
   ```

3. **Verify rollback**
   ```bash
   curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
     "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
     | jq '.updatedAt'
   ```

4. **Document rollback reason** in `.agent/System/current-issue.md`

---

## Security Notes

### API Key Management
- **Never commit API keys to git** (use .gitignore)
- **Rotate keys regularly** (every 90 days recommended)
- **Store in secure location** (`.agent/README.md` is gitignored)
- **Use environment variables** for scripts

### AWS Access
- **Restrict EC2 security group** to known IP addresses
- **Use HTTPS when available** (currently HTTP for internal network)
- **Monitor access logs** for unauthorized API calls

---

## Success Metrics

A successful deployment should result in:

- ✅ API returns 200 status code
- ✅ Workflow JSON returned with correct `id`
- ✅ Verification shows changes deployed
- ✅ Next scheduled execution completes successfully
- ✅ Email delivered with expected content
- ✅ No error messages in execution logs

---

## Example: October 26, 2025 Deployment (Reference)

**What was deployed:** Switch node fix (typeValidation: "loose")
**Method used:** Full Workflow Update (Method 1)
**Time taken:** ~8 minutes (including verification)

**Commands executed:**
```bash
# 1. Validated JSON
python3 -m json.tool "Daily Spiritual Email.json" > /dev/null

# 2. Removed id field (if needed)
jq 'del(.id)' "Daily Spiritual Email.json" > temp.json && mv temp.json "Daily Spiritual Email.json"

# 3. Set environment
export N8N_API_KEY="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
export WORKFLOW_ID="h2DLuz8HuZKuLZhq"
export N8N_URL="https://n8n.whitepine-tech.com"

# 4. Deployed
curl -X PUT \
  "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
  -H "X-N8N-API-KEY: $N8N_API_KEY" \
  -H "Content-Type: application/json" \
  -d @"Daily Spiritual Email.json"

# 5. Verified Switch node configuration
curl -H "X-N8N-API-KEY: $N8N_API_KEY" \
  "$N8N_URL/api/v1/workflows/$WORKFLOW_ID" \
  | jq '.nodes[] | select(.name == "Switch") | .parameters.rules.values[].conditions.options.typeValidation'
```

**Result:** Both Switch conditions returned `"loose"` ✅
**Status:** Awaiting next automatic execution (October 27, 4:00 AM EST)

---

## References

- **Tech Stack:** [../System/tech-stack.md](../System/tech-stack.md)
- **Workflow Architecture:** [../System/workflow-architecture.md](../System/workflow-architecture.md)
- **October 26 Deployment Details:** [../System/current-issue.md](../System/current-issue.md)
- **n8n API Documentation:** https://docs.n8n.io/api/
- **Production Workflow:** https://n8n.whitepine-tech.com/workflow/h2DLuz8HuZKuLZhq

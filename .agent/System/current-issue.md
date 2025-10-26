# Current Issue: Switch Node Routing Failure

**Last Updated:** October 13, 2025
**Status:** Fix identified, awaiting implementation

---

## Problem Summary

The "Daily Spiritual Email" workflow stops execution at the Switch node when the Catholic Readings API returns 404 for saint data. The workflow should continue and send an email with readings-only content, but instead terminates early.

---

## Technical Analysis

### Data Structure Mismatch

**When saint API returns 404, the actual data structure is:**
```json
{
  "error": {
    "message": "404 - \"<!DOCTYPE html>...\"",
    "status": 404
  }
}
```

**But the Switch node currently checks for:**
- Condition 1: `{{ $json.saint.name }}` (doesn't exist in error response)
- Condition 2: `{{ $json.data }}` (also doesn't exist in error response)

**Result:** Both Switch outputs return empty arrays `[[], []]`, workflow stops

---

## The Fix

### Switch Node Configuration Changes

**Condition 1 (Saint Found):**
- **Old:** `{{ $json.saint.name }}` with operator `string.notEmpty`
- **New:** `{{ $json.saint }}` with operator `object.notEmpty`

**Condition 2 (No Saint):**
- **Old:** `{{ $json.data }}` with operator `string.contains` "Page not found"
- **New:** `{{ $json.error }}` with operator `object.notEmpty`

---

## Attempted Solutions

### Attempt 1: MCP Partial Workflow Update
- **Tool:** `mcp__n8n__n8n_update_partial_workflow`
- **Result:** FAILED - MCP returned "success" but changes not persisted to database
- **Issue:** Complex nested Switch node structure not handled correctly by MCP

### Attempt 2: n8n Workflow Specialist Subagent
- **Tool:** Task agent with n8n-workflow-specialist
- **Result:** FAILED - Claimed to apply updates but verification showed no changes
- **Issue:** Same MCP limitation - partial updates silently fail on complex nodes

### Attempt 3: Manual JSON Edit + Import
- **Approach:** Created corrected JSON file for manual import
- **File:** `.agent/Tasks/Daily Spiritual Email - FIXED.json`
- **Status:** Ready to import, but requires manual action in n8n UI

---

## Execution Log Evidence

**Execution ID:** 124 (localhost n8n)
**Date:** October 13, 2025

```json
{
  "Get Saint": [{
    "executionStatus": "success",
    "data": {
      "main": [[{
        "json": {
          "error": {
            "message": "404 - ...",
            "status": 404
          }
        }
      }]]
    }
  }],
  "Switch": [{
    "executionStatus": "success",
    "data": {
      "main": [[], []]  // ← Empty arrays prove neither condition matched
    }
  }]
}
```

---

## Lessons Learned

### MCP Limitations Discovered

1. **Partial updates fail silently** on complex nested node configurations
2. **No verification mechanism** - MCP returns success without checking persistence
3. **Switch nodes particularly problematic** - Deep JSON nesting causes issues

### Best Practices Going Forward

1. **Simple node fixes** - Use n8n UI directly (faster, more reliable)
2. **Complex workflow changes** - Use MCP for creating new workflows, not updating existing ones
3. **Always verify MCP changes** - Use `n8n_get_workflow` to confirm updates were applied
4. **Fallback to manual methods** - Don't waste time fighting MCP limitations

---

## Implementation Path

### Recommended: Direct UI Edit (2 minutes)

1. Open http://localhost:5678
2. Open "Daily Spiritual Email" workflow
3. Click Switch node
4. Update both conditions as specified above
5. Save and test

### Alternative: Import Fixed Workflow

1. Import `.agent/Tasks/Daily Spiritual Email - FIXED.json`
2. Delete old broken workflow
3. Test new workflow

---

## Success Criteria

- [ ] Switch node routes correctly when saint data exists
- [ ] Switch node routes correctly when saint data returns 404
- [ ] Email is sent successfully in both scenarios
- [ ] Workflow deployed to AWS production server
- [ ] This issue marked as resolved

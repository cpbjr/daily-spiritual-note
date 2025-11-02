# Switch Node Configuration for API Error Handling

**Created:** October 27, 2025
**Purpose:** Document correct Switch node configuration for handling API responses with optional data
**Context:** Daily Spiritual Email workflow - Catholic Readings API integration

---

## Problem Overview

When working with APIs that return **different JSON structures** based on success/failure (e.g., `{saint: {...}}` vs `{error: {...}}`), the Switch node must correctly route execution based on which fields are present.

### Common Pitfall

Using `"operation": "notEmpty"` when you should use `"operation": "exists"` leads to routing failures when fields are null, empty, or undefined.

---

## Operator Comparison

### `exists` Operator (CORRECT for API routing)

**What it checks:** Whether a field is **present in the JSON object**

**Returns TRUE when:**
- Field exists with any value: `{ "saint": {...} }`
- Field exists with null: `{ "saint": null }`
- Field exists with empty object: `{ "saint": {} }`

**Returns FALSE when:**
- Field is missing: `{ "error": {...} }` (no saint field)

**Use case:** Checking API response structure (success vs error)

### `notEmpty` Operator (WRONG for API routing)

**What it checks:** Whether a field is **present AND has a non-empty value**

**Returns TRUE when:**
- Field exists with non-empty value: `{ "saint": { "name": "..." } }`

**Returns FALSE when:**
- Field is missing: `{ "error": {...} }`
- Field is null: `{ "saint": null }`
- Field is empty object: `{ "saint": {} }`
- Field is empty string: `{ "saint": "" }`

**Use case:** Validating that required data has actual content (not just presence)

---

## Correct Switch Node Configuration

### Template for API Error Handling

```json
{
  "parameters": {
    "rules": {
      "values": [
        {
          "conditions": {
            "options": {
              "caseSensitive": true,
              "leftValue": "",
              "typeValidation": "strict",
              "version": 2
            },
            "conditions": [
              {
                "id": "c1",
                "leftValue": "={{ $json.dataField }}",
                "rightValue": "",
                "operator": {
                  "type": "object",
                  "operation": "exists",
                  "singleValue": true
                }
              }
            ],
            "combinator": "and"
          },
          "renameOutput": true,
          "outputKey": "Data Found"
        },
        {
          "conditions": {
            "options": {
              "caseSensitive": true,
              "leftValue": "",
              "typeValidation": "strict",
              "version": 2
            },
            "conditions": [
              {
                "id": "c2",
                "leftValue": "={{ $json.error }}",
                "rightValue": "",
                "operator": {
                  "type": "object",
                  "operation": "exists",
                  "singleValue": true
                }
              }
            ],
            "combinator": "and"
          },
          "renameOutput": true,
          "outputKey": "Error Occurred"
        }
      ]
    },
    "options": {}
  },
  "type": "n8n-nodes-base.switch",
  "typeVersion": 3.2
}
```

### Required Configuration Fields

| Field | Location | Value | Purpose |
|-------|----------|-------|---------|
| `caseSensitive` | options | `true` | Field name matching mode |
| `leftValue` | options | `""` | Default left value (empty for dynamic) |
| `typeValidation` | options | `"strict"` | Type checking mode (safe with `exists`) |
| `version` | options | `2` | Switch node configuration version |
| `operation` | operator | `"exists"` | Check field presence, not value |
| `singleValue` | operator | `true` | Single field check (not array) |

---

## Type Validation: `strict` vs `loose`

### When to Use `strict` (RECOMMENDED with `exists`)

✅ **Safe with `exists` operator** because:
- `exists` returns boolean (true/false) - no type coercion needed
- Doesn't attempt to evaluate field values
- Only checks JSON structure

✅ **Benefits:**
- Catches configuration errors early
- Enforces correct expression syntax
- Fails fast if data structure is unexpected

### When to Use `loose`

❌ **Only needed when:**
- Using operators that require value comparison (`equals`, `contains`, etc.)
- Dealing with inconsistent API data types (string vs number)
- Intentionally allowing type coercion

⚠️ **Risks:**
- Silent failures if data structure changes
- Harder to debug routing issues
- May mask configuration problems

---

## Deployment Checklist

Before deploying Switch node changes:

- [ ] Verify `"operation": "exists"` is used (not `notEmpty`)
- [ ] Confirm `"typeValidation": "strict"` in options
- [ ] Include all required fields: `leftValue`, `version`, `singleValue`
- [ ] Validate JSON syntax: `python3 -m json.tool workflow.json > /dev/null`
- [ ] Test both routing paths with real API responses
- [ ] Compare configuration line-by-line with working production version

---

## Case Study: Daily Spiritual Email

### Original Problem (October 13, 2025)

**Symptom:** Workflow stopped at Switch node when saint API returned 404

**Root Cause:** Used `"operation": "notEmpty"` which failed when:
- API returned `{ "error": {...} }` (no saint field)
- Switch expected `{ "saint": {...} }` or `{ "data": "..." }`

**Incorrect Diagnosis:** Assumed `typeValidation: "strict"` was rejecting empty strings

### Attempted Fix #1 (October 26, 2025 - WRONG)

**Change Made:** `typeValidation: "strict"` → `"loose"` and `notEmpty` → `notEmpty`

**Result:** Did not solve the problem (operator still wrong)

**Lesson:** Changing `typeValidation` masked the real issue

### Correct Fix (October 27, 2025 - VERIFIED)

**Changes Made:**
1. `"operation": "notEmpty"` → `"exists"` (both conditions)
2. Reverted `typeValidation: "loose"` → `"strict"` (safe with `exists`)
3. Added missing fields: `leftValue`, `version`, `singleValue`

**Result:** Both routing paths work correctly:
- `{ "saint": {...} }` → Routes to "Saint Found"
- `{ "error": {...} }` → Routes to "No Saint"

**Verification:** Hosted production version (working since October 27, 2025) uses this exact configuration

---

## Common Scenarios

### Scenario 1: Optional API Field

**API Response:**
```json
// Success with data
{ "saint": { "name": "St. Peter" } }

// Success without data
{ "saint": null }

// Error
{ "error": { "message": "404" } }
```

**Correct Configuration:**
```json
{
  "leftValue": "={{ $json.saint }}",
  "operator": {
    "type": "object",
    "operation": "exists"  // ← Handles all three cases
  }
}
```

### Scenario 2: Required Field with Content Validation

**API Response:**
```json
// Valid
{ "user": { "name": "John", "email": "john@example.com" } }

// Invalid (missing data)
{ "user": {} }

// Invalid (null)
{ "user": null }
```

**Correct Configuration:**
```json
{
  "leftValue": "={{ $json.user }}",
  "operator": {
    "type": "object",
    "operation": "notEmpty"  // ← Requires non-empty object
  }
}
```

---

## Troubleshooting

### Switch Node Returns Empty Arrays

**Symptom:** `"data": { "main": [[], []] }`

**Cause:** Neither condition matched - check operator choice

**Solution:** Use `exists` for structure checking, `notEmpty` for value validation

### Workflow Stops at Switch Node

**Symptom:** No downstream nodes execute

**Cause:** All routing conditions returned false

**Solution:**
1. Verify field names match API response exactly
2. Check if using correct operator (`exists` vs `notEmpty`)
3. Add fallback routing path (Condition 3: "Otherwise")

### Type Validation Errors

**Symptom:** "Type validation failed" in execution log

**Cause:** `strict` mode with value-based operators

**Solution:**
- Keep `strict` if using `exists` (no value evaluation)
- Use `loose` only for value-based operators (`equals`, `contains`)

---

## References

- **Production Workflow:** https://n8n.whitepine-tech.com/workflow/h2DLuz8HuZKuLZhq (workflow ID: `h2DLuz8HuZKuLZhq`)
- **Local Workflow:** `Projects/Daily Summary/Daily Spiritual Email.json`
- **Issue Documentation:** `.agent/System/current-issue.md`
- **n8n Switch Node Docs:** https://docs.n8n.io/integrations/builtin/core-nodes/n8n-nodes-base.switch/

---

**Last Updated:** October 27, 2025
**Verified Working:** October 27, 2025 (hosted production)

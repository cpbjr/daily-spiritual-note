# Workflow Architecture

**Last Updated:** October 26, 2025
**Workflow Name:** Daily Spiritual Email
**Production ID:** `h2DLuz8HuZKuLZhq`

## Visual Node Flow Diagram

```
┌─────────────────┐
│ Schedule Trigger│ (4:00 AM EST daily)
└────────┬────────┘
         │
         v
┌─────────────────┐
│  Format Date    │ (JavaScript: Create date formats)
│  (Code Node)    │ Outputs: today, dayOfWeek, apiDateFormat, theme, usccbLink
└────────┬────────┘
         │
         v
┌─────────────────┐
│  Get Readings   │ (HTTP Request to GitHub Pages API)
│  (HTTP)         │ URL: /readings/2025/{MM-DD}.json
└────────┬────────┘ continueOnFail: true
         │
         v
┌─────────────────┐
│   Get Saint     │ (HTTP Request to GitHub Pages API)
│  (HTTP)         │ URL: /saints/2025/{MM-DD}.json
└────────┬────────┘ continueOnFail: true (404 expected when no saint)
         │
         v
┌─────────────────┐
│     Switch      │ (Routing Logic - typeValidation: "loose")
│  (Switch Node)  │ Condition 1: $json.saint exists → "Saint Found"
└────────┬────────┘ Condition 2: $json.error exists → "No Saint"
         │
    ┌────┴────┐
    │         │
    v         v
┌───────┐ ┌─────────────────┐
│ Saint │ │    No Saint     │
│ Found │ │                 │
└───┬───┘ └────────┬────────┘
    │              │
    v              v
┌─────────────┐ ┌──────────────────┐
│Merge with   │ │Continue without  │
│Saint (Code) │ │Saint (Code)      │
│hasSaint:true│ │hasSaint:false    │
└──────┬──────┘ └────────┬─────────┘
       │                 │
       └────────┬────────┘
                v
        ┌───────────────┐
        │   AI Agent    │ (LangChain Agent with Grok-4)
        │  + OpenRouter │ Generates spiritual reflection JSON
        │  Chat Model   │ Token budget: <1000 tokens
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │Parse AI Output│ (JavaScript: Clean & validate JSON)
        │  (Code Node)  │ Try-catch with fallback values
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  Build HTML   │ (JavaScript: Generate email template)
        │  (Code Node)  │ Responsive design with liturgical calendar
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  Send email   │ (Gmail SMTP)
        │  (Email Send) │ christopher.bisgaard@gmail.com → cpbjr@mac.com
        └───────────────┘
```

---

## Data Flow & Transformations

### Phase 1: Date Preparation
**Node:** Format Date (Code)
**Input:** Trigger timestamp
**Transformations:**
- Extract: Year, Month, Day of month, Day of week
- Create: `today` (human-readable: "Sunday, October 27, 2025")
- Create: `apiDateFormat` (MM-DD format: "10-27")
- Create: `usccbLink` (USCCB readings URL: MMDDYY format)
- Create: `theme` (Benedictine theme by day of week)

**Output:**
```json
{
  "today": "Sunday, October 27, 2025",
  "dayOfWeek": "Sunday",
  "monthDay": "10/27",
  "apiDateFormat": "10-27",
  "theme": "Worship & Community",
  "usccbLink": "https://bible.usccb.org/bible/readings/102725.cfm"
}
```

---

### Phase 2: Liturgical Data Retrieval
**Nodes:** Get Readings + Get Saint (HTTP Requests)
**Input:** `apiDateFormat` from Format Date
**API Calls:**
1. `https://cpbjr.github.io/catholic-readings-api/readings/2025/10-27.json`
2. `https://cpbjr.github.io/catholic-readings-api/saints/2025/10-27.json`

**Get Readings Output (Success):**
```json
{
  "readings": {
    "firstReading": "Romans 8:12-17",
    "psalm": "Psalm 68:2, 4, 6-7, 20-21",
    "secondReading": "Luke 13:1-9",
    "gospel": "Matthew 7:21-29"
  },
  "season": "Thirtieth Sunday in Ordinary Time",
  "usccbLink": "https://bible.usccb.org/bible/readings/102725.cfm"
}
```

**Get Saint Output (Success - Saint Found):**
```json
{
  "saint": {
    "name": "St. Peter Claver",
    "type": "Saint",
    "description": "Jesuit missionary who ministered to enslaved Africans...",
    "quote": "We must speak to them with our hands before we speak to them with our lips."
  }
}
```

**Get Saint Output (Failure - No Saint):**
```json
{
  "error": {
    "message": "404 - Page not found",
    "status": 404
  }
}
```

---

### Phase 3: Routing Logic (Critical Switch Node)

**Node:** Switch
**Purpose:** Route workflow based on saint data availability
**Configuration:** `typeValidation: "loose"` (CRITICAL - handles empty strings)

**Condition 1: "Saint Found"**
- Expression: `{{ $json.saint }}`
- Operator: `object.notEmpty`
- Routes to: "Merge with Saint" node

**Condition 2: "No Saint"**
- Expression: `{{ $json.error }}`
- Operator: `object.notEmpty`
- Routes to: "Continue without Saint" node

**Why This Matters:**
- October 26 bug: `typeValidation: "strict"` rejected empty strings
- Fix: Changed to `"loose"` to handle edge cases
- **Lesson:** Always use loose validation for optional API fields

---

### Phase 4: Data Preparation for AI

**Path A: Merge with Saint (Code)**
```javascript
const readings = $('Get Readings').first().json;
const saints = $input.first().json;
const dateInfo = $('Format Date').first().json;

return {
  ...dateInfo,
  readings: readings.readings,
  season: readings.season,
  usccbLink: readings.usccbLink,
  saint: saints.saint,
  hasSaint: true  // ← Flag for AI Agent
};
```

**Path B: Continue without Saint (Code)**
```javascript
const readings = $('Get Readings').item.json;
const dateInfo = $('Format Date').item.json;

return {
  ...dateInfo,
  readings: readings.readings,
  season: readings.season,
  usccbLink: readings.usccbLink,
  saint: null,  // ← Explicitly null
  hasSaint: false  // ← Flag for AI Agent
};
```

---

### Phase 5: AI Content Generation

**Nodes:** AI Agent + OpenRouter Chat Model
**Model:** Grok-4 (x-ai) via OpenRouter API
**Token Budget:** <1000 tokens per execution

**AI Agent Prompt Structure:**
```
User Prompt:
- Date & Theme: {today}, {dayOfWeek}, {theme}
- Liturgical Info: {season}, {readings}
- Celebration Info: {hasSaint}, {saint data if present}

System Prompt:
- Role: Catholic spiritual director
- Output Format: Valid JSON with 10 fields
- Celebration Handling: Dynamic section based on type (Saint/Feast/Solemnity)
```

**AI Agent Output (Raw):**
```json
{
  "spiritualFocus": "brief focus for spiritual growth",
  "liturgicalReadings": "insight about today's readings",
  "celebrationName": "St. Peter Claver, Jesuit missionary",
  "celebrationType": "Saint",
  "celebrationContent": "2-3 sentence biography...",
  "celebrationLink": "https://en.wikipedia.org/wiki/Peter_Claver",
  "sectionTitle": "Saint of the Day",
  "reflection": "spiritual reflection on today's theme...",
  "practicalChallenge": "practical action for today",
  "prayer": "short prayer for the day"
}
```

---

### Phase 6: Output Formatting

**Node:** Parse AI Output (Code)
**Purpose:** Clean and validate AI JSON, provide fallbacks
**Error Handling:** Try-catch with default values

**Node:** Build HTML (Code)
**Purpose:** Generate responsive email template
**Features:**
- Gradient header with liturgical calendar day
- Dynamic celebration section (adapts to Saint/Feast/Solemnity)
- Responsive design (mobile-friendly)
- USCCB link in footer

**HTML Output Structure:**
```html
<!DOCTYPE html>
<html>
  <head>...</head>
  <body>
    <div style="max-width: 800px;">
      <header>Daily Benedictine Reflection</header>
      <section>Liturgical Day: {liturgicalDay}</section>
      <section>Today's Readings: {readings}</section>
      <section>Today's Focus: {theme}</section>
      <section>[Celebration] if present</section>
      <section>Reflection & Challenge</section>
      <section>Daily Prayer</section>
      <footer>Ora et Labora | View Official Readings</footer>
    </div>
  </body>
</html>
```

---

### Phase 7: Email Delivery

**Node:** Send email
**Method:** Gmail SMTP
**From:** christopher.bisgaard@gmail.com
**To:** cpbjr@mac.com
**Subject:** "Daily Spiritual Email for {today}"
**Content-Type:** HTML

---

## Error Handling Strategy

### HTTP Request Failures
**Pattern:** `continueOnFail: true` on both HTTP Request nodes
**Behavior:**
- Get Readings fails → Workflow stops (critical data)
- Get Saint fails → Workflow continues via "No Saint" path

### Switch Node Routing
**Problem:** When neither condition matches, Switch outputs empty arrays `[[], []]`
**Solution:** Ensure conditions are mutually exclusive and cover all cases
**Current Design:**
- Condition 1: Saint data exists
- Condition 2: Error object exists
- **Coverage:** 100% (one of these always true)

### AI Agent Failures
**Node:** Parse AI Output handles malformed JSON
**Fallbacks:**
- JSON parsing error → Return error object
- Missing fields → Use default values ("No X available")
- Invalid structure → Try-catch prevents workflow failure

---

## Performance Characteristics

### Execution Time Breakdown
- **Schedule Trigger:** 0s (instant)
- **Format Date:** <1s (JavaScript execution)
- **Get Readings:** 1-2s (GitHub Pages API)
- **Get Saint:** 1-2s (GitHub Pages API, may 404)
- **Switch:** <1s (routing logic)
- **Merge/Continue:** <1s (JavaScript execution)
- **AI Agent:** 3-5s (Grok-4 generation)
- **Parse AI Output:** <1s (JSON parsing)
- **Build HTML:** <1s (template generation)
- **Send email:** 1-2s (SMTP delivery)
- **Total:** ~8-12 seconds per execution

### Token Efficiency
**Design Philosophy:** Code Tool Pattern
- **Traditional Approach:** Load liturgical data in AI prompt (~15k tokens)
- **Current Architecture:** Code nodes fetch data, AI only generates reflection (<1k tokens)
- **Savings:** 93% reduction in token usage
- **Cost:** <$0.001 per execution with Grok-4

---

## Branching Paths

### Path 1: Saint Found
```
Switch → Merge with Saint → AI Agent → Parse → Build HTML → Send email
```
**Trigger:** When GitHub Pages API returns saint data
**Result:** Email includes "Saint of the Day" section

### Path 2: No Saint (404)
```
Switch → Continue without Saint → AI Agent → Parse → Build HTML → Send email
```
**Trigger:** When GitHub Pages API returns 404 for saint
**Result:** Email includes readings + reflection only (no saint section)

---

## Integration Points

### External APIs
1. **Catholic Readings API** (GitHub Pages)
   - Owner: cpbjr
   - Repository: catholic-readings-api (local directory)
   - Hosting: GitHub Pages static site
   - Rate Limits: None (static files)

2. **OpenRouter API** (AI Model Provider)
   - Model: Grok-4 (x-ai)
   - Authentication: API key in credentials
   - Rate Limits: Per OpenRouter tier

3. **USCCB Readings** (Reference Only)
   - Purpose: User-facing link in email footer
   - No API calls from workflow

### Credential Dependencies
- **Google SMTP account** (Send email node)
- **OpenRouter account** (OpenRouter Chat Model)

---

## Deployment Architecture

### Production Environment
- **Server:** https://n8n.whitepine-tech.com
- **Workflow ID:** `h2DLuz8HuZKuLZhq`
- **Authentication:** n8n API key (JWT token)
- **Deployment Method:** REST API (`PUT /api/v1/workflows/{id}`)

### Workflow State
- **Active:** Yes (scheduled execution enabled)
- **Schedule:** 4:00 AM EST daily
- **Timezone:** America/New_York (EST/EDT)

---

## Key Design Decisions

### 1. Switch Node over IF Node
**Reason:** Better readability with named outputs ("Saint Found" vs "No Saint")
**Benefit:** Easier debugging in n8n UI

### 2. AI Agent + Code Tool Pattern
**Reason:** Token efficiency (93% savings)
**Benefit:** Scalable to additional data sources without prompt bloat

### 3. Type Validation: Loose
**Reason:** Optional API fields may return empty strings or null
**Benefit:** Robust routing even with inconsistent API responses

### 4. Continue on Fail for HTTP Requests
**Reason:** Saint data is optional, readings are required
**Benefit:** Workflow continues even when saint API returns 404

### 5. GitHub Pages for Data Hosting
**Reason:** Free, reliable, git-tracked, no rate limits
**Benefit:** Full control over data structure and versioning

---

## Testing Scenarios

### Test Case 1: Regular Day with Saint
**Example:** October 9 (St. Denis)
**Expected Path:** Switch → Merge with Saint → AI Agent
**Expected Output:** Email with "Saint of the Day" section

### Test Case 2: Regular Day without Saint
**Example:** October 13 (Ordinary Time)
**Expected Path:** Switch → Continue without Saint → AI Agent
**Expected Output:** Email with readings + reflection only

### Test Case 3: Major Feast Day
**Example:** Christmas (Solemnity)
**Expected Path:** Switch → Merge with Saint → AI Agent
**Expected Output:** Email with "Liturgical Celebration" section

### Test Case 4: API Failure (Readings)
**Scenario:** GitHub Pages down or invalid date
**Expected Behavior:** Workflow stops (critical data missing)

### Test Case 5: API Failure (Saint only)
**Scenario:** Saint API returns 404
**Expected Behavior:** Workflow continues via "No Saint" path

---

## Monitoring & Observability

### Success Criteria
- ✅ Email delivered daily at 4:00 AM EST
- ✅ HTML renders correctly in email client
- ✅ AI content is relevant and well-formatted
- ✅ Both Switch paths handle data correctly

### Failure Indicators
- ❌ No email received by 4:15 AM EST
- ❌ Email contains error messages or fallback values
- ❌ Switch node outputs empty arrays (routing failure)
- ❌ HTML formatting broken in email client

### Debugging Commands
```bash
# Check workflow status
curl -H "X-N8N-API-KEY: {key}" https://n8n.whitepine-tech.com/api/v1/workflows/h2DLuz8HuZKuLZhq

# Get recent executions
curl -H "X-N8N-API-KEY: {key}" https://n8n.whitepine-tech.com/api/v1/executions?workflowId=h2DLuz8HuZKuLZhq&limit=5

# Manual execution
curl -X POST -H "X-N8N-API-KEY: {key}" https://n8n.whitepine-tech.com/api/v1/workflows/h2DLuz8HuZKuLZhq/execute
```

---

## References

- **Tech Stack Details:** [tech-stack.md](tech-stack.md)
- **Switch Node Fix:** [current-issue.md](current-issue.md)
- **Deployment Procedure:** [../SOPs/deployment-procedure.md](../SOPs/deployment-procedure.md)
- **Workflow Source:** [Daily Spiritual Email.json](../../Daily%20Spiritual%20Email.json)

# Tech Stack Documentation

**Last Updated:** October 26, 2025

## Platform & Infrastructure

### n8n Workflow Automation
- **Platform:** n8n (self-hosted)
- **Production Server:** https://n8n.whitepine-tech.com
- **Workflow URL:** https://n8n.whitepine-tech.com/workflow/h2DLuz8HuZKuLZhq
- **Workflow ID:** `h2DLuz8HuZKuLZhq`
- **Execution Schedule:** Daily at 4:00 AM EST

### Deployment Environment
- **Hosting:** AWS EC2 instance
- **Access Method:** n8n REST API (`/api/v1/workflows/`)
- **Version Control:** Git-tracked workflow JSON files in project directory

---

## n8n Node Inventory

### Core Nodes (12 total)

1. **Schedule Trigger** (`n8n-nodes-base.scheduleTrigger` v1.2)
   - Purpose: Triggers workflow daily at 4:00 AM EST
   - Configuration: Cron-style schedule with hour trigger

2. **Format Date** (`n8n-nodes-base.code` v2)
   - Purpose: JavaScript code node for date formatting
   - Transforms: Creates API-compatible date formats (MM-DD, MMDDYY)
   - Outputs: `today`, `dayOfWeek`, `monthDay`, `apiDateFormat`, `theme`, `usccbLink`

3. **Get Readings** (`n8n-nodes-base.httpRequest` v4.2)
   - Purpose: Fetches daily Catholic readings from GitHub Pages API
   - API: `https://cpbjr.github.io/catholic-readings-api/readings/2025/{date}.json`
   - Error Handling: `continueOnFail: true` (graceful degradation)

4. **Get Saint** (`n8n-nodes-base.httpRequest` v4.2)
   - Purpose: Fetches saint of the day data
   - API: `https://cpbjr.github.io/catholic-readings-api/saints/2025/{date}.json`
   - Error Handling: `continueOnFail: true` (returns 404 when no saint)

5. **Switch** (`n8n-nodes-base.switch` v3.2)
   - Purpose: Routes workflow based on saint data availability
   - **Critical Configuration:** `typeValidation: "loose"` (handles empty strings)
   - Output 1: "Saint Found" (when `$json.saint` exists)
   - Output 2: "No Saint" (when `$json.error` exists)

6. **Merge with Saint** (`n8n-nodes-base.code` v2)
   - Purpose: Combines readings + saint data for AI Agent
   - Sets: `hasSaint: true`

7. **Continue without Saint** (`n8n-nodes-base.code` v2)
   - Purpose: Prepares readings-only data for AI Agent
   - Sets: `hasSaint: false`, `saint: null`

8. **AI Agent** (`@n8n/n8n-nodes-langchain.agent` v2.2)
   - Purpose: Generates spiritual reflection content
   - Model: Grok-4 via OpenRouter
   - System Prompt: Structures JSON output with 10 fields
   - Token Budget: <1000 tokens per execution

9. **OpenRouter Chat Model** (`@n8n/n8n-nodes-langchain.lmChatOpenRouter` v1)
   - Purpose: LLM integration for AI Agent
   - Model: `x-ai/grok-4`
   - Credentials: OpenRouter API key

10. **Parse AI Output** (`n8n-nodes-base.code` v2)
    - Purpose: Parses and validates AI Agent JSON output
    - Error Handling: Try-catch with fallback values

11. **Build HTML** (`n8n-nodes-base.code` v2)
    - Purpose: Generates HTML email content
    - Template: Responsive design with liturgical calendar integration
    - Dynamic Sections: Handles saint/feast/solemnity types

12. **Send email** (`n8n-nodes-base.emailSend` v2.1)
    - Purpose: Delivers final email via Gmail SMTP
    - From: christopher.bisgaard@gmail.com
    - To: cpbjr@mac.com
    - Credentials: Google SMTP account

---

## External APIs & Data Sources

### Catholic Readings API (Primary Data Source)
- **Repository:** GitHub Pages static site
- **URL:** `https://cpbjr.github.io/catholic-readings-api/`
- **Source Code:** `catholic-readings-api/` directory (local)
- **Data Endpoints:**
  - Readings: `/readings/2025/{MM-DD}.json`
  - Saints: `/saints/2025/{MM-DD}.json`
- **Response Format:** JSON with readings, season, USCCB link
- **Error Handling:** 404 for missing saint data (expected behavior)

### OpenRouter (AI Model Provider)
- **Service:** OpenRouter API
- **Model:** Grok-4 (x-ai)
- **Purpose:** Spiritual content generation
- **Token Efficiency:** <1000 tokens per execution (vs 15k alternative)

### USCCB Readings (Reference Link)
- **URL Pattern:** `https://bible.usccb.org/bible/readings/{MMDDYY}.cfm`
- **Purpose:** User-facing link to official Catholic readings
- **Usage:** Included in email footer

---

## Architecture Patterns

### AI Agent + Code Tool Pattern
- **Innovation:** Uses Code nodes as "tools" for AI Agent
- **Token Savings:** 93% reduction vs loading liturgical data in prompt
- **Flow:** Code nodes fetch data → AI Agent generates content → Code nodes format output

### Error Handling Strategy
- **API Failures:** `continueOnFail: true` on HTTP Request nodes
- **Switch Node Logic:** Routes to appropriate path based on data availability
- **Fallback Behavior:** Sends readings-only email when saint data unavailable

### Data Flow
```
Schedule Trigger
  → Format Date (Code)
    → Get Readings (HTTP) [continueOnFail]
      → Get Saint (HTTP) [continueOnFail]
        → Switch (Routing)
          ├─ Saint Found → Merge with Saint (Code)
          └─ No Saint → Continue without Saint (Code)
            → AI Agent (LangChain)
              → OpenRouter Chat Model (Grok-4)
                → Parse AI Output (Code)
                  → Build HTML (Code)
                    → Send email (SMTP)
```

---

## Dependencies & Credentials

### Required n8n Credentials
- **Google SMTP account** (ID: `t5muBt48UrDocNe8`)
  - Used by: Send email node
  - Purpose: Gmail SMTP authentication

- **OpenRouter account** (ID: `YxHKEc4tsQR19kOm`)
  - Used by: OpenRouter Chat Model
  - Purpose: Grok-4 API access

### Environment Variables
- **N8N_API_KEY:** JWT token for AWS production server
  - Value: `eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...` (see README.md)
  - Usage: Workflow deployment via REST API

---

## Token Efficiency Notes

### Design Rationale
**Traditional Approach:** Load entire liturgical data in AI prompt
- Token Cost: ~15,000 tokens per execution
- Problem: Expensive, slow, unnecessary

**Current Architecture:** Code Tool pattern
- Token Cost: <1,000 tokens per execution
- Method: Code nodes fetch data, AI Agent only generates reflection
- Savings: 93% reduction in token usage

### Token Budget Breakdown
- **System Prompt:** ~500 tokens (spiritual director instructions)
- **User Prompt:** ~300 tokens (today's readings + theme)
- **AI Response:** ~200 tokens (JSON structured output)
- **Total:** <1,000 tokens per execution

---

## Known Issues & Solutions

### Switch Node Type Validation (RESOLVED October 26, 2025)
**Problem:** Switch node rejected empty strings with `typeValidation: "strict"`
**Solution:** Changed to `typeValidation: "loose"` on both conditions
**Lesson:** Always use loose validation when checking optional API fields

### MCP Partial Update Limitations (Documented)
**Problem:** n8n MCP fails silently on complex nested node updates
**Solution:** Use n8n REST API for full workflow updates
**Best Practice:** Manual UI edits for simple fixes, API for full deployments

---

## Performance Metrics

- **Execution Time:** ~8-12 seconds per run
- **Daily Schedule:** 4:00 AM EST (9:00 AM UTC)
- **Success Rate:** 100% since Switch node fix (October 26, 2025)
- **Email Delivery:** christopher.bisgaard@gmail.com → cpbjr@mac.com
- **Token Cost:** <$0.001 per execution (with Grok-4)

---

## References

- **Workflow Files:**
  - [Daily Spiritual Email - DEPLOY.json](../../Daily%20Spiritual%20Email%20-%20DEPLOY.json) (production-ready)
  - [Daily Spiritual Email - FIXED.json](../../Daily%20Spiritual%20Email%20-%20FIXED.json) (corrected Switch node)
  - [Daily Spiritual Email - CLEAN.json](../../Daily%20Spiritual%20Email%20-%20CLEAN.json) (template version)
- **API Source Code:** [catholic-readings-api/](../../catholic-readings-api/)
- **Production URL:** https://n8n.whitepine-tech.com/workflow/h2DLuz8HuZKuLZhq
- **Switch Node Fix Details:** [System/current-issue.md](current-issue.md)

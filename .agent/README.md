# Daily Summary Project Documentation

**Status**: ✅ IN PRODUCTION (Python)
**Priority**: 5 (Maintenance Only)
**Last Updated**: 2026-01-02

## Project Overview

Daily Catholic readings delivery with AI-generated spiritual content, migrated from n8n to a standalone Python application.

## Status

✅ **IN PRODUCTION (STABLE)** - Migrated to Python on 2026-01-01
🚀 **Active** - Running daily at 4:00 AM MDT (10:00 AM UTC)
📧 **Delivering** - Daily emails with readings, reflections, and historical events
🔧 **Server** - Hosted on Hetzner (whitepine)

## Tech Stack

- **Python 3.12** - Core application logic
- **AI Providers** - Dual support for xAI (Grok-4) and Google (Gemini)
- **SMTP** - Apple iCloud Mail (cpbjr@mac.com)
- **Deployment** - Cron-based execution on Ubuntu 24.04 (Hetzner)

## 📚 Universal n8n Patterns

For patterns that apply to ALL n8n projects, see workspace SOPs:
- [n8n-mcp-first-development.md](../../.agent/SOPs/n8n-mcp-first-development.md) - MCP-first methodology (MANDATORY)
- [n8n-switch-node-routing-patterns.md](../../.agent/SOPs/n8n-switch-node-routing-patterns.md) - Switch vs IF routing (includes Catholic API pattern)
- [n8n-workflow-modification-checklist.md](../../.agent/SOPs/n8n-workflow-modification-checklist.md) - How to modify workflows
- [n8n-debugging-strategies.md](../../.agent/SOPs/n8n-debugging-strategies.md) - Debugging techniques
- [n8n-deployment-checklist.md](../../.agent/SOPs/n8n-deployment-checklist.md) - Deployment procedures

**Project-specific SOPs** (unique to Daily Summary):
- [switch-node-configuration.md](SOPs/switch-node-configuration.md) - Catholic API error handling (consolidated into workspace SOP)
- [deployment-procedure.md](SOPs/deployment-procedure.md) - Spiritual email deployment

## Documentation Structure

### System/ (Current State)
- **[current-issue.md](System/current-issue.md)** - Switch node routing failure (RESOLVED)
- **[workflow-error-handling.md](System/workflow-error-handling.md)** - Error handling configuration (outdated, needs revision)

### SOPs/ (How-To Procedures)
- **[deployment-procedure.md](SOPs/deployment-procedure.md)** - Full deployment guide
- **[switch-node-configuration.md](SOPs/switch-node-configuration.md)** - Switch node operator reference

### Tasks/ (Active Work)
- **[current-sprint.md](Tasks/current-sprint.md)** - Project status and completed tasks

### Root Level (Project Directory)
- **[Daily Spiritual Email.json](../Daily%20Spiritual%20Email.json)** - Production workflow (all features deployed)
- **[PRD.md](../PRD.md)** - Complete project requirements and architecture
- **[catholic-readings-api/](../catholic-readings-api/)** - GitHub Pages API source code
- **[Documentation/Archive/](../Documentation/Archive/)** - Old workflow versions and task files

## Recent Changes

### October 27, 2025 - Historical Events Feature Deployed to Production ✅
**Action:** Implemented and deployed "Today in History" section
**Production Workflow:** `Daily Spiritual Email` on n8n.whitepine-tech.com
**Feature:** Three historical events per day (one required from 1972 or 1992, two from diverse years)
**Implementation:** AI-generated content using Grok-4's historical knowledge, no external APIs
**Token Impact:** ~350 token increase per execution (~1,150 total, still highly efficient)
**Status:** ✅ VERIFIED WORKING IN PRODUCTION
**Test Workflow Archived:** `Documentation/Archive/Daily Spiritual Email-historical-20251027.json`

### October 26, 2025 - Switch Node Fix Deployed ✅
**Action:** Deployed corrected workflow to production server
**Method:** Full workflow update via n8n API
**Root Cause:** `typeValidation: "strict"` rejected empty strings when checking `$json.saint`
**Solution:** Changed both Switch conditions to `typeValidation: "loose"`
**Verification:** Configuration confirmed deployed, email delivery verified
**Status:** ✅ RESOLVED - Workflow operating normally

### October 13, 2025 - Switch Node Routing Failure Discovered
**Issue:** Workflow stops at Switch node when saint data unavailable (404 response)
**Root Cause:** Switch checks `$json.saint.name` and `$json.data`, but API returns `$json.error`
**Status:** ✅ **RESOLVED**

See [System/current-issue.md](System/current-issue.md) for complete technical analysis and deployment details.

## Quick Reference

**Production Path:** `/home/deploy/daily-summary`
**Schedule:** 4:00 AM MDT (10:00 AM UTC) daily
**Email Delivery:** cpbjr@mac.com → cpbjr@mac.com
**API Endpoint:** https://cpbjr.github.io/catholic-readings-api/

## Notes

This project demonstrates token-efficient AI agent patterns and serves as a reference for graceful error handling in n8n workflows.

# Daily Summary Project Documentation

**Status**: Active (Production)
**Priority**: 5
**Last Updated**: 2025-10-13

## Project Overview

Daily Catholic readings delivery with AI-generated spiritual content.

## Status

⚠️ **BROKEN** - Switch node routing failure (discovered 2025-10-13)
🔧 **Fix Ready** - Awaiting implementation (2-minute UI fix)

## Tech Stack

- **n8n workflows** - AI Agent + Code Tool architecture
- **Token-efficient design** - <1000 tokens (vs 15k alternative)
- **Liturgical data** - GitHub Pages API for readings
- **Production Server** - AWS EC2 at http://3.16.157.215:5678

## Documentation Structure

### System/ (Current State)
- **[current-issue.md](System/current-issue.md)** - Switch node routing failure (ACTIVE ISSUE)
- **[workflow-error-handling.md](System/workflow-error-handling.md)** - Error handling configuration (outdated, needs revision)

### SOPs/ (How-To Procedures)
- *Empty* - To be populated with operational procedures

### Tasks/ (Active Work)
- **[START-HERE.md](Tasks/START-HERE.md)** - Current task: Fix Switch node (estimated 2 minutes)

### Root Level (Project Directory)
- **[Daily Spiritual Email - FIXED.json](../Daily%20Spiritual%20Email%20-%20FIXED.json)** - Corrected workflow ready to import
- **[PRD.md](../PRD.md)** - Complete project requirements and architecture
- **[catholic-readings-api/](../catholic-readings-api/)** - GitHub Pages API source code
- **[Documentation/Archive/](../Documentation/Archive/)** - Old workflow versions and task files

## Recent Changes

### October 13, 2025 - Switch Node Routing Failure Discovered
**Issue:** Workflow stops at Switch node when saint data unavailable (404 response)
**Root Cause:** Switch checks `$json.saint.name` and `$json.data`, but API returns `$json.error`
**Status:** Fix identified, awaiting implementation
**Next Action:** Apply 2-minute UI fix (see Tasks/START-HERE.md)

See [System/current-issue.md](System/current-issue.md) for complete technical analysis.

## Quick Reference

**Production Workflow ID:** `XOsIARJl8r1E7SWJ`
**Schedule:** 4:00 AM EST daily
**Email Delivery:** christopher.bisgaard@gmail.com → cpbjr@mac.com
**API Endpoint:** https://cpbjr.github.io/catholic-readings-api/

## Notes

This project demonstrates token-efficient AI agent patterns and serves as a reference for graceful error handling in n8n workflows.

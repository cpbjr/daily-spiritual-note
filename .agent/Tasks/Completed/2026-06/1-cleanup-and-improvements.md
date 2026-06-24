# Task 1 - Cleanup & Improvements ✅

**Completed**: 2026-06-24

## What Was Done

Surfaced the saint quote that was silently dropped every day, switched to Nous Research as the primary AI provider with xAI as automatic failover, updated the retired grok model, fixed scripture links to NRSV-CI (Catholic edition), and replaced AI-generated celebration URLs that frequently 404'd with deterministic Wikipedia links.

## Key Changes

- **Saint quote now appears** in every celebration email as a styled blockquote — verbatim from the API, not routed through the LLM
- **Nous `Hermes-4-405B` is primary**; xAI `grok-4.20-0309-reasoning` auto-fallback (replaces retired `grok-4-fast-reasoning`)
- **Scripture links** switched to NRSV-CI (YouVersion id 2015) — correctly covers deuterocanonical readings
- **Celebration link** is now a deterministic Wikipedia search URL; `celebrationLink` removed from AI output schema
- Unified OpenAI-compatible helper for both providers; JSON fence-stripping added for robust parsing

## Notes

Grokipedia was investigated as a preferred link source but returns 404 for all Catholic saint articles — Wikipedia search URL used instead (always resolves). Implementation plan archived from `.agent/Tasks/Implementation/2026-06-24-cleanup-and-improvements.md`.

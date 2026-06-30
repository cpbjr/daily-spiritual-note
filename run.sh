#!/bin/bash
# run.sh - Wrapper for Daily Summary Cron Job
set -euo pipefail

# Change to the project directory. This also makes .env discoverable by
# config.py, which loads it directly via pydantic-settings — so no manual
# `export` of .env is needed here (the old `export $(cat .env | xargs)` form
# broke on comments and quoted/space-containing values).
cd /home/deploy/daily-summary

# Run the script using the virtual environment's python
./venv/bin/python3 main.py "$@" >> cron.log 2>&1


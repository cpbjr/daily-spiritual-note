#!/bin/bash
# run.sh - Wrapper for Daily Summary Cron Job

# Change to the project directory
cd /home/cpbjr/WhitePineTech/Projects/Daily\ Summary

# Source environment variables if they are not picked up by cron
if [ -f .env ]; then
    export $(cat .env | xargs)
fi

# Run the script using the virtual environment's python
./venv/bin/python3 main.py "$@" >> cron.log 2>&1


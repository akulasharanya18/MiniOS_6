#!/bin/bash

set -e

echo "======================================"
echo "       MiniOS Automated Test"
echo "======================================"

printf "help\n" | ./minios

printf "system\n" | ./minios

printf "processes\n" | ./minios

printf "run echo Hello_from_MiniOS\n" | ./minios

printf "submit test_job\n" | ./minios

printf "workers\n" | ./minios

printf "race\n" | ./minios

printf "sync\n" | ./minios

printf "queue\n" | ./minios

echo "======================================"
echo "       All tests completed"
echo "======================================"

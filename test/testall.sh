#!/usr/bin/env bash


: "${PRERUN:=}"

# Run each test executable and exit with a non-zero status as soon as one fails.
while IFS= read -r file; do
    echo "Running $file..."
    if ! $PRERUN "$file"; then
        echo "-----------------------"
        exit 1
    fi
    echo "-----------------------"
done < <(find . -type f -executable -name 'test_*')

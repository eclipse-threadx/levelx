#!/bin/bash
# Copyright (c) 2026 Eclipse ThreadX contributors
# SPDX-License-Identifier: MIT

set -euo pipefail
cd "$(dirname "$0")"

# Verify rejection of damaged inputs using a completed coverage run.
base=coverage_report/per_configuration/default_build_coverage
backup=$(mktemp -d)
cp "$base.json" "$backup/input.json"
cp "$base.xml" "$backup/input.xml"
# Restore the original inputs on success or failure.
restore() {
    cp "$backup/input.json" "$base.json"
    cp "$backup/input.xml" "$base.xml"
    rm -r "$backup"
}
trap restore EXIT

# Require the collector to reject each damaged input.
expect_failure() {
    if ./coverage.sh --merge > "$backup/output.log" 2>&1; then
        echo "Coverage accepted $1." >&2
        exit 1
    fi
    echo "Rejected $1."
}

rm "$base.json"
expect_failure 'a missing JSON input'
: > "$base.json"
expect_failure 'an empty JSON input'
printf '{"files":[]}\n' > "$base.json"
expect_failure 'an unmeasured JSON input'
cp "$backup/input.json" "$base.json"
printf '<coverage lines-valid="0"/>\n' > "$base.xml"
expect_failure 'an unmeasured XML input'
cp "$backup/input.xml" "$base.xml"
./coverage.sh --merge

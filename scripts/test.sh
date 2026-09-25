#!/bin/bash
##############################################################################
# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT
##############################################################################

set -euo pipefail

test_dir="$(dirname "$(realpath "$0")")/../test/cmake"
"$test_dir/run.sh" test all
if [ "${TX_COVERAGE:-OFF}" = ON ]; then
    "$test_dir/check_coverage.sh"
fi

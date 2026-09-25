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

cd "$(dirname "$0")"
for dependency in threadx filex; do
    revision=$(cat "$dependency-revision.txt")
    repository="https://github.com/eclipse-threadx/$dependency.git"
    if [ ! -e "$dependency" ]; then
        git init -q "$dependency"
        git -C "$dependency" remote add origin "$repository"
        timeout 180 git -C "$dependency" fetch --depth 1 origin "$revision"
        git -C "$dependency" checkout -q --detach FETCH_HEAD
    fi
    if [ "$(git -C "$dependency" rev-parse HEAD)" != "$revision" ] ||
       [ -n "$(git -C "$dependency" status --porcelain --untracked-files=no)" ]; then
        echo "$dependency checkout does not match its pinned revision." >&2
        exit 1
    fi
done

bootstrap=threadx/scripts/cmake_bootstrap.sh
if [ ! -f "$bootstrap" ]; then
    echo "ThreadX bootstrap script is missing." >&2
    exit 1
fi

if [ "${1:-}" = test ] && [ "${2:-}" = all ] && [ "${TX_COVERAGE:-OFF}" = ON ]; then
    ./coverage.sh --clean
fi

ln -sfn "$bootstrap" .run.sh
exec ./.run.sh "$@"

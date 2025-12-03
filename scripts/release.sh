#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_ZIP="$SCRIPT_DIR/../build/dfu_application.zip"

tag=$(git describe --tags --abbrev=0 2>/dev/null || echo "")

# Ensure GitHub CLI is available
if ! command -v gh >/dev/null 2>&1; then
  echo "Error: GitHub CLI 'gh' is not installed or not in PATH." >&2
  exit 1
fi

# Guard: ensure build artifact exists
if [[ ! -f "$BUILD_ZIP" ]]; then
  echo "Error: build artifact not found at $BUILD_ZIP" >&2
  exit 1
fi

# Guard: ensure a tag is available
if [[ -z "$tag" ]]; then
  echo "Error: no tag found. Create/push a tag first." >&2
  exit 1
fi

gh release create "$tag" --generate-notes "$BUILD_ZIP#dfu_application_$tag"

#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
VERSION_FILE="$SCRIPT_DIR/../VERSION"

part=${1:-patch}

case "$part" in
  major|minor|patch|tweak) ;;
  *)
    echo "Usage: $(basename "$0") <major|minor|patch|tweak>" >&2
    exit 1
    ;;
esac

read_val() {
  local key="$1"
  sed -n "s/^${key}[[:space:]]*=[[:space:]]*//p" "$VERSION_FILE"
}

major=$(read_val "VERSION_MAJOR")
minor=$(read_val "VERSION_MINOR")
patch=$(read_val "PATCHLEVEL")
tweak=$(read_val "VERSION_TWEAK")
extraversion=$(read_val "EXTRAVERSION")

major=${major:-0}
minor=${minor:-0}
patch=${patch:-0}
tweak=${tweak:-0}
extraversion=${extraversion:-}

case "$part" in
  major)
    major=$((major + 1))
    minor=0
    patch=0
    tweak=0
    ;;
  minor)
    minor=$((minor + 1))
    patch=0
    tweak=0
    ;;
  patch)
    patch=$((patch + 1))
    tweak=0
    ;;
  tweak)
    tweak=$((tweak + 1))
    ;;
esac

cat > "$VERSION_FILE" <<EOF
VERSION_MAJOR = $major
VERSION_MINOR = $minor
PATCHLEVEL = $patch
VERSION_TWEAK = $tweak
EXTRAVERSION = ${extraversion}
EOF

echo "Bumped version: ${major}.${minor}.${patch}.${tweak}${extraversion:+-$extraversion}"

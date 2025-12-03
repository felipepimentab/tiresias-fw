#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
VERSION_FILE="$SCRIPT_DIR/../VERSION"
BUILD_ZIP="$SCRIPT_DIR/../build/dfu_application.zip"

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

tag="v${major}.${minor}.${patch}"
if [[ "$tweak" =~ ^[0-9]+$ ]] && [ "$tweak" -gt 0 ]; then
  tag="${tag}.${tweak}"
fi
if [[ -n "$extraversion" ]]; then
  tag="${tag}-${extraversion}"
fi

if ! git rev-parse --git-dir >/dev/null 2>&1; then
  echo "Error: not a git repository" >&2
  exit 1
fi

# Stage VERSION and commit bump (do not include build artifacts)
git add "$VERSION_FILE"
if ! git diff --cached --quiet -- "$VERSION_FILE"; then
  git commit -m "Bump version to $tag"
else
  echo "No changes in VERSION to commit; continuing to tag."
fi

if git rev-parse "refs/tags/$tag" >/dev/null 2>&1; then
  echo "Tag '$tag' already exists." >&2
  exit 1
fi

git tag -a "$tag" -m "Release $tag"
echo "Created tag: $tag"

# Determine remote to push to (prefer 'origin')
remote="origin"
if ! git remote | grep -qx "origin"; then
  remote=$(git remote | head -n1 || true)
fi
if [[ -z "$remote" ]]; then
  echo "Error: no git remote configured; cannot push." >&2
  exit 1
fi

# Push commit and tag
echo "Pushing commit to '$remote'..."
git push "$remote" HEAD
echo "Pushing tag '$tag' to '$remote'..."
git push "$remote" "$tag"
echo "Pushed commit and tag to '$remote'"

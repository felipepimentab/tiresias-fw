#!/bin/bash

# Script to bump version numbers in the project
# Usage: ./scripts/bump_version.sh <major|minor|patch>

set -e

if [ $# -ne 1 ]; then
  echo "Usage: $0 <major|minor|patch>"
  exit 1
fi

TYPE=$1
VERSION_FILE="src/version.h"

# Extract current version
MAJOR=$(grep "TIRESIAS_VERSION_MAJOR" "$VERSION_FILE" | grep -o '[0-9]\+')
MINOR=$(grep "TIRESIAS_VERSION_MINOR" "$VERSION_FILE" | grep -o '[0-9]\+')
PATCH=$(grep "TIRESIAS_VERSION_PATCH" "$VERSION_FILE" | grep -o '[0-9]\+')

# Calculate new version
case $TYPE in
  major)
    NEW_MAJOR=$((MAJOR + 1))
    NEW_MINOR=0
    NEW_PATCH=0
    ;;
  minor)
    NEW_MAJOR=$MAJOR
    NEW_MINOR=$((MINOR + 1))
    NEW_PATCH=0
    ;;
  patch)
    NEW_MAJOR=$MAJOR
    NEW_MINOR=$MINOR
    NEW_PATCH=$((PATCH + 1))
    ;;
  *)
    echo "Invalid version type. Use 'major', 'minor', or 'patch'."
    exit 1
    ;;
esac

NEW_VERSION="$NEW_MAJOR.$NEW_MINOR.$NEW_PATCH"
CURRENT_VERSION="$MAJOR.$MINOR.$PATCH"

echo "Bumping version from $CURRENT_VERSION to $NEW_VERSION"

# Update version.h
sed -i "" "s/#define TIRESIAS_VERSION_MAJOR $MAJOR/#define TIRESIAS_VERSION_MAJOR $NEW_MAJOR/" "$VERSION_FILE"
sed -i "" "s/#define TIRESIAS_VERSION_MINOR $MINOR/#define TIRESIAS_VERSION_MINOR $NEW_MINOR/" "$VERSION_FILE"
sed -i "" "s/#define TIRESIAS_VERSION_PATCH $PATCH/#define TIRESIAS_VERSION_PATCH $NEW_PATCH/" "$VERSION_FILE"
sed -i "" "s/#define TIRESIAS_VERSION_STRING \"$CURRENT_VERSION\"/#define TIRESIAS_VERSION_STRING \"$NEW_VERSION\"/" "$VERSION_FILE"

# Update version badge in README.md
sed -i "" "s/Version-[0-9]\+\.[0-9]\+\.[0-9]\+-blue/Version-$NEW_VERSION-blue/" "README.md"

# Prepare CHANGELOG.md
DATE=$(date +%Y-%m-%d)
if ! grep -q "\[Unreleased\]" "CHANGELOG.md"; then
  echo "Error: CHANGELOG.md does not contain an [Unreleased] section."
  exit 1
fi

# Update CHANGELOG.md - replace [Unreleased] with the new version
# macOS sed requires a different approach for inserting lines
awk -v ver="## [$NEW_VERSION] - $DATE" '/## \[Unreleased\]/{print; print ""; print ver; next}1' "CHANGELOG.md" > "CHANGELOG.md.tmp" && mv "CHANGELOG.md.tmp" "CHANGELOG.md"

echo "Version bumped to $NEW_VERSION"
echo "Please review the changes and update the CHANGELOG.md with the appropriate details."
echo "Then commit with: git commit -am 'Bump version to $NEW_VERSION'"
#!/bin/bash

# Script to push a version commit and tag to the remote repository
# Usage: ./scripts/release.sh <version>

set -e

if [ $# -ne 1 ]; then
  echo "Usage: $0 <version>"
  echo "Example: $0 0.1.0"
  exit 1
fi

VERSION=$1

# Validate version format
if ! [[ $VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "Error: Version must be in the format X.Y.Z (e.g., 0.1.0)"
  exit 1
fi

# Check if we're in the project root directory
if [ ! -f "src/version.h" ]; then
  echo "Error: This script must be run from the project root directory"
  exit 1
fi

# Check if the version in version.h matches the provided version
VERSION_IN_FILE=$(grep "TIRESIAS_VERSION_STRING" "src/version.h" | grep -o '"[0-9]\+\.[0-9]\+\.[0-9]\+"' | tr -d '"')
if [ "$VERSION_IN_FILE" != "$VERSION" ]; then
  echo "Error: Version mismatch. The version in src/version.h is $VERSION_IN_FILE, but you specified $VERSION"
  echo "Please run ./scripts/bump_version.sh first or provide the correct version"
  exit 1
fi

# Check if there are uncommitted changes
if ! git diff --quiet HEAD; then
  echo "Error: There are uncommitted changes in the repository"
  echo "Please commit all changes before releasing"
  exit 1
fi

# Check if the commit message contains the version
LAST_COMMIT_MSG=$(git log -1 --pretty=%B)
if ! echo "$LAST_COMMIT_MSG" | grep -q "Bump version to $VERSION"; then
  echo "Warning: The last commit message does not contain 'Bump version to $VERSION'"
  echo "It is recommended to use the commit message 'Bump version to $VERSION'"
  read -p "Do you want to continue anyway? (y/n) " -n 1 -r
  echo
  if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    exit 1
  fi
fi

# Create and push the tag
echo "Creating tag v$VERSION..."
git tag -a "v$VERSION" -m "Release version $VERSION"

# Push the commit and tag to the remote repository
echo "Pushing commit and tag to remote repository..."
git push origin main
git push origin "v$VERSION"

echo "\nRelease v$VERSION has been successfully pushed to the remote repository"
echo "The GitHub Actions workflow will automatically create a GitHub release"
echo "and attach the compiled firmware binaries"
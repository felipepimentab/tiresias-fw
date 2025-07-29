# Versioning Guide

## Overview

The Tiresias Firmware project follows [Semantic Versioning 2.0.0](https://semver.org/) with the format `MAJOR.MINOR.PATCH`:

- **MAJOR**: Incremented for incompatible API changes
- **MINOR**: Incremented for backward-compatible functionality additions
- **PATCH**: Incremented for backward-compatible bug fixes

For more details on our versioning and release process, see [VERSIONING.md](../VERSIONING.md) in the project root.

## Using the Version Bumping Script

To simplify the version bumping process, we provide a script that automatically updates version numbers in the codebase.

### Prerequisites

- Bash shell
- Git

### Usage

```bash
./scripts/bump_version.sh <major|minor|patch>
```

For example, to bump the patch version:

```bash
./scripts/bump_version.sh patch
```

### What the Script Does

1. Updates the version numbers in `src/version.h`
2. Updates the version badge in `README.md`
3. Prepares the `CHANGELOG.md` by adding a new version section with the current date

### After Running the Script

After running the script, you should:

1. Review the changes made to the files
2. Update the `CHANGELOG.md` with detailed information about the changes in this version
3. Commit the changes with the message "Bump version to X.Y.Z"
4. Follow the remaining steps in the release process as described in [VERSIONING.md](../VERSIONING.md)

## Continuous Integration

The project uses GitHub Actions for continuous integration. The CI workflow runs on every push to the main branch and on pull requests, performing the following checks:

- Builds the firmware for the nRF5340 DK
- Runs static code analysis

All CI checks must pass before a release can be created.

## Automated Release Process

When you push a tag with the format `vX.Y.Z` to the repository, a GitHub Actions workflow automatically:

1. Builds the firmware
2. Creates a GitHub release
3. Attaches the compiled firmware binaries
4. Adds the release notes from `CHANGELOG.md`

## Pull Requests

When submitting a pull request, please:

1. Indicate the type of change (MAJOR, MINOR, PATCH) in the pull request template
2. Update the `CHANGELOG.md` file with your changes under the [Unreleased] section
3. Ensure all CI checks pass

## Hotfixes

For critical bug fixes that need to be applied to a released version, follow the hotfix process described in [VERSIONING.md](../VERSIONING.md).
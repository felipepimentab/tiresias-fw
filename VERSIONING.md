# Versioning and Release Process

This document outlines the versioning and release process for the Tiresias Firmware project.

## Semantic Versioning

This project follows [Semantic Versioning 2.0.0](https://semver.org/) with the format `MAJOR.MINOR.PATCH`:

- **MAJOR**: Incremented for incompatible API changes
- **MINOR**: Incremented for backward-compatible functionality additions
- **PATCH**: Incremented for backward-compatible bug fixes

Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.

## Version File

The version information is maintained in `src/version.h`. This file defines:

- `TIRESIAS_VERSION_MAJOR`: The major version number
- `TIRESIAS_VERSION_MINOR`: The minor version number
- `TIRESIAS_VERSION_PATCH`: The patch version number
- `TIRESIAS_VERSION_STRING`: The complete version string

## Release Process

### 1. Preparing a Release

1. Update the version numbers in `src/version.h`
2. Update the `CHANGELOG.md` with details of the changes
3. Commit these changes with the message "Bump version to X.Y.Z"
4. Ensure all CI checks pass on the main branch

### 2. Creating a Release

1. Create a Git tag for the new version:
   ```bash
   git tag -a vX.Y.Z -m "Release version X.Y.Z"
   ```

2. Push the tag to the remote repository:
   ```bash
   git push origin vX.Y.Z
   ```

3. The GitHub Actions workflow will automatically:
   - Build the firmware
   - Create a GitHub release
   - Attach the compiled firmware binaries
   - Add the release notes from CHANGELOG.md

The release workflow is defined in `.github/workflows/release.yml`.

### 3. Post-Release

1. Update the version in `src/version.h` to the next development version (e.g., increment the patch version and add "-dev")
2. Add a new "Unreleased" section to the CHANGELOG.md
3. Commit these changes with the message "Begin development on next version"

## Release Checklist

Before creating a release, ensure:

- [ ] All tests pass
- [ ] Documentation is updated
- [ ] Version numbers are updated in `src/version.h`
- [ ] CHANGELOG.md is updated with all notable changes
- [ ] All changes are committed and pushed to the main branch
- [ ] The code builds successfully
- [ ] All CI checks pass

## Continuous Integration

The project uses GitHub Actions for continuous integration. The CI workflow is defined in `.github/workflows/ci.yml` and runs on every push to the main branch and on pull requests. It performs the following checks:

- Builds the firmware for the nRF5340 DK
- Runs static code analysis

All CI checks must pass before a release can be created.

## Hotfix Process

For critical bug fixes that need to be applied to a released version:

1. Create a branch from the release tag: `git checkout -b hotfix/X.Y.Z vX.Y.Z`
2. Fix the issue and commit the changes
3. Update the version in `src/version.h` (increment the PATCH version)
4. Update the CHANGELOG.md
5. Commit these changes
6. Tag the new version: `git tag -a vX.Y.(Z+1) -m "Hotfix release X.Y.(Z+1)"`
7. Push the tag: `git push origin vX.Y.(Z+1)`
8. Create a GitHub release for the new tag
9. Merge the hotfix back to the main branch if applicable
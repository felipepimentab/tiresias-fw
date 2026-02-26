# Versioning

This project uses [Zephyr's application version management system](https://docs.zephyrproject.org/latest/build/version/index.html) as a base alongside GitHub releases to share the compiled application binary used for DFU over BLE.

The application's version is stored in the `VERSION` file on the repository root. It is composed of:

- VERSION_MAJOR
- VERSION_MINOR
- PATCHLEVEL
- VERSION_TWEAK
- EXTRAVERSION

## Tags and Releases

Tags and release names are derived from these fields as:

- vMAJOR.MINOR.PATCH[.TWEAK][-EXTRAVERSION]

Examples:

- `0.4.4`           → tag `v0.4.4`
- `1.2.0.3`         → tag `v1.2.0.3`
- `2.0.1-alpha`     → tag `v2.0.1-alpha`

## Files and Scripts

- Version data: `VERSION`
- Bump version: `scripts/version.sh`
- Create and push tag: `scripts/tag.sh`
- Create GitHub release and upload DFU zip: `scripts/release.sh`

## Release workflow

> [!WARNING]
> Before making any changes to the version ensure:
>
> - The application builds with no errors.
> - The application was tested and works as expected.
> - The application was built for the `TIRESIAS DK nRF5340 application MCU` target.
> - The repository for the board files is up to date.
> - The build artifact was generated as `build/dfu_application.zip`.

### Update the version

Bump the desired part (major, minor, patch, or tweak):

```bash
sh scripts/version.sh patch
```

This updates the `VERSION` file, resetting lower-order components as appropriate and keeping `EXTRAVERSION` unchanged.

After bumping the version perform the build again so that the version string on the firmware are properly updated.

### Create and Push a Tag

Create an annotated tag from the current `VERSION` contents and push it (also commits the `VERSION` change if needed):

```bash
sh scripts/tag.sh
```

This creates a tag named `vMAJOR.MINOR.PATCH[.TWEAK][-EXTRAVERSION]` and pushes both the commit (if any) and the tag to the remote.

### Publish a GitHub Release (Upload DFU Zip)

With the tag present and `build/dfu_application.zip` available, create a GitHub release and attach the DFU file:

Requirements:

- GitHub CLI (`gh`) installed and authenticated (`gh auth login`)
- Remote pointing at the GitHub repository

Run:

```bash
sh scripts/release.sh
```

This creates a release for the latest tag and uploads `build/dfu_application.zip` as an asset named `dfu_application_<tag>`.

## Notes

- `EXTRAVERSION` can be edited directly in `VERSION` (e.g., `alpha`, `beta`, `rc1`). It is appended to the tag as a suffix.
- `VERSION_TWEAK` is an additional numeric component for fine-grained increments beyond PATCH when needed.
- Keep `VERSION` under version control; build artifacts do not need to be committed for the release step since `release.sh` reads the local file when invoked.

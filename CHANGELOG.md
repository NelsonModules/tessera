# Changelog

All notable changes to the `tessera` module are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-10-14

### Added

- Initial release: a native, dependency-free procedural-generation toolbox
  exposed under the `tessera.` namespace.
- Single-thread kernel tuning (pointer-walked reaction-diffusion stencil,
  single-`floor` gradient-noise sampling) for a WebAssembly-friendly build.

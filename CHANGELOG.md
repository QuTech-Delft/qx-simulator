# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [ 0.5.3 ] - [ 2023-01-02 ]

### Added

- Python 3.11 support

### Changed

-

### Removed

-

## [ 0.5.2 ] - [ 2022-12-7 ]

### Added

-

### Changed

- Fixed version number incorrectly set in previous release

### Removed

-

## [ 0.5.1 ] - [ 2022-12-6 ]

### Added

- Readthedocs documentation

### Changed

- Fixed Python versions on Linux
- Updated README with new API functions
- Tidy tests and circuits folder

### Removed

- Python 3.6 support

## [ 0.5.0 ] - [ 2022-11-15 ]

### Added

- "-j <filename>" CLI option to output JSON to a file
- Possible to use the simulator without OpenMP.

### Changed

- Measure_all gate added; does no longer measure each qubit separately.
- Move to C++11: smart pointers, etc.
- Format of output has been changed. You no longer need to add a "display" gate to
output the quantum state at the end of the circuit.
- Now uses the new Libqasm API instead of the old one.
- Output complex amplitudes when not averaging measurement register.
- Do not add automatically a measure_all when doing measurement averaging;
average the measurement register only.

### Removed

- Most preprocessor macros. E.g. for gate creation.
- A lot of old and useless code was removed. A lot of dead code removed.

### Fixed

- Close input file when done.
- Code style has been completely fixed automatically with clang-format.
- Headers no longer include .cc file.
- Moved implementations to .cc files instead of headers.

## [ 0.4.2 ] - [ 2021-06-01 ]

### Added

-

### Changed

-

### Removed

-

### Fixed

- Wheels no longer require exotic CPU extensions to work

## [ 0.4.1 ] - [ 2021-05-20 ]

### Added

- Continuous integration for as far as automated tests exist
- Automated release build and distribution logic for PyPI
- Installation logic in build system
- qxelarator wheels include qx-simulator binary (and friends)

### Changed

-

### Removed

-

### Fixed

- Build on MacOS
- Build on Windows using MSVC
- Python build process on various platforms

## [ 0.4.0 ] - [ 2021-05-20 ]

### Added

-

### Changed

- Replaced XPU-based threading with OpenMP

### Removed

-

### Fixed

- Various bugs and inefficiencies related to multithreading

## [ 0.3.0 ] - [ 2019-05-01 ]

### Added

- qxelerator to support cqasm v1.0
- setup instructions in README.md

### Changed

-

### Removed

-

### Fixed

-

## [ 0.2.5 ] - [ 2018-12-21 ]

### Added

-

### Changed

-

### Removed

-

### Fixed

- Fixed issue with libqasm submodule, this release is now with the correct version

## [ 0.2.4 ] - [ 2018-12-14 ]

### Added

-

### Changed

-

### Removed

- Removed -u 0 in Jenkinsfile again

### Fixed

- Single qubit operations now accept integers as arguments, previously it required floats

## [ 0.2.3 ] - [ 2018-11-05 ]

### Added

-

### Changed

- Updated the libqasm to get the hotfix

### Removed

-

### Fixed

- Fixed pthread mutex locking bug in XPU worker.

## [ 0.2.2 ] - [ 2018-09-25 ]

### Added

-

### Changed

-

### Removed

-

### Fixed

- Fixed bug in measurement and preparation implementation for large number of qubits.
- Fixed verbose GNU Compiler warnings under Linux.

## [ 0.2.1 ] - [ 2018-09-04 ]

### Added

- Enabled quantum noise simulation using the Depolarizing Error Model
- Added multishot simulation mode to save simulation time on the Quantum Inspire web platform.
- Enabled Identity gate support

### Changed

-

### Removed

-

### Fixed

-

## [ 0.2.0 ] - [ 2018-08-23 ]

### Added

- Initial cQASM 1.0 support.
- Integrated with libqasm 0.1.1 release.
- Added support for new quantum gates:
  - Flexible controlled phase shift (CR and CRK)
  - Measurement and Preparation in X/Y/Z basis: added measure_x, measure_y, prep_x and prep_y.
- Added support for Single Gate Multiple Qubits (SGMQ).
- Added a new set of unit tests.

### Changed

- Re-implemented classical NOT gate for full binary-controlled gate support.

### Removed

-

### Fixed

- Fixed overflow bug when simulating more than 31 qubits.
- Fixed bug in qubit register measurement ("measure_all" instruction).

# Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [ 0.7.0 ] - [ 2024-04-17 ]

### Added

- Integrate with libqasm 0.6.3 release.

### Changed

- Rename `tests` folder to `test`.
- Update GitHub workflows.

### Removed

- cQasm 1.x support.
- `circuits` folder.


## [ 0.6.5 ] - [ 2023-11-13 ]

### Added

- Python 3.12 support.

### Changed

- Uses Conan.
- New GitHub workflows.
- Working ARM runners in the CI.

### Removed

- 

## [ 0.6.4 ] - [ 2023-09-21 ]

### Added

- Meaningful error message when parsing or simulation fails.

### Changed

- Does not crash when too many qubits asked.

### Removed

-

## [ 0.6.3 ] - [ 2023-09-19 ]

### Added

- Address sanitizer CMake mode.

### Changed

- Removed submodules and `deps` folder, replaced with CMake FetchContent.
- C++23.
- gtest instead of doctest.
- Various fixes to GitHub actions.
- Fixed bug in bitset due to bool automatic casting to 32 bits.
- Documentation formatting fixes.
- Doc chapter about internals.

### Removed

-

## [ 0.6.2 ] - [ 2023-02-21 ]

### Added

- `depolarizing_channel` error model and docs.
- Ability to pass constant seed for deterministic simulation.
- `RandomTest` doing Kolmogorov-Smirnov statistical test to match random numbers with their ideal probability distribution.

### Changed

- Fixed docs.
- Source files use .hpp/.cpp extensions.
- Random number distributions (integer min/max and real 0-1) are done in-house for portability and consistency of results.

### Removed

- 

## [ 0.6.1 ] - [ 2023-02-01 ]

### Added

- Linux arm64 Python wheel and Linux arm64 test workflow (GCC-Release).
- `classical not` gate is back.
- `state` section in JSON now contains a new entry `norm` which is the square of the modulus of the complex number.
- C++ integration test.
- All gates can be controlled (including `prep`, `measure`). This is not new but was accidentally removed in 0.6.0.
- New Python API `execute_string` and `execute_file` without having to create `qxelarator.QX()`
  and returning user-friendly Python type.

### Changed

- Fixed README.
- `results` section in JSON contains integers instead of floats (those integers being the number of occurrences).
- In relation to previous point, `results` only relates to measurement register averaging,
  and no longer to quantum state when doing a single shot.
  For that, use the "state" section in the JSON.
- Output float precision is 8 decimals instead of 6.

### Removed

- Most Python tests. Already covered in C++ integration test, and they didn't check much anyway.

## [ 0.6.0 ] - [ 2023-01-19 ]

Almost a complete rewrite. No functional change except it's faster.

### Added

- Ability to easily implement any quantum gate by simply giving an arbitrary unitary matrix in `Gates.h`.
- Performance speedup using a single thread.
- Some proper linear algebra primitives using template metaprogramming.
- `abseil-cpp` dependency for `flat_hash_map`.
- Ability to compile the simulator with a maximum number of qubits.
  In the future this will be using templates as well to switch data structures based on the input quantum circuit.

### Changed

- Quantum state is stored as a sparse array implemented using a `flat_hash_map` from `abseil-cpp`.
- C++20 in place of C++14.
- Code style: camel case.

### Removed

- OpenMP and multithreading.
- SSE, AVX and other intrinsics.
- `get_measurement_outcome` and `get_state` methods. Now accessible via JSON output.

## [ 0.5.5 ] - [ 2023-01-12 ]

### Added

- `set_string` method to parse a cQASM string directly instead of a cQASM file.

### Changed

- Fixed issue #118: quantum state is always displayed, as well as measurement register averaging.

### Removed
- 

## [ 0.5.4 ] - [ 2023-01-06 ]

### Added

- Tests for `measure` gate.

### Changed

- Fixed issues #91 and #114.
- As part of #114, switch to sequential/simple `measure` gate.

### Removed

- QFT implementation (never instantiated, so no functional change).
- qx-server no longer exists, together with the network libraries.
- "Microcode" generation functions (never used either).
- Parallel gates in `gate.h` (they were executed sequentially).
- Some non-needed and/or obsolete code.

## [ 0.5.3 ] - [ 2023-01-02 ]

### Added

- Python 3.11 support.

### Changed

-

### Removed

-

## [ 0.5.2 ] - [ 2022-12-7 ]

### Added

-

### Changed

- Fixed version number incorrectly set in previous release.

### Removed

-

## [ 0.5.1 ] - [ 2022-12-6 ]

### Added

- ReadTheDocs documentation.

### Changed

- Fixed Python versions on Linux.
- Updated README with new API functions.
- Tidy tests and circuits folder.

### Removed

- Python 3.6 support.

## [ 0.5.0 ] - [ 2022-11-15 ]

### Added

- `-j <filename>` CLI option to output JSON to a file.
- Possible to use the simulator without OpenMP.

### Changed

- `measure_all` gate added. Does no longer measure each qubit separately.
- Move to C++11: smart pointers, etc.
- Format of output has been changed.
  You no longer need to add a `display` gate to output the quantum state at the end of the circuit.
- Now uses the new libqasm API instead of the old one.
- Output complex amplitudes when not averaging measurement register.
- Do not add automatically a measure_all when doing measurement averaging.
  Average the measurement register only.

### Removed

- Most preprocessor macros. E.g., for gate creation.
- A lot of old and useless code was removed. A lot of dead code removed.

### Fixed

- Close input file when done.
- Code style has been completely fixed automatically with clang-format.
- Headers no longer include `.cc` file.
- Moved implementations to `.cc` files instead of headers.

## [ 0.4.2 ] - [ 2021-06-01 ]

### Added

-

### Changed

-

### Removed

-

### Fixed

- Wheels no longer require exotic CPU extensions to work.

## [ 0.4.1 ] - [ 2021-05-20 ]

### Added

- Continuous integration for as far as automated tests exist.
- Automated release build and distribution logic for PyPI.
- Installation logic in build system.
- qxelarator wheels include qx-simulator binary (and friends).

### Changed

-

### Removed

-

### Fixed

- Build on MacOS.
- Build on Windows using MSVC.
- Python build process on various platforms.

## [ 0.4.0 ] - [ 2021-05-20 ]

### Added

-

### Changed

- Replaced XPU-based threading with OpenMP.

### Removed

-

### Fixed

- Various bugs and inefficiencies related to multithreading.

## [ 0.3.0 ] - [ 2019-05-01 ]

### Added

- qxelerator to support cqasm v1.0.
- setup instructions in README.md.

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

- Fixed issue with libqasm submodule, this release is now with the correct version.

## [ 0.2.4 ] - [ 2018-12-14 ]

### Added

-

### Changed

-

### Removed

- Removed `-u 0` in Jenkins file again.

### Fixed

- Single qubit operations now accept integers as arguments, previously it required floats.

## [ 0.2.3 ] - [ 2018-11-05 ]

### Added

-

### Changed

- Updated the libqasm to get the hotfix.

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

- Enabled quantum noise simulation using the Depolarizing Error Model.
- Added multi-shot simulation mode to save simulation time on the Quantum Inspire web platform.
- Enabled Identity gate support.

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
  - Flexible controlled phase shift (`CR` and `CRK`)
  - Measurement and Preparation in X/Y/Z basis: added `measure_x`, `measure_y`, `prep_x` and `prep_y`.
- Added support for Single Gate Multiple Qubits (SGMQ).
- Added a new set of unit tests.

### Changed

- Re-implemented classical NOT gate for full binary-controlled gate support.

### Removed

-

### Fixed

- Fixed overflow bug when simulating more than 31 qubits.
- Fixed bug in qubit register measurement ("measure_all" instruction).

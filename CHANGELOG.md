# Changelog

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

### Types of changes:
- **Added** for new features.
- **Changed** for changes in existing functionality.
- **Fixed** for any bug fixes.
- **Removed** for now removed features.


## [ 0.7.5 ] - [ xxxx-yy-zz ]

### Added
- Documentation: GitHub Actions `doc` workflow.


## [ 0.7.4 ] - [ 2025-01-29 ]

### Added
- Integrate with libqasm 0.6.9 release:
  - Add `SWAP` gate instruction.
  - Add `barrier`, `wait`, and `ìnit` non-gate instructions.
- The register manager now holds a 'dirty bitset' for virtual qubit and bit indices.


## [ 0.7.3 ] - [ 2025-01-27 ]

### Added
- Integrate with libqasm 0.6.8 release:
  - Add gate modifiers. Notice though that `pow` only works with integer exponents.
- Documentation: GitHub pages.
- Linters: `.clang-format` and `.clang-tidy`.

### Changed
- Implement instructions as a hierarchy.
- Implement `GateConvertor` as a `CircuitBuilder`.
- Implement `BasisVector` as a `boost::dynamic_bitset<uint32_t>`.
- Implement `RegisterManager` as a singleton.
- Change `QuantumState` by taking the measurement register out of it.
- Change `Circuit::execute` to return a `SimulationIterationContext`.
- Change `Matrix` and `DenseUnitaryMatrix` to use `std::vector` instead of `std::array`.
- Change file, functions, and variable names.

### Removed
- `abseil` library dependency.


## [ 0.7.2 ] - [ 2024-11-20 ]

### Added
- Integrate with libqasm 0.6.7 release:
  - Add `reset` instruction.
  - Make QX simulator aware of bit (register) variables. 

### Changed
- Update `test` and `assets` workflows.


## [ 0.7.1 ] - [ 2024-05-22 ]

### Added
- Integrate with libqasm 0.6.6 release:
  - Allow multiple qubit/bit (register) definitions and mid-circuit measurements.


## [ 0.7.0 ] - [ 2024-04-17 ]

### Added
- Integrate with libqasm 0.6.3 release.

### Changed
- Rename `tests` folder to `test`.
- Update GitHub workflows.

### Removed
- cQASM 1.x support.
- `circuits` folder.


## [ 0.6.5 ] - [ 2023-11-13 ]

### Added
- Python 3.12 support.
- Conan as package manager.

### Changed
- New GitHub workflows.

### Fixed
- ARM runners in the CI.


## [ 0.6.4 ] - [ 2023-09-21 ]

### Added
- Meaningful error message when parsing or simulation fails.

### Fixed
- Does not crash when too many qubits asked.


## [ 0.6.3 ] - [ 2023-09-19 ]

### Added
- Address sanitizer CMake mode.

### Changed
- C++23.
- Manage dependencies via CMake FetchContent.
- `gtest` instead of `doctest`.
- Doc chapter about internals.

### Fixed
- GitHub Actions.
- Bug in bitset due to bool automatic casting to 32 bits.
- Documentation formatting.

### Removed
- Git submodules and `deps` folder.


## [ 0.6.2 ] - [ 2023-02-21 ]

### Added
- `depolarizing_channel` error model and docs.
- Ability to pass constant seed for deterministic simulation.
- `RandomTest` doing Kolmogorov-Smirnov statistical test to match random numbers with their ideal probability distribution.

### Changed
- Source files use `.hpp`/`.cpp` extensions.
- Random number distributions (integer min/max and real 0-1) are done in-house for portability and consistency of results.

### Fixed
- Docs.


## [ 0.6.1 ] - [ 2023-02-01 ]

### Added
- Linux/ARM64 Python wheel and Linux/ARM64 test workflow (gcc/Release).
- `classical not` gate is back.
- `state` section in JSON now contains a new entry `norm` which is the square of the modulus of the complex number.
- C++ integration test.
- All gates can be controlled (including `prep`, `measure`). This is not new but was accidentally removed in 0.6.0.
- New Python API `execute_string` and `execute_file` without having to create `qxelarator.QX()`
  and returning user-friendly Python type.

### Changed
- `results` section in JSON contains integers instead of floats (those integers being the number of occurrences).
- In relation to previous point, `results` only relates to measurement register averaging,
  and no longer to quantum state when doing a single shot.
  For that, use the "state" section in the JSON.
- Output float precision is 8 decimals instead of 6.

### Fixed
- README.

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

### Fixed
- Issue #118: quantum state is always displayed, as well as measurement register averaging.

 
## [ 0.5.4 ] - [ 2023-01-06 ]

### Added
- Tests for `measure` gate.

### Changed
- Switch to sequential/simple `measure` gate (as part of the fix for issue #114).

### Fixed
- Issues #91 and #114.

### Removed
- QFT implementation (never instantiated, so no functional change).
- `qx-server` no longer exists, together with the network libraries.
- "Microcode" generation functions (never used either).
- Parallel gates in `gate.h` (they were executed sequentially).
- Some unnecessary and/or obsolete code.


## [ 0.5.3 ] - [ 2023-01-02 ]

### Added
- Python 3.11 support.


## [ 0.5.2 ] - [ 2022-12-7 ]

### Fixed
- Version number incorrectly set in previous release.


## [ 0.5.1 ] - [ 2022-12-6 ]

### Added
- ReadTheDocs documentation.

### Changed
- Update README with new API functions.
- Tidy tests and circuits folder.

### Fixed
- Python versions on Linux.

### Removed
- Python 3.6 support.


## [ 0.5.0 ] - [ 2022-11-15 ]

### Added
- `measure_all` gate. Does no longer measure each qubit separately.
- `-j <filename>` CLI option to output JSON to a file.
- Possible to use the simulator without OpenMP.

### Changed
- Move to C++11: smart pointers, etc.
- Format of output has been changed.
  You no longer need to add a `display` gate to output the quantum state at the end of the circuit.
- Use the new libqasm API instead of the old one.
- Output complex amplitudes when not averaging measurement register.
- Do not add automatically a `measure_all` when doing measurement averaging. Average the measurement register only.

### Fixed
- Close input file when done.
- Code style has been completely fixed automatically with clang-format.
- Headers no longer include `.cc` file.
- Move implementations to `.cc` files instead of headers.

### Removed
- Most preprocessor macros, e.g., for gate creation.
- A lot of old and useless code, and a lot of dead code.


## [ 0.4.2 ] - [ 2021-06-01 ]

### Fixed
- Wheels no longer require exotic CPU extensions to work.


## [ 0.4.1 ] - [ 2021-05-20 ]

### Added
- Continuous Integration for as far as automated tests exist.
- Automated release build and distribution logic for PyPI.
- Installation logic in build system.
- `qxelarator` wheels include QX simulator binary (and friends).

### Fixed
- Build on MacOS.
- Build on Windows using MSVC.
- Python build process on various platforms.


## [ 0.4.0 ] - [ 2021-05-20 ]

### Changed
- Replace XPU-based threading with OpenMP.

### Fixed
- Various bugs and inefficiencies related to multithreading.


## [ 0.3.0 ] - [ 2019-05-01 ]

### Added
- `qxelerator` to support cqasm v1.0.
- Setup instructions in README.md.


## [ 0.2.5 ] - [ 2018-12-21 ]

### Fixed
- Issue with libqasm submodule. This release is now with the correct version.


## [ 0.2.4 ] - [ 2018-12-14 ]

### Fixed
- Single qubit operations now accept integers as arguments, previously it required floats.

### Removed
- `-u 0` in Jenkins file again.


## [ 0.2.3 ] - [ 2018-11-05 ]

### Changed
- Update libqasm to get the hotfix.

### Fixed
- pthread mutex locking bug in XPU worker.


## [ 0.2.2 ] - [ 2018-09-25 ]

### Fixed
- Bug in measurement and preparation implementation for large number of qubits.
- Verbose GNU Compiler warnings under Linux.


## [ 0.2.1 ] - [ 2018-09-04 ]

### Added
- Multi-shot simulation mode to save simulation time on the Quantum Inspire web platform.
- Enable quantum noise simulation using the Depolarizing Error Model.
- Enable Identity gate support.


## [ 0.2.0 ] - [ 2018-08-23 ]

### Added
- Initial cQASM 1.0 support.
- Integrate with libqasm 0.1.1 release.
- Support for new quantum gates:
  - Flexible controlled phase shift (`CR` and `CRK`)
  - Measurement and Preparation in X/Y/Z basis: added `measure_x`, `measure_y`, `prep_x` and `prep_y`.
- Support for Single Gate Multiple Qubits (SGMQ).
- New set of unit tests.

### Changed
- Re-implement classical NOT gate for full binary-controlled gate support.

### Fixed
- Overflow bug when simulating more than 31 qubits.
- Bug in qubit register measurement ("measure_all" instruction).

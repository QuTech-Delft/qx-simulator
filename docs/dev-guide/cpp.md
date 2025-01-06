You can build the C++ binaries from the project's root directory.

```shell
conan build . -pr:a=conan/profiles/tests-debug -b missing
```

You can test the C++ binaries:

```shell
cd build/Debug
ctest -C Debug --output-on-failure
```

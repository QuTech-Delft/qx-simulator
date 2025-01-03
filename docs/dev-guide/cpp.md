You can build the C++ binaries from the project's root directory.

```shell
conan build . -pr:a=tests-debug -b missing
```

You can test the C++ binaries:

```shell
cd test/Debug
ctest -C Debug --output-on-failure
```

QX simulator can be executed from the command line.

```shell
build/Release$ ./qx-simulator -c 1000 ../../test/qxelarator/bell_pair.cq
```

In the example above:

- `qx-simulator` is the name of the executable.
- `-c` can be used, optionally, to indicate a number of iterations (`1000` in this case).
- `../test/qxelarator/bell_pair.cq` is the path to the file containing the cQASM program. 

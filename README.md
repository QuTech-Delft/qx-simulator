# QX build

To build run the following in the current directory:

mkdir build && cd build && cmake ../ && make

This creates a bin folder containing subfolders:
  /bin/qx-server
  /bin/qx-simulator
Each containing its respective executable

To install you can further run:
  make install

which installs the two executables under the default bin directory, usually:
  /usr/local/bin

To change this, cmake can be ran with the following prefix:

  cmake -DCMAKE_INSTALL_PREFIX=<desired_absolute_path>

# QXELERATOR build

Follow the instructions in [README.md](qxelarator/README.md) in the qxelerator directory.

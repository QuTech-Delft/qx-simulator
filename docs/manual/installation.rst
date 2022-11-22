============
Installation
============


Installation of the Python package
----------------------------------

To install QXelarator easily, you can use pip directly:

::

    pip install qxelarator

The package is available on PyPI `here <https://pypi.org/project/qxelarator/>`_ .
This will install the latest released version.
For normal users, this is the only thing you have to do, so you can stop reading.


Installation from source
-----------

QX-simulator can be built from source, provided you have installed some dependencies:

* Bison (for building libqasm)
* Flex (for building libqasm)

On top of that, you will need a C++ compiler with support for C++14, ``make`` (for Linux) and ``cmake``.

::
    
    git clone https://github.com/QuTech-Delft/qx-simulator.git

    cd qx-simulator


Building the Python package
~~~~~~~~~~~~~~~~~~~~~~~~~~~

To build QXelarator yourself from source and add it to your local Python packages, inside the repo:

::

    NPROCS=16 python3 -m pip install -v -e .


Building the C++ executable from source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is particularily useful for debugging purposes, as the executable can then be run under ``gdb``, for instance. To do so,
do not forget to build it in debug mode, otherwise compiler optimizations will make debugging more difficult.

::

    mkdir build

    cd build

    cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release

    make -j16 qx-simulator


This will produced an optimized binary ``qx-simulator`` (``-DCMAKE_BUILD_TYPE=Debug`` to disable optimizations).

To build the C++ tests, add the following option to the ``cmake`` call: ``-DQX_BUILD_TESTS=ON``. To run them,
call ``ctest``.
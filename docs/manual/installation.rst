============
Installation
============


Installation of the Python package
----------------------------------

To install QXelarator easily, you can use pip directly:

.. code-block:: bash

    pip install qxelarator

The package is available on PyPI `here <https://pypi.org/project/qxelarator/>`_ .
This will install the latest released version.
For normal users, this is the only thing you have to do, so you can stop reading.


Installation from source
-----------

QX-simulator can be built from source.
On the ARM platform, you need to install yourself some dependencies:

* Bison (for building libqasm)
* Flex (for building libqasm)

On top of that, you will need a C++ compiler with support for C++20, ``make`` (for Linux), ``cmake`` and ``conan``:

.. code-block:: bash

    python3 -m pip install --upgrade pip conan


First and foremost, clone and visit the sources:

.. code-block:: bash
    
    git clone https://github.com/QuTech-Delft/qx-simulator.git

    cd qx-simulator


Building the Python package
~~~~~~~~~~~~~~~~~~~~~~~~~~~

To build QXelarator yourself from source and add it to your local Python packages, inside the repo:

.. code-block:: bash

    NPROCS=16 python3 -m pip install -v -e .

You will need to have SWIG installed for the above to work.

Building the C++ executable from source with Conan
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is particularly useful for debugging purposes, as the executable can then be run under ``gdb``, for instance. To do so,
do not forget to build in debug mode.

.. code-block:: bash

    conan profile detect
    conan build . -pr=conan/profiles/tests-Debug -b missing


Look into `conan/profiles/` to find other profiles to build with.
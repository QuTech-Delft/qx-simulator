#!/usr/bin/env python3

import os, platform, shutil, sys, re
from setuptools import setup, Extension
from distutils.dir_util import copy_tree
from distutils import log

from distutils.command.clean import clean as _clean
from setuptools.command.build_ext import build_ext as _build_ext
from distutils.command.build import build as _build
from setuptools.command.install import install as _install
from distutils.command.bdist import bdist as _bdist
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
from distutils.command.sdist import sdist as _sdist
from setuptools.command.egg_info import egg_info as _egg_info

root_dir = os.getcwd()  # root of the repository
src_dir = root_dir + os.sep + "src"  # C++ source directory
inc_dir = root_dir + os.sep + "include"  # C++ include directory
pysrc_dir = root_dir + os.sep + "python"  # Python source files
target_dir = root_dir + os.sep + "pybuild"  # python-specific build directory
build_dir = target_dir + os.sep + "build"  # directory for setuptools to dump various files into
dist_dir = target_dir + os.sep + "dist"  # wheel output directory
cbuild_dir = target_dir + os.sep + "cbuild"  # cmake build directory
prefix_dir = target_dir + os.sep + "prefix"  # cmake install prefix
srcmod_dir = (
    pysrc_dir + os.sep + "qxelarator"
)  # qxelarator Python module directory, source files only
module_dir = (
    target_dir + os.sep + "qxelarator"
)  # qxelarator Python module directory for editable install

# Copy the hand-written Python sources into the module directory that we're
# telling setuptools is our source directory, because setuptools insists on
# spamming output files into that directory. This is ugly, especially because
# it has to run before setup() is invoked, but seems to be more-or-less
# unavoidable to get editable installs to work.
if not os.path.exists(target_dir):
    os.makedirs(target_dir)
copy_tree(srcmod_dir, module_dir)


def get_version(verbose=0):
    """Extract version information from source code"""

    matcher = re.compile('[\t ]*#define[\t ]+QX_VERSION[\t ]+"(.*)"')
    version = None
    with open(os.path.join(inc_dir, "qx", "Version.hpp"), "r") as f:
        for ln in f:
            m = matcher.match(ln)
            if m:
                version = m.group(1)
                break

    if verbose:
        print("get_version: %s" % version)

    return version


def read(fname):
    with open(os.path.join(os.path.dirname(__file__), fname)) as f:
        return f.read()


class clean(_clean):
    def run(self):
        _clean.run(self)
        if os.path.exists(target_dir):
            shutil.rmtree(target_dir)


class build_ext(_build_ext):
    def run(self):
        from plumbum import local, FG, ProcessExecutionError

        # If we were previously built in a different directory,
        # nuke the cbuild dir to prevent inane CMake errors.
        # This happens when the user does pip install . after building locally
        if os.path.exists(cbuild_dir + os.sep + "CMakeCache.txt"):
            with open(cbuild_dir + os.sep + "CMakeCache.txt", "r") as f:
                for line in f.read().split("\n"):
                    line = line.split("#")[0].strip()
                    if not line:
                        continue
                    if line.startswith("QX_BINARY_DIR:STATIC"):
                        config_dir = line.split("=", maxsplit=1)[1]
                        if os.path.realpath(config_dir) != os.path.realpath(cbuild_dir):
                            print("removing pybuild/cbuild to avoid CMakeCache error")
                            shutil.rmtree(cbuild_dir)
                        break

        # Figure out how many parallel processes to build with.
        if self.parallel:
            nprocs = str(self.parallel)
        else:
            nprocs = os.environ.get("NPROCS", "1")

        # Figure out how setuptools wants to name the extension file and where
        # it wants to place it.
        target = os.path.abspath(self.get_ext_fullpath("qxelarator._qxelarator"))

        # Build the Python extension and "install" it where setuptools expects it.
        if not os.path.exists(cbuild_dir):
            os.makedirs(cbuild_dir)

        # Configure and build using Conan
        with local.cwd(root_dir):
            build_type = os.environ.get("CMAKE_BUILD_TYPE", "Release")
            build_tests = os.environ.get("QX_BUILD_TESTS", "False")
            cpu_compatibility_mode = os.environ.get("QX_CPU_COMPATIBILITY_MODE", "False")

            cmd = local['conan']['profile']['detect']['--force']
            cmd & FG

            cmd = (local['conan']['create']['.']
                ['--version'][get_version()]
                ['-s:h']['compiler.cppstd=23']
                ['-s:h']["build_type=" + build_type]

                ['-o']['qx/*:build_python=True']
                ['-o']['qx/*:build_tests=' + build_tests]
                ['-o']['qx/*:cpu_compatibility_mode=' + cpu_compatibility_mode]
                # The Python library needs the compatibility headers
                ['-o']["qx/*:libqasm_compat=True"]
                ['-o']['qx/*:python_dir=' + re.escape(os.path.dirname(target))]
                ['-o']['qx/*:python_ext=' + re.escape(os.path.basename(target))]
                # (Ab)use static libs for the intermediate libraries
                # to avoid dealing with R(UN)PATH nonsense on Linux/OSX as much as possible
                ['-o']["qx/*:shared=False"]

                ['-b']['missing']
                ['-tf']['']
            )
            if platform.system() == "Darwin":
                cmd = cmd['-c']['tools.build:defines=["_LIBCPP_DISABLE_AVAILABILITY"]']
            cmd & FG

            # Update data_files for the installed files.
            file_dict = {}
            for directory in ("bin", "include", "lib", "lib64"):
                real_directory = os.path.join(prefix_dir, directory)
                if not os.path.isdir(real_directory):
                    continue
                file_list = [
                    os.path.join(path, name)
                    for path, _, filenames in os.walk(real_directory)
                    for name in filenames
                ]
                file_list = filter(
                    lambda f: os.path.isfile(f) and not os.path.islink(f), file_list
                )
                file_list = list(
                    map(lambda f: os.path.relpath(f, real_directory), file_list)
                )
                if file_list:
                    install_directories = [directory]
                    if directory.startswith("lib"):
                        if sys.platform == "linux" or sys.platform == "linux2":
                            install_directories = ["lib", "lib64"]
                        else:
                            install_directories = ["lib"]
                    for install_directory in install_directories:
                        for filename in file_list:
                            install_path = os.path.join(
                                install_directory, os.path.dirname(filename)
                            )
                            if install_path not in file_dict:
                                file_dict[install_path] = []
                            file_dict[install_path].append(
                                os.path.join(real_directory, filename)
                            )
            for install_path, files in file_dict.items():
                self.distribution.data_files.append((install_path, files))


class build(_build):
    def initialize_options(self):
        _build.initialize_options(self)
        self.build_base = os.path.relpath(build_dir)

    def run(self):
        # Make sure the extension is built before the Python module is "built",
        # otherwise SWIG's generated module isn't included.
        # See https://stackoverflow.com/questions/12491328
        self.run_command("build_ext")
        _build.run(self)


class install(_install):
    def run(self):
        # See https://stackoverflow.com/questions/12491328
        self.run_command("build_ext")
        _install.run(self)


class bdist(_bdist):
    def finalize_options(self):
        _bdist.finalize_options(self)
        self.dist_dir = os.path.relpath(dist_dir)


class bdist_wheel(_bdist_wheel):
    def run(self):
        if platform.system() == "Darwin":
            os.environ["MACOSX_DEPLOYMENT_TARGET"] = "10.10"
        _bdist_wheel.run(self)
        impl_tag, abi_tag, plat_tag = self.get_tag()
        archive_basename = "{}-{}-{}-{}".format(
            self.wheel_dist_name, impl_tag, abi_tag, plat_tag
        )
        wheel_path = os.path.join(self.dist_dir, archive_basename + ".whl")
        if platform.system() == "Darwin":
            from delocate.delocating import delocate_wheel

            delocate_wheel(wheel_path)


class sdist(_sdist):
    def finalize_options(self):
        _sdist.finalize_options(self)
        self.dist_dir = os.path.relpath(dist_dir)


class egg_info(_egg_info):
    def initialize_options(self):
        _egg_info.initialize_options(self)
        self.egg_base = os.path.relpath(target_dir)


setup(
    name="qxelarator",
    version=get_version(),
    description="qxelarator Python Package",
    long_description=read("README.md"),
    long_description_content_type="text/markdown",
    author="QuTech, TU Delft",
    url="https://github.com/QuTech-Delft/qx-simulator",
    classifiers=[
        "License :: OSI Approved :: Apache Software License",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS",
        "Operating System :: Microsoft :: Windows",
        "Programming Language :: Python :: 3 :: Only",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Topic :: Scientific/Engineering",
    ],
    packages=["qxelarator"],
    package_dir={"": "pybuild"},
    # This will be populated during the build.
    data_files=[],
    # NOTE: the library build process is completely overridden to let CMake
    # handle it; setuptools' implementation is horribly broken. This is here
    # just to have the rest of setuptools understand that this is a Python
    # module with an extension in it.
    ext_modules=[Extension("qxelarator._qxelarator", [])],
    cmdclass={
        "bdist": bdist,
        "bdist_wheel": bdist_wheel,
        "build_ext": build_ext,
        "build": build,
        "install": install,
        "clean": clean,
        "egg_info": egg_info,
        "sdist": sdist,
    },
    setup_requires=[
        'conan',
        "plumbum",
        'delocate; platform_system == "Darwin"',
    ],
    install_requires=[
        'msvc-runtime; platform_system == "Windows"',
    ],
    tests_require=["pytest"],
    zip_safe=False,
)

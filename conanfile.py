import os
import re
import sys

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import copy


class QxConan(ConanFile):
    name = "qx"

    # Optional metadata
    license = "Apache-2.0"
    homepage = "https://github.com/QuTech-Delft/qx-simulator"
    url = "https://github.com/conan-io/conan-center-index"
    description = "A universal quantum computer simulator. Allows simulation of a quantum circuit written in cQasm."
    topics = "quantum simulation"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "asan_enabled": [True, False],
        "build_python": [True, False],
        "build_tests": [True, False],
        "cpu_compatibility_mode": [True, False],
        "python_dir": [None, "ANY"],
        "python_ext": [None, "ANY"],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "asan_enabled": False,
        "build_python": False,
        "build_tests": False,
        "cpu_compatibility_mode": False,
        "python_dir": None,
        "python_ext": None
    }

    exports_sources = "CMakeLists.txt", "include/*", "python/*", "src/*", "tests/*"

    def build_requirements(self):
        self.requires("abseil/20230125.3")
        # TODO: remove tree-gen dependency once libqasm can be required directly
        self.tool_requires("tree-gen/1.0.7")
        if self.settings.arch != "armv8":
            self.tool_requires("zulu-openjdk/11.0.19")
        if self.options.build_tests:
            self.requires("gtest/1.14.0")

    def requirements(self):
        self.requires("antlr4-cppruntime/4.13.1")
        self.requires("fmt/10.2.1")
        # TODO: remove tree-gen dependency once libqasm can be required directly
        self.requires("tree-gen/1.0.7")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        self.folders.source = "."
        self.folders.build = os.path.join("build", str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")

        self.cpp.package.libs = ["qx"]
        self.cpp.package.includedirs = ["include"]
        self.cpp.package.libdirs = ["lib"]

        self.cpp.source.includedirs = ["include"]
        self.cpp.build.libdirs = ["."]

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["ASAN_ENABLED"] = self.options.asan_enabled
        tc.variables["QX_BUILD_PYTHON"] = self.options.build_python
        tc.variables["QX_BUILD_TESTS"] = self.options.build_tests
        tc.variables["QX_CPU_COMPATIBILITY_MODE"] = self.options.cpu_compatibility_mode
        tc.variables["QX_PYTHON_DIR"] = self.options.python_dir
        tc.variables["QX_PYTHON_EXT"] = self.options.python_ext
        tc.variables["PYTHON_EXECUTABLE"] = re.escape(sys.executable)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.md", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["qx"]

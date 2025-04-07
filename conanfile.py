import os
import re
import sys

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import copy

required_conan_version = ">=1.60.0 <2 || >=2.0.6"


class QxConan(ConanFile):
    name = "qx"
    license = "Apache-2.0"
    homepage = "https://github.com/QuTech-Delft/qx-simulator"
    url = "https://github.com/conan-io/conan-center-index"
    description = "A universal quantum computer simulator. Allows simulation of a quantum circuit written in cQasm."
    topics = "quantum simulation"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "asan_enabled": [True, False],
        "build_python": [True, False],
        "cpu_compatibility_mode": [True, False],
        "python_dir": [None, "ANY"],
        "python_ext": [None, "ANY"],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "asan_enabled": False,
        "build_python": False,
        "cpu_compatibility_mode": False,
        "python_dir": None,
        "python_ext": None
    }
    exports_sources = "CMakeLists.txt", "include/*", "python/*", "src/*", "tests/*"

    @property
    def _should_build_test(self):
        return not self.conf.get("tools.build:skip_test", default=True, check_type=bool)

    def build_requirements(self):
        self.tool_requires("libqasm/1.2.0")
        if self._should_build_test:
            self.test_requires("gtest/1.15.0")

    def requirements(self):
        self.requires("boost/1.85.0")
        self.requires("eigen/3.4.0")
        self.requires("fmt/11.0.2", transitive_headers=True)
        self.requires("libqasm/1.2.0", transitive_headers=True)
        self.requires("range-v3/0.12.0", transitive_headers=True)

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
        tc.variables["QX_BUILD_TESTS"] = self._should_build_test
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

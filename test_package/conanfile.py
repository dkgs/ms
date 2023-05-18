import os
from conan import ConanFile
from conan.tools.build import can_run


class msTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def imports(self):
        self.copy("ms", "", "bin")

    def requirements(self):
        self.requires(self.tested_reference_str)

    def test(self):
        if not os.path.exists("ms"):
            raise "ms executable not found"

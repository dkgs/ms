from conan import ConanFile
from conans import CMake


class msRecipe(ConanFile):
    name = "ms"
    version = "1.0.0"
    package_type = "application"

    # Optional metadata
    license = "MIT License"
    author = "Mickaël GUERRA"
    url = "https://github.com/dkgs/ms"
    description = "Simple movie service"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    generators = "cmake", "txt"

    def build_requirements(self):
        self.build_requires("boost/1.81.0")
        self.build_requires("spdlog/1.11.0")
        self.build_requires("catch2/2.13.10")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    

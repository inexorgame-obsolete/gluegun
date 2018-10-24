from conans import ConanFile, CMake
import os
import shutil

class nanomsgxxConan(ConanFile):
    name = "nanomsgxx"
    version = "0.2.1"
    url="https://github.com/achille-roussel/nanomsgxx"
    generators = "cmake", "txt"
    settings = "os", "compiler", "build_type", "arch"
    exports = "assets/*"
    requires = "nanomsg/1.0.0@inexorgame/testing"
    short_paths = True
    options = {"enable_tests": [True, False],
               }
    default_options = "enable_tests=False"

    def source(self):
        self.run("git clone https://github.com/achille-roussel/nanomsgxx")

        self.output.info("Copying assets")
        shutil.move("assets/CMakeLists.txt", "nanomsgxx")
        os.mkdir("nanomsgxx/cmake")
        shutil.move("assets/cmake/CompilerOptions.cmake", "nanomsgxx/cmake")
        shutil.move("assets/cmake/DetectCXXCompiler.cmake", "nanomsgxx/cmake")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="nanomsgxx")
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*", dst="bin", src="install/bin")
        self.copy("*", dst="include", src="install/include")
        self.copy("*", dst="lib", src="install/lib")

    def package_info(self):
        self.cpp_info.libs = ["nanomsgxx"]

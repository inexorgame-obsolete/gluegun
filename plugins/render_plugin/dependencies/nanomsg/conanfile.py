from conans import ConanFile, CMake
from conans.tools import replace_in_file
import os
import shutil

class nanomsgConan(ConanFile):
    name = "nanomsg"
    version = "1.0.0"
    url="https://github.com/nanomsg/nanomsg.git"
    generators = "cmake", "txt"
    settings = "os", "compiler", "build_type", "arch"
    short_paths = True
    options = {"shared": [True, False],
               "enable_doc": [True, False],
               "enable_getaddrinfo_a": [True, False],
               "enable_tests": [True, False],
               "enable_tools": [True, False],
               "enable_nanocat": [True, False],
               }
    default_options = "shared=False", \
        "enable_doc=False", \
        "enable_getaddrinfo_a=True", \
        "enable_tests=False", \
        "enable_tools=True", \
        "enable_nanocat=True"

    def source(self):
        self.run("git clone https://github.com/nanomsg/nanomsg.git")
        self.run("cd nanomsg && git checkout tags/1.0.0")

    def build(self):
        conan_magic_lines = '''#  Platform checks.

# Conan.io config
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
'''
        replace_in_file("nanomsg/CMakeLists.txt", "#  Platform checks.", conan_magic_lines)
        
        conan_magic_lines = '''
        # cmake_policy (SET CMP0042 NEW)
        '''
        replace_in_file("nanomsg/CMakeLists.txt", "cmake_policy (SET CMP0042 NEW)", conan_magic_lines)

        cmake = CMake(self)

        for option_name in self.options.values.fields:
            activated = getattr(self.options, option_name)
            if option_name == "shared":
               cmake.definitions["NN_STATIC_LIB"] = "OFF" if activated else "NN_STATIC_LIB=ON"
            elif option_name == "enable_doc":
               cmake.definitions["NN_ENABLE_DOC"] = "ON" if activated else "NN_ENABLE_DOC=OFF"
            elif option_name == "enable_getaddrinfo_a":
               cmake.definitions["NN_ENABLE_GETADDRINFO_A"] = "ON" if activated else "NN_ENABLE_GETADDRINFO_A=OFF"
            elif option_name == "enable_tests":
               cmake.definitions["NN_TESTS"] = "ON" if activated else "NN_TESTS=OFF"
            elif option_name == "enable_tools":
               cmake.definitions["NN_TOOLS"] = "ON" if activated else "NN_TOOLS=OFF"
            elif option_name == "enable_nanocat":
               cmake.definitions["NN_ENABLE_NANOCAT"] = "ON" if activated else "NN_ENABLE_NANOCAT=OFF"
            else:
               the_option += "ON" if activated else "OFF"

        cmake.configure(source_folder="nanomsg")
        cmake.build()
        cmake.install()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", dst="include", src="install/include")
        self.copy("*.dll", dst="bin", src="install/bin")
        self.copy("*.lib", dst="lib", src="install/lib")
        self.copy("*.a", dst="lib", src="install/lib")
        self.copy("*.so*", dst="lib", src="install/lib")
        self.copy("*.dylib", dst="lib", src="install/lib")
        self.copy("nanocat*", dst="bin", src="install/bin")
        self.copy("*.*", dst="lib/pkgconfig", src="install/lib/pkgconfig")

    def package_info(self):
        self.cpp_info.libs = ["nanomsg"]

        if not self.options.shared:
            self.cpp_info.defines.extend(["NN_STATIC_LIB=ON"])

        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.libs.append('mswsock')
                self.cpp_info.libs.append('ws2_32')
        elif self.settings.os == "Linux":
            self.cpp_info.libs.append('anl')
            self.cpp_info.libs.append('pthread')
            self.cpp_info.libs.append('rt')

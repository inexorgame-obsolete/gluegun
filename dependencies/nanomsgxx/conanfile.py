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
    requires = "nanomsg/1.0.0@Wi3ard/stable"
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
        cmake = CMake(self.settings)

        cmake_options = []
        for option_name in self.options.values.fields:
            activated = getattr(self.options, option_name)
            the_option = "%s=" % option_name.upper()
            if option_name == "enable_tests":
               the_option = "NNXX_TESTS=ON" if activated else "NNXX_TESTS=OFF"
            else:
               the_option += "ON" if activated else "OFF"
            cmake_options.append(the_option)

        cmake_cmd_options = " -D".join(cmake_options)

        cmake_conf_command = 'cmake %s/nanomsgxx %s -DCMAKE_INSTALL_PREFIX:PATH=install -D%s' % (self.conanfile_directory, cmake.command_line, cmake_cmd_options)
        self.output.warn(cmake_conf_command)
        self.run(cmake_conf_command)

        self.run("cmake --build . --target install %s" % cmake.build_config)

    def package(self):
        self.copy("*", dst="bin", src="install/bin")
        self.copy("*", dst="include", src="install/include")
        self.copy("*", dst="lib", src="install/lib")

    def package_info(self):
        self.cpp_info.libs = ["nanomsgxx"]

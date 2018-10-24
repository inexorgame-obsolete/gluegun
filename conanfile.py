from conans import ConanFile, CMake
class Pkg(ConanFile):
    settings = "os", "compiler", "arch", "build_type"
    requires = ("nanomsgxx/0.2.1@inexorgame/testing")
    generators = "cmake"
    exports_sources = "core.cpp"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.h", src="src", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["databaseio"]


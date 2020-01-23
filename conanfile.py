from conans import ConanFile

class QtEngineConan(ConanFile):
    name = "QtEngine"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = "qt/5.14.0@bincrafters/stable", "freetype/2.9.0@bincrafters/stable", "bzip2/1.0.6@conan/stable"
    generators = "cmake", "qt"
    build_policy = "missing"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

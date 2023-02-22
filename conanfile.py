from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

required_conan_version = ">=1.53.0"


class JsonLdCppConan(ConanFile):
    name = "jsonld-cpp"
    url = "https://github.com/dcdpr/jsonld-cpp"
    description = "Open Source C library that provides a set of parsers and serializers that generate Resource Description Framework (RDF) triples by parsing syntaxes or serialize the triples into a syntax."
    topics = "xml", "parser", "validation"
    homepage = "https://librdf.org/raptor/"
    license = "Apache License Version 2.0"

    settings = "os", "arch", "compiler", "build_type"
    default_options = {
        "shared": False,
        "fPIC": True
    }
    options = {name: [True, False] for name in default_options.keys()}
    exports_sources = "*"

    def configure(self):
        #self.options["rapidcheck"].enable_gmock = True
        self.options["rapidcheck"].enable_gtest = True

    def requirements(self):
        #self.requires("boost/1.81.0")
        self.requires("nlohmann_json/3.11.2")

        #self.requires("cpr/1.10.0", private=True)
        #FIXME: Is this dependency just pulled for uriparser ? Is there a good reason to pull http-link-header-cpp ?
        self.requires("http-link-header-cpp/0.9.0@amazon/testing", private=True)

        self.requires("gtest/1.13.0", private=True, override=True)
        self.requires("rapidcheck/cci.20220514", private=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["JSONLDCPP_BUILD_EXAMPLES"] = True
        tc.variables["JSONLDCPP_BUILD_TESTS"] = True
        tc.variables["CMAKE_VERBOSE_MAKEFILE"] = True
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        project_variables = {
            "JSONLDCPP_BUILD_EXAMPLES": False,
            "JSONLDCPP_BUILD_TESTS": False,
            "CMAKE_VERBOSE_MAKEFILE": True,
        }
        cmake.configure(variables=project_variables)
        cmake.build()
        #FIXME: tests are failing
        #cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']  # Ordered list of include paths
        self.cpp_info.libs = ["jsonld-cpp", ]  # The libs to link against
        self.cpp_info.system_libs = []  # System libs to link against
        self.cpp_info.libdirs = ['lib']  # Directories where libraries can be found

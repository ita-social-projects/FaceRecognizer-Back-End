from conans import ConanFile, tools, MSBuild

class RealTimeFaceMaskDetector(ConanFile):
    name = "realtime_face_mask_detector"
    version = "0.1"
    url = "https://github.com/OleksiiiSemko/Real-Time-Face-Mask-Detector-Server.git"
    settings = "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {
                        "shared": False,
                        "qt:shared": True,
                        "opencv:shared": True,
                        "cryptopp:shared": False
                    }
    exports_sources = "src/*", "build/*"
    requires = "qt/5.15.2", "opencv/4.5.0", "cryptopp/8.2.0"
    generators = "msbuild"

    def build(self):
        msbuild = MSBuild(self)
        msbuild.build_env.include_paths.append("C:\\SQLAPI\\include")
        msbuild.build_env.include_paths.append("C:\\Qt\\include")
        msbuild.build_env.lib_paths.append("C:\\SQLAPI\\vs2019\\x86_64\\lib")
        msbuild.build_env.lib_paths.append("C:\\Qt\\lib")
        msbuild.build("..\\RealTimeFaceMaskDetector\\RealTimeFaceMaskDetector.sln")

    def package(self):
        # Copy the executable
        self.copy(pattern="tool_a*", dst="bin", keep_path=False)
        # Copy the labraries
        if self.options.shared:
            self.copy(pattern="*.dll", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug", keep_path=False)
            self.copy(pattern="*.lib", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug", keep_path=False)
        else:
            self.copy("*.h", dst="include", src="src")
            self.copy("*.lib", dst="lib", keep_path=False)

    def imports(self):
        self.copy(pattern="*.dll", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug", keep_path=False)
        self.copy(pattern="*.lib", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug", keep_path=False)
        self.copy(pattern="*.dll", src="C:\\SQLAPI\\vs2019\\x86_64\\bin", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug", keep_path=False)
        self.copy(pattern="*.lib", dst="..\\RealTimeFaceMaskDetector\\EncryptDecryptECBMode", root_package="cryptopp", keep_path=False)
        self.copy(pattern="*", dst="..\\3rdPartyLibs\\cryptopp820", root_package="cryptopp", keep_path=True)
        self.copy(pattern="*", dst="C:\\Qt", root_package="qt", keep_path=True)
        self.copy(pattern="*", dst="..\\3rdPartyLibs\\opencv", root_package="opencv", keep_path=True)
        self.copy(pattern="*/platforms/*", dst="..\\RealTimeFaceMaskDetector\\x64\\Debug\\platforms", root_package="qt", keep_path=False)
        self.copy(pattern="*/platforms/*", dst="..\\RealTimeFaceMaskDetector\\TCPClientUI\\x64\\Debug\\platforms", root_package="qt", keep_path=False)
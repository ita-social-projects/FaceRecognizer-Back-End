cmake_minimum_required(VERSION 3.15)
project(EncyptDecryptECBMode)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../3rdPartyLibs/cryptopp820/include)
link_directories(../../3rdPartyLibs/cryptopp820/x64/Output/Release)

set(SOURCES
        pch.h pch.cpp
        cpp.hint
        ICrypt.h
        dllmain.cpp
        framework.h
        EncryptDecryptRSA.h EncryptDecryptRSA.cpp
        EncryptDecryptAES_ECBMode.h EncryptDecryptAES_ECBMode.cpp)

add_compile_definitions(ENCRYPTDECRYPTECBMODE_EXPORTS)

add_library(EncyptDecryptECBMode ${SOURCES})

target_link_libraries(EncyptDecryptECBMode cryptlib.lib)
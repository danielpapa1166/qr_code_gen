cmake_minimum_required(VERSION 3.16)

project(qr_code_gen VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

option(QR_CODE_GEN_BUILD_SANITY_APP "Build the manual sanity-check application" ON)

add_subdirectory(src)

if(QR_CODE_GEN_BUILD_SANITY_APP)
    add_subdirectory(test)
endif()
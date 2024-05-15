cmake_minimum_required(VERSION 3.28)
project(hetropod)

set(CMAKE_CXX_STANDARD 17)

find_package(Fcitx5Core REQUIRED)
include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")

add_subdirectory(src)
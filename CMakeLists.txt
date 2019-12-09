cmake_minimum_required (VERSION 3.8)

project ("echo")

set (LIBRARY_PATH ${PROJECT_SOURCE_DIR}/shared/lib)

add_subdirectory ("client")
add_subdirectory ("server")

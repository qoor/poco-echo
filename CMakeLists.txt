cmake_minimum_required (VERSION 3.8)

project ("echo")

set (LIBRARY_PATH ${PROJECT_SOURCE_DIR}/shared/lib)

if (WIN32)
	add_subdirectory ("client")
else ()
	add_subdirectory ("server")
endif ()

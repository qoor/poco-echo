cmake_minimum_required (VERSION 3.8)

include_directories (${PROJECT_SOURCE_DIR}/shared/include)

set (SOURCE_FILE
	"src/main.cpp"
	"src/main.h"
)

add_executable (echo-server ${SOURCE_FILE})

target_link_libraries (echo-server ${PROJECT_SOURCE_DIR}/shared/lib/windows/PocoFoundationd.lib)
target_link_libraries (echo-server ${PROJECT_SOURCE_DIR}/shared/lib/windows/PocoNetd.lib)

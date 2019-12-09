cmake_minimum_required (VERSION 3.8)

include_directories (${PROJECT_SOURCE_DIR}/shared/include)

set (SOURCE_FILE
	"src/StdInc.h"
	"src/main.cpp"
	"src/main.h"
	"src/COption.cpp"
	"src/COption.h"
)

add_executable (echo-client ${SOURCE_FILE})

target_link_libraries (echo-client ${PROJECT_SOURCE_DIR}/shared/lib/windows/PocoFoundationd.lib)
target_link_libraries (echo-client ${PROJECT_SOURCE_DIR}/shared/lib/windows/PocoNetd.lib)

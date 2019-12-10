cmake_minimum_required (VERSION 3.8)

include_directories (${PROJECT_SOURCE_DIR}/shared/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

set (SOURCE_FILE
	"src/StdInc.h"
	"src/main.cpp"
	"src/main.h"
	"src/COption.cpp"
	"src/COption.h"
	"src/CClient.cpp"
	"src/CClient.h"
	"src/CActionManager.cpp"
	"src/CActionManager.h"
	"src/CPacketHandler.cpp"
	"src/CPacketHandler.h"

	"src/packet/CMessage.cpp"
	"src/packet/CMessage.h"
)

add_executable (echo-client ${SOURCE_FILE})

target_link_libraries (echo-client ${LIBRARY_PATH}/Poco/windows/PocoFoundationd.lib)
target_link_libraries (echo-client ${LIBRARY_PATH}/Poco/windows/PocoNetd.lib)

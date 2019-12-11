cmake_minimum_required (VERSION 3.8)

include_directories (${PROJECT_SOURCE_DIR}/shared/include)
include_directories (${PROJECT_SOURCE_DIR}/shared/include/Network)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/vendor/include)

set (SOURCE_FILE
	"src/StdInc.h"
	"src/main.cpp"
	"src/main.h"
	"src/CServer.cpp"
	"src/CServer.h"
	"src/CWhiteList.cpp"
	"src/CWhiteList.h"
	"src/CPacketHandler.cpp"
	"src/CPacketHandler.h"
	"src/CActionManager.cpp"
	"src/CActionManager.h"
	"src/CPacketTranslator.cpp"
	"src/CPacketTranslator.h"

	"src/packets/CPacket.cpp"
	"src/packets/CPacket.h"
	"src/packets/CMessagePacket.cpp"
	"src/packets/CMessagePacket.h"
	"src/packets/CMessageSendPacket.cpp"
	"src/packets/CMessageSendPacket.h"

	${PROJECT_SOURCE_DIR}/shared/include/Network/CNetwork.cpp
	${PROJECT_SOURCE_DIR}/shared/include/Network/CNetwork.h
)

add_executable (echo-server ${SOURCE_FILE})

target_link_libraries (echo-server ${LIBRARY_PATH}/Poco/windows/PocoFoundationd.lib)
target_link_libraries (echo-server ${LIBRARY_PATH}/Poco/windows/PocoNetd.lib)
target_link_libraries (echo-server ${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/sqlite/sqlite3.lib)

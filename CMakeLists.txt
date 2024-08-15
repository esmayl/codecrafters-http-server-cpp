cmake_minimum_required(VERSION 3.13)
project(http-server-starter-cpp)

if(MINGW)
    link_libraries(ws2_32 wsock32)
endif()

set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard

include(FetchContent)
FetchContent_Declare(
        zlib
        URL https://zlib.net/current/zlib.tar.gz
)
FetchContent_MakeAvailable(zlib)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(server ${SOURCE_FILES}
        src/HttpPacket.cpp
        src/HttpPacket.h
        src/enums/HTTPMETHOD.h
        src/controllers/EchoController.cpp
        src/controllers/EchoController.h
        src/controllers/UserAgentController.cpp
        src/controllers/UserAgentController.h
        src/Globals.cpp
        src/Globals.h
        src/controllers/FileController.cpp
        src/controllers/FileController.h
        src/enums/CONTENTTYPE.cpp
        src/enums/CONTENTTYPE.h
        src/SocketWrapper.cpp
        src/SocketWrapper.h
        src/WebServer.cpp
        src/WebServer.h
        src/utilities/ThreadPool.cpp
        src/utilities/ThreadPool.h
        src/utilities/StringUtils.cpp
        src/utilities/StringUtils.h
)
target_link_libraries(server pthread zlibstatic)
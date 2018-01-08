cmake_minimum_required(VERSION 3.7)
project(MessengerServer)

set(CMAKE_CXX_STANDARD 14)

add_library(asio INTERFACE )
target_include_directories(asio INTERFACE lib/asio)

add_library(easyloggingpp lib/easyloggingpp/src/easylogging++.cc)
target_include_directories(easyloggingpp INTERFACE lib/easyloggingpp/src)

add_library(argparse INTERFACE )
target_include_directories(argparse INTERFACE lib/argparse )

include_directories(src)

set(SOURCE_FILES src/main.cpp src/MessengerServer.cpp src/MessengerServer.hpp src/Client.cpp src/Client.hpp src/ProtocolMessage.hpp src/ProtocolMessage.cpp src/Account.cpp src/Account.hpp src/Registrar.cpp src/Registrar.hpp src/utils.hpp src/constants.hpp)
add_executable(MessengerServer ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} asio easyloggingpp argparse pthread)
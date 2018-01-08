cmake_minimum_required(VERSION 3.7)
project(MessengerServer)

set(CMAKE_CXX_STANDARD 14)

add_library(asio INTERFACE )
target_include_directories(asio INTERFACE lib/asio)

add_library(easyloggingpp lib/easyloggingpp/src/easylogging++.cc)
target_include_directories(easyloggingpp INTERFACE lib/easyloggingpp/src)

add_library(argparse INTERFACE )
target_include_directories(argparse INTERFACE lib/argparse )

set(SOURCE_FILES main.cpp MessengerServer.cpp MessengerServer.hpp Client.cpp Client.hpp ProtocolMessage.hpp ProtocolMessage.cpp Account.cpp Account.hpp Registrar.cpp Registrar.hpp utils.hpp constants.hpp)
add_executable(MessengerServer ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} asio easyloggingpp argparse pthread)
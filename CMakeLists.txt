cmake_minimum_required(VERSION 3.11)
project(chat-udp
  VERSION 0.1.0
  DESCRIPTION "Simple Chat via UDP"
  LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt4 4.8.7 REQUIRED COMPONENTS QtCore QtGui QtNetwork)

set(CMAKE_WIN32_EXECUTABLE TRUE)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(server)
add_subdirectory(client)

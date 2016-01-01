cmake_minimum_required(VERSION 3.0)

set(CMAKE_SYSTEM_NAME Windows)
#set(CMAKE_CROSSCOMPILING 1)

#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_C_FLAGS "-I. -lstatic")
set(CMAKE_CXX_FLAGS "-I. -static")

#set(OPENSSL_ROOT_DIR "/home/vladimir/dev/public/netfix/openssl-master/include/openssl")
#set(OPENSSL_SSL_LIBRARIES "/usr/lib/libssl.so.1.0.0")
#set(OPENSSL_SSL_LIBRARIES "/usr/lib/")
#set(OPENSSL_INCLUDE_DIR "/usr/include/openssl")
set(OPENSSL_ROOT_DIR "openssl/")
set(OPENSSL_INCLUDE_DIR "openssl/include")
set(OPENSSL_LIBRARIES "openssl/lib")
# Search OpenSSL
#find_package(PkgConfig REQUIRED)
#pkg_search_module(OPENSSL REQUIRED openssl)

#if( OPENSSL_FOUND )
    #include_directories(${OPENSSL_INCLUDE_DIRS})
    #message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
    #else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
    #endif()

#project name
project(netfix)

#openssl
find_package(OpenSSL REQUIRED)

#files to compile
file(GLOB SOURCES src/*.cpp)

#includes
file(GLOB INCLUDES include/*.h)

#I prefer to have this here instead of putting '../include/' in all of my .cpp files
include_directories(include)
#include_directories(/usr/include)

set(BINARY_NAME "netfix")

add_executable(${BINARY_NAME} main.cpp ${SOURCES} ${INCLUDES})

#here we link the required libs to our executable
target_link_libraries(${BINARY_NAME} ${OPENSSL_LIBRARIES} ws2_32)
#target_link_libraries(${BINARY_NAME} ssl crypto)

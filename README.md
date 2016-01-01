# netfix

A lightweight, cross-paltform HTTP library written in C++

# Contributions guidelines
* We are doing art, we write beautiful code
* Cross-platform, remember ?
* Must be lightweight
* K.I.S.S (Keep it Simple, Stupid)

# TODO
* CMake with profiles for each platform
* Certs verification
* Remove all the hardcoded stuff (done, if you find anything I might have forgot let me know)
* ~~Distinguish dynamically HTTP from HTTPS (only SSL currently, so divide the appropriate parts into functions) (and conquer)~~
* User-agent, HTTP version...etc -> give more control to the user
* Extend the current code to make an Object Oriented version of it (currently everything is static)
* POST
* File upload
* Follow redirect (hmm make the option available)
* GET RID OF THIS HEAVY **** OPENSSL WHICH PRODUCE A 1.3 *** MEGABYTES BINARY ON WINDOWS OMFG INSTEAD OF 189 KILOBYTES

# Setup

## Linux

* Download the source code and compile. Don't forget to add the flags -lcrypto -lssl
* OPTIONNALLY in C::B (Code::Blocks) : Build Options / Search Directories => the "include" folder (otherwise it will complain it can't find stuff)

## Windows with MSVC

* Download and install openssl (32 bits, full) : http://slproweb.com/products/Win32OpenSSL.html
* Configuration Properties/C/C++/General/Additional Include Directories: openssl include folder (C:\OpenSSL-Win32\include)
* Configuration Properties/C/C++/Code Generation/Runtime Library => /MT (if you want to have a statically linked binary)
* Linker/General/Additional Library Directiories: C:\OpenSSL-Win32\lib
* Linker/Input/Additional Dependencies : libeay32MT.lib;ssleay32MT.lib (the statically linked ones in C:\OpenSSL-Win32\lib\VC\static)
* Other linker additional dependencies : Ws2_32.lib (sockets) legacy_stdio_definitions.lib (the openssl statically linked libs I used were compiled with an older version of MSVC. See main.cpp for details).

# netfix

A lightweight, cross-paltform HTTP library written in C++

#Announcement
netfix has migrated to [WolfSSL] (https://github.com/wolfSSL/wolfssl) -> binary size is finally acceptable

# Contributions guidelines
* We are doing art, we write beautiful code (critics are most welcome)
* Cross-platform, remember ?
* Must be lightweight
* K.I.S.S (Keep it Simple, Stupid)

# TODO
* CMake with profiles for each platform
* Certs verification
* Remove all the hardcoded stuff (done, if you find anything I might have forgotten let me know)
* ~~Distinguish dynamically HTTP from HTTPS (only SSL currently, so divide the appropriate parts into functions) (and conquer)~~
* User-agent, HTTP version...etc -> give more control to the user
* Extend the current code to make an Object Oriented version of it (currently everything is static)
* POST
* File upload (in progress)
* Follow redirect (hmm make the option available)
* ~~GET RID OF THIS HEAVY **** OPENSSL WHICH PRODUCE A 1.3 *** MEGABYTES BINARY ON WINDOWS OMFG INSTEAD OF 189 KILOBYTES (done)~~
* Finish to implement all the specifications of HTTP/HTTPS
* Refactor

# Setup
Since the migration of netfix to [WolfSSL] (https://github.com/wolfSSL/wolfssl), the openssl part is obsolete but kept for future reference in case you want to switch back to openssl (for this you would just have to comment/uncomment the correct includes instructions in HTTPRequest.cpp). 

## Linux
* Download [wolfSSL] (https://wolfssl.com/wolfSSL/download/downloadForm.php) and unzip it
* cd into wolfssl directory
* ./configure --enable-static --enable-opensslextra --disable-fastmat
* make
* sudo make install
* Now you can compile netfix, but don't forget to link against wolfssl and m :p
* If you want to statically compile netfix, build with -Xlinker -Bstatic -lwolfssl -Xlinker -Bdynamic  -lm :
![Code::Blocks configuration] (https://cloud.githubusercontent.com/assets/13260466/14666556/74859822-06da-11e6-80ca-16b0cd1e8e98.png)

## Windows
### MSVC
* Download [wolfSSL] (https://wolfssl.com/wolfSSL/download/downloadForm.php) and unzip it
* Open the Project in Visual Studio and build. DLL will be in DLL Release
* Configuration Properties/C/C++/General/Additional Include Directories: wolfssl root directory
* Configuration Properties/C/C++/Code Generation/Runtime Library => /MT (if you want to have a statically linked binary)
* If you chose the static way, make sure you compiled wolfssl with /MT too in Configuration Manager
* Linker/General/Additional Library Directiories: C:\wolfssl-3.9.0\Release
* Linker/Input/Additional Dependencies : wolfssl.lib (the statically linked one in C:\wolfssl-3.9.0\Release if you want static)
* Other linker additional dependencies : Ws2_32.lib (sockets) legacy_stdio_definitions.lib  to avoid link error 2001 unresolved external symbol strncopy

### MinGW
* Wasn't able to do it. Please help me.

 
## Linux (openssl version)

* Download the source code and compile. Don't forget to add the flags -lcrypto -lssl
* OPTIONNALLY in C::B (Code::Blocks) : Build Options / Search Directories => the "include" folder (otherwise it will complain it can't find stuff)

## Windows (openssl version)

### MSVC

* Download and install openssl (32 bits, full) : http://slproweb.com/products/Win32OpenSSL.html
* Configuration Properties/C/C++/General/Additional Include Directories: openssl include folder (C:\OpenSSL-Win32\include)
* Configuration Properties/C/C++/Code Generation/Runtime Library => /MT (if you want to have a statically linked binary)
* Linker/General/Additional Library Directiories: C:\OpenSSL-Win32\lib
* Linker/Input/Additional Dependencies : libeay32MT.lib;ssleay32MT.lib (the statically linked ones in C:\OpenSSL-Win32\lib\VC\static)
* Other linker additional dependencies : Ws2_32.lib (sockets) legacy_stdio_definitions.lib (the openssl statically linked libs I used were compiled with an older version of MSVC. See main.cpp for details).

### MinGW

Unfortunately I wasn't able to find statically linked libs of openssl compiled with the right version of mingw. So here are the steps to successfully build everything (you can use mine if you don't want to compile your own) :
* Download the openssl sources from https://github.com/openssl/openssl/archive/master.zip
* Unzip
* Install mingw and [MSYS](http://www.mingw.org/wiki/MSYS) with the help of mingw-get-setup.exe from http://sourceforge.net/projects/mingw/files/Installer/
* Install ActiveState Perl from https://www.activestate.com/activeperl/downloads
* Run the following commands from a MSYS console (NOTE : /c/openssl is the path to the openssl source folder you just downloaded):
  * cd /c/openssl-master
  * perl Configure mingw no-shared --prefix=/c/openssl-master
  * make depend
  * make
  * make install

* You should now have a working copy of libcrypto.a and libssl.a
* Link these libs to your compiler in this order : libssl.a libcrypto (this is important)
* Don't forget to link against ws32_32, gdi32 and wsock32





//if the compiler is MSVC (as MSC_VER is also set by Intel Compiler, the condition had to be adapted)
#if _MSC_VER && !__INTEL_COMPILER
//#include "stdafx.h"

/*
    * Temporary fix for the following errors :
    ** unresolved external symbol __imp__fprintf
    ** unresolved external symbol __imp____iob_func

    => libeay32MT.lib and sseay32MT.lib are causing this with VS2015 because they have been compiled with a previous version of MSVC, according to :
    -   https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2

    * TODO : recompile openssl with the right version of MSVC
*/
    /*FILE _iob[] = { *stdin, *stdout, *stderr };

    extern "C" FILE * __cdecl __iob_func(void)
    {
        return _iob;
    }*/

#endif // _MSC_VER

#include <iostream>
#include "HTTPRequest.h"
#include <exception>
#include <string>

int main()
{
    std::cout << "Hello world!" << std::endl;
    try {
        //std::cout << HTTPRequest::uploadFile("https://vae.li/565aaee9929758_uploadfile.php",443, "", "test.txt");
        std::cout << HTTPRequest::get("http://perdu.com") << std::endl;
        std::cout << HTTPRequest::get("https://google.ch") << std::endl;


    }

    catch(std::exception const& e)  {
        std::cerr << "exception caught: " << e.what() << '\n';
    }

    //int i = 0;
    //std::cin >>  i;
return 0;
}

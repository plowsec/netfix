#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#if defined(WIN32)
#include <winsock2.h>

#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //types in_addr, in_addr_t, in_port_t
#include <unistd.h>
#include <netdb.h> //gethostbyname

//compatibility with winsock2.h
#define INVALID_SOCKET -1 //constant defined in winsock2.h
#define SOCKET_ERROR -1 //constant defined in winsock2.h
#define closesocket(s) close(s) //closesocket is from winsock2.h as well

//type definitions of Microsoft
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
//Windef.h
typedef int SOCKET;
#else
#error : not available for your platform (yet)
#endif

#include <iostream>

class HTTPRequest
{
    public:
        HTTPRequest();
        virtual ~HTTPRequest();
        static std::string get(std::string url, int port=0, std::string opt_headers="");

    protected:
    private:
        static void initWS();
        static void endWS();
        static int sock_error();
        static SOCKET createSocket(std::string hostname, int port);
        static std::string simpleRequest(std::string content, SOCKET *sock);
        static std::string secureRequest(std::string content, SOCKET *sock);

        static bool isSchemaMissing(std::string url);
        static bool isHTTPS(std::string url);
        static std::string getHost(std::string url);
        static std::string getResource(std::string url);
        static int getPort(std::string url, int port);
        static std::size_t findFirstSlash(std::string url);
};

#endif // HTTPREQUEST_H

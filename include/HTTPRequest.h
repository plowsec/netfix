#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>

#if defined(linux)
//Windef.h
typedef int SOCKET ;
#endif

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
        static int create_socket(std::string hostname, int port);
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

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>

class HTTPRequest
{
    public:
        HTTPRequest();
        virtual ~HTTPRequest();
        static std::string request();
        static std::string get(std::string url, int port=80, std::string opt_headers="");

    protected:
    private:
        static void init();
        static void end();
        static int sock_error();
        static bool isSchemaMissing(std::string url);
        static bool isHTTPS(std::string url);
        static std::string getHost(std::string url);
        static std::string getResource(std::string url);
        static std::size_t findFirstSlash(std::string url);
};

#endif // HTTPREQUEST_H

#ifdef WIN32
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
typedef struct sockaddr_in SOCKADDR_IN ;
typedef struct sockaddr SOCKADDR ;
typedef struct in_addr IN_ADDR ;
#else
#error : not available for your platform (yet)
#endif

#include "HTTPRequest.h"
#include "Exception.h"
#include "Constants.h"

#include <exception>
#include <errno.h>
#include <openssl/ssl.h>

HTTPRequest::HTTPRequest()
{
    //ctor
}

HTTPRequest::~HTTPRequest()
{
    //dtor
}

bool HTTPRequest::isSchemaMissing(std::string url)  {

    std::string schema = "http";
    if(url.substr(0, schema.size()).compare(schema) !=0)
        return true;

    return false;
}

bool HTTPRequest::isHTTPS(std::string url)  {

    std::string criteria = "https";

    if(url.substr(0, criteria.size()).compare(criteria) == 0)
        return true;

    return false;
}

std::size_t HTTPRequest::findFirstSlash(std::string url) {

    std::string schema = "http";
    std::size_t pos = url.find("://");

    //we search for the first '/' after the part 'http://' for example
    if(pos != std::string::npos && (pos == schema.size() || pos == schema.size()+1)) {

        return url.find_first_of("/", pos+3);
    }

    throw Exception("Given URL is invalid");

}

std::string HTTPRequest::getHost(std::string url)   {
    std::size_t slash_pos = 0;
    std::size_t pos = url.find("://");

    //we search for the first '/' after the part 'http://' for example

    if((slash_pos = findFirstSlash(url)) != std::string::npos)

        //found the first '/', no we return everything between 'http://' and the previously found '/'
        return url.substr(pos+3, slash_pos - (pos + 3));

    //there is no '/' in the given url, except in the access method (http://), let's check if there is a '?'
    else if((slash_pos = url.find_first_of("?")) != std::string::npos)
        return url.substr(pos+3, slash_pos - (pos + 3));

    else
        return url.substr(pos+3);
}

std::string HTTPRequest::getResource(std::string url)   {

    std::size_t slash_pos = 0;

    //if the first '/' is found, return eveything from its position
    if((slash_pos = findFirstSlash(url)) != std::string::npos)
        return url.substr(slash_pos);

    //if a '?' is found, return everything from its position
    else if((slash_pos = url.find_first_of("?")) != std::string::npos)
        return "/" + url.substr(slash_pos);

    //no resource
    return std::string("/");

}

int HTTPRequest::getPort(std::string url, int port) {

    if(port == 0)    {

        if(isHTTPS(url))
            return Constants::DEFAULT_HTTPS_PORT;

        return Constants::DEFAULT_HTTP_PORT;
    }

    return port;
}

std::string HTTPRequest::get(std::string url, int port, std::string opt_headers)    {

    std::string hostname = "";
    std::string resource = "";
    std::string response = "";

    int httpPort = 0;
    SOCKET sock = 0;

    if(isSchemaMissing(url))
        throw Exception("Missing schema. Perhaps you meant http://" + url + " ?");

    hostname = getHost(url);
    resource = getResource(url);
    httpPort = getPort(url, port);

    std::cout << "[*] Host : " << hostname << std::endl;
    std::cout << "[*] Resource : " << resource << std::endl;
    std::cout << "[*] Port : " << httpPort << std::endl;

    std::string http_request = "";
    http_request += "GET /" + resource + " HTTP/1.1\n";
    http_request += "Host: " + hostname + "\n";
    http_request += "Connection: close\n";
    http_request += "\n";

    //Windows specific code
    initWS();

    sock = create_socket(hostname, httpPort);

    if(isHTTPS(url))    {
        std::cout << "[*] SSL : yes" << std::endl;
        response = secureRequest(http_request, &sock);
    }

    else    {
        std::cout << "[*] SSL : no" << std::endl;
        response = simpleRequest(http_request, &sock);
    }

    closesocket(sock);
    endWS();

    return response;

}

SOCKET HTTPRequest::create_socket(std::string hostname, int port)   {

    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = {0};
    SOCKET sock = 0;

    /*creation of the socket.
        ** domain : AF_INET => protocol TCP/IP
        ** type of socket : SOCK_STREAM if TCP (continous stream such as files or pipes, SOCK_DGRAM if UDP (chunks)
        ** protocol : 0 to let the OS choose the most appropriate protocol
    */

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == INVALID_SOCKET)  {
        int error = sock_error();
        throw Exception("socket() failed. Error code : ", error);
    }

    hostinfo = gethostbyname(hostname.c_str());

    if(hostinfo == NULL)    {

        throw Exception("Unknown host : " + hostname);
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;

    if(connect(sock, (SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)  {
        int error = sock_error();
        throw Exception("connect() failed. Error code : ", error);
    }

    return sock;
}

std::string HTTPRequest::simpleRequest(std::string content, SOCKET *sock) {

    char buffer[Constants::RESPONSE_MAX_SIZE];
    int n = 0;

    if(send(*sock, content.c_str(), content.size(), 0) < 0)    {
        int error = sock_error();
        throw Exception("send() failed. Error code : ", error);
    }

    if((n = recv(*sock, buffer, sizeof(buffer) -1, 0)) < 0)  {
        int error = sock_error();
        throw Exception("send() failed. Error code : ", error);
    }

    buffer[n] = '\0';

    return std::string(buffer);
}

std::string HTTPRequest::secureRequest(std::string content, SOCKET *sock) {

    char buffer[Constants::RESPONSE_MAX_SIZE];
    int bytes = 0;

    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());

    if(ssl_ctx == NULL) {
        std::cout << "fail" << std::endl;
    }

    SSL *conn = SSL_new(ssl_ctx);
    SSL_set_fd(conn, *sock);

    if(SSL_connect(conn) != 1)  {
        std::cout << "failsdsd" << std::endl;
    }

    SSL_write(conn, content.c_str(), strlen(content.c_str()));

    bytes = SSL_read(conn, buffer, sizeof(buffer));
    buffer[bytes] = 0;

    SSL_free(conn);
    SSL_CTX_free(ssl_ctx);

    return std::string(buffer);
}

int HTTPRequest::sock_error()   {
//allow retrieval of error code in a portable way
#ifdef WIN32
    return WSAGetLastError();
#elif defined(linux)
    return errno;
#endif
}

void HTTPRequest::initWS()    {
#ifdef WIN32

    //the WSAData struct will be filled by WSAStartup and will contain informations about the Windows Socket implementation
    WSADATA wsaData;

    //WSAStartup initiates the use of the Winsock DLL by our process.
    //First parameter : the version of Windows Socket requested. WORD => unsigned int. MAKEWORD(lowbyte, highbyte)
    //Second parameter : a pointer to the WSAData structure that will receive the details of the Winsock implementation
    int returnValue = WSAStartup(MAKEWORD(2,2), &wsaData);

    if(returnValue != 0)    {
        throw Exception("WSAStartup failed and returned : ",returnValue);
    }
#endif
}

void HTTPRequest::endWS() {
#ifdef WIN32

    //Terminate the use of the Winsock.dll
    WSACleanup();

#endif
}






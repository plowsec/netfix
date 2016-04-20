
//if the compiler is MSVC (as MSC_VER is also set by Intel Compiler, the condition had to be adapted)
#if _MSC_VER && !__INTEL_COMPILER
//#include "stdafx.h"
//if your compiler ignore the line below, simply link ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
//libeay32 and ssleay32

//gethostbyname is deprecated according to MSVC. We don't care.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // _MSC_VER


#include "HTTPRequest.h"
#include "Exception.h"
#include "Constants.h"
#include "Files.h"

#include <exception>
#include <errno.h>
#include <string.h>
//#include <openssl/ssl.h>
/*#include <wolfssl/ssl.h>*/
#include <wolfssl/openssl/ssl.h>

//hello
HTTPRequest::HTTPRequest()
{
    //ctor
}

HTTPRequest::~HTTPRequest()
{
    //dtor
}

void HTTPRequest::addHeaderField(const std::string& fieldName, const std::string& fieldValue, std::string *http_request) {
    *http_request += fieldName + Constants::HTTP_HEADER_FIELD_SEPARATOR + fieldValue + Constants::CARRIAGE_RETURN;
}

void HTTPRequest::initRequestHeader(const std::string& method, const std::string& resource, std::string *http_request)  {
    *http_request += method + Constants::SPACE + resource + Constants::SPACE + Constants::HTTP_VERSION + Constants::CARRIAGE_RETURN;
}

bool HTTPRequest::isSchemaMissing(std::string url)  {

    std::string schema = Constants::HTTP_SCHEMA;

    if(url.substr(0, schema.size()).compare(schema) != 0)
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

    //if port != 0, it means that the user specified a port. In case he did not, we will use the default ports (80 or 443)
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

    sock = createSocket(hostname, httpPort);

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

std::string HTTPRequest::post(std::string url, int port, std::string opt_headers, std::string content)    {

    //TO COMPLETE AND CLEAN
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

    sock = createSocket(hostname, httpPort);

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

std::string HTTPRequest::uploadFile(std::string url, int port, std::string opt_headers, std::string filepath)    {

    std::string hostname = "";
    std::string resource = "";
    std::string response = "";
    std::string fileContent = "";
    std::string http_request = "";
    std::string http_body = "";

    int httpPort = 0;
    SOCKET sock = 0;

    if(isSchemaMissing(url))
        throw Exception("Missing schema. Perhaps you meant http://" + url + " ?");

    try {
        fileContent = Files::readFile(filepath);
    }
    catch(std::exception const& e)  {
        throw;
    }

    hostname = getHost(url);
    resource = getResource(url);
    httpPort = getPort(url, port);

    //size / replace, condition
    //define a function for this

    initRequestHeader(Constants::HTTP_METHOD_POST, resource, &http_request);
    addHeaderField(Constants::HTTP_HEADER_FIELD_HOST, hostname, &http_request);
    addHeaderField(Constants::HTTP_HEADER_FIELD_CONTENT_TYPE, Constants::HTTP_HEADER_MULTIPART, &http_request);
    addHeaderField(Constants::HTTP_HEADER_FIELD_CONTENT_LENGTH, StringUtils::intToString(231), &http_request);

    //body of request
    http_request += Constants::HTTP_BOUNDARY_START + Constants::CARRIAGE_RETURN;
    http_request += "Content-Disposition: form-data; name=\"" + Constants::FILE_POST_PARAMETER_NAME +"\";";
    http_request += " filename=\"" + filepath + "\"" + Constants::CARRIAGE_RETURN;
    http_request += "Content-Type: plain/text" + Constants::CARRIAGE_RETURN + Constants::CARRIAGE_RETURN;
    http_request += fileContent + Constants::CARRIAGE_RETURN+Constants::CARRIAGE_RETURN;
    http_request += "--" + Constants::HTTP_BOUNDARY + "--" + Constants::CARRIAGE_RETURN;
    //http_request += "Connection: close\n";
    //http_request += Constants::CARRIAGE_RETURN;

    std::cout << http_request << std::endl;

    //Windows specific code
    initWS();

    sock = createSocket(hostname, httpPort);

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
SOCKET HTTPRequest::createSocket(std::string hostname, int port)   {

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

    /*
    * NB : we are using wolfSSL with OpenSSL compatibility. It means that you can simply change the include at
    * the top in order to switch to openssl without changing a single line of code here.
    * Obviously, don't forget to update your linking options (-lssl -lcrypto for openssl, -lwolfssl for wolfssl)
    */

    char buffer[Constants::RESPONSE_MAX_SIZE];
    int bytes = 0;

    int errorCode = 0;
    SSL_load_error_strings();
    SSL_library_init();
    //SSLv23_client_method is deprecated (and not exportedin the libs I compiled...)

    //wolfSSL_Init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(TLSv1_1_client_method());


    /*wolfSSL_Init();
    WOLFSSL_CTX *ssl_ctx = wolfSSL_CTX_new(wolfTLSv1_1_client_method());*/
    if(ssl_ctx == NULL) {
        std::cout << "fail" << std::endl;
    }

    //line below mandatory to avoid error 188 (we are not verifying certs yet)
    /*
    *
    * From the github page : https://github.com/cyassl/cyassl
    * CyaSSL takes a different approach to certificate verification than OpenSSL does.
    * The default policy for the client is to verify the server, this means that if
    * you don't load CAs to verify the server you'll get a connect error, no signer
    * error to confirm failure (-188).
    */
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, 0);


    SSL *conn = SSL_new(ssl_ctx);
    SSL_set_fd(conn, *sock);

    if((errorCode = SSL_connect(conn)) != 1)  {

    /*WOLFSSL* ssl;
    if((ssl = wolfSSL_new(ssl_ctx))==NULL)  {*/
        std::cout << "failsdsd" << std::endl;
        int errcode = SSL_get_error(conn, errorCode);
        std::cout << errcode << std::endl;

    }

    //wolfSSL_set_fd(ssl, *sock);

    bytes = SSL_write(conn, content.c_str(), strlen(content.c_str()));

    /*
    * error checking for later :

    char errorString[80];
    int err = SSL_get_error(conn, bytes);
    ERR_error_string(err, errorString);

    */

    //wolfSSL_write(ssl, content.c_str(), strlen(content.c_str()));

    bytes = SSL_read(conn, buffer, sizeof(buffer));

    //bytes = wolfSSL_read(ssl, buffer, sizeof(buffer));
    buffer[bytes] = 0;

    SSL_free(conn);
    SSL_CTX_free(ssl_ctx);
/*
    wolfSSL_free(ssl);
    wolfSSL_CTX_free(ssl_ctx);
    wolfSSL_Cleanup();*/

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






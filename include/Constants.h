#include <iostream>
#include <string>

namespace Constants {

    const int DEFAULT_HTTP_PORT = 80;
    const int DEFAULT_HTTPS_PORT = 443;
    const int RESPONSE_MAX_SIZE = 4096;
    const std::string HTTP_BOUNDARY = "---------------------------7da24f2e50046";
    const std::string HTTP_BOUNDARY_START = "-----------------------------7da24f2e50046";
    const std::string HTTP_BOUNDARY_END = "-----------------------------7da24f2e50046--";
    const std::string HTTP_METHOD_POST = "POST";
    const std::string HTTP_METHOD_GET = "GET";
    const std::string HTTP_VERSION = "HTTP/1.1";
    const std::string HTTP_HEADER_FIELD_SEPARATOR = ": ";
    const std::string HTTP_SCHEMA = "http";
    const std::string HTTP_HEADER_FIELD_HOST = "Host";
    const std::string HTTP_HEADER_FIELD_CONTENT_TYPE = "Content-Type";
    const std::string HTTP_HEADER_FIELD_CONTENT_LENGTH = "Content-Length";
    const std::string HTTP_HEADER_MULTIPART = "multipart/form-data; boundary="+HTTP_BOUNDARY;
    const std::string CARRIAGE_RETURN = "\r\n";
    const std::string SPACE = " ";
    const std::string FILE_POST_PARAMETER_NAME = "gift_delivery";
};

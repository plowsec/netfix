#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <string>

class StringUtils
{
public:
	StringUtils();
	virtual ~StringUtils();

    static std::string &ltrim(std::string &s);
    static std::string &rtrim(std::string &s);
    static std::string &trim(std::string &s);

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);
	static const std::wstring stringToWString(const std::string &s);
	static const std::string wstringToString(const std::wstring &ws);
	static std::string intToString(int number);
	static std::string replaceAllOccurences(std::string search, std::string replace, std::string subject);

protected:
private:
};


#endif // STRINGUTILS_H

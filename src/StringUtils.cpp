
//if the compiler is MSVC (as MSC_VER is also set by Intel Compiler, the condition had to be adapted)
#if _MSC_VER && !__INTEL_COMPILER
//#include "stdafx.h"
#endif // _MSC_VER

#include "StringUtils.h"
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


StringUtils::StringUtils() {}
StringUtils::~StringUtils() {}

//https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string& StringUtils::ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
std::string& StringUtils::rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
std::string& StringUtils::trim(std::string &s) {
        return ltrim(rtrim(s));
}

//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string>& StringUtils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> StringUtils::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

const std::wstring StringUtils::stringToWString(const std::string &s) {
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

const std::string StringUtils::wstringToString(const std::wstring &ws) {
	std::string sTmp(ws.begin(), ws.end());
	return sTmp;
}

std::string StringUtils::intToString(int number) {
	std::ostringstream ss;
	ss << number;
    return ss.str();
}

std::string StringUtils::replaceAllOccurences(std::string search, std::string replace, std::string subject)
{
	std::string::size_type n = 0;
	while ((n = subject.find(search, n)) != std::string::npos)
	{
		subject.replace(n, search.size(), replace);
		n += replace.size();
	}

	return subject;
}


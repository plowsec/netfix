
#include "StringUtils.h"


StringUtils::StringUtils() {}
StringUtils::~StringUtils() {}

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


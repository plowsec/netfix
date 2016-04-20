#ifndef FILES_H
#define FILES_H

#include <cstdlib> //for malloc and free
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#if defined(WIN_32)
#include <windows.h>
#define _WIN32_IE 0x0500 //SHGFP_TYPE_CURRENT
#include <ShlObj.h>
#include <Shlwapi.h>
#endif

#include "StringUtils.h"

#define MAX_LENGTH 256

class Files
{
public:
	Files();
	virtual ~Files();

	static std::string readFile(std::string path);
	static void writeFile(std::string path, std::string content);
	static std::vector<std::string> getFileNamesInDirectory(std::string directory);
	static std::string getPathFromCSIDL(int id);
	static std::string getTempDir();

protected:
private:
};

#endif // FILES_H

//if the compiler is MSVC (as MSC_VER is also set by Intel Compiler, the condition had to be adapted)
#if _MSC_VER && !__INTEL_COMPILER
//#include "stdafx.h"
#endif // _MSC_VER


#include "Files.h"
#include "Exception.h"

Files::Files() {}
Files::~Files() {}

std::string Files::readFile(std::string path) {

	std::string content;
	std::string line;
	std::ifstream myfile(path.c_str());
	if (myfile.is_open()) {

		while (getline(myfile, line)) {
			content += line;
		}
		myfile.close();
		return content;
	}

	throw Exception("Error while opening file : " + path);
}

void Files::writeFile(std::string path, std::string content) {

    //it ain't cross-platform dude
	std::ofstream file(path.c_str());
	std::string my_string = content;
	file << my_string;
}
std::vector<std::string> Files::getFileNamesInDirectory(std::string directory) {

	//source = http://www.dreamincode.net/forums/topic/367121-List-all-file-names-in-a-directory/

    //we need to make this cross-plaform
	std::vector<std::string> files;
    #if defined(WIN_32)
	WIN32_FIND_DATA fileData;
	HANDLE hFind;

	if (!((hFind = FindFirstFile(directory.c_str(), &fileData)) == INVALID_HANDLE_VALUE)) {
		while (FindNextFile(hFind, &fileData)) {
			files.push_back(fileData.cFileName);
		}
	}

	FindClose(hFind);
	#endif
	return files;
}

std::string Files::getPathFromCSIDL(int id) {

    #if defined(WIN_32)
	TCHAR returnedPath[MAX_PATH];
	std::string returnedString = "";
	HRESULT hr = SHGetFolderPath(NULL, id | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, returnedPath);

	if (SUCCEEDED(hr)) {
		std::string tempReturnedPath = returnedPath;
		return tempReturnedPath;
	}

	/*
	//if above won't compile, comment it and then this :
	if (SUCCEEDED(hr))
	returnedString = StringUtils::wstringToString(tempReturnedPath);

	return returnedString;

	*/
	#endif
	return std::string();
}

std::string Files::getTempDir() {

	char *buffer = (char*)malloc(MAX_LENGTH);
	std::string temp_dir;

    #if defined(WIN_32)
	if (GetTempPath(MAX_LENGTH, buffer) != 0) {
		temp_dir = std::string(buffer);
		free(buffer);
		return temp_dir;
	}


    #endif
	free(buffer);
	throw std::string("getTempDir failed");
}

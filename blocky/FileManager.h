#pragma once
#include <string>
#include <fstream>

namespace FileManager{
	std::string readLine(std::string path, int num);
	void writeLine(std::string path, std::string mess, int num);
	int getLineLength(std::string path, int num);
	bool isFile(std::string path);
	void openFile(std::string path);
	void deleteLine(std::string path, int index);
}	

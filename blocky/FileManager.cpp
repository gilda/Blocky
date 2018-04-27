#include "FileManager.h"

namespace FileManager{

	// returns a string with current line. zero based line numbering
	std::string readLine(std::string path, int num){
		std::fstream file(path, std::ios::in|std::ios::out);
		std::string str;
		
		for(int i = 0; i<=num; i++){
			std::getline(file, str);
		}

		file.close();
		return str;
	}

	// writes mess to path file at specified line
	void writeLine(std::string path, std::string mess, int num){
		std::fstream file(path, std::ios::in|std::ios::out);
		std::string str;
		
		file.seekg(0);
		for(int i = 0; i<num; i++){
			std::getline(file, str);
		}
		file.seekp(file.tellg());

		file.write(mess.c_str(), mess.length());
		file.close();
	}

	// returns the length of the string in line no. num
	int getLineLength(std::string path, int num){
		return FileManager::readLine(path, num).length();
	}

}

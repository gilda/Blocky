#include "FileManager.h"

namespace FileManager{

	// returns a string with current line. zero based line numbering
	// if no such file exists it will not create a new one and will ret ""
	std::string readLine(std::string path, int num){
		if(FileManager::isFile(path)){
			std::fstream file(path, std::ios::in|std::ios::out);
			std::string str;
		
			for(int i = 0; i<=num; i++){
				std::getline(file, str);
			}

			file.close();
			return str;
		}else{
			return "";
		}
	}

	// writes mess to path file at specified line
	// if no file exists it will open a new one
	void writeLine(std::string path, std::string mess, int num){
		if(FileManager::isFile(path)){
			std::fstream file(path, std::ios::in|std::ios::out);
			std::string str;
		
			file.seekg(0);
			for(int i = 0; i<num; i++){
				std::getline(file, str);
			}
			file.seekp(file.tellg());

			file.write(mess.c_str(), mess.length());
			file.close();
		}else{
			FileManager::openFile(path);
			
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
	}

	// returns the length of the string in line number num
	// if file does not exist will return false
	int getLineLength(std::string path, int num){
		return FileManager::readLine(path, num).length();
	}

	// returns true if file exists else false
	bool isFile(std::string path){
		std::ifstream ifile(path);
		return (bool)ifile;
	}

	// opens a new file with path name
	void openFile(std::string path){
		std::fstream file(path, std::ios::in|std::ios::out|std::ios::trunc);
		file.close();
	}

	// deletes the index'th line
	void deleteLine(std::string path, int index){
		// str = FileManager::readLine(index)
		// open new file and write all text but str
		// remove source file
		// rename temp file to std::streing path
	}

}

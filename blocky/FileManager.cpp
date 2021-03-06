#include "stdafx.h"
#include "FileManager.h"

namespace FileManager{

	// returns a string with current line. zero based line numbering
	// if no such file exists it will not create a new one and will ret ""
	std::string readLine(std::string path, int num){
		// check if file exists
		if(FileManager::isFile(path)){
			std::fstream file(path, std::ios::in|std::ios::out);
			std::string str;
		
			// loop until on right line
			for(int i = 0; i<=num; i++){
				std::getline(file, str);
			}

			// close file handle
			file.close();
			return str+"\n"; // return the string with line ending
		}else{
			return "";
		}
	}

	// writes mess to path file at specified line
	// if no file exists does nothing
	void writeLine(std::string path, std::string mess, int num){
		// check if file exists
		if(FileManager::isFile(path)){
			std::fstream file(path, std::ios::in|std::ios::out|std::ios::app);

			// open temporary file
			std::string tpath = path+"temp";
			FileManager::openFile(tpath);
			std::fstream tfile(tpath, std::ios::in|std::ios::out|std::ios::trunc);

			std::string str;
			mess += "\n";
			int i = 0;
			file.seekg(0, std::ios::beg);
			bool empty = true;

			// read all file but num'th line to tempo file
			// make num'th line mess
			while(empty){
				if(!std::getline(file, str)){
					empty = false;
				}
				if(i==num){
					tfile.write(mess.c_str(), mess.length());
				}else if(str!=""){
					str += "\n";
					tfile.write(str.c_str(), str.length());
				}
				i++;
			}
			
			// close file handles
			file.close();
			tfile.close();

			remove(path.c_str()); // remove original file
			rename(tpath.c_str(), path.c_str()); // rename temp to original
		}
	}

	// returns the length of the string in line number num zero based
	// if file does not exist will return false
	int getLineLength(std::string path, int num){
		return (int)FileManager::readLine(path, num).length();
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
	// zero based
	void deleteLine(std::string path, int index){
		std::fstream file(path, std::ios::in|std::ios::out);
		
		//open temp file
		std::string tpath = path+"temp";
		FileManager::openFile(tpath);
		std::fstream tfile(tpath, std::ios::in|std::ios::out|std::ios::trunc);
		
		std::string str;
		std::string erase = FileManager::readLine(path, index);
		int i=0;

		// copy all lines but index to temp file
		while(std::getline(file, str)){
			if(i!=index){
				str += "\n";
				tfile.write(str.c_str(), str.length());
			}
			i++;
		}

		// close file handles
		file.close();
		tfile.close();

		remove(path.c_str()); // remove the original file
		rename(tpath.c_str(), path.c_str()); // rename temp file to original
	}

	// returns the last line number in the file
	// zero based
	int getLastLineNum(std::string path){
		std::fstream file(path, std::ios::in|std::ios::out);
		std::string str;
		int i = 0;

		while(std::getline(file, str)){
			i++;
		}

		return i;
	}

}

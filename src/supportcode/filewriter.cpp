#include "filewriter.h"

using namespace std;

FileWriter::FileWriter() {

}

FileWriter::FileWriter(string filename) {
  file.open (filename);
  closed_file_ = false;
}

FileWriter::~FileWriter() {
    if (!closed_file_){
	file.close();
    }
}

void FileWriter::WriteLine(string line) {
    file << line;
}


void FileWriter::CloseFile(){
    if (!closed_file_){
	file.close();
    }
}

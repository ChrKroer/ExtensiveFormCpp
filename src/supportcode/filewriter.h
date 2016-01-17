#ifndef FILEWRITERH
#define FILEWRITERH
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>

class FileWriter {

public:
    FileWriter();
    FileWriter(std::string filename);
    ~FileWriter();

    void WriteLine(std::string line);
    
    void CloseFile();

private:
    std::ofstream file;

    bool closed_file_;
};

#endif

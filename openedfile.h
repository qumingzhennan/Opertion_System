#pragma once
#include "File.h"
using namespace std;
class OpenedFile
{
private:
    File *file;
public:
	OpenedFile *next;
	OpenedFile();
    OpenedFile(File *new_file);
	~OpenedFile();
    File * getFile(){ return file; }
};


#include"head.h"

OpenedFile::OpenedFile() 
{
    file = new File("", "");
	next = NULL;
}


OpenedFile::OpenedFile(File *new_file) :file(new_file)
{
	next = NULL;
}


OpenedFile::~OpenedFile()
{
}

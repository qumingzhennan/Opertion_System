#pragma once
#include<ctime>
#include <map>
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <vector>
#include <sstream>
#include "DiskManager.h"
#include "FileManager.h"
#include "OpenedFile.h"
#include "File.h"
typedef struct directoryNode directoryNode;
typedef directoryNode* directoryList;
typedef struct directoryNode {
    string name;
    directoryNode *child;
    directoryNode *brother;
    directoryNode *father;
}directoryNode;//文件节点
typedef directoryNode* directoryList;

const int STACK_SIZE = 2000;
using namespace std;
enum return_info {
    SUCCESS, RIGHT_PROBLEM, FILES_EXIST_PROBLEM, TARGET_NOT_EXIST_PROBLEM,
    OUT_OF_DISK_CPACITY_PROBLEM, SAME_NAME_EXISTS_PROBLEM, INVALID_CHARACTER,TARGET_DIRECTORY_INVALID
    ,NO_DATA_IN_BUFFER,FILE_OPENED,FCB_ERROR
};

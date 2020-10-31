#include "Utils.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using namespace std;

bool CheckOrCreateDirectory(std::string path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
        {
            return false;
        }
        else
        {
            return true;
        }
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

string GetFullPath(string path)
{
    char buf[4096];
    return string(realpath(path.c_str(), buf));
}

void GetFilesInDirectory(const string &path, vector<string> &v)
{
    for (const auto &entry : fs::directory_iterator(GetFullPath(path)))
        v.push_back(entry.path().filename());
}
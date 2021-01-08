// Copyright @ Roman Balayan [2021]

#ifndef _DUPLICATES_H_
#define _DUPLICATES_H_

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "md5.h"

namespace fs = boost::filesystem;

fs::path setDir();
std::unordered_map<size_t, std::vector<fs::directory_entry> > getFilesFromDir(fs::path dirPath);
bool cmpFiles(fs::directory_entry file1, fs::directory_entry file2);
std::vector<fs::directory_entry> findDuplicates(fs::path dir1, fs::path dir2);

#endif // _DUPLICATES_H_


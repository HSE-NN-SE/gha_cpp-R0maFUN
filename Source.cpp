// Copyright @ Roman Balayan [2021]

#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>

#include "duplicates.h"

namespace fs = boost::filesystem;

int main() {

    Directory* dir1 = new Directory;
    dir1->setDir();
    dir1->setFiles();

    Directory* dir2 = new Directory;
    dir2->setDir();
    dir2->setFiles();
    
    dir1->findDuplicates(dir2);
    dir1->printDuplicates();
    delete dir1;
    delete dir2;

    return 0;
}
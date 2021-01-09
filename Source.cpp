// Copyright @ Roman Balayan [2021]

#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>

#include "duplicates.h"

namespace fs = boost::filesystem;

int main() {

    Directory dir1;
    dir1.setDir();
    dir1.setFiles();

    Directory dir2;
    dir2.setDir();
    dir2.setFiles();
    
    dir1.findDuplicates(&dir2);
    dir1.printDuplicates();


    return 0;
}
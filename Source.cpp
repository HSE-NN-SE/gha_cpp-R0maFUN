// Copyright @ Roman Balayan [2021]

#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>

#include "duplicates.h"

namespace fs = boost::filesystem;

int main() {
    fs::path dir1 = setDir();
    fs::path dir2 = setDir();

    std::vector<fs::directory_entry> duplicates = findDuplicates(dir1, dir2);

    std::cout << duplicates.size() / 2 << " duplicates found." << std::endl;
    for (auto it = duplicates.begin(); it != duplicates.end(); ++it)
        std::cout << *it << std::endl;

    return 0;
}
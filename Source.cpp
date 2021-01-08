// Copyright @ Roman Balayan [2021]

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "duplicates.h"

namespace fs = boost::filesystem;

int main() {
    fs::path p("./test1");
    if (p.empty()) {
        std::cout << "empty dir";
        return 1;
    }

    std::unordered_map<size_t, std::vector<fs::directory_entry> > table = getFilesFromDir(p);
    for (auto val : table) {
        for(auto it = val.second.begin(); it != val.second.end(); ++it)
            std::cout << *it << std::endl;
    }

    fs::directory_entry f1 = table[13][0];
    fs::directory_entry f2 = table[30][0];

    /*std::cout << "#1 File is " << (cmpFiles(table.begin()->second[0], (table.begin()++)->second[0]) ? "" : "not") << "equal to #3 File" << std::endl;*/
    std::cout << "#1 File is " << (cmpFiles(f1, f2) ? "" : "not ") << "equal to #3 File" << std::endl;

    std::cout << "Test";
    return 0;
}
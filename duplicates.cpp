// Copyright @ Roman Balayan [2021]

#include "duplicates.h"
#include <conio.h>
#include "consoleio.h"

#define BLOCK_LEN 3

fs::path setDir()
{
    fs::path cur_path = fs::current_path();
    fs::path selected_path; // path of the directory selected in the menu
    size_t selected_id = 0; // id of the directory selected in the menu
    char c = '0';
    size_t j = 0;
    cls();
    do {

        switch (c) {
        case 's':
            selected_id = selected_id + 1 > j - 1 ? 0 : selected_id + 1;
            break;
        case 'w':
            selected_id = selected_id == 0 ? j - 1 : selected_id - 1;
            break;
        case 'd':
            cur_path = selected_path; // selecting and going inside the directory
            cls();
            break;
        case 'a':
            cur_path = cur_path.parent_path(); // getting back to the parent directory
            cls();
            break;
        case 'e':
            return cur_path;
        default:
            break;
        }
        
        j = 0;
        
        fs::directory_iterator end_itr;
        setCursorPosition(0, 0);
        std::cout << "[" << cur_path.filename() << "]          " << std::endl; // path of the directory we will return if user press 'e'
        for (fs::directory_iterator itr(cur_path); itr != end_itr; ++itr) {
            if (fs::is_directory(itr->path())) {
                setCursorPosition(0, j+1);
                if (j == selected_id)
                {
                    selected_path = itr->path();
                    std::cout << "-> ";
                }
                std::cout << itr->path().filename() << "          " << std::endl;
                ++j;
            }
        }
    } while (c = _getch());
}

std::unordered_map<size_t, std::vector<fs::directory_entry>> getFilesFromDir(fs::path dirPath)
{
    std::unordered_map<size_t, std::vector<fs::directory_entry>> table; // will store the files in the map where key is the size of file and 
                                                                        // value is the vector of files in the same directory with the same size
    if (fs::is_directory(dirPath)) {
        std::cout << dirPath << " is a directory containing:\n";
        fs::directory_iterator end_itr;
        for (fs::directory_iterator itr(dirPath); itr != end_itr; ++itr) {
            if (is_regular_file(itr->path())) { // will not save the directories
                size_t file_size = fs::file_size(itr->path());
                std::cout << "Found file - " << itr->path().filename() << " [" << file_size << " bytes]" << std::endl;
                if (table.count(file_size) > 0) // if we already have the files with the same size
                    table[file_size].push_back(*itr);
                else
                    table[file_size] = std::vector<fs::directory_entry>(1, *itr); // if not - creating the vector which contains single file
            }
        }
    }
    return table;
}

bool cmpFiles(fs::directory_entry file1, fs::directory_entry file2)
{
    if (fs::file_size(file1.path()) != fs::file_size(file2.path()))
        return false;
    char* buf = new char[BLOCK_LEN + 1];
    std::ifstream is1(file1.path().c_str(), std::ifstream::binary); // input stream for the first file
    std::ifstream is2(file2.path().c_str(), std::ifstream::binary); // input stream for the second file
    while (!is1.eof()) {
        is1.read(buf, BLOCK_LEN); // reading block from the first file
        if (is1)
            buf[BLOCK_LEN] = '\0';
        else
            buf[is1.gcount()] = '\0';
        std::string hashed1 = md5(buf); // hashing the block

        is2.read(buf, BLOCK_LEN); // reading block from the second file
        if (is2)
            buf[BLOCK_LEN] = '\0';
        else
            buf[is2.gcount()] = '\0';
        std::string hashed2 = md5(buf); // hashing the block
        if (hashed1 != hashed2) // comparing the hashes of blocks from different files
            return false;
    }
    return true;
}

std::vector<fs::directory_entry> findDuplicates(fs::path dir1, fs::path dir2)
{
    std::unordered_map<size_t, std::vector<fs::directory_entry> > firstDirFiles  = getFilesFromDir(dir1);
    std::unordered_map<size_t, std::vector<fs::directory_entry> > secondDirFiles = getFilesFromDir(dir2);
    std::vector<fs::directory_entry> duplicates;

    for (auto pair : firstDirFiles) {
        // does second dir contain files with the same size?
        if (secondDirFiles.count(pair.first) > 0) {
            // going through the files with the same size in both directories
            for (auto firstDirFile = pair.second.begin(); firstDirFile != pair.second.end(); ++firstDirFile) {
                for (auto secondDirFile = secondDirFiles[pair.first].begin(); secondDirFile != secondDirFiles[pair.first].end(); ++secondDirFile) {
                    if (cmpFiles(*firstDirFile, *secondDirFile)) {
                        duplicates.push_back(*firstDirFile);
                        duplicates.push_back(*secondDirFile);
                    }
                }
            }
        }
    }
    return duplicates;
}

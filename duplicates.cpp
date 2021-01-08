// Copyright @ Roman Balayan [2021]

#include "duplicates.h"
#include <conio.h>
#include "consoleio.h"

#define BLOCK_LEN 1024 // length of the blocks to hasn in bytes
#define MAX_FILES_DISPLAY 9 // amount of directories displayed in the menu

fs::path setDir()
{
    fs::path cur_path = fs::current_path(); // path of the current directory
    fs::path selected_path; // path of the sub-directory selected in the menu
    size_t selected_id = 0; // id of the sub-directory selected in the menu
    char c = '0'; // pressed key
    size_t j = 0;
    size_t offset = 0; // need to display maximum amount(9) of files (defined)
    size_t total_directories = 0; // amount of sub-directories of the selected directory
    cls();
    do {

        switch (c) {
        case 's':
            if (total_directories > MAX_FILES_DISPLAY) {
                if (selected_id + offset + 1 >= total_directories) {
                    offset = 0;
                    selected_id = 0;
                }
                else if (selected_id + 1 == MAX_FILES_DISPLAY)
                    ++offset;
                else
                    ++selected_id;
            }
            else
                selected_id = selected_id + 1 > j - 1 ? 0 : selected_id + 1;
            break;
        case 'w':
            if (total_directories > MAX_FILES_DISPLAY) {
                if (selected_id == 0 && offset > 0) {
                    offset--;
                }
                else if (selected_id == 0 && offset == 0)
                {
                    offset = total_directories - MAX_FILES_DISPLAY;
                    selected_id = MAX_FILES_DISPLAY - 1;
                }
                else
                    --selected_id;
            }
            else
                selected_id = selected_id == 0 ? j - 1 : selected_id - 1;
            break;
        case 'd':
            cur_path = selected_path; // selecting and going inside the directory
            selected_id = 0;
            offset = 0;
            cls();
            break;
        case 'a':
            cur_path = cur_path.parent_path(); // getting back to the parent directory
            selected_id = 0;
            offset = 0;
            cls();
            break;
        case 'e':
            return cur_path;
        default:
            break;
        }
        
        j = 0;
        total_directories = 0;
        // Printing sub-directories of the selected directory
        fs::directory_iterator end_itr;
        setCursorPosition(0, 0);
        std::cout << "[" << cur_path.filename() << "]          " << std::endl; // path of the directory we will return if user press 'e'
        for (fs::directory_iterator itr(cur_path); itr != end_itr; ++itr) {
            if (fs::is_directory(itr->path())) {
                // show only the directories in the needed range
                if (j < offset || j - offset >= MAX_FILES_DISPLAY) {
                    ++j;
                    ++total_directories;
                    continue;
                }
                setCursorPosition(0, j-offset+1); // works the way faster than system("cls")
                if (j - offset == selected_id)
                {
                    selected_path = itr->path();
                    std::cout << "-> ";
                }
                std::cout << itr->path().filename() << "                                                       " << std::endl; // Spaces for cleaning the line
                ++j;
                ++total_directories;
            }
        }

        // Information footer
        std::cout << std::endl;
        std::cout << "[w] - to go up" << std::endl
                  << "[s] - to go down" << std::endl
                  << "[d] - to go inside the directory" << std::endl
                  << "[a] - to go back from the directory" << std::endl
                  << "[e] - to select the directory" << std::endl;
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

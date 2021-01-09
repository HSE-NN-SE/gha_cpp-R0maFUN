// Copyright @ Roman Balayan [2021]

#include "duplicates.h"
#include <conio.h>
#include "consoleio.h"

const size_t BLOCK_LEN = 1024; // length of the blocks to hasn in bytes
const size_t MAX_FILES_DISPLAY = 9; // amount of directories displayed in the menu

Directory::Directory(fs::path dirPath) : path(dirPath)
{
    if (fs::is_directory(this->path)) {
        std::cout << this->path << " is a directory containing:\n";
        for(fs::recursive_directory_iterator itr(this->path), end_itr; itr != end_itr; ++itr){
            if (is_regular_file(itr->path())) { // will not save the directories
                size_t file_size = fs::file_size(itr->path());
                std::cout << "Found file - " << itr->path().filename() << " [" << file_size << " bytes]" << std::endl;
                this->files.insert(std::pair<size_t, File*>(file_size, new File(*itr)));
            }
        }
    }
}

Directory::~Directory()
{
    for (auto el : this->files)
        delete el.second;
}

void Directory::setDir()
{
    fs::path* cur_path = new fs::path;
    *cur_path = fs::current_path(); // path of the current directory
    fs::path* selected_path = new fs::path; // path of the sub-directory selected in the menu
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
            *cur_path = cur_path->parent_path(); // getting back to the parent directory
            selected_id = 0;
            offset = 0;
            cls();
            break;
        case 'e':
            //return cur_path;
            this->path = *cur_path;
            if(cur_path == selected_path)
                delete cur_path;
            else {
                delete selected_path;
                delete cur_path;
            }
            return;
        default:
            break;
        }

        j = 0;
        total_directories = 0;
        // Printing sub-directories of the selected directory
        fs::directory_iterator end_itr;
        setCursorPosition(0, 0);
        std::cout << "[" << cur_path->filename() << "]          " << std::endl; // path of the directory we will return if user press 'e'
        for (fs::directory_iterator itr(*cur_path); itr != end_itr; ++itr) {
            if (fs::is_directory(itr->path())) {
                // show only the directories in the needed range
                if (j < offset || j - offset >= MAX_FILES_DISPLAY) {
                    ++j;
                    ++total_directories;
                    continue;
                }
                setCursorPosition(0, j - offset + 1); // works the way faster than system("cls")
                if (j - offset == selected_id)
                {
                    *selected_path = itr->path();
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

void Directory::setFiles()
{
    if (fs::is_directory(this->path)) {
        std::cout << this->path << " is a directory containing:\n";
        for (fs::recursive_directory_iterator itr(this->path), end_itr; itr != end_itr; ++itr) {
            if (is_regular_file(itr->path())) { // will not save the directories
                size_t file_size = fs::file_size(itr->path());
                std::cout << "Found file - " << itr->path().filename() << " [" << file_size << " bytes]" << std::endl;
                this->files.insert(std::pair<size_t, File*>(file_size, new File(*itr)));
            }
        }
    }
}

void Directory::findDuplicates(Directory* secondDir)
{
    for (auto pair : this->files) {
        // does second dir contain files with the same size?
        if (secondDir->files.count(pair.first) > 0) {
            // going through the files with the same size in the second directory
            auto range = secondDir->files.equal_range(pair.first); // iterators of the files in the second dir with the same size
            for (auto secDirFileItr = range.first, end_itr = range.second; secDirFileItr != end_itr; ++secDirFileItr) {
                if (*pair.second == *secDirFileItr->second)
                    duplicates.push_back(std::pair<File*, File*>(pair.second, secDirFileItr->second));
            }
        }
    }
}

void Directory::printDuplicates()
{
    std::cout << this->duplicates.size() << " duplicates found:" << std::endl;
    for (auto it = this->duplicates.begin(); it != this->duplicates.end(); ++it)
        std::cout << it->first->getPath() << "\n" << it->second->getPath() << "\n\n";
}

bool File::operator==(File& file2)
{
    if (fs::file_size(this->value.path()) != fs::file_size(file2.value.path()))
        return false;
    char* buf = new char[BLOCK_LEN + 1];
    std::ifstream is1(this->value.path().c_str(), std::ifstream::binary); // input stream for the first file
    std::ifstream is2(file2.value.path().c_str(), std::ifstream::binary); // input stream for the second file
    size_t block_id = 0;
    size_t blocks_amount = fs::file_size(this->value.path()) / BLOCK_LEN;
    while (!is1.eof() && block_id < blocks_amount) {
        std::string hashed1;
        std::string hashed2;

        // does we have this block?
        if (block_id >= this->md5hashes.size())
        {
            is1.seekg(BLOCK_LEN * block_id); // set cursor to the begining of the needed block
            is1.read(buf, BLOCK_LEN); // reading block from the first file
            if (is1)
                buf[BLOCK_LEN] = '\0';
            else
                buf[is1.gcount()] = '\0';
            hashed1 = md5(buf); // hashing the block
            this->md5hashes.push_back(hashed1); // save the hash for this file
        }
        else
            hashed1 = this->md5hashes[block_id]; // get saved hash
        // does we have this block?
        if (block_id >= file2.md5hashes.size()) {
            is2.seekg(BLOCK_LEN * block_id); // set cursor to the begining of the needed block
            is2.read(buf, BLOCK_LEN); // reading block from the second file
            if (is2)
                buf[BLOCK_LEN] = '\0';
            else
                buf[is2.gcount()] = '\0';
            hashed2 = md5(buf); // hashing the block
            file2.md5hashes.push_back(hashed2);
        }
        else
            hashed2 = file2.md5hashes[block_id];
        if (hashed1 != hashed2) // comparing the hashes of blocks from different files
        {
            delete buf;
            return false;
        }
        block_id++;
    }
    delete buf;
    return true;
}

fs::path File::getPath()
{
    return this->value.path();
}

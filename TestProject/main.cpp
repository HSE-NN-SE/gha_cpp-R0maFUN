#include <iostream>
#include <fstream>
#include <time.h>
#include "Genetic.h"

void addDel(std::string filename) {
    std::ifstream in(filename);
    std::ofstream out("citiesDel.csv");

    std::string line;

    while (std::getline(in, line)) {
        line += ',';
        out << line;
        out << std::endl;
    }
    out.close();
    in.close();
}

int main() {
    //addDel("cities.csv");
    srand(time(NULL));
    std::cout << "Reading the data ...\n";
    Genetic genetic("cities1000.csv", 80);
    std::cout << "Starting the algorithm...\n";
    genetic.start(10000);

    return 0;
}
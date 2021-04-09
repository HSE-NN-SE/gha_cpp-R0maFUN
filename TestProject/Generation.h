#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>

#include "Individ.h"
#include "Genetic.h"

//class Individ;

class Generation {
public:
    Generation(unsigned int individsAmount = 40) : individsAmount(individsAmount) {}
    Generation(Generation& gen);
    Generation(std::string filename = "data.txt", unsigned int individsAmount = 40); // should be in the fabric
    Generation(std::vector<std::shared_ptr<Individ> >& individs);
    void addIndivid(std::shared_ptr<Individ> ind);
    void clear();
    void sortIndivids();
    void printInfo();
private:
    std::vector<std::shared_ptr<Individ> > individs;
    Individ best_solution;
    unsigned int individsAmount;
    friend class Genetic;
};
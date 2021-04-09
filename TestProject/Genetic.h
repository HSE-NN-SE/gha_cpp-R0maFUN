#pragma once

#include "Individ.h"
#include "Generation.h"
#include "constants.h"

class Generation;

class Genetic {
public:
    Genetic(std::string filename = "cities.csv", unsigned int genSize = 50);
    void create_random_generation();
    bool make_selection(float survive_percentage = 0.5, float bad_individs_chance = 0);
    bool make_mutations(float probability);
    bool crossingover(std::uint8_t top_individs_amount, std::uint8_t tournment_size, std::uint8_t tournments_amount); // cross top4 individs + tournment
    bool getBest();
    void printBest(std::string filename = "result.txt");
    void start(std::uint16_t generations_amount = 50);
private:
    std::unique_ptr<Generation> generation;
    unsigned int genSize;
    //Individ best_solution;
    double bestWay;
};
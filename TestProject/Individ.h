#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <iostream>
//#include "functions.h"
#include <numeric>

//#include "Genetic.h"

class Individ;
class City;

using IndividPtr = std::shared_ptr<Individ>;
using CityPtr = std::shared_ptr<City>;

class City {
public:
    City(unsigned int id, float x, float y) : id(id), x(x), y(y) { }
    float calcWay(City& dest) const;
    unsigned int getId() const;
private:
    unsigned int id;
    float x, y;
    friend class Individ;
    friend class Genetic;
};

class Individ {
public:
    Individ() : cashedEval(0) {}
    Individ(std::vector<std::shared_ptr<City> >& genes);
    double evaluate();
    // return vector of children
    std::vector<IndividPtr> simple_cross(std::shared_ptr<Individ>& partner, unsigned int childrenAmount = 2);
    bool simple_mutation(unsigned int replacesAmount = 2);
    void print();
    double getEval() const;
protected:
    std::vector<std::shared_ptr<City> > genes;
    double cashedEval;
    friend class Genetic;
};

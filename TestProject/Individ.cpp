#include "Individ.h"

Individ::Individ(std::vector<std::shared_ptr<City> >& genes) : genes(genes)
                                                             , cashedEval(0){
}

double Individ::evaluate()
{
    /*return std::accumulate(this->genes.begin(), this->genes.end(), 0, [](std::shared_ptr<City>& a, std::shared_ptr<City>& b) {
        return a->calcWay(*b);
        }) + this->genes[0]->calcWay(*(this->genes[this->genes.size() - 1]));*/
    double sum = 0;
    for (int i = 0; i < this->genes.size() - 1; ++i) {
        sum += this->genes[i]->calcWay(*(this->genes[i + 1]));
    }
    sum += this->genes[0]->calcWay(*(this->genes[this->genes.size() - 1]));
    this->cashedEval = sum;
    return sum;
}

std::vector<IndividPtr> Individ::simple_cross(std::shared_ptr<Individ>& partner, unsigned int childrenAmount)
{
    std::vector<IndividPtr> result;
    if (this->evaluate() == partner->evaluate())
        return result;
    for (int i{ 0 }; i < childrenAmount; ++i) {
        Individ* individ = new Individ(this->genes);
        IndividPtr child(individ);
        
        unsigned int initId = rand() * rand() % this->genes.size();
        while(child->genes[initId]->id == partner->genes[initId]->id)
            initId = rand() * rand() % this->genes.size();
        if (initId == 0)
            initId++;
        CityPtr xCityA;
        CityPtr yCityA;
        unsigned int currentId = initId, foundId;
        int iterCounter = 0;
        do {
            xCityA = child->genes[currentId];
            yCityA = partner->genes[currentId];
            //if (xCityA->id == yCityA->id)
            //    break;
            // ищем yCity в X
            auto xCityB_iter = std::find_if(std::next(child->genes.begin()), child->genes.end(), [&yCityA](std::shared_ptr<City>& city) {
                    return city->getId() == yCityA->getId(); // .id?
                });
            if (xCityB_iter == child->genes.end()) {
                std::cout << "smth went wrong";
            }
            // currentId = id yCity в X
            foundId = xCityB_iter - child->genes.begin();

            // меняем местами xCity и yCity в первом родителе
            City tmp = *child->genes[currentId];
            *child->genes[currentId] = *child->genes[foundId];
            *child->genes[foundId] = tmp;
            //std::iter_swap(xCityA, xCityB_iter);
            //City tmp = *xCityA;
            //xCityA = *xCityB_iter;
            //**xCityB_iter = tmp;
            currentId = foundId;
            ++iterCounter;
            //system("cls");
            //std::cout << "Crossing [" << (static_cast<double>(iterCounter) / (this->genes.size() * 0.0001)) * 100 << "%]\n";
        } while (currentId != initId /*&& iterCounter < this->genes.size() * 0.0001*/); // maybe should not do that much iterations [optimization]
        child->evaluate();
        result.push_back(child);
    }

    return result;
}

bool Individ::simple_mutation(unsigned int replacesAmount)
{
    unsigned int id1 = 0, id2 = 0;
    for (int i{ 0 }; i < replacesAmount; ++i) {
        while (id1 == id2 || id1 == 0 || id2 == 0) {
            id1 = rand() * rand() % this->genes.size();
            id2 = rand() * rand() % this->genes.size();
        }
        std::iter_swap(this->genes.begin() + id1, this->genes.begin() + id2);
    }
    this->evaluate();
    return true;
}

void Individ::print()
{
    std::cout << "Way: " << (this->cashedEval ? this->cashedEval : this->evaluate()) << std::endl;
    for (auto& city : this->genes)
        std::cout << city->id << " ";
    std::cout << std::endl;
}

double Individ::getEval() const
{
    return this->cashedEval;
}

float City::calcWay(City& dest) const {
    return sqrt(pow(this->x - dest.x, 2) + pow(this->y - dest.y, 2));
}

unsigned int City::getId() const
{
    return this->id;
}

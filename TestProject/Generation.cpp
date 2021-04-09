#include "Generation.h"

std::vector<std::vector<std::string> > getNextLineAndSplitIntoTokens
(std::istream& str) {
    std::vector<std::vector<std::string> > result;
    std::string line;

    std::string cell;
    std::vector<std::string > row;
    int i = 0;
    int j = 0, k = 1;
    std::getline(str, cell);
    while (std::getline(str, cell, ',')) {
        if (i % 3 == 0) {
            result.push_back(row);
            row.clear();
            i = 0;
        }
        row.push_back(cell);
        ++i;
        if (j == 1000) {
            std::cout << j * k << "lines readed\n";
            k++;
            j = 0;
        }
        ++j;
    }

    return result;
}

Generation::Generation(Generation& gen)
{
    this->individs = gen.individs;
    this->best_solution = gen.best_solution;
    this->individsAmount = gen.individsAmount;
}

Generation::Generation(std::string filename, unsigned int individsAmount) : individsAmount(individsAmount)
{
    std::ifstream in(filename);
    //auto rows = getNextLineAndSplitIntoTokens(in);
    std::vector<CityPtr> cities;

    std::string line;

    std::string cell;
    std::vector<std::string > row;

    std::getline(in, cell);
    int i = 0;
    int j = 0, k = 1;
    while (std::getline(in, cell, ',')) {
        if (i == 3) {
            std::shared_ptr<City> city = std::make_shared<City>(std::atoi(row[0].c_str()),
                std::stof(row[1]),
                std::stof(row[2]));
            cities.push_back(city);
            row.clear();
            i = 0;
            if (j == 1000) {
                std::cout << j * k << "lines readed\n";
                k++;
                j = 0;
            }
            ++j;
        }
        row.push_back(cell);
        ++i;
    }

    /*for (auto& row : rows) {
        std::shared_ptr<City> city = std::make_shared<City>(std::atoi(row[0].c_str()),
                                                            std::stof(row[1]),
                                                            std::stof(row[2]));
        cities.push_back(city);
    }*/
    IndividPtr individ = std::make_shared<Individ>(cities);
    individ->evaluate();
    this->individs.push_back(individ);
    // make random individs
    auto rng = std::default_random_engine{};
    for (int i {0} ; i < individsAmount; ++i) {
        //std::iter_swap(cities.begin() + i + 1, cities.begin() + i + 3);
        std::shuffle(std::next(cities.begin()), cities.end(), rng);
        IndividPtr newIndivid = std::make_shared<Individ>(cities);
        newIndivid->evaluate();
        this->individs.push_back(newIndivid);
    }
}

Generation::Generation(std::vector<std::shared_ptr<Individ>>& individs) : individs(individs)
                                                                        , individsAmount(individs.size())
{
}

void Generation::addIndivid(std::shared_ptr<Individ> ind) {
    individs.push_back(ind);
    if (individs.size() > this->individsAmount)
        individs.erase(individs.begin());
    this->best_solution = *ind;
}

void Generation::clear()
{
    individs.clear();
}

void Generation::sortIndivids()
{
    std::sort(this->individs.begin(), this->individs.end(), [](std::shared_ptr<Individ>& a, std::shared_ptr<Individ>& b) {
            if (a->getEval() && b->getEval())
                return a->getEval() < b->getEval();
            return a->evaluate() < b->evaluate();
        });
}

void Generation::printInfo()
{
    for (auto& individ : this->individs)
        std::cout << "Way: " << (individ->getEval() ? individ->getEval() : individ->evaluate()) << std::endl;
}

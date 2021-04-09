#include "Genetic.h"

Genetic::Genetic(std::string filename, unsigned int genSize) : genSize(genSize)
{
    this->generation = std::make_unique<Generation>(filename, genSize);
    this->generation->sortIndivids();
    this->generation->best_solution = *this->generation->individs[0];
    this->bestWay = this->generation->best_solution.evaluate();
}

bool Genetic::make_selection(float survive_percentage, float bad_individs_chance)
{
    // 100% survive
    std::vector<IndividPtr> survivedIndivids;
    unsigned int surviveAmount = this->generation->individs.size()*survive_percentage;
    this->generation->sortIndivids();
    auto it = this->generation->individs.begin();
    for (int i{ 0 }; i < surviveAmount; ++i, ++it)
        survivedIndivids.push_back(*it);

    // survive with chance
    auto endIt = this->generation->individs.end();
    for (it; it != endIt; ++it) {
        float roulette = static_cast<double>(rand()) / (RAND_MAX);
        if (roulette <= bad_individs_chance)
            survivedIndivids.push_back(*it);
    }

    this->generation = std::make_unique<Generation>(survivedIndivids);

    return true;
}

bool Genetic::make_mutations(float probability)
{
    bool ret = false;
    for (auto& individ : this->generation->individs) {
        float roulette = static_cast<double>(rand()) / (RAND_MAX);
        if (roulette <= probability) {
            individ->simple_mutation(Constants::mutation_replaces_amount_c);
            ret = true;
        }
    }
    return ret;
}

bool Genetic::crossingover(std::uint8_t top_individs_amount, std::uint8_t tournment_size, std::uint8_t tournments_amount)
{
    /*std::cout << "Sorting..." << std::endl;
    this->generation->sortIndivids();*/
    // get top individs and cross theme
    if (this->generation->individs.size() < top_individs_amount) {
        return false;
    }

    for (int i{ 0 }; i < top_individs_amount / 2; ++i) {
        //system("cls");
        //std::cout << "Crossing [" << (static_cast<double>(i) / top_individs_amount)*2*100 << "%]\n";
        auto X = this->generation->individs[i * 2];
        auto Y = this->generation->individs[i * 2 + 1];
        auto children = X->simple_cross(Y, Constants::children_amount_c);
        for (auto& child : children)
            this->generation->individs.push_back(child);
        /*children = Y->simple_cross(X, Constants::children_amount_c);
        for (auto& child : children)
            this->generation->individs.push_back(child);*/
    }
    // make tournment with chances
    return true;
}

// return true if found new best
bool Genetic::getBest()
{
    std::cout << "CurrBest = " << this->bestWay << std::endl;
    std::cout << "Sorting..." << std::endl;
    this->generation->sortIndivids();
    double way = this->generation->individs[0]->evaluate();
    std::cout << "New way = " << way << std::endl;
    if (way < this->bestWay) {
        this->generation->best_solution = *(this->generation->individs[0]);
        bestWay = way;
        printBest();
        std::cout << "Best way: " << bestWay << std::endl;
        return true;
    }
    return false;
}

void Genetic::printBest(std::string filename)
{
    std::ofstream out(filename);
    for (auto& city : this->generation->best_solution.genes)
        out << city->id << " ";
    out.close();
}

void Genetic::start(std::uint16_t generations_amount)
{
    Generation bestGeneration(this->genSize);
    std::cout << "Sorting..." << std::endl;
    this->generation->sortIndivids();
    unsigned int localMin = 0; // Amount of iterations without finding new best
    for (int i{ 0 }; i < generations_amount; ++i) {
        //system("cls");
        //this->generation->printInfo();
        std::cout << "[#" << i << " Generation ]" << std::endl;
        std::cout << "Crossing\n";
        crossingover(this->generation->individs.size() * 0.2, 1000, 10);
        //getBest();
        system("cls");
        std::cout << "Mutations\n";
        make_mutations(static_cast<double> (3) / this->generation->individs.size());
        if (!getBest()) {
            ++localMin;
        } else {
            localMin = 0;
            bestGeneration.addIndivid(this->generation->individs[0]);
        }
        if (localMin >= 40 && bestGeneration.individs.size() > this->genSize / 3) {
            this->generation = std::make_unique<Generation>(bestGeneration);
            this->generation->sortIndivids();
            bestGeneration.clear();
            localMin = 0;
        }
        std::cout << "Selection\n";
        if(this->generation->individs.size() > this->genSize / 2)
            make_selection(0.8, 0.1);
    }
}

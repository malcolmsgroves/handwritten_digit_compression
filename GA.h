#ifndef __GA_h
#define __GA_h

#include "Individual.h"
#include <vector>
#include <string>

#include "NN.h"

using namespace std;

enum SelectionType {TOURNAMENT, RANK, BOLTZMANN};
enum CrossoverType {UNIFORM, ONEPOINT};

class GA {

public:
    GA(int populationSize,
       string selectionType,
       string crossoverType,
       double crossoverProbability,
       double mutationProbability,
       int generationNumber,
       vector <vector <int> > allClauses,
       int numVariables);
    
    void runGA();
    
private:
    
    vector <vector <int> > clauses;
    vector <individual> population;
    vector <individual> breeding_population;
    double crossover_probability, mutation_probability, start_time, end_time;
    int generations, population_size, num_clauses, num_variables, best_generation;
    CrossoverType crossover_type;
    SelectionType selection_type;
    void fitness();
    
    void boltzmann_selection();
    void tournament_selection();
    void rank_selection();
    vector<individual> generate_initial_population();
    void mutation();
    individual one_point_crossover(individual parent_a, individual parent_b);
    individual uniform_crossover(individual parent_a, individual parent_b);
    void extract_and_print_answer(individual best_individual);
    individual get_best();
    void debug_print();
    void print_individual(individual ind);
};
#endif

#ifndef __GA_h
#define __GA_h


#include <vector>
#include <string>

#include "NN.h"

using namespace std;

struct Individual {
    int number_correct;
    vector <int> compression_vector;
};

struct NN_Parameters {
    double learning_rate;
    Problem train_prob;
    Problem test_prob;
    int num_outputs;
    int max_epochs;
};

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
       NN_Parameters nnParameters);
    
    void runGA();
    
private:
    
    vector <vector <int> > clauses;
    vector <Individual> population;
    vector <Individual> breeding_population;
    double crossover_probability, mutation_probability, start_time, end_time;
    int generations, population_size, num_clauses;
    int num_variables, best_generation, num_symbols, map_size;
    CrossoverType crossover_type;
    SelectionType selection_type;
    NN_Parameters nn_parameters;
    
    void fitness();
    
    void boltzmann_selection();
    void tournament_selection();
    void rank_selection();
    vector<Individual> generate_initial_population();
    void mutation();
    Individual one_point_crossover(Individual parent_a, Individual parent_b);
    Individual uniform_crossover(Individual parent_a, Individual parent_b);
    void extract_and_print_answer(Individual best_individual);
    Individual get_best();
    void debug_print();
    void print_Individual(Individual ind);
};
#endif

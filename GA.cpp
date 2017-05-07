#include "GA.h"

#include <ctype.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>
#include <time.h>
#include <string>

using namespace std;

// GA constructor with initializer list
GA::GA(int populationSize, string selectionType, string crossoverType,
       double crossoverProbability, double mutationProbability,
       int generationNumber, int numSymbols, NN_Parameters nnParameters) :
net(nnParameters.learning_rate, nnParameters.train_prob,
   nnParameters.test_prob, nnParameters.num_outputs,
   nnParameters.max_epochs, numSymbols)
{
    this->population_size = populationSize;
    this->crossover_probability = crossoverProbability;
    this->mutation_probability = mutationProbability;
    this->generations = generationNumber;
    this->best_generation = 0;
    this->map_size = nnParameters.train_prob.inputs.size();
    this->num_symbols = numSymbols;
    this->nn_parameters = nnParameters;
    
    this->population = generate_initial_population();
    
    if(selectionType == "ts" ) {
        this->selection_type = TOURNAMENT;
    } else if(selectionType == "bs") {
        this->selection_type = BOLTZMANN;
    } else if(selectionType == "rs") {
        this->selection_type = RANK;
    } else {
        cout << "Selection type not recognized" << endl;
    }
    
    if(crossoverType == "uc") {
        this->crossover_type = UNIFORM;
    } else if(crossoverType == "1c") {
        this->crossover_type = ONEPOINT;
    } else {
        cout << "Crossover type not recognized" << endl;
    }
}


void GA::runGA() {
    
    start_time = clock();
    srand(clock());
    
    Individual best_overall_individual = population[0];
    Individual best_after_update;
    
    for(int i = 0; i < generations; i++) {
        cout << "0" << endl;
        fitness();  // evaluate fitness of population
        cout << "1" << endl;
        best_after_update = get_best();
        cout << "2" << endl;
        // for measuring which generation produce the best Individual
        if(best_after_update.number_correct > best_overall_individual.number_correct) {
            best_overall_individual = best_after_update;
            best_generation = i+1;
        }
        cout << "3" << endl;
        // perform selection
        switch(selection_type) {
            case TOURNAMENT:
                tournament_selection();
                break;
            case BOLTZMANN:
                boltzmann_selection();
                break;
            case RANK:
                rank_selection();
                break;
        }
        
        for(int j = 0; j < population_size-1; j++) { //size of breeding population
            Individual ind;
            
            switch(crossover_type) {
                case UNIFORM:
                    ind = uniform_crossover(breeding_population[2*j],
                                            breeding_population[2*j+1]);
                    break;
                case ONEPOINT:
                    ind = one_point_crossover(breeding_population[2*j],
                                              breeding_population[2*j+1]);
                    break;
            }
            
            population[j] = ind;
        }//for pop
        mutation();
    }//for gen
    end_time = clock();
    extract_and_print_answer(best_overall_individual);
}

void GA::debug_print() {
    cout << endl << endl;
    fitness();
    for(int i = 0; i < population_size; i++) {
        for(int j = 0; j < map_size+1; j++) {
            cout << population[i].compression_vector[j];
        }
        cout << "   " << population[i].number_correct << endl;
    }
}

void GA::print_Individual(Individual ind) {
    for(int i = 1; i <= map_size; i++) {
        cout << ind.compression_vector[i] << " ";
    }
    cout << endl;
}
void GA::extract_and_print_answer(Individual best_individual) {
    fitness();//get final fitness update
    Individual ind = get_best();
    if(ind.number_correct > best_individual.number_correct) {
        best_individual = ind;
        best_generation = generations;
    }
    
    int num_train_inputs = nn_parameters.test_prob.inputs.size();
    cout << " Number of symbols: " << num_symbols << endl;
    cout << " Number of tests: " << nn_parameters.test_prob.inputs.size() << endl;
    cout << " Number of tests satisfied: " << best_individual.number_correct << endl;
    cout << " Percentage of tests satisfied: " << 100.0*best_individual.number_correct/(num_train_inputs*1.0)<< "%" << endl;
    cout << " GA result: " << endl;
    cout << " ";
    for(int i = 1; i <= map_size; i++) {
        cout << best_individual.compression_vector[i] << " ";
    }
    cout << endl;
    cout << " Generation where best assignment was found: " << best_generation << endl;
    cout << " Time to solve: " << (end_time-start_time)/CLOCKS_PER_SEC << endl;
    cout << endl;
    
}

Individual GA::get_best() {
    int index_of_fittest = 0;
    for(int i = 0; i < population_size; i++) {
        if(population[index_of_fittest].number_correct < population[i].number_correct) index_of_fittest = i;
    }
    return population[index_of_fittest];
}

vector <Individual> GA::generate_initial_population() {
    
    vector<Individual> population; // to return
    
    for(int i = 0; i < population_size; i++) {
        vector<int> compressionVector;
        for(int j = 0; j < map_size+1; j++) { // first bit is trash for easy indexing
            int random;
            random = rand() % num_symbols;
            compressionVector.push_back(random);
        }
        Individual ind;
        ind.number_correct = -1;
        ind.compression_vector = compressionVector;
        population.push_back(ind);
    }
    return population;
}

void GA::fitness() {
    
    // for every Individual in the population
    for(int i = 0; i < population_size; i++) {
        cout << "i" << endl;
        net.compression_vector = population[i].compression_vector;
        net.train();
        population[i].number_correct = net.test();
        cout << population[i].number_correct << endl;
    }//for pop
}//func


void GA::boltzmann_selection() {
    
    breeding_population.clear();
    vector<long double> boltzmann_weights;  //declare size equal to popultion size
    long double boltzmann_sum = 0;  // DISCRETE DISTRUBTION RANDOM GENERATOR
    
    
    for (int i = 0; i < population_size; ++i) {
        long double k = expl(population[i].number_correct);
        boltzmann_weights.push_back(k);
        boltzmann_sum += k;
    }
    for(int i = 0; i < population_size * 2; ++i) {
        long double random_weight;
        random_weight = boltzmann_sum*double(rand())/RAND_MAX;
        int boltzmann_index = 0;
        //cout << random_weight << endl;
        while(random_weight > 0) {
            random_weight -= boltzmann_weights[boltzmann_index];
            boltzmann_index++;
            
        }
        boltzmann_index--;  //because we went one past
        breeding_population.push_back(population[boltzmann_index]);
    }
}


void GA::tournament_selection() {
    breeding_population.clear();
    
    int first_random_index;
    int second_random_index;
    
    for(int i = 0; i < population_size*2; ++i) {
        
        // get two random indices
        first_random_index = rand() % (population_size-1);
        second_random_index = rand() % (population_size-1);
        if(population[first_random_index].number_correct > population[second_random_index].number_correct) {
            breeding_population.push_back(population[first_random_index]);
        }
        else {
            breeding_population.push_back(population[second_random_index]);
        }
    }
}


// comparison overload fucntion for Individuals
bool compare_individual_satisfication(Individual a, Individual b){
    return a.number_correct < b.number_correct;
}


void GA::rank_selection() {
    //cout << "in rs " << endl;
    breeding_population.clear();
    ;
    // sorts the population in ascending order
    sort(population.begin(), population.end(), compare_individual_satisfication);
    int gaussian_sum = population_size * (population_size + 1) / 2;
    
    // weight probabilities by index from least
    for(int i = 0; i < population_size * 2; ++i) {
        int random_number;
        random_number = 1+rand() % (gaussian_sum); //CHECK THE +1 OH GOD
        // GOOD AREA FOR BUGS
        int index = ceil((-1 + sqrt(1 + 8 * random_number)) / 2);
        //cout << "Individual number: " << index << endl;
        breeding_population.push_back(population[index-1]);
        // I THINK THIS -1 STOPS THE SEGFAULT?
    }
}

void GA::mutation() {
    for(int i = 0; i < population_size; i++) { //all Individuals
        for(int j = 0; j < map_size+1; j++) { //first bit is trash
            double random; // = distribution(generator);
            random = double(rand())/RAND_MAX;
            if(random <= mutation_probability) { // if rand is less than prob[0,1]
                int random_symbol = rand() / num_symbols;
                population[i].compression_vector[j] = random_symbol;
            }
        }
    }
}

Individual GA::one_point_crossover(Individual parent_a, Individual parent_b) {
    
    // for crossover probability
    double random_number = double(rand()) / RAND_MAX;
    
    Individual new_Individual;
    new_Individual.number_correct = -1;
    
    // crossover
    if(random_number < crossover_probability) {
        int random_index;
        random_index = rand() % map_size;
        for(int i = 0; i < random_index; ++i) {
            new_Individual.compression_vector.push_back(parent_a.compression_vector[i]);
        }
        for(int j = random_index; j < parent_a.compression_vector.size(); ++j) {
            new_Individual.compression_vector.push_back(parent_b.compression_vector[j]);
        }
    } else { // don't crossover
        // MAY PASS BY REFERENCE?
        new_Individual.compression_vector = parent_a.compression_vector;
    }
    //print_Individual(new_Individual);
    //cout << endl;
    return new_Individual;
}

Individual GA::uniform_crossover(Individual parent_a, Individual parent_b) {
    //cout << "in uc " << endl;
    ;
    // initialize new Individual
    Individual new_Individual;
    new_Individual.number_correct = -1;
    double random1;
    random1 = double(rand())/RAND_MAX;
    // crossover
    if(random1 < crossover_probability) {
        for(int i = 0; i < parent_a.compression_vector.size(); ++i) {// for parent selection
            // pick parent
            int random2;
            random2 = rand() % 2;
            if(random2 == 0) {
                new_Individual.compression_vector.push_back(parent_a.compression_vector[i]);
            } else {
                new_Individual.compression_vector.push_back(parent_b.compression_vector[i]);
            }
        }
    } else { // don't crossover
        new_Individual.compression_vector = parent_a.compression_vector;
    }
    return new_Individual;
}

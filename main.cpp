#include "main.h"

using namespace std;

int main(int argc, char** argv) {
  cout << "hello world" << endl;
  

  int population_size = stoi(argv[1]);
  string selection_type = argv[2];
  string crossover_type = argv[3];
  double crossover_probability = stod(argv[4]);
  double mutation_probability = stod(argv[5]);
  int num_generations = stoi(argv[6]);
  
  Problem train;
  Problem test;
  

  NN_Parameters nn_params;
  nn_params.learning_rate = .5;
  nn_params.train_prob = train;
  nn_params.test_prob = test;
  nn_params.num_outputs = 10;
  nn_params.max_epochs = 10;
  
  cout << "a" << endl;

  GA rat_king(population_size,selection_type,crossover_type,
	      crossover_probability,mutation_probability,num_generations,nn_params);

  return 0;

}

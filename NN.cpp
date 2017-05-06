#include "NN.h"

using namespace std;


// constructor with initializer list and weight initialization
NN::NN(double learningRate, Problem train_prob, Problem test_prob,
       int numOutputs, int maxEpochs, int numSymbols,
       vector<int> compressionVector):
num_train_inputs(train_prob.num_inputs),
num_test_inputs(test_prob.num_inputs),
num_outputs(numOutputs),
map_size(train_prob.map_size),
learning_rate(learningRate),
train_inputs(train_prob.inputs),
test_inputs(test_prob.inputs),
train_targets(train_prob.targets),
test_targets(test_prob.targets),
max_epochs(maxEpochs),
num_symbols(numSymbols),
compression_vector(compressionVector)
{
    vector<vector<int>> train_zeros (num_train_inputs,vector<int> (num_symbols, 0));
    this->compressed_train_inputs = train_zeros;
    vector<vector<int>> test_zeros (num_test_inputs,vector<int> (num_symbols, 0));
    this->compressed_test_inputs = test_zeros;
    
}

/*
 Purpose: Initialize the weights of the perceptron.
 */
void NN::initialize_weights() {
    
    // create all the output nodes
    for(int i = 0; i < num_outputs; i++) {
        
        output o;
        outputs.push_back(o);
        for(int j = 0; j < map_size; j++) {
            
            // determined that a [-0.15, 0.15] random range
            // worked well in project 4
            outputs[i].weights.push_back(0.3*double(rand())/double(RAND_MAX) - 0.15);
            
        }
        
        // add a bias node
        outputs[i].weights.push_back(1);
    }
}

void NN::compress_maps() {
    
    for(int map = 0; map < num_train_inputs; map++) {
        for(int bit = 0; bit < map_size; bit++) {
            if(train_inputs[map][bit] == 1) {
                int symbol = compression_vector[bit];
                compressed_train_inputs[map][symbol]++;
            }
        }
    }
}




/*
 Purpose:   Reset the NN by clearing the weights
 and reinitializing them so it can be
 trained again.
 */

void NN::reset() {
    outputs.clear();
    initialize_weights();
    for(int map = 0; map < num_train_inputs; map++) {
        for(int bit = 0; bit < map_size; bit++) {
            compressed_train_inputs[map][symbol] = 0;
        
        }
    }
}

/*
 Purpose: Train the perceptron, update the weights
 Return: A vector of the % test problems correct
 every epoch.
 */
void NN::train() {
    
    reset();
    compress_maps();
    
    vector<double> percent_correct;
    
    // for every epoch
    for(int i = 0; i < max_epochs; i++) {
        
        // for every training input
        for(int input = 0; input < num_train_inputs; ++input) {
            
            double target = train_targets[input];
            
            // calculate the input . weight dot product
            for(int output = 0; output < num_outputs; ++output) {
                
                double dot_product = 0;
                
                for(int weight_index = 0; weight_index <= map_size; weight_index++) {
                    if(weight_index < map_size) {
                        dot_product += train_inputs[input][weight_index]*outputs[output].weights[weight_index];
                    }
                    else {
                        // bias node
                        dot_product += outputs[output].weights[weight_index];
                        
                    }
                } // every node weight
                
                // calculate activation function and derivative
                double g = activation_function(dot_product);
                double g_prime = ddx_activation_function(dot_product);
                
                // update weights
                update_weights(output, input, g, g_prime, target);
                
                
            }
        } // every training input
        
    } // every epoch
}


/*
 Purpose:    Update the output node weights based on the
 activation function and derivative value.
 */
void NN::update_weights(int output_index, int input_index, double g, double g_prime, double target) {
    
    
    if(num_outputs == 10) {  // for 10 output nodes
        if(output_index != target) {
            target = 0.0; // if index does not match target digit
        } else {
            target = 1.0; // if index matches target
        }
    }
    else {
        target = (target + .5) / 10; // calculate target activation value
    }
    
    // update every weight on output node
    for(int i = 0; i < map_size+1; i++) {
        if(i < map_size) {
            // update function
            outputs[output_index].weights[i] += learning_rate * (target - g) * g_prime * train_inputs[input_index][i];
        }
        else {
            // bias node
            outputs[output_index].weights[i] += learning_rate * (target - g) * g_prime;
        }
    }
}

/*
 Purpose:    Zigmoid activation function for perceptron.
 Return:     Function value.
 */
double NN::activation_function(double x) {
    return 1/(1+exp(.5-x));
}

/*
 Purpose:   Derivative of zigmoid activation function
 for perceptron.
 Return:    Function derivative.
 */
double NN::ddx_activation_function(double x) {
    return (1.64872 * exp(x)) / pow((1.64872 + exp(x)), 2);
}

/*
 Purpose:    Test the perceptron on the test inputs
 Return:     Percent of correct test problems.
 */
double NN::test() {
    
    int num_correct = 0;
    
    // for every test image
    for(int input = 0; input < num_test_inputs; input++) {
        
        int target = test_targets[input];
        
        double answer_value = -1;
        int answer = 0;
        
        // calculate the output at every node
        for(int output = 0; output < num_outputs; output++) {
            
            double dot_product = 0;
            
            // for every weight connected to the output
            for(int weight_index = 0; weight_index <= map_size; weight_index++) {
                
                // real inputs
                if(weight_index < map_size) {
                    dot_product += test_inputs[input][weight_index]*outputs[output].weights[weight_index];
                }
                //bias node
                else {
                    dot_product += outputs[output].weights[weight_index];
                }
            }
            
            double g = activation_function(dot_product);
            
            // if one output, check if g rounds to the target
            if(num_outputs == 1) {
                
                if(target == int(10*g - .5)) {
                    num_correct++;
                }
            }
            
            // set 10 output answer to largest index of
            // largest activation function
            else if(g > answer_value) {
                answer = output;
                answer_value = g;
            }
        }
        
        // check if the node with the largest
        // activation is the target index
        if(num_outputs == 10) {
            if(answer == target) {
                ++num_correct;
            }
        }
        
        
    }
    cout << "inputs " << num_test_inputs << endl;
    return num_correct;
}

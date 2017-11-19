# Handwritten Digit Compression
*Final Project for NIC*

Genetic Algorithm (GA) / Neural Network hybrid for image compression and optical digit recognition. The program uses a genetic algorithm to assign integer symbols to each bit of an image. A perceptron is then trained with the sum of bits of each symbol as the input using a training image set. Finally, the perceptron is tested on a test image set.

## Running
First ```make``` the project

Then run

> ./RK

with the following parameters, in this order:

__Population:__
  [Positive integer]
  
  
__Selection Type:__
  [String]
  
  "bs" - Botlzmann Selection
  
  "ts" - Tournament Selection
  
  "rs" - Rank Selection // This is the best one
  
  
__Crossover Type:__
  [String]
  
  "uc" - Uniform Crossover
  
  "1c" - 1-Point Crossover
  
  "nc30" - 30-Point Crossover // Replace '30' with any positive integer for any N-Point crossover
  
  
__Crossover Probability:__
  [Double, between 0 and 1]
  
  
__Mutation Probability:__
  [Double, between 0 and 1]
  
  
__Number of Generations/Iterations:__
  [Positive integer]
  
  
__Number of Symbols:__
  [Positive integer]
  
  
Example command, pretty good results:

./RK 100 rs uc .5 .01 200 32


## Results
Our tests yielded two notable results
  1.  Individuals with only 16 symbols were able to achieve over 80% digit recognition accuracy. This is significant because 8x8 grid compression, which effectively has 64 symbols, was unable to achieve over 70% accuracy. This method of compression gave more accurate results with 1/4 the number of perceptron inputs.
  2.  The algorithm was able to achieve 93.2% accuracy with 32 symbols. In comparison, the peak accuracy for the perceptron with uncompressed 32x32 bitmap inputs was 94.6%. This algorithm was able to achieve comparable accuracy with 1000 fewer perceptron nodes. 
  
## Future Directions
  * It is likely that even better results are possible if the parameters for the GA and perceptron are further optimized. Given more time and computing power, the parameter space should be explored more thoroughly. The mutation rate will be an important target for further optimization, as it enables the algorithm to escape local minima.
  * Adding a hidden layer to the neural network may improve the fitness of our algorithm and make it possible to compress images with even fewer symbols while maintaining or improving the accuracy.

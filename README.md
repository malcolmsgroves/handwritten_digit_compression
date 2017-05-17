# rat_king
Final Project for NIC: GA / Perceptron hybrid for image compression and digit recognition.

After making the project, run

> ./RK

with the following parameters, in this order:

Population:
  <Positive integer>
  
Selection Type:
  <String>
  
  "bs" - Botlzmann Selection
  
  "ts" - Tournament Selection
  
  "rs" - Rank Selection // This is the best one
  
Crossover Type:
  <String>
  
  "uc" - Uniform Crossover
  
  "1c" - 1-Point Crossover
  
  "nc30" - 30-Point Crossover // Replace '30' with any positive integer for any N-Point crossover
  
Crossover Probability:
  <Double, between 0 and 1>
  
Mutation Probability:
  <Double, between 0 and 1>
  
Number of Generations/Iterations:
  <Positive integer>
  
Number of Symbols:
  <Positive integer>
  
Example command, pretty good results:

./RK 100 rs uc .5 .01 200 32

# Cellular Automata for Pedestrians
Thesis project that implements a cellular automata to simulate the evacuation of pedestrians in an emergency situation. We aim to work with the sensibility variables k_D and k_s in order to research how knowledge of the location of the door will affect evacuation times.

Acknowledgements and credits go to Dr. Lakoari Nouredinne, PhD and Dr. Julio César Pérez Sansalvador, PhD for their assesment and constant help in the work of this thesis.
Special thanks are given to Adam Sobol, Universitetet i Oslo student, for his help with Python and a C++ graphic handler.

# Reference 
Kirchner, A., & Schadschneider, A. (2002). Simulation of evacuation processes using a bionics-inspired cellular automaton model for pedestrian dynamics. Physica A: Statistical Mechanics And Its Applications, 312(1-2), 260-276. doi: 10.1016/s0378-4371(02)00857-9

# Purpose
This is a cellular automata model based on Kirchner and Schadchneider's model(2002). It aims to study the impact of kD and kS on evacuation time. These variables will affect individual pedestrians differently

# To do
~- Update exponential function~
~- Average all dynamic fields of a run to get a heatmap~
~- Try with a room sized 63x63 after exponential update~
- Implement different kD and kS for "lost" and "non lost" pedestrians
- Get data with all updates done
- Comment the complete code

# Problems found
- Exponential function returns NaN values for big rooms (bigger than 35x35). Since every cell is a scale of a 40 x 40 cm cell, the maximum simulated room has an area of 196 squared meters, which is a small room in comparison to similar models. This is the *primary* problem to solve. FIXED
- The program is not user friendly. This should be fixed to allow this program to be useful for educational purposes. 

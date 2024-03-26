## **This is a university project for the course "Fundamentals of Compiler Design".** 
Above you will find a simulation of a NFA-λ(Nondeterministic Finite Automaton with λ transitions) that reads the automaton from a .txt file and outputs all the possible paths that an input sting (defined on the input alphabet) takes in the given automaton.

## **.txt file structure:**
states:0,1,2,3\
initial state:0\
input alphabet:0,1\
output alphabet:0,1,2,3\
transitions:\
0 1 1 1\
0 1 3 *\
1 0 2 *\
1 1 3 *\
2 0 1 0\
2 1 1 1\
3 0 1 2\
3 1 1 3


// * == λ (null string -> |λ| = 0)\
// transitions structure: **current state**, **char from input**, **next state**, **value of transition**\


flags and their significance:- 

STOP_AT_DANGLING -> upon encountering a dangling node -> Stop or randomly restart at another node 
RANDOM_INIT      -> start walk at random node or start m random walks @ each of the NNodes
USE_VISITS       -> 0 - Use Ending Node, 1 & 2 - Use Visited node 


Below are the parameter settings for each of the algorithm:
---------------------------------------
ALGORITHM 1:- 
    bool STOP_AT_DANGLING = false;                        
    bool RANDOM_INIT = true;                             
    int USE_VISITS = 0;                                   

---------------------------------------
ALGORITHM 2:-
    bool STOP_AT_DANGLING = false;                        
    bool RANDOM_INIT = false;                             
    int USE_VISITS = 0; 

---------------------------------------
ALGORITHM 3:- 
    bool STOP_AT_DANGLING = false;                        
    bool RANDOM_INIT = false;                             
    int USE_VISITS = 2; 

---------------------------------------
ALGORITHM 4:- 
    bool STOP_AT_DANGLING = true;                        
    bool RANDOM_INIT = false;                             
    int USE_VISITS = 1; 

---------------------------------------
ALGORITHM 5:- 
    bool STOP_AT_DANGLING = true;                        
    bool RANDOM_INIT = true;                             
    int USE_VISITS = 1; 

---------------------------------------


#!/bin/bash

## Compile our two program files
g++ -std=c++1z -pthread -O3 -march=native -o code_5 main_control_5.cpp generalize_attempt1_anoosh.cpp #Game_5.cpp

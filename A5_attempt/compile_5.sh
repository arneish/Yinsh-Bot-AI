
#!/bin/bash

## Compile our two program files
g++ -std=c++11 -O3 -march=native -o code main_control.cpp Game.cpp

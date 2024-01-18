#!/bin/bash

# Run the application with opengl

source='main.cpp'
target='main'
if [ $# -eq 2 ]; then
    source=$1
    target=$2
else [ $# -eq 1 ]
    source=$1
    target=${source%.*}
fi

if [ ! -d "obj" ]; then
    mkdir obj
fi

target="obj/$target"

g++ -std=c++11 $source -o $target gglib.cpp -lGL -lGLU -lglut 
./$target
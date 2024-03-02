#! /bin/bash

# take the input file path
# check if the input file path is provided
# take the output file directory
if [ -z "$2" ]
then
    echo "usage: ./build_and_run.sh <input_file_path> <output_file_directory>"
    exit 1
fi
input_file_path=$1
output_file_directory=$2

if [ ! -d "$output_file_directory" ]
then
    mkdir -p $output_file_directory
fi

g++ -std=c++11 1905004_classes.cpp 1905004_main.cpp -o 1905004_main -lGL -lGLU -lglut
./1905004_main $input_file_path $output_file_directory
rm 1905004_main
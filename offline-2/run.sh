#! /bin/bash

# check if the number of arguments is correct
if [ $# -lt 1 ]; then
  echo "Usage: run.sh [-d <output_dir>] scene_file.txt"
  exit 1
fi

# check if necessary files exist
if [ ! -f "scene_parser.cpp" ]; then
  echo "scene_parser.cpp not found"
  exit 1
fi
if [ ! -f "libggutil.hpp" ]; then
  echo "libggutil.hpp not found"
  exit 1
fi
if [ ! -f "libgg.hpp" ]; then
  echo "libgg.hpp not found"
  exit 1
fi
if [ ! -f "libggutil.cpp" ]; then
  echo "libggutil.cpp not found"
  exit 1
fi
if [ ! -f "libgg.cpp" ]; then
  echo "libgg.cpp not found"
  exit 1
fi

output_dir="output"
if [ "$1" == "-d" ]; then
  echo "Output directory: $2"
  output_dir=$2
  shift
  shift
fi
if [ ! -d "$output_dir" ]; then
  mkdir $output_dir
fi

scene=$1
scene_parser="scene_parser.cpp"
scene_parser_obj="scene_parser"
main="main.cpp"
main_obj="main"

cp $scene_parser $scene libggutil.hpp libgg.hpp libggutil.cpp libgg.cpp $output_dir/
cd $output_dir

scene=${scene##*/}

# running the scene parser
g++ -o $scene_parser_obj $scene_parser
./$scene_parser_obj $scene $main

# running the scene source code
g++ -o $main_obj $main libggutil.cpp libgg.cpp
./$main_obj

# cleaning up
# rm $scene_parser_obj $main_obj
# rm $scene_parser libggutil.hpp libgg.hpp libggutil.cpp libgg.cpp

cd ..




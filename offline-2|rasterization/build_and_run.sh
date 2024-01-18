#! /bin/bash

# check if the number of arguments is correct
if [ $# -lt 2 ]; then
  echo "Usage: run.sh [-d <output_dir>] <scene_file_path> <config_file_path>"
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
if [ ! -f "bitmap_image.hpp" ]; then
  echo "bitmap_image.hpp not found"
  exit 1
fi

output_dir="output"
if [ "$1" == "-d" ]; then
  output_dir=$2
  shift
  shift
fi
if [ ! -d "$output_dir" ]; then
  mkdir $output_dir
fi

scene=$1
config=$2
scene_parser="scene_parser.cpp"
scene_parser_obj="scene_parser"
main="main.cpp"
main_obj="main"

cp $scene_parser $scene $config bitmap_image.hpp libggutil.hpp libggutil.cpp libgg.hpp libgg.cpp $output_dir/
cd $output_dir

echo "Output directory: $output_dir ready"

scene=${scene##*/}
config=${config##*/}

# running the scene parser
g++ -o $scene_parser_obj $scene_parser
./$scene_parser_obj $scene $config $main
echo "Scene parsing done"

# running the scene source code
g++ -o $main_obj $main bitmap_image.hpp libggutil.cpp libgg.cpp
./$main_obj
echo "Scene rendering done"

# cleaning up
rm $scene_parser_obj $main_obj
# rm $scene_parser libggutil.hpp libgg.hpp libggutil.cpp libgg.cpp bitmap_image.hpp
echo "Clean up done"

cd ..




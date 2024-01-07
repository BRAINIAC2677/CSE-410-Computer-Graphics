#! /bin/bash

# create obj dir if not exists
if [ ! -d "obj" ]; then
  mkdir obj
fi

# check if -g flag is set
if [ "$1" == "-g" ]; then
  echo "Debug mode"
  g++ -o obj/libgg_tester_dbg libgg_tester.cpp libggutil.cpp libgg.cpp -g
  gdb ./obj/libgg_tester_dbg
else
  g++ -o obj/libgg_tester libgg_tester.cpp libggutil.cpp libgg.cpp
  ./obj/libgg_tester
fi



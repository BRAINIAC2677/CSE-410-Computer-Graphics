#! /bin/bash

# create obj dir if not exists
if [ ! -d "obj" ]; then
  mkdir obj
fi

# if last arg is -g, compile with debug symbols
if [ "$#" -eq 1 ] && [ "$1" == "-g" ]; then
  g++ -g -o obj/libggutil_tester libggutil_tester.cpp libggutil.cpp
  gdb obj/libggutil_tester
  exit 0
fi

g++ -o obj/libggutil_tester libggutil_tester.cpp libggutil.cpp
./obj/libggutil_tester

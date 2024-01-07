#! /bin/bash

# create obj dir if not exists
if [ ! -d "obj" ]; then
  mkdir obj
fi

g++ -o obj/libggutil_tester libggutil_tester.cpp libggutil.cpp
./obj/libggutil_tester

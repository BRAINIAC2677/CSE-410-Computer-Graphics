# CSE-410 | Computer Graphics

This repository contains my implementations of the assignments of the course CSE-410: Computer Graphics by BUET.


- ## Offline-1 | OpenGL
    <img src="/assets/magic-cube.png" alt="Magic Cube" width="500"/>

    - [Problem Specification](/offline-1|opengl/problem_spec.pdf)
    - ### How To Run?
        - enter into offline-1|opengl directory
        - run the following command
            ```bash
            ./build_and_run.sh <source_file_name>
            ``` 
            e.g.
            ```bash
            ./build_and_run.sh magic_cube.cpp
            ```
    - ### File Description
        - [Task-1 | camera movement](/offline-1|opengl/camera_controller.cpp)
        - [Task-2 | rolling a ball](/offline-1|opengl/rolling_ball.cpp)
        - [Task-3 | magic cube](/offline-1|opengl/magic_cube.cpp)


- ## Offline-2 | Rasterization
    <img src="/assets/rasterized.bmp" alt="rasterized" width="500"/>

    - [Problem Specification](/offline-2|rasterization/problem_spec.pdf)
    - ### How To Run?
        - enter into offline-2|rasterization directory
        - run the following command
            ```bash
            ./build_and_run.sh -d <directory_name> <scene_file_path> <config_file_path>
            ``` 
            e.g.
            ```bash
            ./build_and_run.sh -d output1 ios/1/scene.txt ios/1/config.txt
            ```
    - ### File Description
        `libGG` is a tiny version of openGL library I implemented specifically for this offline.
        - [bitmap_image.hpp](/offline-2|rasterization/bitmap_image.hpp) - external bitmap image library
        - [libggutil.hpp](/offline-2|rasterization/libggutil.hpp) - declartion of utility classes and functions for libgg library
        - [libggutil.cpp](/offline-2|rasterization/libggutil.cpp) - implementation of utility classes and functions for libgg library
        - [libgg.hpp](/offline-2|rasterization/libgg.hpp) - declartion of libgg library
        - [libgg.cpp](/offline-2|rasterization/libgg.cpp) - implementation of libgg library
        - [scene_parser.cpp](/offline-2|rasterization/scene_parser.cpp) - parse scene.txt and config.txt files and generate c++ code with libgg library
        
- ## Offline-3 | Ray Tracing
    <img src="/assets/ray-traced.bmp" alt="raytraced" width="500"/>

    - [Problem Specification](/offline-3|ray-tracing/problem_spec.pdf)
    - How To Run?
        - enter into offline-3|ray-tracing directory
        - run the following command
            ```bash
            ./build_and_run.sh <input_file_path> <output_file_directory>
            ``` 
            e.g.
            ```bash
            ./build_and_run.sh ios/scene_test.txt output
            ```
        - `NOTE:` **Unlike mentioned in spec, my implementation doesn't expect blank line at the end of object, light and camera description in the input file.**
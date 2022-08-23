cmake_minimum_required(VERSION 3.16)
project(Segmentation)

set(CMAKE_CXX_STANDARD 17)

add_executable(Segmentation main.cpp Convolution.h Sets.h Filters.h Image.h Converter.h Utilities.h Miscellanious.h File.h Graph_segment.h Image_segment.h)
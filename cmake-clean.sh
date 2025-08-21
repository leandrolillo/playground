#!/bin/bash
find . -name "Makefile" -delete
find . -name "cmake_install.cmake" -delete
find . -type d -name CMakeFiles -prune -exec rm -rf {} \;
rm CMakeCache.txt
rm -rf target/*
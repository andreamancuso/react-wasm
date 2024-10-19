#!/bin/bash
rm -fr build
cmake -S . -B build
cd build
make
./Google_Tests_run

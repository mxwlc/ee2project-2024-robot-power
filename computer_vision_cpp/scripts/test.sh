#!/bin/bash
cd ..
cmake .
make RPEyes -j8

./bin/RPEyes -v
./bin/RPEyes -v
./bin/RPEyes -v
./bin/RPEyes -v
./bin/RPEyes -v

./bin/RPEyes
./bin/RPEyes
./bin/RPEyes
./bin/RPEyes
./bin/RPEyes
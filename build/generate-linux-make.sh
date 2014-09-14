#!/bin/sh

DIR32=linux32-make
DIR64=linux64-make
GEN="Unix Makefiles"

mkdir -p ${DIR32}/debug
mkdir -p ${DIR64}/debug
mkdir -p ${DIR32}/release
mkdir -p ${DIR64}/release

cd ${DIR32}/debug
cmake -G "${GEN}" -DCMAKE_MACHINE_TYPE=32 -DCMAKE_BUILD_TYPE=Debug ../../..
cd -

cd ${DIR32}/release
cmake -G "${GEN}" -DCMAKE_MACHINE_TYPE=32 -DCMAKE_BUILD_TYPE=Release ../../..
cd -

cd ${DIR64}/debug
cmake -G "${GEN}" -DCMAKE_MACHINE_TYPE=64 -DCMAKE_BUILD_TYPE=Debug ../../..
cd -

cd ${DIR64}/release
cmake -G "${GEN}" -DCMAKE_MACHINE_TYPE=64 -DCMAKE_BUILD_TYPE=Release ../../..
cd -

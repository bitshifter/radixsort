@echo off

set DIR64=win64

md %DIR64%

cd %DIR64%
cmake ..\..
cd ..


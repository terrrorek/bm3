@echo off
cd build
premake5.exe vs2022
msbuild idk.sln
cd ../out
bm.exe %*
cd ..

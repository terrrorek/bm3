@echo off
cd build
premake5.exe vs2022
msbuild idk.sln
cd ../out
echo =============================
lcc.exe -target=symbolic test.c -
echo =============================
lcc.exe -target=scvm test.c -
echo =============================
cd ..

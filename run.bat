scons --debug-symbols --float-math --no-optimize
cd out
bm.exe %*
cd ..

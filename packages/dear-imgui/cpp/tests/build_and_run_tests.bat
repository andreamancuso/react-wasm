rmdir /s /q build 2>nul
cmake -S . -B build
cmake --build ./build --target Google_tests_run
build\Debug\Google_Tests_run.exe

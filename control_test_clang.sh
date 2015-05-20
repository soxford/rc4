clang++ -std=c++11 -stdlib=libc++ -Os ./tests/correctness\ tests/rc4Test.cpp -o test1
./test1
clang++ -std=c++11 -stdlib=libc++ -Os ./tests/optimControlTest.cpp -o test2
./test2 ./HomeOptimisationTests.txt
rm ./test1
rm ./test2

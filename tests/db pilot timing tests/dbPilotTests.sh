clang++ -std=c++11 -stdlib=libc++ -Os ../db\ control\ tests/dbControlTest.cpp -o test1
./test1 ./dbHomeTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os dbNoHistogramLookupTest.cpp -o test2
./test2 ./dbHomeTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os dbNoRC4GenerationTest.cpp -o test3
./test3 ./dbHomeTestData.txt

rm test1 test2 test3 



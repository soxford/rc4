clang++ -std=c++11 -stdlib=libc++ -Os ../control\ tests/stackAllocatedControlTest.cpp -o test1
./test1 CorrectedMTStackHomePilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noRe-KeyingTest.cpp -o test2
./test2 CorrectedMTStackHomePilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noRC4GenerationTest.cpp -o test3
./test3 CorrectedMTStackHomePilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os staticKeyTest.cpp -o test4
./test4 CorrectedMTStackHomePilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noHistogramLookupTest.cpp -o test5
./test5 CorrectedMTStackHomePilotTestData.txt

rm test1 test2 test3 test4 test5



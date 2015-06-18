clang++ -std=c++11 -stdlib=libc++ -Os ../sb\ control\ tests/staticAllocatedControlTest.cpp -o test1
./test1 HomeStaticPilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noRe-KeyingTest.cpp -o test2
./test2 HomeStaticPilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noRC4GenerationTest.cpp -o test3
./test3 HomeStaticPilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os staticKeyTest.cpp -o test4
./test4 HomeStaticPilotTestData.txt

clang++ -std=c++11 -stdlib=libc++ -Os noHistogramLookupTest.cpp -o test5
./test5 HomeStaticPilotTestData.txt

rm test1 test2 test3 test4 test5



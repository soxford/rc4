g++ -std=c++11  -o3 ../sb\ control\ tests/staticAllocatedControlTest.cpp -o test1
./test1 LabStaticPilotTestData.txt

g++ -std=c++11  -o3 noRe-KeyingTest.cpp -o test2
./test2 LabStaticPilotTestData.txt

g++ -std=c++11  -o3 noRC4GenerationTest.cpp -o test3
./test3 LabStaticPilotTestData.txt

g++ -std=c++11  -o3 staticKeyTest.cpp -o test4
./test4 LabStaticPilotTestData.txt

g++ -std=c++11  -o3 noHistogramLookupTest.cpp -o test5
./test5 LabStaticPilotTestData.txt

rm test1 test2 test3 test4 test5



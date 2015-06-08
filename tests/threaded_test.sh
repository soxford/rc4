g++ -std=c++11 -o3 stackAllocatedControlTest.cpp -o test1
./test1 ThreadedLabTestData.txt

g++ -std=c++11 -o3 threadedControlTest.cpp -o test2 -pthread
./test2 ThreadedLabTestData.txt

rm test1 test2

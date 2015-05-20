g++ -std=c++11 -o3 ./tests/correctness\ tests/rc4Test.cpp -o test1
./test1
g++ -std=c++11 -o3 ./tests/stackAllocatedControlTest.cpp -o test2
./test2 ./LabOptimisationTests.txt
rm ./test1
rm ./test2

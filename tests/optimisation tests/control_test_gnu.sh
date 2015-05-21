#shell script to check correctness of RC4Stream implementation and provide control test timing output
g++ -std=c++11 -o3 ../correctness\ tests/rc4Test.cpp -o test1
./test1
g++ -std=c++11 -o3 ../control\ tests/stackAllocatedControlTest.cpp -o test2
./test2 ./LabOptimisationTests.txt
rm ./test1
rm ./test2

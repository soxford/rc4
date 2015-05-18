g++ -std=c++11 -o3 ../correctness\ tests/rc4Test.cpp -o test1
./test1
g++ -std=c++11 -o3 $1 -o test2
./test2 ./LabOptimPilotTests.txt
rm ./test1
rm ./test2

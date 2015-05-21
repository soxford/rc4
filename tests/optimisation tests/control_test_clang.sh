#shell script to check correctness of RC4Stream implementation and provide control test timing output
clang++ -std=c++11 -stdlib=libc++ -Os ../correctness\ tests/rc4Test.cpp -o test1
./test1
clang++ -std=c++11 -stdlib=libc++ -Os ../control\ tests/stackAllocatedControlTest.cpp -o test2
./test2 ./HomeOptimisationTests.txt
rm ./test1
rm ./test2

#shell script to check correctness of RC4Stream implementation and provide control test timing output
clang++ -std=c++11 -stdlib=libc++ -Os ../correctness\ tests/rc4Test.cpp -o test1
./test1
clang++ -std=c++11 -stdlib=libc++ -Os ./dbControlTest.cpp -o test2
./test2 ./dbHomeTestData.txt
rm ./test1
rm ./test2

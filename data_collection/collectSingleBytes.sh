#shell script to check correctness of RC4Stream implementation and provide data collection of singlebytes
clang++ -std=c++11 -stdlib=libc++ -Os ../tests/correctness\ tests/rc4Test.cpp -o test1
./test1
clang++ -std=c++11 -stdlib=libc++ -Os ./collect_single_byte_data.cpp -o collect
./collect
rm ./test1
rm ./collect

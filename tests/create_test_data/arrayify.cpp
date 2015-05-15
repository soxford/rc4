#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
using namespace std;
int main(int argc, char* argv[]) {
   const char* inFileName = "encryptionOutput.bin";	
   const char* outFileName = "encryptionOutput.txt";

   //open log file for appending data
   ifstream inFile;	   
   ofstream outFile ;
   inFile.open(inFileName, ios::in | ios::binary); 
   outFile.open(outFileName, ios::app);
   if(!outFile.is_open() || !inFile.is_open()){
      cout << "Error: failed to open file" << endl;
      return 1;
   }
	unsigned char c;
	while(!inFile.eof()){
		inFile >> noskipws >> c;
		outFile << ", 0x" << std::hex << std::setw(2) << std::setfill('0') << (int) c;
	}

	outFile << endl;
	inFile.close();
	outFile.close();
	return 0;
}

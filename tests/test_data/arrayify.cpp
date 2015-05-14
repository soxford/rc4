#include <iostream>
#include <fstream>
#include <cstdint>
using namespace std;
int main(int argc, char* argv[]) {
	
   const char* fileName = "encryptionOutput.txt";

   //open log file for appending data
   ofstream logFile ;
   logFile.open(fileName, ofstream::app);
   if(!logFile.is_open()){
      cout << "Error: failed to open file named " << fileName << endl;
      return 1;
   }
	unsigned char c;
	for(int	i = 0; i < 257; i ++){
		cin >> c;
		logFile << ", 0x" << std::hex<< (int) c;
	}

	logFile << endl;
	logFile.close();
	return 0;
}

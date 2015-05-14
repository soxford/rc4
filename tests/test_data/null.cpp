#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char* argv[]) {
	
   const char* fileName = "encryptionInput.bin";

   //open log file for appending data
   ofstream logFile ;
   logFile.open(fileName, ofstream::app);
   if(!logFile.is_open()){
      cout << "Error: failed to open file named " << fileName << endl;
      return 1;
   }
	for(int	i = 0; i < 257; i ++)
		logFile << '\0';

	logFile.close();
	return 0;
}

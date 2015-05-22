//Output is in matlab readable format
//TODO file description
#include <string>
#include <ctime>
#include <fstream>
#include <cstdio>
#include "../../MT19937_RandomSource.cpp"

//Experimental parameters
int BATCH_SIZE = (1 << 30); //the number of pages per batch
int TOTAL_NO_OF_BATCHES = (1 << 5); //The number of batches to be run in this experiment
int PAGE_LENGTH = 257; //The number of output bytes per page

//function declarations
void dumpSingleByteData(ostream &outStream, unsigned long ** histograms);
using namespace std;

int main(int argc, const char *argv[])
{
    //TODO encapsulate in a try block

        //initialize the file names for data storage based on the date, time of starting the experiment
        string tmpFileName0, tmpFileName1, dataOutputFileName;
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        tmpFileName0 = "./single_byte_data/0 SingleByteData " + (now->tm_year + 1900) + "-"
                                           + (now->tm_mon + 1) + "-"
                                           +  now->tm_mday + " "
                                           +  now->tm_hour + "-"
                                           +  now->tm_min
                                           + ".tmp";

        tmpFileName1 = "./single_byte_data/1 SingleByteData " + (now->tm_year + 1900) + "-"
                                           + (now->tm_mon + 1) + "-"
                                           +  now->tm_mday + " "
                                           +  now->tm_hour + "-"
                                           +  now->tm_min
                                           + ".tmp";

        dataOutputFileName = "./single_byte_data/SingleByteData " + (now->tm_year + 1900) + "-"
                                                   + (now->tm_mon + 1) + "-"
                                                   +  now->tm_mday + " "
                                                   +  now->tm_hour + "-"
                                                   +  now->tm_min
                                                   + ".txt"; //TODO confirm file format
        //declare to output files
        ofstream tmpFile, dataOutputFile;

        //initialize the histogram and stream/key variables
        unsigned long histograms[PAGE_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH];
        for (int i = 0; i < PAGE_LENGTH; i++) {
          for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
             histograms[i][j] = 0;
          }
        }

        //Construct the key;
        RC4Stream::Key key;

        //Construct the RC4 stream
        RC4Stream rc4Stream;

        //initialize Random Number Generation algorithm 
        RC4Stream::Key::RandomSource randomSource;
        randomSource.initializeRandomNoGen();

        //Loop over the number of batches
        for(int batchNo = 0; batchNo < TOTAL_NO_OF_BATCHES; batchNo++){

            //for each batch produce BATCH_SIZE rc4 output pages from independent random keys and store in the histogram
            for(int pageNo = 0; pageNo < BATCH_SIZE; pageNo++){
                        
                //random key generation
                randomSource.selectRandomKey(key);
                 
                //rekey
                rc4Stream.keySchedule(key);
                 
                //run RC4 stream algorithm and collect output in histogram counters
                for (int i = 0; i < PAGE_LENGTH; i++) {
                   histograms[i][rc4Stream.PRGRound()]++; //increment the relevant histogram count
                }

            }

            //At the end of the batch output the histogram to a tmp file (tagged either 0 or 1 depending on the parity of the batch number, i.e. overwrite the last but one tmp file) include batch number and total so far and date/time (create a method for this which takes a file name in and writes in a usable format)
            if(batchNo & 1) {
                tmpFile.open(tmpFileName0, ios::trunc);
            } else {
                tmpFile.open(tmpFileName1, ios::trunc);
            }
            
            dumpSingleByteData(tmpFile, histograms);

            tmpFile.close();
        }

        //once all batches are complete output the final histogram data to a final data file including total number of batches as well as (in a comment) the date time of completion
        dataOutputFile.open(dataOutputFileName, ios::app);

        // delete the tmp files
        remove(tmpFileName0); 
        remove(tmpFileName1);
    //TODO catch all exceptions and output a failure report including timing and batch number
}


//dumps the single byte histogram data to the outStream. Assumes that the histogram has dimensions PAGE_LENGTH and PERMUTATION_ARRAY_LENGTH.
void dumpSingleByteData(ostream &outStream, unsigned long ** histograms){
            //TODO confirm file format!!!!!!!!!
            outStream << "histograms = [" << endl;
            for(int i = 0; i < PAGE_LENGTH; i++) {
                outStream << "\t";
                for(int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++){
                    outStream << histograms[i][j] << " ";   
                }
                outStream << ";" << endl;
            }
            outStream << "];" << endl;
            
            outStream << endl << "batchNo = " << batchNo << ";" << endl;
}

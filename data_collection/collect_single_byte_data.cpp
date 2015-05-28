/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: collect_single_byte_data.cpp
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: Produces 257 single byte outputs from an RC4Stream from randomly generated RC4Stream::Key values and collects these values in a histogram count, by byte stream position (0 - 256 inclusive). These values are then output to a data file which can be loaded for analysis in matlab. The data is dumped at intervals during generation to ensure any failure in the process is not catastophic to data gathering.
 * Date: May 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cstdio>
#include "../MT19937_RandomSource.cpp"

//Experimental parameters
int BATCH_SIZE = (1 << 30); //the number of pages per batch
int TOTAL_NO_OF_BATCHES = (1 << 5); //The number of batches to be run in this experiment
int PAGE_LENGTH = 257; //The number of output bytes per page

//function declarations
void dumpSingleByteData(std::ostream &outStream, unsigned long histograms[][RC4Stream::PERMUTATION_ARRAY_LENGTH], int numberOfBatches);
using namespace std;

int main(int argc, const char *argv[])
{
    //TODO encapsulate in a try block
        //initialize the file names for data storage based on the date, time of starting the experiment
        ostringstream oss;
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        oss << "./single_byte_data/" << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteDataxtmp0"
                                           << ".m";
        auto tmp0 = oss.str();
        const char* tmpFileName0 = tmp0.c_str(); 

        oss.str(string());
        oss << "./single_byte_data/" << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteDataxtmp1"
                                           << ".m";
        auto tmp1 = oss.str();
        const char* tmpFileName1 = tmp1.c_str(); 

        oss.str(string());
        oss << "./single_byte_data/" << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteData"
                                           << ".m";
       auto tmp2 = oss.str();
        const char* dataOutputFileName = tmp2.c_str(); 

        //declare the output file streams
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
        MT19937_RandomSource randomSource;
        randomSource.initializeRandomNoGen();

        //Loop over the number of batches
        for(int batchNo = 1; batchNo <= TOTAL_NO_OF_BATCHES; batchNo++){

            //for each batch produce BATCH_SIZE rc4 output pages from independent random keys and store in the histogram
            for(int pageNo = 0; pageNo < BATCH_SIZE; pageNo++){
                        
                //random key generation
                randomSource.selectRandomKey(key);
                 
                //rekey
                rc4Stream.keySchedule(key);
                 
                //run RC4 stream algorithm and collect output in histogram counters
                for (int i = 0; i < PAGE_LENGTH; i++) {
                   (histograms[i][rc4Stream.PRGRound()])++; //increment the relevant histogram count
                }

            }

            //At the end of the batch output the histogram to a tmp file (tagged either 0 or 1 depending on the parity of the batch number, i.e. overwrite the last but one tmp file) include batch number and total so far and date/time (create a method for this which takes a file name in and writes in a usable format)
            if(batchNo & 1) {
                tmpFile.open(tmpFileName1, ios::trunc);
            } else {
                tmpFile.open(tmpFileName0, ios::trunc);
            }
            
            dumpSingleByteData(tmpFile, histograms, batchNo);

            tmpFile.close();
        }

        //once all batches are complete output the final histogram data to a final data file including total number of batches as well as (in a comment) the date time of completion
        dataOutputFile.open(dataOutputFileName, ios::app);
        dumpSingleByteData(dataOutputFile, histograms, TOTAL_NO_OF_BATCHES);
        dataOutputFile.close();

        // delete the tmp files
        remove(tmpFileName0); 
        remove(tmpFileName1);
    //TODO catch all exceptions and output a failure report including timing and batch number
}


//dumps the single byte histogram data to the outStream. Assumes that the histogram has dimensions PAGE_LENGTH and PERMUTATION_ARRAY_LENGTH.
void dumpSingleByteData(std::ostream &outStream, unsigned long histograms[][RC4Stream::PERMUTATION_ARRAY_LENGTH], int numberOfBatches){
            outStream << "% The histogram counts of the output bytes, each row is the histogram for a single output position from 1 - 257 inclusive" << endl;
            outStream << "histograms = [ " << endl;
            for(int i = 0; i < PAGE_LENGTH; i++) {
                outStream << '\t';
                for(int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++){
                    outStream << histograms[i][j] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;
            
            outStream << endl << "% The total Number of pages of data" << endl 
                              <<"numberOfPages = " << numberOfBatches << '*' << BATCH_SIZE << ';' << endl;
            outStream << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                              << "format longe" << endl;
            outStream << endl << "% The experimental probability values observed for the output bytes" << endl
                              << "experimentalProbs = histograms ./ numberOfPages";
}

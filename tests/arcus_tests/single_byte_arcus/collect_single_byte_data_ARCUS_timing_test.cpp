/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: collect_single_byte_data ARCUS.cpp
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: Produces 512 single byte outputs from an RC4Stream from randomly generated RC4Stream::Key values and collects these values in a histogram count, by byte stream position (1 - 512 inclusive). These values are then output to a data file which can be loaded for analysis in matlab. The data is dumped at intervals during generation to ensure any failure in the process is not catastophic to data gathering.
 * Date: May 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include "../../../MT19937_RandomSource.cpp"
#define PAGE_LENGTH 256 //The number of output bytes per page
#define PAGE_COUNT 2 //The number of pages of output per key
//Experimental parameters
uint64_t BATCH_SIZE = (1 << 25 ); //the number of pages per batch
uint32_t TOTAL_NO_OF_BATCHES = (1 << 1); //The number of batches to be run in this experiment
uint64_t histograms[PAGE_COUNT][PAGE_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH];
//function declarations
void dumpSingleByteData(std::ostream &outStream, uint64_t histograms[][PAGE_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH], uint32_t numberOfBatches, int proc_id);
using namespace std;

int main(int argc, char *argv[])
{
    //TODO encapsulate in a try block
    	//variable for tming test
   	clock_t begin, end;
	begin = clock();
  	int my_proc_id;

  	/*Initialize MPI*/
  	MPI_Init(&argc, &argv);
  
  	/*Work out how this process fits with the others*/
  	MPI_Comm_rank(MPI_COMM_WORLD,&my_proc_id);

        //initialize the file names for data storage based on the date, time of starting the experiment
        ostringstream oss;
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        oss << "./" << 'x' <<(now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteDataxtmp" << my_proc_id
					   << "A"
                                           << ".m";
        auto tmp0 = oss.str();
        const char* tmpFileName0 = tmp0.c_str(); 

        oss.str(string());
        oss << "./" << 'x' << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteDataxtmp" << my_proc_id
					   << "B"
                                           << ".m";
        auto tmp1 = oss.str();
        const char* tmpFileName1 = tmp1.c_str(); 

        oss.str(string());
        oss << "./x" << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "SingleByteData" << my_proc_id
                                           << ".m";
       auto tmp2 = oss.str();
        const char* dataOutputFileName = tmp2.c_str(); 

        //declare the output file streams
        ofstream tmpFile, dataOutputFile;

        //initialize the histogram and stream/key variables
        for (int i = 0; i < PAGE_COUNT ; i++) {
        	for (int j = 0; j < PAGE_LENGTH; j++) {
          		for (int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++) {
             			histograms[i][j][k] = 0;
          		}
        	}
	}

        //Construct the key;
        RC4Stream::Key key;

        //Construct the RC4 stream
        RC4Stream rc4Stream;

	//initialize seeding rng to be used to provide seeds which differ between processes with very high probability (my_proc_id will range from 0 to 31 at most)
	mt19937 seeder(time(0) + (my_proc_id * 10));
	uint_fast32_t seed;
	for(uint32_t i = 0; i < my_proc_id; i++) { //throw away my_proc_id outputs from the seeder
		seeder();
	}
	seed = seeder();
	cout << "Seed for process " << my_proc_id << ": " << seed << endl;
	
        //initialize Random Number Generation algorithm 
        MT19937_RandomSource randomSource;
	//Provide seed for initializing the RNG
        randomSource.initializeRandomNoGen(seed);

        //Loop over the number of batches
        for(uint32_t batchNo = 1; batchNo <= TOTAL_NO_OF_BATCHES; batchNo++){

            //for each batch produce BATCH_SIZE rc4 output pages from independent random keys and store in the histogram
            for(uint64_t pageNo = 0; pageNo < BATCH_SIZE; pageNo++){
                        
                //random key generation
                randomSource.selectRandomKey(key);
                 
                //rekey
                rc4Stream.keySchedule(key);
                 
                //run RC4 stream algorithm and collect output in histogram counters
                //collect page 1 data
               	for (int j = 0; j < PAGE_LENGTH; j++) {
                 	(histograms[0][j][rc4Stream.PRGRound()])++; //increment the relevant histogram count
                }
		//collect page 2 data
               	for (int j = 0; j < PAGE_LENGTH; j++) {
                 	(histograms[1][j][rc4Stream.PRGRound()])++; //increment the relevant histogram count
                }

            }

            //At the end of the batch output the histogram to a tmp file (tagged either 0 or 1 depending on the parity of the batch number, i.e. overwrite the last but one tmp file) include batch number and total so far and date/time (create a method for this which takes a file name in and writes in a usable format)
            if(batchNo & 1) {
                tmpFile.open(tmpFileName0, ios::trunc);
            } else {
                tmpFile.open(tmpFileName1, ios::trunc);
            }
            
            dumpSingleByteData(tmpFile, histograms, batchNo, my_proc_id);

            tmpFile.close();
        }

        //once all batches are complete output the final histogram data to a final data file including total number of batches as well as (in a comment) the date time of completion
        dataOutputFile.open(dataOutputFileName, ios::app);
        dumpSingleByteData(dataOutputFile, histograms, TOTAL_NO_OF_BATCHES, my_proc_id);
        dataOutputFile.close();

        // delete the tmp files
        remove(tmpFileName0); 
        remove(tmpFileName1);

      	end = clock();
      	double time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;
	cout << "Time spent on process "<< my_proc_id << ": " << time_spent << endl;
   	MPI_Finalize();
   	exit(EXIT_SUCCESS);
	
}


//dumps the single byte histogram data to the outStream. Assumes that the histogram has dimensions 2 x PAGE_LENGTH x PERMUTATION_ARRAY_LENGTH.
void dumpSingleByteData(std::ostream &outStream, uint64_t histograms[][PAGE_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH], uint32_t numberOfBatches, int proc_id){
            outStream << "% The histogram counts of the output bytes in page 1, each row is the histogram for a single output position from 1 - 256 inclusive" << endl;
            outStream << "histogramsPage1_id" << proc_id << " = [ " << endl;
            for(int j = 0; j < PAGE_LENGTH; j++) {
                outStream << '\t';
                for(int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++){
                    outStream << histograms[0][j][k] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;
            
            outStream << "% The histogram counts of the output bytes in page 2, each row is the histogram for a single output position from 257 - 512 inclusive" << endl;
            outStream << "histogramsPage2_id" << proc_id << " = [ " << endl;
            for(int j = 0; j < PAGE_LENGTH; j++) {
                outStream << '\t';
                for(int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++){
                    outStream << histograms[1][j][k] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;

            outStream << endl << "% The total Number of Keys used" << endl 
                              <<"numberOfKeys = " << numberOfBatches << '*' << BATCH_SIZE << ';' << endl;
            outStream << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                              << "format longe" << endl;
}

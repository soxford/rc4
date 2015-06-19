/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: collect_double_byte_data.cpp
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: Will produce histograms indexed by the internal state of the RC4 Stream (_i) and then indexed by the value of the the byte output and the value of the successor byte. The biases being investigated are longer term and require _permutationArray and _j to be "random" so we will ignore the first 2^10 bytes per AF:13. The quantity of bytes required is tbd but the requirement will be to get a long stream (perhaps 2^40 bytes for each key as per AF:13)
 * Date: June 2015
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream> //TODO remove when testing done
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cstdio>
#include <mpi.h>
#include "../MT19937_RandomSource.cpp"
//Define the number of histograms to collect as per dbSelectiveHistArcTest
#define HISTOGRAM_COUNT 4

//TODO change single byte code to meet double byte requirements once any testing is complete.
//Experimental parameters
int64_t BATCH_SIZE = ( ((int64_t)1) << 34); //the number of bytes per batch TODO will be (1 << 40) or (1 << 38)
int32_t TOTAL_NO_OF_BATCHES = (1 << 1); //The number of batches to be run in this experiment TODO will be (1 << 10) ?

//allocate space to hold table of results 4x256x256 as per dbSelectiveHistArcTest
uint64_t histograms[HISTOGRAM_COUNT][RC4Stream::PERMUTATION_ARRAY_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH]; 

//function declarations
void dumpDoubleByteData(std::ostream &outStream, uint64_t histograms[][RC4Stream::PERMUTATION_ARRAY_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH], uint32_t numberOfBatchesSoFar, int proc_id);
using namespace std;

int main(int argc, char *argv[])
{
  	int my_proc_id;

  	/*Initialize MPI*/
  	MPI_Init(&argc, &argv);
  
  	/*Work out how this process fits with the others*/
  	MPI_Comm_rank(MPI_COMM_WORLD,&my_proc_id);

        //initialize the file names for data storage based on the date, time of starting the experiment
        ostringstream oss;
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        oss << "./double_byte_data/" << 'x' <<(now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "DoubleByteDataxtmp" << my_proc_id
					   << "A"
                                           << ".m";
        auto tmp0 = oss.str();
        const char* tmpFileName0 = tmp0.c_str(); 

        oss.str(string());
        oss << "./double_byte_data/" << 'x' << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "DoubleByteDataxtmp" << my_proc_id
					   << "B"
                                           << ".m";
        auto tmp1 = oss.str();
        const char* tmpFileName1 = tmp1.c_str(); 

        oss.str(string());
        oss << "./double_byte_data/" << 'x' << (now->tm_year + 1900) << 'x'
                                           << (now->tm_mon + 1) << 'x'
                                           <<  now->tm_mday << 'x'
                                           <<  now->tm_hour << 'x'
                                           <<  now->tm_min << 'x'
                                           << "DoubleByteData" << my_proc_id
                                           << ".m";
       auto tmp2 = oss.str();
        const char* dataOutputFileName = tmp2.c_str(); 

        //declare the output file streams
        ofstream tmpFile, dataOutputFile;

        //initialize the histogram and stream/key variables
        for (int i = 0; i < HISTOGRAM_COUNT; i++) {
          for (int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
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
	for(int i = 0; i < my_proc_id; i++) { //throw away my_proc_id outputs from the seeder
		seeder();
	}
	seed = seeder();
	//TODO remove cout post testing
	cout << "Seed for process " << my_proc_id << ": " << seed << endl;
	
        //initialize Random Number Generation algorithm 
        MT19937_RandomSource randomSource;
	//Provide seed for initializing the RNG
        randomSource.initializeRandomNoGen(seed);
	
	//initialize variables for collecting data 
	int first_byte = 0, second_byte = 0;
        //Loop over the number of batches
        for(int batchNo = 1; batchNo <= TOTAL_NO_OF_BATCHES; batchNo++){
	    //generate and schedule a new key for each batch
            //random key generation
            randomSource.selectRandomKey(key);
                
            //rekey
            rc4Stream.keySchedule(key);

	    //discard the first 2^10 output bytes as per AF:13
	    for (int skipPosition = 0; skipPosition < (1 << 10) ; skipPosition++) {
		rc4Stream.PRGRound();
            }
	    //loop to generate the output stream and collect results
	    first_byte = rc4Stream.PRGRound(); //loop assumes that the first byte has been collected
	      
	    for (int64_t i = 0; i < BATCH_SIZE ; i++) {
		second_byte = rc4Stream.PRGRound();
		//map indices to histograms selectively as follows
		if(first_byte == 0x00){//collect data of the form (i, 0, k) in histograms[0][i][k]
			 histograms[0][i & MASK_8][second_byte]++; //increment the relevant histogram count
		} else if (first_byte == 0xff) { //(i, 255, k) -> histograms[1][i][k]
			 histograms[1][i & MASK_8][second_byte]++; //increment the relevant histogram count
		} else if (second_byte == 0xff) { //(i, j, 255) -> histograms[2][i][j]
			 histograms[2][i & MASK_8][first_byte]++; //increment the relevant histogram count
		} else { //all other (i, j, k) -> histograms[3][j][k]
			 histograms[3][first_byte][second_byte]++;
		}
		first_byte = second_byte; //prepare for next loop
	    }

            //At the end of the batch output the histogram to a tmp file (tagged either 0 or 1 depending on the parity of the batch number, i.e. overwrite the last but one tmp file) include batch number and total so far and date/time (create a method for this which takes a file name in and writes in a usable format)
            if(batchNo & 1) {
                tmpFile.open(tmpFileName1, ios::trunc);
            } else {
                tmpFile.open(tmpFileName0, ios::trunc);
            }
            
            dumpDoubleByteData(tmpFile, histograms, batchNo, my_proc_id);

            tmpFile.close();
        }

        //once all batches are complete output the final histogram data to a final data file including total number of batches as well as (in a comment) the date time of completion
        dataOutputFile.open(dataOutputFileName, ios::app);
        dumpDoubleByteData(dataOutputFile, histograms, TOTAL_NO_OF_BATCHES, my_proc_id);
        dataOutputFile.close();

        // delete the tmp files
        remove(tmpFileName0); 
        remove(tmpFileName1);
	
   	MPI_Finalize();
   	exit(EXIT_SUCCESS);
}


//dumps the single byte histogram data to the outStream. Assumes that the histogram has dimensions HISTOGRAM_COUNT and PERMUTATION_ARRAY_LENGTH.
void dumpDoubleByteData(std::ostream &outStream, uint64_t histograms[][RC4Stream::PERMUTATION_ARRAY_LENGTH][RC4Stream::PERMUTATION_ARRAY_LENGTH], uint32_t numberOfBatches, int proc_id){

            outStream << "% The histogram counts of the output bytes from rc4" << endl
		      << "% The histograms count occurrences of events of the form (i, j, k) = (i, S_i, S_i+1) where i is the _i partial-state variable for rc4 and S_i is an output from rc4 in that state and S_i+1 is the subsequent output" << endl << endl;

	    //First Histogram
	    outStream << "% The entry histogrami0k[i][k] is the count of occurences of (i, 0x00, k)  " << endl
            	      << "histogram_i0k_id" << proc_id << " = [ " << endl;
            for(int i = 0; i < RC4Stream::PERMUTATION_ARRAY_LENGTH; i++) {
                outStream << '\t';
                for(int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++){
                    outStream << histograms[0][i][k] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;
	
	    //Second Histogram
	    outStream << endl << "% The entry histogrami255k[i][k] is the count of occurences of (i, 0xff, k)  " << endl
            	      << "histogram_i255k_id" << proc_id << " = [ " << endl;
            for(int i = 0; i < RC4Stream::PERMUTATION_ARRAY_LENGTH; i++) {
                outStream << '\t';
                for(int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++){
                    outStream << histograms[1][i][k] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;

	    //Third Histogram
	    outStream << endl << "% The entry histogramij255[i][j] is the count of occurences of (i, j, 0xff)  " << endl
            	      << "histogram_ij255_id" << proc_id << " = [ " << endl;
            for(int i = 0; i < RC4Stream::PERMUTATION_ARRAY_LENGTH; i++) {
                outStream << '\t';
                for(int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++){
                    outStream << histograms[2][i][j] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;

	    //Final/Fourth Histogram
	    outStream << endl << "% The entry histogramDefault[j][k] is the count of occurences of (i, j, k) for all i, which were not counted in any other histogram" << endl
            	      << "histogramDefault_id" << proc_id << " = [ " << endl;
            for(int j = 0; j < RC4Stream::PERMUTATION_ARRAY_LENGTH; j++) {
                outStream << '\t';
                for(int k = 0; k < RC4Stream::PERMUTATION_ARRAY_LENGTH; k++){
                    outStream << histograms[3][j][k] << ' ';   
                }
                outStream << ';' << endl;
            }
            outStream << "\t];" << endl;


	    //Record the number of bytes collected and calculate the experimental conditional probabilities
            outStream << endl << "% The total Number of bytes of data collected for each index i" << endl 
                              <<"numberOfBytesCollectedPerI = " << numberOfBatches << '*' << BATCH_SIZE << "/" << RC4Stream::PERMUTATION_ARRAY_LENGTH << ';' << endl

	     		      //TODO double check supplementation of histograms here
			      //Ensure histogramij255 is complete
		      << endl << "% Ensure that the histograms contain all required data by supplementing them from each other where necessary" << endl
			      << "% Supplement histogram_ij255_id" << proc_id << " from the last columns of i0k and i255k" << endl
			      << "histogram_ij255_id" << proc_id << "(:, 1) = histogram_i0k_id" << proc_id << "(:, 256);" << endl
			      << "histogram_ij255_id" << proc_id << "(:, 256) = histogram_i255k_id" << proc_id << "(:, 256);" << endl << endl

			      //Ensure experimentalProbsDefault is complete
			      << "% Supplement histogramDefault from the previous three histograms" << endl
			      << "histogramDefault_id" << proc_id << "(1, :) = sum(histogram_i0k_id" << proc_id << ");" << endl
			      << "histogramDefault_id" << proc_id << "(256, :) = sum(histogram_i255k_id" << proc_id << ");" << endl
			      << "histogramDefault_id" << proc_id << "(2:255, 256) = (sum(histogram_ij255_id" << proc_id << "(:, 2:255))).\';" << endl

             	      << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                              << "format longe" << endl
             	      << endl << "% The experimental conditional probability values observed for the output double bytes" << endl
                      	      << "experimentalCondProbs_i0k_id" << proc_id << " = histogram_i0k_id" << proc_id << " ./ numberOfBytesCollectedPerI;" << endl
                      	      << "experimentalCondProbs_i255k_id" << proc_id << " = histogram_i255k_id" << proc_id << " ./ numberOfBytesCollectedPerI;" << endl
                      	      << "experimentalCondProbs_ij255_id" << proc_id << " = histogram_ij255_id" << proc_id << " ./ numberOfBytesCollectedPerI;" << endl
                      	      << "experimentalProbsDefault_id" << proc_id << " = histogramDefault_id" << proc_id << " ./ (numberOfBytesCollectedPerI * " << RC4Stream::PERMUTATION_ARRAY_LENGTH << ");" << endl;

	    //print intstructions on using the data histograms
	    outStream << endl << "%Print out instructions for using the data histograms/probabilities" << endl
		      	      << "fprintf(\"experimentalCondProbs_i0k_id" << proc_id << "(i+1,k+1) contains the experimental probability conditioned on the value of i for (i, 0x00, k), i.e. the event the the rc4 stream output 0x00 followed by k given initial partial state i.\\n\");" << endl
		      	      << "fprintf(\"experimentalCondProbs_i255k_id" << proc_id << "(i+1, k+1) contains the experimental probability conditioned on the value of i for (i, 0xff, k), i.e. the event the the rc4 stream output 0xff followed by k given initial partial state i.\\n\");" << endl
		      	      << "fprintf(\"experimentalCondProbs_ij255_id" << proc_id << "(i+1, j+1) contains the experimental probability conditioned on the value of i for (i, j, 0xff), i.e. the event the the rc4 stream output j followed by 0xff given initial partial state i.\\n\");" << endl
		      	      << "fprintf(\"experimentalProbsDefault_id" << proc_id << "(j+1, k+1) contains the experimental probability (NOT conditioned on the value of i) for (i, j, k) for all i. i.e. the event the the rc4 stream output j followed by k regardless of the initial partial state i.\\n\");";
}

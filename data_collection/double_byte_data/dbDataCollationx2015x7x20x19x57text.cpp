/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: 
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: outputs to stdout the matlab code required to load the 128 different sets of 4 histograms into one complete set of 4 incorporating all of the data collected
 * License: 
 * Date: Aug 2015   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
using namespace std;
int totalNoOfParallelDataRuns = 128;
int main(int argc, const char *argv[])
{
   cout << "x2015x7x20x19x57xDoubleByteData0;" << endl
        << "histogram_i1k = histogram_i1k_id0;" << endl
        << "histogram_i254k = histogram_i254k_id0;" << endl
        << "histogram_i128k = histogram_i128k_id0;" << endl
        << "histogram_ij254 = histogram_ij254_id0;" << endl << endl; 

   for (int i = 1; i < totalNoOfParallelDataRuns; i++) {
      cout << "x2015x7x20x19x57xDoubleByteData" << i << ";" << endl
           << "histogram_i1k = histogram_i1k + histogram_i1k_id" << i << ";" << endl
           << "histogram_i254k = histogram_i254k + histogram_i254k_id" << i << ";" << endl
           << "histogram_i128k = histogram_i128k + histogram_i128k_id" << i << ";" << endl
           << "histogram_ij254 = histogram_ij254 + histogram_ij254_id" << i << ";" << endl;
      cout << "clearvars -except histogram_i1k histogram_i254k histogram_i128k histogram_ij254 numberOfBytesCollectedPerI;" << endl << endl;
   }
   cout << "numberOfBytesCollectedPerI = numberOfBytesCollectedPerI * " << totalNoOfParallelDataRuns << ";" << endl; 

   cout << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                << "format longe" << endl
                << endl << "% The experimental conditional probability values observed for the output double bytes" << endl
                << "experimentalCondProbs_i1k = histogram_i1k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_i254k = histogram_i254k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_i128k = histogram_i128k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_ij254 = histogram_ij254 ./ (numberOfBytesCollectedPerI * 256);" << endl;

   //print intstructions on using the data 
   cout      << endl << "%Print out instructions for using the data histograms/probabilities" << endl
                     << "fprintf(\'experimentalCondProbs_i1k(i+1,k+1) contains the experimental probability conditioned on the value of i for (i, 0x01, k), i.e. the event the the rc4 stream output 0x00 followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_i254k(i+1, k+1) contains the experimental probability conditioned on the value of i for (i, 0xfe, k), i.e. the event the the rc4 stream output 0xff followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_i128k(i+1, j+1) contains the experimental probability conditioned on the value of i for (i, j, 0x80), i.e. the event the the rc4 stream output j followed by 0xff given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_ij254(j+1, k+1) contains the experimental probability (NOT conditioned on the value of i) for (i, j, 0xfe) for all i. i.e. the event the the rc4 stream output j followed by k regardless of the initial partial state i.\\n\');";
   return 0;
}

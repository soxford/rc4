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
   cout << "x2015x8x15x12x14xDoubleByteData0;" << endl
        << "histogram_i13k = histogram_i13k_id0;" << endl
        << "histogram_i240k = histogram_i240k_id0;" << endl
        << "histogram_i129k = histogram_i129k_id0;" << endl
        << "histogram_ij240 = histogram_ij240_id0;" << endl << endl; 

   for (int i = 1; i < totalNoOfParallelDataRuns; i++) {
      cout << "x2015x8x15x12x14xDoubleByteData" << i << ";" << endl
           << "histogram_i13k = histogram_i13k + histogram_i13k_id" << i << ";" << endl
           << "histogram_i240k = histogram_i240k + histogram_i240k_id" << i << ";" << endl
           << "histogram_i129k = histogram_i129k + histogram_i129k_id" << i << ";" << endl
           << "histogram_ij240 = histogram_ij240 + histogram_ij240_id" << i << ";" << endl;
      cout << "clearvars -except histogram_i13k histogram_i240k histogram_i129k histogram_ij240 numberOfBytesCollectedPerI;" << endl << endl;
   }
   cout << "numberOfBytesCollectedPerI = numberOfBytesCollectedPerI * " << totalNoOfParallelDataRuns << ";" << endl; 

   cout << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                << "format longe" << endl
                << endl << "% The experimental conditional probability values observed for the output double bytes" << endl
                << "experimentalCondProbs_i13k = histogram_i13k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_i240k = histogram_i240k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_i129k = histogram_i129k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_ij240 = histogram_ij240 ./ numberOfBytesCollectedPerI;" << endl;

   //print intstructions on using the data 
   cout      << endl << "%Print out instructions for using the data histograms/probabilities" << endl
                     << "fprintf(\'experimentalCondProbs_i13k(i+1,k+1) contains the experimental probability conditioned on the value of i for (i, 0x0d1, k), i.e. the event the the rc4 stream output 0x00 followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_i240k(i+1, k+1) contains the experimental probability conditioned on the value of i for (i, 0xf0, k), i.e. the event the the rc4 stream output 0xff followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_i129k(i+1, j+1) contains the experimental probability conditioned on the value of i for (i, j, 0x81), i.e. the event the the rc4 stream output j followed by 0xff given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_ij240(j+1, k+1) contains the experimental probability (NOT conditioned on the value of i) for (i, j, 0xf0) for all i.\')";
   return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: 
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: outputs to stdout the matlab code required to load the 128 different sets of 4 histograms into one complete set of 4 incorporating all of the data collected
 * License: 
 * Date: June 2015   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
using namespace std;
int totalNoOfParallelDataRuns = 128;
int main(int argc, const char *argv[])
{
   cout << "x2015x6x19x22x9xDoubleByteData0;" << endl
        << "histogram_i0k = histogram_i0k_id0;" << endl
        << "histogram_i255k = histogram_i255k_id0;" << endl
        << "histogram_ij255 = histogram_ij255_id0;" << endl
        << "histogramDefault = histogramDefault_id0;" << endl << endl; 

   for (int i = 1; i < totalNoOfParallelDataRuns; i++) {
      cout << "x2015x6x19x22x9xDoubleByteData" << i << ";" << endl
           << "histogram_i0k = histogram_i0k + histogram_i0k_id" << i << ";" << endl
           << "histogram_i255k = histogram_i255k + histogram_i255k_id" << i << ";" << endl
           << "histogram_ij255 = histogram_ij255 + histogram_ij255_id" << i << ";" << endl
           << "histogramDefault = histogramDefault + histogramDefault_id" << i << ";" << endl;
      cout << "clearvars -except histogram_i0k histogram_i255k histogram_ij255 histogramDefault numberOfBytesCollectedPerI;" << endl << endl;
   }
   cout << "numberOfBytesCollectedPerI = numberOfBytesCollectedPerI * " << totalNoOfParallelDataRuns << ";" << endl; 

   cout << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                << "format longe" << endl
                << endl << "% The experimental conditional probability values observed for the output double bytes" << endl
                << "experimentalCondProbs_i0k = histogram_i0k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_i255k = histogram_i255k ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalCondProbs_ij255 = histogram_ij255 ./ numberOfBytesCollectedPerI;" << endl
                << "experimentalProbsDefault = histogramDefault ./ (numberOfBytesCollectedPerI * 256);" << endl;

   //print intstructions on using the data 
   cout      << endl << "%Print out instructions for using the data histograms/probabilities" << endl
                     << "fprintf(\'experimentalCondProbs_i0k(i+1,k+1) contains the experimental probability conditioned on the value of i for (i, 0x00, k), i.e. the event the the rc4 stream output 0x00 followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_i255k(i+1, k+1) contains the experimental probability conditioned on the value of i for (i, 0xff, k), i.e. the event the the rc4 stream output 0xff followed by k given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalCondProbs_ij255(i+1, j+1) contains the experimental probability conditioned on the value of i for (i, j, 0xff), i.e. the event the the rc4 stream output j followed by 0xff given initial partial state i.\\n\');" << endl
                     << "fprintf(\'experimentalProbsDefault(j+1, k+1) contains the experimental probability (NOT conditioned on the value of i) for (i, j, k) for all i. i.e. the event the the rc4 stream output j followed by k regardless of the initial partial state i.\\n\');";
   return 0;
}

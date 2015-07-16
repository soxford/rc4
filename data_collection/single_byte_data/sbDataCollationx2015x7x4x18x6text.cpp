/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Title: 
 * Author: Simon Campbell, simonhmcampbell@gmail.com
 * Description: outputs to stdout the matlab code required to load the 128 different sets of 2 histograms into one complete set of 2 incorporating all of the data collected
 * License: 
 * Date: July 2015   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
using namespace std;
int totalNoOfParallelDataRuns = 128;
int main(int argc, const char *argv[])
{
   cout << "x2015x7x4x18x6xSingleByteData0;" << endl
        << "histogramsPage1 = histogramsPage1_id0;" << endl
        << "histogramsPage2 = histogramsPage2_id0;" << endl << endl; 

   for (int i = 1; i < totalNoOfParallelDataRuns; i++) {
      cout << "x2015x7x4x18x6xSingleByteData" << i << ";" << endl
           << "histogramsPage1 = histogramsPage1 + histogramsPage1_id" << i << ";" << endl
           << "histogramsPage2 = histogramsPage2 + histogramsPage2_id" << i << ";" << endl;
      cout << "clearvars -except histogramsPage1 histogramsPage2 numberOfKeys;" << endl << endl;
   }
   cout << "numberOfKeys = numberOfKeys * " << totalNoOfParallelDataRuns << ";" << endl; 

   cout << endl << "% Set the format to longe to capture the differences in the experimental probabilities" << endl
                << "format longe" << endl
                << endl << "% The experimental probability values observed for the output byte positons" << endl
                << "experimentalProbsPage1 = histogramsPage1 ./ numberOfKeys;" << endl
                << "experimentalProbsPage2 = histogramsPage2 ./ numberOfKeys;" << endl;

   return 0;
}

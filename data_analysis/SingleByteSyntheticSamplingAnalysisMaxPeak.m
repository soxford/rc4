%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the average number of
%guesses to get the correct message byte by matching the peak value in
%order
format longe;
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
%No of synthesised samplings for page 1
samplecountPage1 = 28;
[logSyntheticSampleSizesPage1,averageNoOfGuessesPage1] = GuessingEntropyWithMaxPeak(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 experimentalProbsPage2
save('MaxPeakSamplingSynthesisResultsGuessingEntropySingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 37;
[logSyntheticSampleSizesPage2,averageNoOfGuessesPage2] = GuessingEntropyWithMaxPeak(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 averageNoOfGuessesPage2 experimentalProbsPage2;
save('MaxPeakSamplingSynthesisResultsGuessingEntropySingleBytesFirst512Positions.mat');

clearvars;
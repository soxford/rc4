%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the average number of
%guesses to get the correct message byte by matching the peak value in
%order
%GUESSING ENTROPY ANALYSIS
format longe;
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
clearvars -except experimentalProbsPage1 experimentalProbsPage2

%No of synthesised samplings for page 1
samplecountPage1 = 34;
[logSyntheticSampleSizesPage1,averageNoOfGuessesPage1] = GuessingEntropyWithMaxPeak(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 experimentalProbsPage2
save('MaxPeakRERUNResultsGuessingEntropySingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 44;
[logSyntheticSampleSizesPage2,averageNoOfGuessesPage2] = GuessingEntropyWithMaxPeak(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 averageNoOfGuessesPage2 experimentalProbsPage2;
save('MaxPeakRERUNResultsGuessingEntropySingleBytesFirst512Positions.mat');

clearvars;

%PERCENTAGE CORRECT ANALYSIS
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
clearvars -except experimentalProbsPage1 experimentalProbsPage2
%No of synthesised samplings for page 1
samplecountPage1 = 55;
[logSyntheticSampleSizesPage1,maxPeakPercentCorrectOnFirstTryPage1] = PercentageFirstTryWithMaxPeak(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 maxPeakPercentCorrectOnFirstTryPage1 experimentalProbsPage1 experimentalProbsPage2
save('MaxPeakRERUNResultsPercentCorrectSingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 70;
[logSyntheticSampleSizesPage2,maxPeakPercentCorrectOnFirstTryPage2] = PercentageFirstTryWithMaxPeak(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 maxPeakPercentCorrectOnFirstTryPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 maxPeakPercentCorrectOnFirstTryPage2 experimentalProbsPage2;
save('MaxPeakRERUNResultsPercentCorrectSingleBytesFirst512Positions.mat');

clearvars;
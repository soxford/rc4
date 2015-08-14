%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes based on collected data. This data is then used to measure the average number of
%guesses to get the correct message byte by using Bayesian analysis to
%identify the most likely message bytes in order.
format longe;
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
%No of synthesised samplings for page 1
samplecountPage1 = 24;
[logSyntheticSampleSizesPage1,averageNoOfGuessesPage1] = GuessingEntropyWithBayes(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 experimentalProbsPage2
save('BayesianSamplingSynthesisResultsGuessingEntropySingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 33;
[logSyntheticSampleSizesPage2,averageNoOfGuessesPage2] = GuessingEntropyWithBayes(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 averageNoOfGuessesPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 averageNoOfGuessesPage2 experimentalProbsPage2;
save('BayesianSamplingSynthesisResultsGuessingEntropySingleBytesFirst512Positions.mat');

clearvars;
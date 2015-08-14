%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes based on collected data. This data is then used to measure the average number of
%guesses to get the correct message byte by using Bayesian analysis to
%identify the most likely message bytes in order.
format longe;
%BAYES ANALYSIS
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
%No of synthesised samplings for page 1
samplecountPage1 = 45;
[logSyntheticSampleSizesPage1,bayesPercentCorrectOnFirstTryPage1] = PercentageFirstTryWithBayes(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 bayesPercentCorrectOnFirstTryPage1 experimentalProbsPage1 experimentalProbsPage2
save('BayesianSamplingSynthesisResultsPercentCorrectSingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 50;
[logSyntheticSampleSizesPage2,bayesPercentCorrectOnFirstTryPage2] = PercentageFirstTryWithBayes(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 bayesPercentCorrectOnFirstTryPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 bayesPercentCorrectOnFirstTryPage2 experimentalProbsPage2;
save('BayesianSamplingSynthesisResultsPercentCorrectSingleBytesFirst512Positions.mat');

clearvars;

%MAX PEAK ANALYSIS
load('../data_collection/single_byte_data/probsx2015x7x4x18x6x.mat');
%No of synthesised samplings for page 1
samplecountPage1 = 55;
[logSyntheticSampleSizesPage1,maxPeakPercentCorrectOnFirstTryPage1] = PercentageFirstTryWithMaxPeak(experimentalProbsPage1, samplecountPage1);

clearvars -except logSyntheticSampleSizesPage1 maxPeakPercentCorrectOnFirstTryPage1 experimentalProbsPage1 experimentalProbsPage2
save('MaxPeakSamplingSynthesisResultsPercentCorrectSingleBytesFirst512Positions.mat');

%No of synthesised samplings for page 2
samplecountPage2 = 70;
[logSyntheticSampleSizesPage2,maxPeakPercentCorrectOnFirstTryPage2] = PercentageFirstTryWithMaxPeak(experimentalProbsPage2, samplecountPage2);

clearvars -except logSyntheticSampleSizesPage1 maxPeakPercentCorrectOnFirstTryPage1 experimentalProbsPage1 logSyntheticSampleSizesPage2 maxPeakPercentCorrectOnFirstTryPage2 experimentalProbsPage2;
save('MaxPeakSamplingSynthesisResultsPercentCorrectSingleBytesFirst512Positions.mat');

clearvars;
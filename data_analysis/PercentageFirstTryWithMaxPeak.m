%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the Percentage of guesses that are correct on the first try using Bayesian analysis to
%identify max peak message byte.
function [logSampleSizes,percentCorrectOnFirstTry] = PercentageFirstTryWithMaxPeak(experimentalProbs, samplecount)% Number of outcomes for each byte position, required size of the noise
% vector = k
% Number of byte positions = bytePositionCount
[bytePositionCount, k] = size(experimentalProbs);
%base power for each sample size
basepower = 8;
%number of sample sizes tested
%samplecount = 37;
%get the index of the maximum peak in the distribution of the first byte
%(i.e. the index that would correctly identify the message byte if we
%looked for the maximum peak to break the cipher
correctDistributions = experimentalProbs.';
[~, correctValIndices] = max(correctDistributions, [], 1);
%matrix for collecting results for each synthetic experiment
percentCorrectOnFirstTry = zeros(samplecount, bytePositionCount);
%Number of synthesised samplings overwhich we will average performance
noOfSynthesisedSamplings = 20000;

for i = 1:samplecount
    power = basepower + i;
    simulatedNoOfSamples = 2^power;
    
    %uniform distribution standard deviation (approximation of the standard
    %deviation for each experimental probability observation)
    sigma = sqrt((1/256)*(1-1/256)/simulatedNoOfSamples);
    
    %Generate noOfSynthesisedSamplings normally distributed vectors of
    %noise of size 256
    Q = null(ones(1,k));
     X = sqrt(k/(k-1))*sigma*Q*randn(k-1,noOfSynthesisedSamplings);
    %X is a 256 by noOfSythesisedSamplings matrix that is meant to have each
    %column summing to 0 and each row being samples from a normal
    %distribution with 0 mean and the appropriate standard deviation for the
    %chosen byte value for the selected byte position (infact using uniform
    %sigma for all values as an approximation)
    
    %Count how many first time guesses are correct by
    %searching through the peaks in order (assume that if multiple values
    %have the same value then all of theme are tried)
    countOfCorrectFirstTimeGuesses = zeros(1, bytePositionCount);

    for col = 1:noOfSynthesisedSamplings
        %Calculate the synthesised sampled distribution
        sythesisedSample = repmat(X(:, col), 1, bytePositionCount) + correctDistributions;
        %check the synthesised sample has the correct max peaks 
        [~, I] = max(sythesisedSample, [], 1);
        successes = (I == correctValIndices);
        %count the number of successesful guesses
        countOfCorrectFirstTimeGuesses = countOfCorrectFirstTimeGuesses + successes;
    end
    %output the percentage of guesses that the samplings would have
    % guessed the correct message byte vlaue sample size
    percentCorrectOnFirstTry(i, :) = countOfCorrectFirstTimeGuesses/noOfSynthesisedSamplings;
end

    logSampleSizes = basepower+1 : basepower+samplecount;
end


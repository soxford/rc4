%Code to generate 20000 synthetic random samples for each ciphertext 
%count from 2^{8+1} up to 2^{8+samplecount}.This data is then used 
%to measure the Percentage of guesses that are correct on the first 
%try using the "max-peak" attack. 
%The inputs are the a matrix of distributions for each byte position
%under consideration, which are taken to be the correct RC4 output 
%distributions, and a samplecount, which indicates the number of 
%different ciphertext counts to be considered. 
%The ouputs are the log2 of the ciphertext counts tried and the average 
%number of guesses required for each ciphertext count value and output
%position.
function [logCiphertextCounts,percentCorrectOnFirstTry] = PercentageFirstTryWithMaxPeak(obsSingleByteDistributions, samplecount)
%Number of synthesised samplings overwhich we will average performance
noOfSynthesisedSamplings = 30000;
%get the (assumed) correct distributions for each byte position
correctDistributions = obsSingleByteDistributions.';
% Number of byte positions = bytePositionCount
[bytePositionCount, k] = size(obsSingleByteDistributions);
%base power for each sample size (ciphertext counts of less than 
%2^8 are likely to be insufficient).
basepower = 8;
%get the index of the maximum peak in the distribution of the first 
%byte (i.e. the index that would correctly identify the message 
%byte if we looked for the maximum peak to break the cipher
[~, correctValIndices] = max(correctDistributions, [], 1);
%matrix for collecting results for each synthetic experiment
percentCorrectOnFirstTry = zeros(samplecount, bytePositionCount);

for i = 1:samplecount
    power = basepower + i;
    simulatedNoOfCipherTextSamples = 2^power;
    
    %uniform distribution standard deviation (used as approximation of
    %the standard deviation for each experimental probability 
    %observation)
    sigma = sqrt((1/256)*(1-1/256)/simulatedNoOfCipherTextSamples);
    
    %Generate noOfSynthesisedSamplings normally distributed vectors of
    %noise of size 256
    Q = null(ones(1,k));
     X = sqrt(k/(k-1))*sigma*Q*randn(k-1,noOfSynthesisedSamplings);
    %X is a 256 by noOfSythesisedSamplings matrix that has each
    %column summing to 0 and each row being samples from a normal
    %distribution with 0 mean and standard deviation sigma.
    
    %Count how many times the first time guess is correct
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
    logCiphertextCounts = basepower+1 : basepower+samplecount;
end


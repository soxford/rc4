%Code to generate 15000 synthetic random samples for each ciphertext 
%count from 2^{8+1} up to 2^{8+samplecount}. This data is then used
%to measure the average number of guesses to get the correct message
%byte using the "max-peak" attack. 
%The inputs are the a matrix of distributions for each byte position
%under consideration, which are taken to be the correct RC4 output 
%distributions, and a samplecount, which indicates the number of 
%different ciphertext counts to be considered. 
%The ouputs are the log2 of the ciphertext counts tried and the average 
%number of guesses required for each ciphertext count value and output
%position.
function [logCiphertextCounts,averageNoOfGuesses] = GuessingEntropyWithMaxPeak(obsSingleByteDistributions, samplecount)
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
[~, correctValIndices] = max(obsSingleByteDistributions, [], 2);
%matrix for collecting results for each synthetic experiment
averageNoOfGuesses = ones(samplecount, bytePositionCount);
%loop to simulate samplings for each ciphertext count value
for i = 1:samplecount
    power = basepower + i;
    simulatedNoOfCipherTextSamples = 2^power;
    
    %uniform distribution standard deviation (used as approximation of the 
    %standard deviation for each experimental probability observation)
    sigma = sqrt((1/256)*(1-1/256)/simulatedNoOfCipherTextSamples);
    
    %Generate noOfSynthesisedSamplings normally distributed vectors of
    %noise of size 256
    Q = null(ones(1,k));
     X = sqrt(k/(k-1))*sigma*Q*randn(k-1,noOfSynthesisedSamplings);
    %X is a 256 by noOfSythesisedSamplings matrix that has each
    %column summing to 0 and each row being samples from a normal
    %distribution with 0 mean and standard deviation sigma.
    
    %Count how many guesses would be required to find the correct value 
    %of m_r (which is 0 in this case) by searching through the peaks in 
    %order (assume that if multiple candidates have the same value then 
    %all of theme are tried to give a worst case analysis)
    sumOfGuessesRequired = zeros(1, bytePositionCount);
    for col = 1:noOfSynthesisedSamplings
        %Calculate the synthesised sampled distributions
        sythesisedSample = repmat(X(:, col), 1, bytePositionCount) + correctDistributions;
        %count the number of elements of this sample which are bigger 
        %than the value that is observed for the correct index for 
        %each byteValue
        for bytePosition = 1:bytePositionCount
            bytePositionSynthesisedSample = sythesisedSample(:, bytePosition);
            [m, n] = size(bytePositionSynthesisedSample(bytePositionSynthesisedSample >= bytePositionSynthesisedSample(correctValIndices(bytePosition,1), 1)));
            sumOfGuessesRequired(1, bytePosition) = sumOfGuessesRequired(1, bytePosition) + m;
        end
    end
    %output the average number of guesses that the samplings would have
    %required to guess the correct message byte vlaue for this byte value
    averageNoOfGuesses(i, :) = sumOfGuessesRequired/noOfSynthesisedSamplings;
end

    logCiphertextCounts = basepower+1 : basepower+samplecount;
end


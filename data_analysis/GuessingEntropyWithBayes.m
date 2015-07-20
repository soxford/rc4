%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the average number of
%guesses to get the correct message byte by using Bayesian analysis to
%identify the most likely message bytes in order.
function [logSampleSizes,averageNoOfGuesses] = GuessingEntropyWithBayes(experimentalProbs, samplecount)
%%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the average number of
%guesses to get the correct message byte by using Bayesian analysis to
%identify the most likely message bytes in order. Number of outcomes for each byte position, required size of the noise 
% vector = k
% Number of byte positions = bytePositionCount
[bytePositionCount, k] = size(experimentalProbs);
%base power for each sample size
basepower = 8;
%number of sample sizes tested is now provided by the function
%samplecount = 35;
%get the index of the maximum peak in the distribution of the first byte
%(i.e. the index that would correctly identify the message byte if we
%looked for the maximum peak to break the cipher
[~, correctValIndices] = max(experimentalProbs, [], 2);
%matrix for collecting results for each synthetic experiment
averageNoOfGuesses = ones(samplecount, bytePositionCount);
%Number of synthesised samplings overwhich we will average performance
noOfSynthesisedSamplings = 15000;

%Permutations on cipher stream values produced by XORing with byte values 0
%-255
xorPermutations = repmat( 0:k-1, k, 1);
for messageByteVal = 0:k-1
   %permute the rows under the xor permutation with each message Byte value
   %and add 1 to make valid indices for use in permuting rows of matrices
   xorPermutations(messageByteVal + 1, :) = bitxor(xorPermutations(messageByteVal + 1, :), messageByteVal) + 1;
end

%Run sampling synthesis and analysis
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
    
    %Count how many guesses would be required to find the correct value by
    %searching through the peaks in order (assume that if multiple values
    %have the same value then all of theme are tried)
    sumOfGuessesRequired = zeros(1, bytePositionCount);
    correctDistributions = experimentalProbs.';
    for col = 1:noOfSynthesisedSamplings
        %Calculate the synthesised sample results
        sythesisedSample = (repmat(X(:, col), 1, bytePositionCount) + correctDistributions)* simulatedNoOfSamples;
        
        %count the number of elements of this sample which are bigger than
        %the value that is observed for the correct index for each
        %byteValue
                    %generate the log compound conditional probability vector for
            %each possible message byte (using bayes the message byte is
            % guessed to be the maximizing value) (if (x0, x1, ..., x255)
            % is the vector of observed frequencies of cipher bytes then
            % the log compund conditional probability for distribution (p0,
            % ..., p255) is xi logpi (einstein summation)
        logCompoundCondProbs = zeros(k, bytePositionCount);
        for messageByte = 0:k-1
            logCompoundCondProbs(messageByte + 1, :) = sum(sythesisedSample .* log(correctDistributions(xorPermutations(messageByte+1, :) , :) ) );
        end
        
        for bytePosition = 1:bytePositionCount
            tmp = logCompoundCondProbs(:, bytePosition);
            % get the number of guesses to get the correct message byte (0)
            % if we use the bayesian/MLE order on the guesses 
            [m, n] = size(tmp(tmp >= tmp(1, 1)));
            sumOfGuessesRequired(1, bytePosition) = sumOfGuessesRequired(1, bytePosition) + m;
        end
        
    end
    %output the average number of guesses that the samplings would have
    %required to guess the correct message byte vlaue for this byte value
    averageNoOfGuesses(i, :) = sumOfGuessesRequired/noOfSynthesisedSamplings;
end

    logSampleSizes = basepower+1 : basepower+samplecount; %These are the xvalues against which we plot the guessing entropy
end



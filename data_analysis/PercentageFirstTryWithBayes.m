%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the Percentage of guesses that are correct on the first try using Bayesian analysis to
%identify the most likely message byte.
function [logSampleSizes, percentCorrectOnFirstTry] = PercentageFirstTryWithBayes(experimentalProbs, samplecount)
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
%matrix for collecting results for each synthetic experiment
percentCorrectOnFirstTry = zeros(samplecount, bytePositionCount);
%Number of synthesised samplings overwhich we will average performance
noOfSynthesisedSamplings = 20000;

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
    
    %Count how many first time guesses are correct by
    %searching through the peaks in order (assume that if multiple values
    %have the same value then all of theme are tried)
    countOfCorrectFirstTimeGuesses = zeros(1, bytePositionCount);
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
        
        % get the message byte values with the highest log likelyhood (i.e.
        % the row indices (-1) for the highest value in each column).
        [~, I] = max(logCompoundCondProbs, [], 1);
        
        % get put 1 where I has 1 and 0 otherwise
        successes = arrayfun(@(x) x==1, I);
        countOfCorrectFirstTimeGuesses = countOfCorrectFirstTimeGuesses + successes;
    end
    %output the percentage number of first guesses that the samplings would have
    %given as the correct message byte vlaue for this sample size
    percentCorrectOnFirstTry(i, :) = countOfCorrectFirstTimeGuesses/noOfSynthesisedSamplings;
end
    logSampleSizes = basepower+1 : basepower+samplecount; %These are the xvalues against which we plot the guessing entropy
end



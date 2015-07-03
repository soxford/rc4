%Code to generate synthetic random samples of single byte rc4 data of
%varying sizes. This data is then used to measure the average number of
%guesses to get the correct message byte by using Bayesian analysis to
%identify the most likely message bytes in order.
%TODODODODODODODODODODODODODODODODO
%modify to use Bayesian analysis to identify the ordering of guesses
format longe;
% Number of outcomes for each byte position, required size of the noise
% vector = k
% Number of byte positions = bytePositionCount
[bytePositionCount, k] = size(experimentalProbs);
%base power for each sample size
basepower = 9;
%number of sample sizes tested
samplecount = 37;
%get the index of the maximum peak in the distribution of the first byte
%(i.e. the index that would correctly identify the message byte if we
%looked for the maximum peak to break the cipher
[~, correctValIndices] = max(experimentalProbs, [], 2);
%matrix for collecting results for each synthetic experiment
averageNoOfGuesses = ones(samplecount, bytePositionCount);

%Number of synthesised samplings overwhich we will average performance
noOfSynthesisedSamplings = 10000;
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
        %Calculate the synthesised sampled distribution
        sythesisedSample = repmat(X(:, col), 1, bytePositionCount) + correctDistributions;
        %count the number of elements of this sample which are bigger than
        %the value that is observed for the correct index for each
        %byteValue
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

    xVals = basepower+1 : basepower+samplecount;
    %plot the average number of guesses against the sample size
    plot(xVals,averageNoOfGuesses(:,3));


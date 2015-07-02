%Code to generate synthetic random samples of single byte rc4 data of varying
%sizes. This data is then used to measure ease of recovering message bytes
%using different attack methodologies.
format longe;
%output byte to check
outputByteNo = 3;
%number of synthetic experiments
samplecount = 12;
%get teh index of the maximum peak in the distribution of the first byte
[~, expInd] = max(experimentalProbs(outputByteNo, :));
%array for collecting results for each synthetic experiment
successRate = ones(samplecount, 1);
%base power for each sample size
basepower = 10;
for i = 1:samplecount
    power = basepower + i;
    simulatedNoOfSamples = 2^power;
    %byte number and value specific standard deviations
    %sigmas = sqrt(experimentalProbs .* (1 - experimentalProbs) / simulatedNoOfSamples)
    k = 256; % Number of outcomes, required size of the noise vector
    %uniform distribution standard deviation
    sigma = sqrt((1/256)*(1-1/256)/simulatedNoOfSamples);

    Q = null(ones(1,k));
    %X = sqrt(k/(k-1))*(((sigmas(1,:).')).*(Q*randn(k-1,simulatedNoOfSamples)));
    %TODO Trying to get different standard deviations accross the range of values.
    %Currently using a constant standard deviation based on the sample size and
    %a uniform distribution of the 256 byte values:
    X = sqrt(k/(k-1))*sigma*Q*randn(k-1,simulatedNoOfSamples);
    %X is a 256 by simulatedNoOfSamples matrix that is meant to have each
    %column summing to one and each row being samples from a normal
    %distribution with 0 mean and the appropriate standard deviation for the
    %chosen byte value for the selected byte position
    %Should we try and specify the accurate sigma based on the best available
    %data, or should we treat sigma as if all byte values have the same standard deviation?
    
    %Find the maximum values for each column and get their row number in
    %Indices
    for col = 1: simulatedNoOfSamples
        X(:, col) = X(:, col) +  experimentalProbs(outputByteNo , :).';
    end
     %free up memory
    [~, Indices] = max(X, [], 1);
    %free up memory
    clearvars X;
    %Count how many samples agree on the maximum value with the correct
    %value
    [m, n] = size(Indices(Indices == expInd));
    clearvars Indices;
    
    %output the percentage observations which gave the correct index for
    %the maximum (i.e. correctly identified the message byte using the
    %maximum peak method)
    successRate(i, 1) = n*100/simulatedNoOfSamples;
end

    xVals = basepower+1 : basepower+samplecount;
    plot(xVals,successRate);
    % Test
    %sum(X(:, 1:10), 1)
    %std(X')

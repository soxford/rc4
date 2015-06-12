%Code to generate synthetic random samples of single byte rc4 data of varying
%sizes. This data is then used to measure ease of recovering message bytes
%using different attack methodologies.

simulatedNoOfSamples = 2^20;
sigmas = sqrt(experimentalProbs .* (1 - experimentalProbs) / simulatedNoOfSamples)
k = 256 % Number of outcomes, required size of the noise vector
sigma = sqrt((1/256)*(1-1/256)/simulatedNoOfSamples)
s = 0 % required sum of the noise vector

Q = null(ones(1,k));
%X = sqrt(k/(k-1))*(((sigmas(1,:).')).*(Q*randn(k-1,simulatedNoOfSamples)));
%TODO Trying to get different standard deviations accross the range of values.
%Currently using a constant standard deviation based on the sample size and
%a uniform distribution of the 256 byte values
X = sqrt(k/(k-1))*sigma*Q*randn(k-1,simulatedNoOfSamples); 
%X is a 256 by simulatedNoOfSamples matrix that is meant to have each
%column summing to one and each row being samples from a normal
%distribution with 0 mean and the appropriate standard deviation for the
%chosen byte value for the selected byte position
%Should we try and specify the accurate sigma based on the best available
%data, or should we treat sigma as if all byte values are the same?

% Test
sum(X(:, 1:10), 1)
%std(X')
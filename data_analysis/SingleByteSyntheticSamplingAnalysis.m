%TODO currently incorrect code, need to check theory and rewrite
%Code to generate synthetic random samples of single byte rc4 data of varying
%sizes. This data is then used to measure ease of recovering message bytes
%using different attack methodologies.

simulatedNoOfSamples = 2^20;
sigmas = sqrt(experimentalProbs .* (1 - experimentalProbs) / simulatedNoOfSamples)
k = 256 % Number of outcomes, required size of the noise vector

s = 0 % required sum of the noise vector

Q = null(ones(1,k));
X = (sigmas*sqrt(k/(k-1)))*Q*randn(k-1,simulatedNoOfSamples);


% Test
sum(X(:,1:10),1)
std(X')
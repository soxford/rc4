%This script plots the experimental probabilities of the values Pr[Z_r =
% 256 - r] for 1 <= r <= 256. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('probs_256_r') == 1
    % produce the X values, these are the byte positions (1 - 256)
    X = 1:256;

    %plot the X values against the Experimental probabilities for the byte
    %positions 1-256 for rc4 output value r in row output r
    plot(X, probs_256_r(:, 1));
    % Now convert the existing tick marks.
    % Define the denominator we want to use.
    baseDenominator = 256;
    power = 2;
    denominator = baseDenominator^power;
    % For R2014a and earlier:
    ax = gca;
    New_Y_Tick = [probs_256_r(1,1) 1/baseDenominator  probs_256_r(128,1) probs_256_r(112,1) probs_256_r(96,1) probs_256_r(80,1) probs_256_r(64,1) probs_256_r(48,1) probs_256_r(32,1) probs_256_r(16,1) ];
    set(ax,'YTick',New_Y_Tick)
    New_Y_Tick_Numerators = New_Y_Tick * denominator;
    
    % Now make up a cell array of all the tick labels.
    for k = 1:length(New_Y_Tick_Numerators)
        y_tick_labels{k} = sprintf('%.6f ', New_Y_Tick_Numerators(k) - baseDenominator);
    end
    y_tick_labels{2} = sprintf('0');
    % Apply our tick marks to the plot.
    set(ax,'YTickLabel',y_tick_labels);
    %limit the x axis
    xlim([1 256]);
    %set the x axis tick marks
    set(ax, 'Xtick', 0:32:256);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    set(ax,'xgrid','on');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position (r)');
    ylabel('Experimental b_{r,256-r}');
    print -depsc SingleByte256_RValueGraph
else
    fprintf('Load single byte data values for probs_256_r\n');
end
%This script plots the experimental probabilities of the values Pr[Z_r =
%r] for 1 <= r <= 256. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('probs_r') == 1
    % produce the X values, these are the byte positions (1 - 256)
    X = 1:256;

    %plot the X values against the Experimental probabilities for the byte
    %positions 1-256 for rc4 output value r in row output r
    plot(X, probs_r(:, 1));
    % Now convert the existing tick marks.
    % Define the denominator we want to use.
    baseDenominator = 256;
    power = 2;
    denominator = baseDenominator^power;
    % For R2014a and earlier:
    ax = gca;
    New_Y_Tick = [probs_r(2,1) 1/baseDenominator probs_r(6 , 1)];
    set(ax,'YTick',New_Y_Tick)
    New_Y_Tick_Numerators = New_Y_Tick * denominator;
    
    % Now make up a cell array of all the tick labels.
    y_tick_labels{1} = sprintf('%.6f', New_Y_Tick_Numerators(1) - baseDenominator);
    y_tick_labels{2} = sprintf('0');
    y_tick_labels{3} = sprintf('%.6f', New_Y_Tick_Numerators(3) - baseDenominator);

    % Apply our tick marks to the plot.
    set(ax,'YTickLabel',y_tick_labels);
    %limit the x axis
    xlim([1 256]);
    %set the x axis tick marks
    set(ax, 'Xtick', 0:32:256);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position (r)');
    ylabel('Experimental b_{r,r}');
    print -depsc SingleByteRValueGraph
else
    fprintf('Load single byte data values for probs_r\n');
end
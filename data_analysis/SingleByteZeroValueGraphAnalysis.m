%This script plots the experimental probabilities of the values Pr[Z_r =
%0x00] for 3 <= r <= 255. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('experimentalProbs') == 1
    % produce the X values, these are the byte positions (1 - 256)
    X = 1:256;

    %plot the X values against the Experimental probabilities for the byte
    %positions 3-255 for rc4 output value 0
    plot(X(3:255), experimentalProbs(3:255, 1));
    % Now convert the existing tick marks.
    % Define the denominator we want to use.
    baseDenominator = 256;
    power = 2;
    denominator = baseDenominator^power;
    % For R2014a and earlier:
    ax = gca;
    New_Y_Tick_Numerators = [256 256.242811 256.351089 257.337057];
    set(ax,'YTick',New_Y_Tick_Numerators ./ denominator)

    % Now make up a cell array of all the tick labels.
    y_tick_labels{1} = sprintf('0');
    for k = 2 : length(New_Y_Tick_Numerators)
        y_tick_labels{k} = sprintf('%.6f', New_Y_Tick_Numerators(k) - baseDenominator);
    end

    % Apply our tick marks to the plot.
    set(ax,'YTickLabel',y_tick_labels);
    %limit the x axis
    xlim([1 256]);
    ylim([1/256 (1/256 + 1.4/256^2)]);
    %set the x axis tick marks
    set(ax, 'Xtick', 0:32:256);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position (r)');
    ylabel('Experimental c_{r, 0}');
    print -depsc SingleByteZeroValueGraph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
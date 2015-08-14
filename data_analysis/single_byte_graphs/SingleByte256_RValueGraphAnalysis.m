%This script plots the experimental probabilities of the values Pr[Z_r =
% 256 - r] for 1 <= r <= 256. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('biases_256_rmod256_page1') == 1
    % produce the X values, these are the byte positions (1 - 256)
    X = 1:256;

    %plot the X values against the Experimental probabilities for the byte
    %positions 1-256 for rc4 output value r in row output r
    plot(X, biases_256_rmod256_page1(:, 1));
    % Now convert the existing tick marks.

    % For R2014a and earlier:
    ax = gca;
    New_Y_Tick = [biases_256_rmod256_page1(1,1) 0  biases_256_rmod256_page1(128,1) biases_256_rmod256_page1(112,1) biases_256_rmod256_page1(96,1) biases_256_rmod256_page1(80,1) biases_256_rmod256_page1(64,1) biases_256_rmod256_page1(48,1) biases_256_rmod256_page1(32,1) biases_256_rmod256_page1(16,1) ];
    set(ax,'YTick',New_Y_Tick);
    
    % Now make up a cell array of all the tick labels.
    for k = 1:length(New_Y_Tick)
        y_tick_labels{k} = sprintf('%.3f ', New_Y_Tick(k));
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
    xlabel('RC4 Output Byte Position ($r$)', 'Interpreter', 'latex');
    ylabel('Experimental $b^r_{256-r}$', 'Interpreter', 'latex');
    print -depsc SingleByte256_RValueGraph
else
    fprintf('Load single byte data values for biases_256_rmod256_page1\n');
end
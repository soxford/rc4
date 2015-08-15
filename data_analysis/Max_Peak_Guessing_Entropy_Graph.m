
%check experimentalProbs exists
if exist('averageNoOfGuessesPage1') == 1
    %Get the indices of the points where the thresholds are crossed
    [~, I3a] = max((averageNoOfGuessesPage1 <= 3), [], 1);
    [~, I30a] = max((averageNoOfGuessesPage1 <= 30), [], 1);
    [~, I3b] = max((averageNoOfGuessesPage2 <= 3), [], 1);
    [~, I30b] = max((averageNoOfGuessesPage2 <= 30), [], 1);
    %Get the number of guesses required for each r and threshold
    Guesses3a = logSyntheticSampleSizesPage1(I3a); 
    Guesses30a = logSyntheticSampleSizesPage1(I30a);
    Guesses3b = logSyntheticSampleSizesPage2(I3b); 
    Guesses30b = logSyntheticSampleSizesPage2(I30b);
    
    % For R2014a and earlier:
    ax = gca;
    plot(1:512, [Guesses3a Guesses3b], 1:512, [Guesses30a Guesses30b])
    %limit the x axis
    xlim([-5 517]);
    ylim([8 50]);
    %set the x axis tick marks
    set(ax, 'Xtick', [0:32:512]);
    set(ax,'TickLabelInterpreter', 'tex');
    New_Y_Tick = get(ax,'YTick');
    % Now make up a cell array of all the tick labels.
    y_tick_labels = cell(length(New_Y_Tick),1);
    for r = 1:length(New_Y_Tick)
        y_tick_labels{r} = sprintf('2^{%d}', New_Y_Tick(r));
    end
    % Apply our tick marks to the plot.
    set(ax,'YTickLabel',y_tick_labels);
    
    %turn on the y grid lines
    set(ax,'ygrid','on');
    set(ax,'xgrid','on');
    legend({'Average Number of Guesses $\leq 3$', 'Average Number of Guesses $\leq 30$'},'Interpreter','latex', 'Location', 'east');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position ($r$)','Interpreter','latex');
    ylabel('Number of Cipher-Texts Required (log scale)','Interpreter','latex');
    print -depsc Max_Peak_Guessing_Entropy_Graph
    clearvars y_tick_labels r New_Y_Tick ax I3a I3b I30a I30b Guesses3a Guesses30a Guesses3b Guesses30b
else
    fprintf('Load single byte data values for \n');
end
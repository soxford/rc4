
%check experimentalProbs exists
if exist('maxPeakPercentCorrectOnFirstTryPage1') == 1
    %Get the indices of the points where the thresholds are crossed
    [~, I50a] = max((maxPeakPercentCorrectOnFirstTryPage1 >= 0.5), [], 1);
    [~, I99a] = max((maxPeakPercentCorrectOnFirstTryPage1 >= 0.99), [], 1);
    [~, I50b] = max((maxPeakPercentCorrectOnFirstTryPage2 >= 0.5), [], 1);
    [~, I99b] = max((maxPeakPercentCorrectOnFirstTryPage2 >= 0.90), [], 1);
    %Get the number of guesses required for each r and threshold
    Guesses50a = logSyntheticSampleSizesPage1(I50a); 
    Guesses99a = logSyntheticSampleSizesPage1(I99a);
    Guesses50b = logSyntheticSampleSizesPage2(I50b); 
    Guesses99b = logSyntheticSampleSizesPage2(I99b);
    
    % For R2014a and earlier:
    ax = gca;
    plot(1:512, [Guesses50a Guesses50b], 1:512, [Guesses99a Guesses99b])
    %limit the x axis
    xlim([-5 517]);
    %ylim([10 42]);
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
    legend({'Percentage Success $\geq 50\%$', 'Percentage Success $\geq 99\%$'},'Interpreter','latex', 'Location', 'southeast');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position ($r$)','Interpreter','latex');
    ylabel('Number of Cipher-Texts Required (log scale)','Interpreter','latex');
    print -depsc Max_Peak_Percent_First_Try_Graph
    clearvars y_tick_labels r New_Y_Tick ax I50a I50b I99a I99b Guesses50a Guesses99a Guesses50b Guesses99b
else
    fprintf('Load single byte data values for \n');
end
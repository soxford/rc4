%Plot the timings of the test code for different histogram sizes
load('ArcusTestData1.mat')

    %plot the X values against the Timing Values
    plot(X, Y, '--s');
    
    % For R2014a and earlier:
    ax = gca;
    set(ax,'TickLabelInterpreter', 'latex');
    %limit the x axis
    xlim([-1 8]);
    ylim([0 8]);
    %set the x axis tick marks
    set(ax, 'Xtick', X);
    set(ax, 'XTickLabel', 2 .^ X);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    set(ax,'xgrid','on');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('$x$, for histogram of dimensions $x \times 256 \times 256$  (log scale)', 'Interpreter', 'latex');
    ylabel('Test Time (s)', 'Interpreter', 'latex');
    print -depsc ArcusTestData1


%check experimentalProbs exists
if exist('experimentalBiases_i129k') == 1
    %Plot the values including the confidence intervals
    plot(0:255, circshift(experimentalBiases_i129k(:,130), 1, 1), 0:255, circshift(experimentalBiasesConfIntLoBound_i129k(:,130),1,1), '--r',0:255, circshift(experimentalBiasesConfIntHiBound_i129k(:,130),1,1), '--r')
    % For R2014a and earlier:
    ax = gca;
    set(ax,'TickLabelInterpreter', 'tex');

    %limit the x axis
    xlim([-5 260]);
    ylim([-1.2 1.2]);
    %set the x axis tick marks
    set(ax, 'Xtick', [0:32:256]);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    legend({'Experimental Estimate', '$99.99\%$ Confidence Interval'},'Interpreter','latex', 'Location','northeast');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('Value of $i$','Interpreter','latex');
    ylabel('Experimental Bias Value ($\overline{b}^{i}_{129,129}$)','Interpreter','latex');
    print -depsc biases_i129129_graph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
%This script plots the experimental probabilities of the values Pr[Z_507 =
%a] for 0 <= a <= 255. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('experimentalBiasesPage2') == 1
    %Plot the values including the confidence intervals
    plot(byteVals, experimentalBiasesPage2(250,:), byteVals, experimentalBiasesConfIntLoBoundPage2(250,:), '--r', byteVals, experimentalBiasesConfIntHiBoundPage2(250,:), '--r')
    % For R2014a and earlier:
    ax = gca;

    %limit the x axis
    xlim([-5 260]);
    ylim([-0.015 0.015]);
    %set the x axis tick marks
    set(ax, 'Xtick', [0:32:256]);
    set(ax,'TickLabelInterpreter', 'tex');
    %turn on the y grid lines
    set(ax,'ygrid','on');
    legend({'Experimental Estimate', '$99.99\%$ Confidence Interval'},'Interpreter','latex');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Value ($a$)','Interpreter','latex');
    ylabel('Experimental Bias Value ($b^{507}_a$)','Interpreter','latex');
    print -depsc Z_507_Distribution_Graph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
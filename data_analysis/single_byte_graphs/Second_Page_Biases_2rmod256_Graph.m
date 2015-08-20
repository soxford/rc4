%This script plots the experimental biases of the values Pr[Z_r =
%2r mod 256] for 257 <= r <= 512 
%These relate to the single byte bias relating to these positions and values 

%check experimentalProbs exists
if exist('experimentalBiasesPage2') == 1
    %Plot the values including the confidence intervals
    plot(257:512, biases_2rmod256_page2(1:256))
    % For R2014a and earlier:
    ax = gca;
    set(ax,'TickLabelInterpreter', 'tex');

    %limit the x axis
    xlim([250 517]);
    ylim([-0.04 0.02]);
    %set the x axis tick marks
    set(ax, 'Xtick', [256:32:512]);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position ($r$)','Interpreter','latex');
    ylabel('Experimental Bias Value','Interpreter','latex');
    print -depsc Second_Page_Biases_2rmod256_Graph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
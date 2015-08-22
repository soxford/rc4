%This script plots the experimental probabilities of the values Pr[Z_r =
%r mod 256] and Pr[Z_r = r+1 mod 256] for 0 <= r <= 255 and various r. 
%These relate to the single byte bias relating to these positions and values 

%check experimentalProbs exists
if exist('experimentalBiasesPage2') == 1
    %Plot the values including the confidence intervals
    plot(260:512, biases_rmod256_page2(4:256), 260:512, biases_rp1mod256_page2(4:256))
    % For R2014a and earlier:
    ax = gca;

    %limit the x axis
    xlim([250 517]);
    ylim([-0.04 0.012]);
    set(ax,'TickLabelInterpreter', 'latex');
    %set the x axis tick marks
    set(ax, 'Xtick', [256:32:512]);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    legend({'$b^r_{r \bmod 256}$', '$b^r_{r + 1 \bmod 256}$'}, 'Interpreter','latex', 'FontSize', 12);
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Position ($r$)','Interpreter','latex');
    ylabel('Experimental Bias Value','Interpreter','latex');
    print -depsc Second_Page_Biases_rmod256_Graph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
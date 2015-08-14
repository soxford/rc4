%This script plots the experimental probabilities of the values Pr[Z_r =
%a] for 0 <= a <= 255 and various r. These relate to the single byte bias relating 
%to these positions and values 

%check experimentalProbs exists
if exist('experimentalBiasesPage2') == 1
    %Plot the values including the confidence intervals
    plot(byteVals, experimentalBiasesPage2(4,:), byteVals, experimentalBiasesPage2(10,:), byteVals, experimentalBiasesPage2(20,:), byteVals, experimentalBiasesPage2(30,:), byteVals, experimentalBiasesPage2(40,:), byteVals, experimentalBiasesPage2(50,:), byteVals, experimentalBiasesPage2(60,:), byteVals, experimentalBiasesPage2(70,:), byteVals, experimentalBiasesPage2(80,:))
    % For R2014a and earlier:
    ax = gca;

    %limit the x axis
    xlim([-5 260]);
    ylim([-0.05 0.06]);
    %set the x axis tick marks
    set(ax, 'Xtick', [0:32:256]);
    %turn on the y grid lines
    set(ax,'ygrid','on');
    legend({'$r = 4 + 256$', '$r = 10 + 256$','$r = 20 + 256$','$r = 30 + 256$','$r = 40 + 256$','$r = 50 + 256$', '$r = 60 + 256$','$r = 70 + 256$', '$r = 80 + 256$'}, 'Interpreter','latex');
    %keep the box off
    set(ax, 'box', 'off');
    xlabel('RC4 Output Byte Value ($a$)','Interpreter','latex');
    ylabel('Experimental Bias Value ($b^{r}_a$)','Interpreter','latex');
    print -depsc Second_Page_Biases_Distribution_Graph
else
    fprintf('Load single byte data values for experimentalProbs\n');
end
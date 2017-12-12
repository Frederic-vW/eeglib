%!/usr/bin/octave
% test triolog function to read and parse Siemens Trio physio log files
% FvW 12/2017

fname0 = 'test.puls';
fname1 = 'test.resp';
fname = fname0;
[t, x] = triolog(fname);

figure('Position', [50 50 1800 300])
plot(t, x, '-k')
xlabel('time [sec]')
ylabel('voltage [uV]')
title('physio log')

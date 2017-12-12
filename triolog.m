%!/usr/bin/octave
% read and parse Siemens Trio physio log files
% FvW 12/2017

function [t, data] = triolog(fname)
%disp(['[+] File: ' fname])

%disp('[+] Reading physio log...')
i = 1;
fid = fopen(fname);
tline = fgetl(fid);
filedata{i} = tline;
while ischar(tline)
    tline = fgetl(fid);
    i = i+1;
    filedata{i} = tline;
end
fclose(fid);
n = length(filedata);

% --- find indices of keys ---
idx_logstart=0;
idx_logstop=0;
idx_samplerate=0;
for i=1:length(filedata)
    if findstr(filedata{i}, 'Physiolog_START')
    	idx_logstart = i;
    end
    if findstr(filedata{i}, 'Physiolog_STOP')
    	idx_logstop = i;
    end
    if findstr(filedata{i}, 'Sampling_Rate')
    	idx_samplerate = i;
    end
end

%  --- parse sampling rate ---
l = filedata{idx_samplerate};
fs = str2num(l(findstr(l, ':')+2:end));
%fprintf('[+] Sampling rate: %0.2f Hz\n', fs)

%disp('[+] Parsing data...')
l = filedata{idx_samplerate+1};

%disp('[+] Splitting data values...')
sp = isspace(l);
if any(sp)
    dw = diff(sp);
    starts = [1, find(dw == -1) + 1];
    stops = [find(dw == 1), length(l)];
    n_values = numel(starts);
    s = cell(1, n_values);
    for i=1:n_values
    	s{i} = l(starts(i):stops(i));
    end                
else
    s = {l};
end
% if you have at least Matlab2013b, you can substitute the
% last block of code, starting with 'sp = ', by:
% s = strsplit(l, ' ');

%disp('[+] Converting strings to numbers...')
data = [];
for i=1:length(s)
    x = str2num(s{i});
    if x
    	data = [data x];
    end
end

%disp('[+] Deleting scanner triggers...')
trigs = [5000, 5002, 6000, 6002];
for trig=trigs
    data(data==trig) = [];
end

% --- time axis ---
t = (0:length(data)-1)/fs;

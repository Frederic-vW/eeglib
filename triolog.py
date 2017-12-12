#!/usr/bin/python
# -*- coding: utf-8 -*-
# read and parse Siemens Trio physio log files
# FvW 12/2017

import numpy as np
import matplotlib.pyplot as plt

def findstr(s, L):
    """Find query string s in a list of strings, return indices.
    
    Args:
        s: query string
        L: list of strings to search
    Returns:
        x: list of indices i, such that s is a substring of L[i]
    """
    x = [i for i, l in enumerate(L) if (l.find(s) >= 0)]
    return x

def triolog(filename, doplot=False):
    """Read and parse Siemens Trio physio log files.
    
    Args:
        filename: full path to log file
        doplot: plot log data after reading (optional)
    Returns:
        t: time axis in sec. (numpy.array)
	data: log data (numpy.array)
    """
    
    # --- read in all data ---
    filedata = []
    with open(filename, "rb") as f:
	for l in f: filedata.append(l.strip("\n"))
    
    # --- find indices of keys ---
    idx_logstart = findstr("Physiolog_START", filedata)[0]
    idx_logstop = findstr("Physiolog_STOP", filedata)[0]
    idx_samplerate = findstr("Sampling_Rate", filedata)[0]
    
    # --- parse sampling rate ---
    l = filedata[idx_samplerate]
    fs = float(l[l.find(":")+2:])
    print("[+] Sampling rate: {:.2f} Hz".format(fs))
    
    # --- parse data ---
    l = filedata[idx_samplerate+1]
    s = l.strip("\n").split(" ")
    while ("" in s): s.remove("")
    data = np.array([float(_) for _ in s])
    del s
    
    # --- delete scanner trigger values
    trigs = [5000, 5002, 6000, 6002]
    for trig in trigs:
	data = np.delete(data, np.where(data==trig)[0])
    
    if doplot:
	t = np.arange(len(data))/fs  # time axis
	fig = plt.figure(1, figsize=(20, 4))
	fig.patch.set_facecolor("white")
	plt.plot(t, data, "-k", linewidth=1)
	plt.xlabel("time [sec]")
	plt.ylabel("voltage [uV]")
	plt.title(filename)
	plt.tight_layout()
	plt.show()
    
    return t, data

def main():
    fname0 = "test.puls"
    fname1 = "test.resp"
    fname = fname0
    print("[+] File: {:s}".format(fname))
    triolog(fname, doplot=True)

if __name__ == "__main__":
    main()

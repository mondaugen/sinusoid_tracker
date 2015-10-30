function [X]=load_raw_spectum(path)
% LOAD_RAW_SPECTRUM
% reads in spectra stored as interleaved doubles in order real, imag, etc..
f=fopen(path,'r');
Y=fread(f,Inf,'double');
fclose(f);
X=Y(1:2:end);
X+=1i*Y(2:2:end);

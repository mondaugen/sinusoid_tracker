function [S,T,F] = read_spectrogram(path,N,H,Fs)
m=1;
f=fopen(path,"r");
if (f == -1)
    error(["Opening file " path]);
end
while (1)
    [data,count]=fread(f,2*N,'double');
    if (count != 2*N)
        break
    end
    X_real=data(1:2:end);
    X_imag=data(2:2:end);
    X_real(:);
    X_imag(:);
    S(:,m)=X_real+1i*X_imag;
    m=m+1;
end
T=(0:(m-1))*H/Fs;
F=(0:(N-1))/N*Fs;
fclose(f);

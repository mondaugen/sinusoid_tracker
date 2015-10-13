function [X] = stft_analysis(x,N,H,window='hanning')
x=x(:).';
L = length(x);
H
h_last = floor((L-N)/H)*H;
h = (0:H:h_last)+1;
N_h = length(h);
X=zeros(N,N_h);
switch (window)
    case 'hanning'
        w=hanning(N+1);
    case 'hamming'
        w=hamming(N+1);
    case 'blackman'
        w=blackman(N+1);
    case 'bartlett'
        w=bartlett(N+1);
    case 'rectangular'
        w=ones(N+1,1);
end
w = w(1:N);
for n=(1:N_h),
    X(:,n) = fft((x(h(n):(h(n)+N-1)).').*w);
end;

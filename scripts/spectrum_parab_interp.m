fs=16000;
N=512;
OS=4;       % 4x oversampling
n_f=10.125;   % index of the frequency, starting at 0
f0=fs*n_f/N; % frequency in Hz
phi=0;      % initial phase
t=((1:N)-1)/fs;
x=exp(1i*2*pi*t*f0);
figure(1);
plot(t,real(x));
w=hanning(N+1)(1:N);
x=x(:);
xw=x.*w;
xw=[xw;zeros(N*(OS-1),1)];
X=fft(xw)/sum(w);
f=((1:(N*OS))-1)/N*fs;
figure(2);
% max with no interpolation
[X_max,n_max]=max(abs(X));
f_max=(n_max-1)/N*fs;
% parabolic interpolation
X_1=log(abs(X(n_max-1)));
X0=log(abs(X(n_max)));
X1=log(abs(X(n_max+1)));
a=0.5*(X1+X_1)-X0;
b=0.5*(X1-X_1);
c=X0;
n_max_=-b/(2*a);
X_max_=exp(a*n_max_*n_max_+b*n_max_+c);
% offset n_max (as it was assumed to be 0 for the polynomial interpolation)
n_max_+=n_max
f_max_=(n_max_-1)/N*fs;
% interpolated complex value
X_1_=X(n_max-1);
X0_=X(n_max);
X1_=X(n_max+1);
a_real=0.5*(real(X1_)+real(X_1_))-real(X0_);
b_real=0.5*(real(X1_)-real(X_1_));
c_real=real(X0_);
X_max_real=a_real*n_max_*n_max_+b_real*n_max_+c_real;
a_imag=0.5*(imag(X1_)+imag(X_1_))-imag(X0_);
b_imag=0.5*(imag(X1_)-imag(X_1_));
c_imag=imag(X0_);
X_max_imag=a_imag*n_max_*n_max_+b_imag*n_max_+c_imag;
X_max__=X_max_real+1i*X_max_imag;
plot(f,abs(X),f_max,X_max,f_max_,X_max_,f_max_,abs(X_max__));

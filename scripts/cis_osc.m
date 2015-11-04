function [a,t,phi] = cis_osc(t,phi,v_,Ts)
% CIS_OSC
% t     - current time
% phi   - current phase
% v_    - frequency function in angular velocity (outputs normalized
%         frequency between 0 and 1), accepts t as argument
% Ts    - the sample period
%
% Synthesizes one sample of a complex exponential. t is incremented by the
% sample period and phi is incremented by the output of the frequency
% function.
a=exp(1i*phi);
t+=Ts;
phi+=v_(t)*2*pi;

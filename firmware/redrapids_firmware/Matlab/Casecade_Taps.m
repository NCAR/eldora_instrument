clear;
%----------------------------------
%Sampling Information
%----------------------------------
sampling_freq = 48e6;
if sampling_freq > 60e6,
    clk_mult = 2;
    downsample = 8;
else
    clk_mult = 3;
    downsample = 12;
end
filter_clk = sampling_freq*clk_mult;
sampling_freq_div2 = sampling_freq/2;

fs = sampling_freq;                  % sampling rate
T=1/fs;                      % sampling period
N=1024;                      % N Points for FFT/IFFT
n=0:1:N-1;                   
f=-fs/2:fs/(N-1):fs/2;       % frequency range vector
%---------------------------------------
% Kaiser Window FIR 128 Tap
%---------------------------------------
wp = 4e6;
ws = 4.5e6;
error = 0.001;

[n,Wn,bta,filtype] = kaiserord( [wp ws], [1 0], [error error], fs);
h_k = fir1(n, Wn, filtype, kaiser(n+1,bta), 'noscale');

if downsample == 8, Coeff_limit = 63; end
if downsample == 12, Coeff_limit = 95; end

while length(h_k) ~= Coeff_limit,
    ws = ws + 0.01e6;
    [n,Wn,bta,filtype] = kaiserord( [wp ws], [1 0], [error error], fs);
    h_k = fir1(n, Wn, filtype, kaiser(n+1,bta), 'noscale');
end
h_k_dec=round(h_k*2^17);

for x = 1:length(h_k_dec)
    if h_k_dec(x) < 0 
        h_k_dec(x) = h_k_dec(x) + 2^18;
    end
end

save('4MHz.kcf','h_k_dec','-ASCII');

h_k = round(h_k*2^17)/2^17;
%h_k(1:length(h_k)) = 1;
figure(1);
subplot(3,1,1);
stem(h_k);
title('Impulse Response of Kaiser Window FIR');
xlabel('N Index');
ylabel('Magnitude');
subplot(3,1,2);
H_k = fftshift(fft(h_k,N));
plot(f, 10*log(abs(H_k)));
title('Frequency Response of Kaiser Window FIR');
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
subplot(3,1,3);
grpdelay(angle(h_k));
%---------------------------------------
% Gaussian FIR 72 Tap
%---------------------------------------
fs=sampling_freq/4;                  % sampling rate
f=-fs/2:fs/(N-1):fs/2;       % frequency range vector
T=1/fs;                      % sampling period

t_g = -35.5:1:35.5; % 72 Coefficents
CF = 2.0122^.5;          % Correction Factor for BW7

PW = 12/12e6;    % Pulse Width
BW = 1/(2*PW*CF);  % 3dB Bandwidth
R_BW = 1/(2*PW);    % Real Bandwidth
alpha = sqrt(log(2))/(sqrt(2)*BW*T);
h_g = (sqrt(pi)/alpha)*exp(-(pi^2/alpha^2).*(t_g.*t_g));
coef_sum = sum(h_g);
h_g = h_g/coef_sum; %Normalize to Gain = 1

h_g_dec = round(h_g*2^17);

for x = 1:length(h_g_dec)
    if h_g_dec(x) < 0 
        h_g_dec(x) = h_g_dec(x) + 2^18;
    end
end

%save('25.gcf','h_g_dec','-ASCII');
h_g = round(h_g*2^17)/2^17;

figure(2);
subplot(3,1,1);
stem(h_g);
title('Impulse Response of Gaussian FIR');
xlabel('N Index');
ylabel('Magnitude');
H_g = fftshift(fft(h_g,N));
subplot(3,1,2);
plot(f, 10*log(abs(H_g)));
Cutoff = 10*log(abs(H_g(round(N*(R_BW+fs/2)/fs))));
title('Frequency Response of Gaussian FIR');
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
subplot(3,1,3);
grpdelay(angle(h_g));
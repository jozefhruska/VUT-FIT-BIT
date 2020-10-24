EXP = 1;

% Solution 1
[s1_Data, s1_Rate] = audioread('xhrusk25.wav');
s1_LengthSamples = length(s1_Data);
s1_LengthSeconds = s1_LengthSamples/s1_Rate;
s1_RepresentedSymbols = s1_LengthSamples/16;
s1_Result = sprintf("Solution 1:\nSample rate: %d\nLength(samples): %d\nLength(seconds): %d\nRepresented symbols: %d\n", s1_Rate, s1_LengthSamples, s1_LengthSeconds, s1_RepresentedSymbols);
disp(s1_Result);

% Solution 2
s2_SourceTxt = textread('xhrusk25.txt');
s2_SelectedSamples = s1_Data(8:16:end);
s2_ConvertedBinary = zeros(2000, 1);

for i = 1:length(s2_SelectedSamples)
    if s2_SelectedSamples(i) > 0
        s2_ConvertedBinary(i) = 1;
    end
end

s2_Compare = xor(s2_SourceTxt(1:end), s2_SourceTxt(1:end));
if sum(s2_Compare) > 0
    fprintf("Solution 2:\nSource & converted signals are not equal!\n")
else
    fprintf("Solution 2:\nSource & converted signals are equal.\n");
end

plot(s1_Data(1:s1_Rate/100));
hold on
stem((1:10) * 16 - 8, s2_ConvertedBinary(1:10));
hold off
title('Solution 2');

if EXP == 1
    print('Solution-2', '-dpng');
end

% Solution 3
s3_B = [0.0192 -0.0185 -0.0185 0.0192];
s3_A = [1.0000 -2.8870 2.7997 -0.9113];
zplane(s3_B, s3_A);
title('Solution 3');

if EXP == 1
    print('Solution-3', '-dpng');
end

% Solution 4
s4_Result = abs(freqz(s3_B, s3_A, s1_Rate/2));
plot(linspace(0, s1_Rate/2, s1_Rate/2), s4_Result);
xlabel('f[Hz]');
ylabel('|H(f)|');
grid;
title('Solution 4');

if EXP == 1
    print('Solution-4', '-dpng');
end

% Solution 5
s5_Result = filter(B, A, s1_Data);

% Solution 6
s6_Shifted = circshift(s5_Result, -16);
s6_ShiftedSelected = s6_Shifted(8:16:end);
s6_ShiftedBinary = zeros(2000, 1);

for i = 1:length(s6_ShiftedSelected)
    if s6_ShiftedSelected(i) > 0
        s6_ShiftedBinary(i) = 1;
    end
end

plot(s1_Data(1:s1_Rate/100));
hold on
plot(s5_Result(1:s1_Rate/100));
plot(s6_Shifted(1:s1_Rate/100));
stem((1:10) * 16 - 8, s6_ShiftedBinary(1:10));
hold off
title('Solution 6');

if EXP == 1
    print('Solution-6', '-dpng');
end

% Solution 7
s7_WrongBits = 0;
for i = 1:length(s6_ShiftedBinary)
    if xor(s6_ShiftedBinary(i), s2_ConvertedBinary(i)) == 1
        s7_WrongBits = s7_WrongBits + 1;
    end
end

s7_Result = sprintf("\nSolution 7:\nWrong bits: %.4g%%", s7_WrongBits/length(s2_ConvertedBinary) * 100);
disp(s7_Result);

% Solution 8
s8_DFT_s = fft(s1_Data);
s8_DFT_s = abs(s8_DFT_s(1:s1_Rate/2));
plot(s8_DFT_s);
xlabel('f[Hz]');
title('Solution 8 - s[n]');

if EXP == 1
    print('Solution-8_s', '-dpng');
end

s8_DFT_ss = fft(s5_Result);
s8_DFT_ss = abs(s8_DFT_ss(1:s1_Rate/2));
plot(s8_DFT_ss);
xlabel('f[Hz]');
title('Solution 8 - ss[n]');

if EXP == 1
    print('Solution-8_ss', '-dpng');
end

% Solution 9
[s9_Heights, s9_Locations] = hist(s1_Data);
s9_Heights = s9_Heights / (length(s1_Data) * (s9_Locations(2) - s9_Locations(1)));
bar(s9_Locations, s9_Heights * 0.15, 'hist');
title('Solution 9 - Histogram');

if EXP == 1
    print('Solution-9__histogram', '-dpng');
end

plot(s9_Locations, hist(s1_Data)/s1_LengthSamples);
title('Solution 9 - Function');

if EXP == 1
    print('Solution-9__function', '-dpng');
end

close;

s9_Integral = sum(s9_Heights) * 0.15;
s9_Result = sprintf("\nSolution 9:\nIntegral: %.4g", s9_Integral);
disp(s9_Result);

% Solution 10
s10_Correlation = linspace(-50, 50, 101);
s10_Result = xcorr(s1_Data, 50, 'biased') / s1_Rate;
figure;
plot(s10_Correlation, s10_Result(s10_Correlation + 51));
title('Solution 10');

if EXP == 1
    print('Solution-10', '-dpng');
end

close;

% Solution 11
s11_Result = xcorr(s1_Data, 50, 'biased');

s11_Output = sprintf("\nSolution 11:\nR[1]: %f\nR[2]: %f\nR[16]: %f", s11_Result(51), s11_Result(52), s11_Result(67));
disp(s11_Output);

% Solution 12
s12_Histogram = linspace(min(s1_Data), max(s1_Data), 50);
s12_Matrix = zeros(50, 50);
[s12_Values, s12_Indexes] = min(abs(repmat(s1_Data(:)', 50, 1) - repmat(s12_Histogram(:), 1, length(s1_Data))));

for i = 1:length(s1_Data) - 1
    s12_Matrix(s12_Indexes(i), s12_Indexes(i + 1)) = s12_Matrix(s12_Indexes(i), s12_Indexes(i + 1)) + 1;
end

s12_Surface = (s12_Histogram(2) - s12_Histogram(1)) ^ 2;
s12_Corners = s12_Matrix / length(s1_Data) / s12_Surface;
figure;

imagesc(s12_Histogram, s12_Histogram, s12_Corners);
colorbar;
axis xy;

if EXP == 1
    print('Solution-12', '-dpng');
end

close;

% Solution 13
s13_Result = sum(sum(s12_Corners)) * s12_Surface;

s13_Output = sprintf("\nSolution 13:\n%.4g", s13_Result);
disp(s13_Output);

% Solution 14
s14_Result = sum(sum(repmat(s12_Histogram(:), 1, L) .* repmat(s12_Histogram(:)', 50, 1) .* s12_Corners)) * s12_Surface;

s14_Output = sprintf("\nSolution 14:\n%.4g - %.4g", s14_Result, s11_Result(51));
disp(s14_Output);
close all
clear, clc

figure(1);
F1 = load('SYNCHR_M_N_sync.dat');
M_N_1 = F1(:, 1);
lyamda = F1(:, 2);
F2 = load('SYNCHR_M_N_theor_all.dat');
M_N_2 = F2(:, 1);
F4 = load('ALOHA_M_N.dat');
M_N_4 = F4(:, 1);
plot(lyamda, M_N_1);
hold on;
plot(lyamda, M_N_2);
hold on;
plot(lyamda, M_N_4);
legend('practical M[N] sync','theoretical M[N]', 'ALOHA M[N]')
xlabel('lamda')
ylabel('M[N]')

figure(2);
F1 = load('SYNCHR_M_D_synchr.dat');
M_D_1 = F1(:, 1);
F2 = load('SYNCHR_M_D_theor_synchr.dat');
M_D_2 = F2(:, 1);
F5 = load('ALOHA_M_D.dat');
M_D_5 = F5(:, 1);
lyamda = F5(:, 2);
plot(lyamda, M_D_1);
hold on;
plot(lyamda, M_D_2);
hold on;
plot(lyamda, M_D_5);
legend('practical sync M[D]','theoretical sync M[D]', 'ALOHA M[D]')
xlabel('lamda')
ylabel('M[D]')

figure(3);
F1 = load('ALOHA_lambda_output.dat');
lambda_output = F1(:, 1);
lyamda = F1(:, 2);
plot(lyamda, lambda_output);
legend('ALOHA lamda output')
xlabel('lambda')
ylabel('lambda')





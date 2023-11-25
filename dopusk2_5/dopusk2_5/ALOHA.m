close all
clear, clc

figure(1);
F1 = load('ALOHA_M_N_BACK.dat');
M_N_1 = F1(:, 1);
lyamda = F1(:, 2);
F2 = load('ALOHA_M_N_1_BACK.dat');
M_N_2 = F2(:, 1);
plot(lyamda, M_N_1);
hold on;
plot(lyamda, M_N_2);
hold on;
legend('ALOHA M[N]','ALOHA ONE M[N]')
xlabel('lamda')
ylabel('M[N]')

figure(2);
F1 = load('ALOHA_M_D_BACK.dat');
M_D_1 = F1(:, 1);
F2 = load('ALOHA_M_D_1_BACK.dat');
M_D_2 = F2(:, 1);
lyamda = F1(:, 2);
plot(lyamda, M_D_1);
hold on;
plot(lyamda, M_D_2);
hold on;
legend('ALOHA M[D]','ALOHA ONE M[D]','practical async M[D]')
xlabel('lamda')
ylabel('M[D]')

figure(3);
F1 = load('ALOHA_lambda_output.dat');
lambda_output = F1(:, 1);
F2 = load('ALOHA_lambda_output_1.dat');
lambda_output1 = F2(:, 1);
lyamda = F1(:, 2);
plot(lyamda, lambda_output);
hold on;
plot(lyamda, lambda_output1);
hold on;

legend('ALOHA lamda output', 'ALOHA ONE lambda output')
xlabel('lambda')
ylabel('lambda')
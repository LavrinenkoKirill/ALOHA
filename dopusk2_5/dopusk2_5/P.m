close all
clear, clc

figure(1);
F1 = load('ALOHA_M_N.dat');
M_N_1 = F1(:, 1);
p = F1(:, 2);
F2 = load('ALOHA_M_N_1.dat');
M_N_2 = F2(:, 1);
plot(p, M_N_1);
hold on;
plot(p, M_N_2);
hold on;
legend('ALOHA M[N]','ALOHA ONE M[N]')
xlabel('p')
ylabel('M[N]')


figure(2);
F1 = load('ALOHA_M_D.dat');
M_D_1 = F1(:, 1);
F2 = load('ALOHA_M_D_1.dat');
M_D_2 = F2(:, 1);
p = F1(:, 2);
plot(p, M_D_1);
hold on;
plot(p, M_D_2);
hold on;
legend('ALOHA M[D]','ALOHA ONE M[D]')
xlabel('p')
ylabel('M[D]')
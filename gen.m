N = 2048;

A = randi([0, 10], N, N);
B = randi([0, 10], N, N);
C = A * B;

nameA = sprintf('A%d.csv', N);
nameB = sprintf('B%d.csv', N);
nameC = sprintf('C%d.csv', N);

writematrix(A, nameA, 'Delimiter', 'space');
writematrix(B, nameB, 'Delimiter', 'space');
writematrix(C, nameC, 'Delimiter', 'space');

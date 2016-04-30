clear;
close all;
clc;

filename = '../../log20.txt';
%cells = read_log_file('../../180');
cells = read_log_file(filename);
K = length(cells);

phase1 = zeros(1, K);
phase2 = zeros(1, K);
phase3 = zeros(1, K);
relative_phase12 = zeros(1, K);
relative_phase23 = zeros(1, K);
relative_phase31 = zeros(1, K);

id = 28;
count = 1;
for i = 1 :  length(cells)
	csi = cells{i}.csi;
	
	%Nrx = cells{i}.nr;
	%Ntx = cells{i}.nc;
	[Nrx, Ntx, N] = size(csi)
	if Nrx ~= 3
		continue;
		Nrx
		Ntx
	end
	[Nrx, Ntx, N] = size(csi)
	csi1 = reshape(csi(1, 1, :), 1, N);
	csi2 = reshape(csi(2, 1, :), 1, N);
	csi3 = reshape(csi(3, 1, :), 1, N);
	
	phase1(count) = angle(csi1(id));
	phase2(count) = angle(csi2(id));
	phase3(count) = angle(csi3(id));
	
	relative_phase12(count) = angle(csi2(id)*conj(csi1(id)));
	relative_phase23(count) = angle(csi3(id)*conj(csi2(id)) );
	relative_phase31(count) = angle(csi1(id)*conj(csi3(id)));
	count = count + 1;
end

figure;
hold on;
plot(phase1, 'r');
plot(phase2, 'g');
plot(phase3, 'b');
hold off
title('phase')
legend('phase1', 'phase2', 'phase3')

figure
hold on
plot(relative_phase12, 'r.')
plot(relative_phase23, 'g.');
plot(relative_phase31, 'b.');
hold off;
title('relative phase')
legend('relative phase12', 'relative phase23', 'relative phase31');
outFilename = sprintf('%s_1.png', filename)
print('-dpng', outFilename);
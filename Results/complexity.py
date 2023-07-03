import matplotlib.pyplot as plt
import numpy as np


def compare_complexity(times1, times2):

    times1_np = np.array(times1)
    times2_np = np.array(times2)


    k, b = np.polyfit(times1_np, times2_np, 1)

    return k

file1_lengths = []
file1_times = []

with open("sha3_256_results_1S.txt", "r") as file1:
    for line in file1:
        parts = line.split()
        length = float(parts[1])
        time = float(parts[3])
        file1_lengths.append(length)
        file1_times.append(time)


file2_lengths = []
file2_times = []

with open("sha3_256_results_3S.txt", "r") as file2:
    for line in file2:
        parts = line.split()
        length = float(parts[1])
        time = float(parts[3])
        file2_lengths.append(length)
        file2_times.append(time)



file3_lengths = []
file3_times = []

with open("sha3_256_results_5S.txt", "r") as file3:
    for line in file3:
        parts = line.split()
        length = float(parts[1])
        time = float(parts[3])
        file3_lengths.append(length)
        file3_times.append(time)


file4_lengths = []
file4_times = []

with open("sha3_256_results_7S.txt", "r") as file4:
    for line in file4:
        parts = line.split()
        length = float(parts[1])
        time = float(parts[3])
        file4_lengths.append(length)
        file4_times.append(time)

variation = compare_complexity(file1_times, file2_times)
variation2 = compare_complexity(file1_times, file3_times)
variation3 = compare_complexity(file1_times, file4_times)
variation23 = compare_complexity(file2_times, file3_times)
variation34 = compare_complexity(file3_times, file4_times)

plt.semilogy(file1_lengths, file1_times, color='blue', linewidth=3,  label='sha3 256')



plt.semilogy(file2_lengths, file2_times, color='red', linewidth=3, label='sha3 256 3 Shares', )
plt.semilogy(file3_lengths, file3_times, color='green', linewidth=3,  label='sha3 256 5 Shares')
plt.semilogy(file4_lengths, file4_times, color='orange', linewidth=3, label='sha3 256 7 Shares')

plt.xlabel('Message Length', fontsize=14)
plt.ylabel('Elapsed Time (log scale)', fontsize=14)
plt.title('Semi-log plot of Elapsed Time vs. Length', fontsize=16)
plt.legend(fontsize=14)

i=len(file1_lengths)-1
plt.plot([file1_lengths[len(file1_lengths)-1], file2_lengths[len(file1_lengths)-1]], [file1_times[len(file1_lengths)-1], file2_times[len(file1_lengths)-1]], 'k--',color='blue')
plt.text(file1_lengths[len(file1_lengths)-1], np.sqrt(file1_times[len(file1_lengths)-1]*file2_times[len(file1_lengths)-1]), f" {variation:.2f}", ha='left',color='blue', fontsize=14)

plt.plot([file2_lengths[len(file1_lengths)-1], file3_lengths[i]], [file2_times[i], file3_times[i]], 'k--',color='red')
plt.text(file2_lengths[i], np.sqrt(file2_times[i]*file3_times[i]), f" {variation23:.2f}", ha='left',color='red', fontsize=14)

plt.plot([file3_lengths[i], file4_lengths[i]], [file3_times[i], file4_times[i]], 'k--',color='green')
plt.text(file3_lengths[i], np.sqrt(file3_times[i]*file4_times[i]), f" {variation34:.2f}", ha='left',color='green', fontsize=14)

plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

print("variation between 3 and 1:",variation)
print("variation between 5 and 1:",variation2)
print("variation between 7 and 1:",variation3)
plt.show()

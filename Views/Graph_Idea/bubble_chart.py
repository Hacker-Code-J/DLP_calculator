import matplotlib.pyplot as plt
import numpy as np

with open('speed.txt', 'r') as file:
    data = file.readlines()

speed_function_1 = [float(data[i].strip()) for i in range(0, len(data), 2)]
speed_function_2 = [float(data[i].strip()) for i in range(1, len(data), 2)]

plt.figure(figsize=(12, 6))

# Normalize data for bubble sizes
size_norm_1 = np.array(speed_function_1) / max(speed_function_1) * 1000
size_norm_2 = np.array(speed_function_2) / max(speed_function_2) * 1000

plt.scatter(range(len(speed_function_1)), speed_function_1, s=size_norm_1, color='cyan', alpha=0.5, label='Function 1')
plt.scatter(range(len(speed_function_2)), speed_function_2, s=size_norm_2, color='magenta', alpha=0.5, label='Function 2')

plt.title('Bubble Chart with Time Evolution')
plt.xlabel('Measurement Index')
plt.ylabel('Speed')
plt.legend()
plt.show()
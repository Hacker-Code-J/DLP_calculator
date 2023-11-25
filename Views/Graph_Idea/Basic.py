import matplotlib.pyplot as plt
import numpy as np

# Read the simulated data from the file
with open('lib_test.txt', 'r') as f:
    lines = f.readlines()

# Split the data into two lists, one for each function
speeds = [float(speed.strip()) for speed in lines]
speed_f1 = speeds[:10000]
speed_f2 = speeds[10000:]

index = np.arange(1, len(speed_f1) + 1)


# Increasing the figure size
plt.figure(figsize=(12, 6))

# Reducing the marker size and adding transparency
marker_size = 2
alpha_value = 0.5

# Plotting both functions' speeds with adjustments
plt.plot(index, speed_f1, label='Function 1', marker='o', markersize=marker_size, alpha=alpha_value, linestyle='-', linewidth=1)
plt.plot(index, speed_f2, label='Function 2', marker='s', markersize=marker_size, alpha=alpha_value, linestyle='-', linewidth=1)

# Adding details to the plot
plt.xlabel('Measurement Number')
plt.ylabel('Speed (seconds)')
plt.title('Speed Comparison between Function 1 and Function 2')
plt.legend()
plt.grid(True)
plt.yscale('log')  # Keeping the logarithmic scale for better visibility

# Show the adjusted plot
plt.show()
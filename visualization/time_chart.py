import matplotlib.pyplot as plt
import numpy as np

# Read data from txt file
with open('times.txt', 'r') as f:
    execution_times = [float(line.strip()) for line in f]

# x values are just the index of each data point
x_values = list(range(len(execution_times)))

# Create a color list using a colormap
colors = plt.get_cmap('viridis')(np.linspace(0, 1, len(execution_times)))

fig, ax = plt.subplots()

# Plot each segment with its own color
for i in range(1, len(execution_times)):
    ax.plot(x_values[i-1:i+1], execution_times[i-1:i+1], color=colors[i], linewidth=2.5)

# Set title and labels
ax.set_title("Measurement of Function Execution Time")
ax.set_xlabel("Measurement Index")
ax.set_ylabel("Execution Time (seconds)")

plt.show()
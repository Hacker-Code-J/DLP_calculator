import matplotlib.pyplot as plt
import numpy as np

with open('speed.txt', 'r') as file:
    data = file.readlines()

function1_speeds = [float(data[i].strip()) for i in range(0, len(data), 2)]
function2_speeds = [float(data[i].strip()) for i in range(1, len(data), 2)]

# Rolling mean for smoothing
window_size = 1
function1_smooth = np.convolve(function1_speeds, np.ones(window_size)/window_size, mode='valid')
function2_smooth = np.convolve(function2_speeds, np.ones(window_size)/window_size, mode='valid')

# Set up the figure and axes for a prettier graph
fig, ax = plt.subplots(figsize=(14, 7), dpi=100)

# Plot the smoothed data with a prettier design
ax.plot(function1_smooth, color='dodgerblue', label='Binary Long DIV', alpha=0.8, linewidth=2)
ax.plot(function2_smooth, color='crimson', label='General Long DIV', alpha=0.8, linewidth=2)

# Setting labels, title, and legend with a prettier font and style
ax.set_ylabel('Speed (ms)', fontsize=14, fontweight='bold')
ax.set_xlabel('Run Number', fontsize=14, fontweight='bold')
ax.set_title('Comparison of Speeds', fontsize=16, fontweight='bold')
ax.legend(frameon=True, framealpha=0.9, shadow=True, fancybox=True)

# Customizing the grid to be less prominent and prettier
plt.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout()

# Enhancing the overall look by setting a face and edge color
fig.patch.set_facecolor('white')
fig.patch.set_edgecolor('lightgrey')

# Show the plot with a prettier design
plt.show()
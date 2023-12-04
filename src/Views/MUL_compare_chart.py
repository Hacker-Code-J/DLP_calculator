import matplotlib.pyplot as plt
import numpy as np

with open('speed.txt', 'r') as file:
    data = file.readlines()

function1_speeds = [float(data[i].strip()) for i in range(0, len(data), 3)]
function2_speeds = [float(data[i].strip()) for i in range(1, len(data), 3)]
function3_speeds = [float(data[i].strip()) for i in range(2, len(data), 3)]

# Calculate the averages
average_function1 = np.mean(function1_speeds)
average_function2 = np.mean(function2_speeds)
average_function3 = np.mean(function3_speeds)

# Rolling mean for smoothing
window_size = 50
function1_smooth = np.convolve(function1_speeds, np.ones(window_size)/window_size, mode='valid')
function2_smooth = np.convolve(function2_speeds, np.ones(window_size)/window_size, mode='valid')
function3_smooth = np.convolve(function3_speeds, np.ones(window_size)/window_size, mode='valid')

# Set up the figure and axes for a prettier graph
fig, ax = plt.subplots(figsize=(14, 7), dpi=100)

# Plot the smoothed data with a prettier design
ax.plot(function1_smooth, color='dodgerblue', label='TextBook', alpha=0.8, linewidth=2)
ax.plot(function2_smooth, color='crimson', label='Improved TextBook', alpha=0.8, linewidth=2)
ax.plot(function3_smooth, color='forestgreen', label='Karatsuba', alpha=0.8, linewidth=2)

# Add horizontal lines to indicate the average values
ax.hlines(average_function1, xmin=0, xmax=len(function1_smooth), colors='dodgerblue', linestyles='dotted', label=f'Avg TextBook: {average_function1:.2f} ms')
ax.hlines(average_function2, xmin=0, xmax=len(function2_smooth), colors='crimson', linestyles='dotted', label=f'Avg ImpTextBook: {average_function2:.2f} ms')
ax.hlines(average_function3, xmin=0, xmax=len(function3_smooth), colors='forestgreen', linestyles='dotted', label=f'Avg Karatsuba: {average_function3:.2f} ms')

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
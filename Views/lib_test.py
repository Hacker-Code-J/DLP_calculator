import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd

# Read the simulated data from the file
with open('lib_test.txt', 'r') as f:
    lines = f.readlines()

# Split the data into two lists, one for each function
speeds = [float(speed.strip()) for speed in lines]
speed_f1 = speeds[:10000]
speed_f2 = speeds[10000:]

# Calculate the averages
average_function1 = np.mean(speed_f1)
average_function2 = np.mean(speed_f2)

# Rolling mean for smoothing
window_size = 50
function1_smooth = np.convolve(speed_f1, np.ones(window_size)/window_size, mode='valid')
function2_smooth = np.convolve(speed_f2, np.ones(window_size)/window_size, mode='valid')

# Set up the figure and axes for a prettier graph
fig, ax = plt.subplots(figsize=(14, 7), dpi=100)

# Plot the smoothed data with a prettier design
ax.plot(function1_smooth, color='dodgerblue', label='FLINT', alpha=0.8, linewidth=2)
ax.plot(function2_smooth, color='crimson', label='PUBAO (Karatsuba)', alpha=0.8, linewidth=2)

# Add horizontal lines to indicate the average values
ax.hlines(average_function1, xmin=0, xmax=len(function1_smooth), colors='dodgerblue', linestyles='dotted', label=f'Avg FLINT: {average_function1:.2f} µs')
ax.hlines(average_function2, xmin=0, xmax=len(function2_smooth), colors='crimson', linestyles='dotted', label=f'Avg PUBAO: {average_function2:.2f} µs')

# Setting labels, title, and legend with a prettier font and style
ax.set_ylabel('Speed (µs)', fontsize=14, fontweight='bold')
ax.set_xlabel('Run Number', fontsize=14, fontweight='bold')
ax.set_title('Comparison of Speeds (Multiplication)', fontsize=16, fontweight='bold')
ax.legend(frameon=True, framealpha=0.9, shadow=True, fancybox=True)

# Customizing the grid to be less prominent and prettier
plt.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout()

# Enhancing the overall look by setting a face and edge color
fig.patch.set_facecolor('white')
fig.patch.set_edgecolor('lightgrey')

# Show the plot with a prettier design
plt.show()
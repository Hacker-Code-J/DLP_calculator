# pip install plotly
import matplotlib.pyplot as plt
import numpy as np

with open('speed.txt', 'r') as file:
    data = file.readlines()

function1_speeds = [float(data[i].strip()) for i in range(0, len(data), 3)]
function2_speeds = [float(data[i].strip()) for i in range(1, len(data), 3)]
function3_speeds = [float(data[i].strip()) for i in range(2, len(data), 3)]

# function1_speeds = [data[i] for i in range(0, len(data), 2)]
# function2_speeds = [data[i] for i in range(1, len(data), 2)]

# Rolling mean for smoothing
window_size = 50
function1_smooth = np.convolve(function1_speeds, np.ones(window_size)/window_size, mode='valid')
function2_smooth = np.convolve(function2_speeds, np.ones(window_size)/window_size, mode='valid')
function3_smooth = np.convolve(function3_speeds, np.ones(window_size)/window_size, mode='valid')

# Set up the figure and axes
fig, ax = plt.subplots(figsize=(10,6))

# Plot the smoothed data
ax.plot(function1_smooth, color='blue', label='Function 1', alpha=0.9)
ax.plot(function2_smooth, color='red', label='Function 2', alpha=0.9)
ax.plot(function3_smooth, color='green', label='Function 3', alpha=0.9)

# Optionally plot raw data with reduced alpha for background
# ax.plot(function1_speeds, color='blue', alpha=0.1)
# ax.plot(function2_speeds, color='red', alpha=0.1)

# Setting labels, title, and legend
ax.set_ylabel('Execution Speed')
ax.set_xlabel('Run Number')
ax.set_title('Comparison of Execution Speeds')
ax.legend()

plt.grid(True)
plt.tight_layout()
plt.show()
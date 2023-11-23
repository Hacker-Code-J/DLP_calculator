import matplotlib.pyplot as plt
import numpy as np

# Function to read speeds from a file
def read_speeds(file_name):
    with open(file_name, 'r') as file:
        data = file.readlines()
    return [float(line.strip()) for line in data]

# Read speeds from each file
function1_speeds = read_speeds('speed1.txt')
function2_speeds = read_speeds('speed2.txt')
function3_speeds = read_speeds('speed3.txt')

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

# Setting labels, title, and legend
ax.set_ylabel('Execution Speed')
ax.set_xlabel('Run Number')
ax.set_title('Comparison of Execution Speeds')
ax.legend()

plt.grid(True)
plt.tight_layout()
plt.show()

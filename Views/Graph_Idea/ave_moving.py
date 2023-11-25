import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm

# Read the simulated data from the file
with open('lib_test.txt', 'r') as f:
    lines = f.readlines()

# Split the data into two lists, one for each function
speeds = [float(speed.strip()) for speed in lines]
speed_f1 = speeds[:10000]
speed_f2 = speeds[10000:]

index = np.arange(1, len(speed_f1) + 1)


# Define the moving average function
def moving_average(data, window_size):
    return np.convolve(data, np.ones(window_size) / window_size, mode='valid')

# Calculate moving averages for both sets of speeds
window_size = 200  # Define the window size for the moving average
speed_f1_avg = moving_average(speed_f1, window_size)
speed_f2_avg = moving_average(speed_f2, window_size)

# Calculate the indices for moving averages
index_avg = np.arange(window_size, len(speed_f1) + 1)

# Create a figure with subplots
fig, ax = plt.subplots(2, 1, figsize=(8, 15))

# Scatter plot for raw data
ax[0].scatter(index, speed_f1, label='Function 1', alpha=0.5, s=10)
ax[0].scatter(index, speed_f2, label='Function 2', alpha=0.5, s=10)
ax[0].set_title('Scatter Plot of Speeds')
ax[0].set_xlabel('Measurement Number')
ax[0].set_ylabel('Speed (seconds)')
ax[0].legend()
ax[0].grid(True)

# Line plot for moving averages
ax[1].plot(index_avg, speed_f1_avg, label='Function 1 MA', linewidth=2)
ax[1].plot(index_avg, speed_f2_avg, label='Function 2 MA', linewidth=2)
ax[1].set_title('Moving Average of Speeds')
ax[1].set_xlabel('Measurement Number (Averaged)')
ax[1].set_ylabel('Speed (seconds)')
ax[1].legend()
ax[1].grid(True)

# Show the plots
plt.tight_layout()
plt.show()
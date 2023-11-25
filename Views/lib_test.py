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

plt.figure(figsize=(12, 6))

# Calculate the difference between the two functions' speeds
difference = np.array(speed_f1) - np.array(speed_f2)

# Plot the original speed data
plt.plot(speed_f1, label='Function 1', alpha=0.6)
plt.plot(speed_f2, label='Function 2', alpha=0.6)

# Add the difference curve
plt.plot(difference, label='Difference (F1 - F2)', color='black', linestyle='--')

plt.title('Performance Speed and Difference Comparison')
plt.xlabel('Measurements')
plt.ylabel('Speed')
plt.legend()
plt.grid(True)
plt.show()
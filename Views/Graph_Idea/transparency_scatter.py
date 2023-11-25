import matplotlib.pyplot as plt
import numpy as np

# Read the simulated data from the file
with open('lib_test.txt', 'r') as f:
    lines = f.readlines()

# Split the data into two lists, one for each function
speeds = [float(speed.strip()) for speed in lines]
speed_function_1 = speeds[:10000]
speed_function_2 = speeds[10000:]

# For a novel visualization, let's use a boxplot, which will give us a good indication of the distribution of the speed data,
# including the median, quartiles, and outliers for both functions.

# To visualize data with large differences, another novel approach is to use a scatter plot with transparency (alpha value).
# Each point represents an individual measurement of speed for the functions, and the transparency helps in visualizing the density.

plt.figure(figsize=(12, 6))

# Generate a scatter plot of all the data points for both functions
plt.scatter(range(len(speed_function_1)), speed_function_1, alpha=0.1, label='Function 1')
plt.scatter(range(len(speed_function_2)), speed_function_2, alpha=0.1, label='Function 2')

plt.title('Scatter Plot of Performance Speeds with Transparency')
plt.xlabel('Measurement Index')
plt.ylabel('Speed')
plt.legend()
plt.grid(True)
plt.show()
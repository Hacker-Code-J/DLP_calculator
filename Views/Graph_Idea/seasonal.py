import matplotlib.pyplot as plt
import numpy as np
from statsmodels.tsa.seasonal import seasonal_decompose

# Read the simulated data from the file
with open('lib_test.txt', 'r') as f:
    lines = f.readlines()

# Split the data into two lists, one for each function
speeds = [float(speed.strip()) for speed in lines]
speed_f1 = speeds[:10000]
speed_f2 = speeds[10000:]

# Perform time series decomposition
# Assuming the period is 365 for daily data within a year for example
result_f1 = seasonal_decompose(speed_f1, period=365, model='additive')
result_f2 = seasonal_decompose(speed_f2, period=365, model='additive')

# Create subplots
fig, axes = plt.subplots(nrows=4, ncols=2, figsize=(14, 8), sharex=True)

# Plot the observed data
axes[0, 0].plot(result_f1.observed)
axes[0, 0].set_title('Function 1 Observed')
axes[0, 1].plot(result_f2.observed)
axes[0, 1].set_title('Function 2 Observed')

# Plot the trend data
axes[1, 0].plot(result_f1.trend)
axes[1, 0].set_title('Function 1 Trend')
axes[1, 1].plot(result_f2.trend)
axes[1, 1].set_title('Function 2 Trend')

# Plot the seasonal data
axes[2, 0].plot(result_f1.seasonal)
axes[2, 0].set_title('Function 1 Seasonality')
axes[2, 1].plot(result_f2.seasonal)
axes[2, 1].set_title('Function 2 Seasonality')

# Plot the residuals
axes[3, 0].plot(result_f1.resid)
axes[3, 0].set_title('Function 1 Residuals')
axes[3, 1].plot(result_f2.resid)
axes[3, 1].set_title('Function 2 Residuals')

# Fine-tune the layout and display the plots
plt.tight_layout()
plt.show()
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

# Create interactive dual range slider plot
fig = make_subplots(rows=1, cols=1)
fig.add_trace(go.Scatter(x=time, y=speed_f1, mode='lines', name='Function 1'))
fig.add_trace(go.Scatter(x=time, y=speed_f2, mode='lines', name='Function 2'))

# Update layout for a range slider
fig.update_layout(
    title='Interactive Speed Comparison with Range Slider',
    xaxis=dict(
        rangeslider=dict(visible=True),
        type='linear'
    )
)

# Show the figure
fig.show()
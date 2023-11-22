import sys
print(sys.executable)

import matplotlib.pyplot as plt
import numpy as np

# 1. Read the file
with open('test.txt', 'r') as f:
    lines = f.readlines()

# 2. Calculate the success rate
total_lines = len(lines)
success_lines = sum(1 for line in lines if line.strip() == 'True')
failure_lines = total_lines - success_lines
success_rate = (success_lines / total_lines) * 100

# 3. Plot the success rate
labels = ['Success', 'Failure']
values = [success_rate, 100 - success_rate]

# Use a visually pleasing style and adjust figure size
plt.figure(figsize=(10, 7))
plt.style.use('ggplot')

# Using a softer color palette and setting the bar width
colors = ['#88CCEE', '#FF6F61']
bar_width = 0.6

bars = plt.bar(labels, values, color=colors, width=bar_width, edgecolor='black')

# Annotate the bars with their respective percentage values and actual counts
for bar in bars:
    yval = bar.get_height()
    count = success_lines if bar.get_x() < 0.5 else failure_lines  # Assuming the success bar is the first one
    plt.text(bar.get_x() + bar.get_width()/2, yval + 1,
             f'{yval:.0f}% ({count}/{total_lines})',  # Adjusted formatting to remove decimal places and add counts
             ha='center', va='bottom', fontsize=12)

plt.ylabel('Percentage (%)')
plt.title('Success Rate')
plt.ylim(0, 110)  # Added a little more room at the top for the annotations

# Display the graph
plt.show()
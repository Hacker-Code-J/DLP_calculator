import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Arc

# First, let's modify the 'read_success_rate_from_file' function to return the styled label
def read_success_rate_from_file(file_path):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
        # Count 'True' as success
        success_count = sum(1 for line in lines if line.strip().lower() == 'true')
        total_count = len(lines)
        success_rate = (success_count / total_count) * 100 if total_count > 0 else 0
        return success_rate, f'{success_rate:.0f}% ({success_count}/{total_count})'
    except FileNotFoundError:
        print(f"No such file: {file_path}")
        return 0, '0/0'

# Adjust the 'draw_gauge_chart' function to move the success rate label lower on the chart
def draw_gauge_chart(success_rate, title='Success Rate', label='', ax=None):
    if ax is None:
        fig, ax = plt.subplots(figsize=(8, 4))

    # Create the arc
    theta1, theta2 = 0, 180  # Arc from 0 to 180 degrees
    radius = 0.9
    arc = Arc([0.5, 0.5], radius, radius, theta1=theta1, theta2=theta2,
              edgecolor='DeepSkyBlue', lw=2, zorder=3)

    # Add the arc to the axes
    ax.add_patch(arc)

    # Create the needle
    max_val = 100
    min_val = 0
    range_val = max_val - min_val
    value = (success_rate / range_val) * 180
    # Create the needle with a higher zorder to bring it to the front
    ax.arrow(0.5, 0.5, 0.4 * np.cos(np.radians(180 - value)), 0.4 * np.sin(np.radians(180 - value)),
            width=0.02, head_width=0.05, head_length=0.1, fc='red', ec='red', zorder=5)


    # Set the limits of the axes
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)

    # Remove the axes
    ax.set_xticks([])
    ax.set_yticks([])

    # Add labels - moving the success rate label lower
    ax.text(0.5, 0.45, label, horizontalalignment='center', verticalalignment='center',
            fontsize=20, fontweight='bold', color='magenta')  # Adjusted vertical position to 0.45
    ax.text(0.5, 0.3, title, horizontalalignment='center', verticalalignment='center',
            fontsize=24, fontweight='bold')

    # Add scale
    for i in range(min_val, max_val + 1, 10):
        angle = (i / range_val) * 180
        x = 0.5 + 0.45 * np.cos(np.radians(180 - angle))
        y = 0.5 + 0.45 * np.sin(np.radians(180 - angle))
        ax.text(x, y, str(i), horizontalalignment='center', verticalalignment='center',
                fontsize=12, fontweight='bold', color='DeepSkyBlue')

    return ax

# The file path should point to the actual file location
file_path = 'test.txt'  # This is an example path

# Read the success rate and label from the file
success_rate, label = read_success_rate_from_file(file_path)

# Draw the gauge chart with the actual label positioned lower
draw_gauge_chart(success_rate, label=label)
plt.show()
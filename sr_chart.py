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

# Custom Visualization
colors = ['#88CCEE', '#FF6F61']
explode = (0.1, 0)  # Explode the success slice a bit for emphasis

fig, ax = plt.subplots(figsize=(8, 8))
wedges, texts, autotexts = ax.pie(values, labels=labels, autopct='%1.2f%%', startangle=90, pctdistance=0.85,
                                  explode=explode, colors=colors, wedgeprops=dict(width=0.4, edgecolor='w'))

# Draw a center circle for 'donut' style and add gradient
centre_circle = plt.Circle((0, 0), 0.60, fc='white')
fig.gca().add_artist(centre_circle)

# Gradient enhancement: Making it more unique by adding gradients to wedges
for w in wedges:
    w.set_edgecolor('w')
    w.set_linewidth(1)
    w.set_facecolor(plt.get_cmap('spring')(np.random.rand()))

# Modify attributes for percentage text inside the donut
for t in autotexts:
    t.set_color('black')
    t.set_fontsize(14)
    t.set_weight('bold')

ax.set_title('Success Rate', fontsize=20, fontweight='bold', color='#333333')
plt.tight_layout()

# Decorative Labels
bbox_props = dict(boxstyle="round,pad=0.3", fc="cyan", ec="b", lw=2, alpha=0.7)

# Modify attributes for label texts outside the donut
for t in texts:
    t.set_color('black')
    t.set_fontsize(15)
    t.set_weight('bold')
    t.set_bbox(bbox_props)

# Display
plt.show()

# colors = ['#88CCEE', '#FF6F61']
# plt.figure(figsize=(8, 8))
# plt.pie(values, labels=labels, colors=colors, autopct='%1.2f%%', startangle=140, wedgeprops=dict(width=0.3))
# centre_circle = plt.Circle((0,0),0.70,fc='white')
# fig = plt.gcf()
# fig.gca().add_artist(centre_circle)
# plt.title('Success Rate')
# plt.tight_layout()
# plt.show()

# # Use a visually pleasing style and adjust figure size
# plt.figure(figsize=(10, 7))
# plt.style.use('ggplot')

# # Using a softer color palette and setting the bar width
# colors = ['#88CCEE', '#FF6F61']
# bar_width = 0.6

# bars = plt.bar(labels, values, color=colors, width=bar_width, edgecolor='black')

# # Annotate the bars with their respective percentage values
# for bar in bars:
#     yval = bar.get_height()
#     plt.text(bar.get_x() + bar.get_width()/2, yval + 1, f'{yval:.2f}%', ha='center', va='bottom', fontsize=12)

# plt.ylabel('Percentage (%)')
# plt.title('Success Rate')
# plt.ylim(0, 110)  # Added a little more room at the top for the annotations

# # Display the graph
# plt.show()
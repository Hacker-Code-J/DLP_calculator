# pip install pandas matplotlib seaborn

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Assume each file has a similar structure, with a column for 'speed_difference'
# and possibly a second column. We need to add a 'FLAG' column to differentiate the data
flag1 = pd.read_csv('flag1.txt', sep='\t', header=None, names=['speed_difference'])
flag1['FLAG'] = 'FLAG1'

flag2 = pd.read_csv('flag2.txt', sep='\t', header=None, names=['speed_difference'])
flag2['FLAG'] = 'FLAG2'

flag3 = pd.read_csv('flag3.txt', sep='\t', header=None, names=['speed_difference'])
flag3['FLAG'] = 'FLAG3'

# Combine the DataFrames
combined_data = pd.concat([flag1, flag2, flag3])

# Now we create a cool and unique plot
# We can use a violin plot as an example, which shows the distribution of the speed differences
plt.figure(figsize=(10, 6))

# Create a color palette
palette = sns.color_palette("cool", 3)

# Use Seaborn to create the plot
sns.violinplot(x='FLAG', y='speed_difference', data=combined_data, palette=palette)

# Customize with unique aesthetics
plt.title('Unique Speed Difference Distribution by FLAG', fontsize=20)
plt.xlabel('FLAG Setting', fontsize=16)
plt.ylabel('Speed Difference', fontsize=16)

# Add some unique touches
for spine in plt.gca().spines.values():
    spine.set_visible(False)

# Create an annotation that will make the plot unique
plt.text(2, max(combined_data['speed_difference']), 'Custom Data Art', fontsize=12, ha='right', color='firebrick')

# Save the plot as an image file with a high resolution
# plt.savefig('unique_speed_difference_plot.png', dpi=300)

# Show the plot
plt.show()


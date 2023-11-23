# pip install pandas matplotlib seaborn

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Assume each file has a similar structure, with a column for 'speed_difference'
# and possibly a second column. We need to add a 'FLAG' column to differentiate the data
flag1 = pd.read_csv('flag1.txt', sep='\t', header=None, names=['speed_difference'])
flag1['FLAG'] = 'FLAG = 2 (64-bit)'

flag2 = pd.read_csv('flag2.txt', sep='\t', header=None, names=['speed_difference'])
flag2['FLAG'] = 'FLAG = 4 (128-bit)'

flag3 = pd.read_csv('flag3.txt', sep='\t', header=None, names=['speed_difference'])
flag3['FLAG'] = 'FLAG = 8 (256-bit)'

flag4 = pd.read_csv('flag4.txt', sep='\t', header=None, names=['speed_difference'])
flag4['FLAG'] = 'FLAG = 16 (512-bit)'

flag5 = pd.read_csv('flag5.txt', sep='\t', header=None, names=['speed_difference'])
flag5['FLAG'] = 'FLAG = 32 (1024-bit)'

# Combine the DataFrames
combined_data = pd.concat([flag1, flag2, flag3, flag4, flag5])

# Now we create a cool and unique plot
# We can use a violin plot as an example, which shows the distribution of the speed differences
plt.figure(figsize=(15, 6))

# Create a color palette
palette = sns.color_palette("cool", 7)

# Use Seaborn to create the plot
sns.violinplot(x='FLAG', y='speed_difference', data=combined_data, palette=palette)

# Customize with unique aesthetics
plt.title('Speed Difference Distribution by FLAG', fontsize=16)
# plt.xlabel('FLAG Setting', fontsize=12)
plt.ylabel('Speed Difference', fontsize=12)

# Add some unique touches
for spine in plt.gca().spines.values():
    spine.set_visible(False)

# Show the plot
plt.show()


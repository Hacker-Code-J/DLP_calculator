import pandas as pd
import matplotlib.pyplot as plt

file_paths = [
    # '3072/flag1.txt',
    '3072/flag2.txt',
    '3072/flag3.txt',
    '3072/flag4.txt',
    '3072/flag5.txt'
]
# file_paths = [
#     # '7680/flag1.txt',
#     '7680/flag2.txt',
#     '7680/flag3.txt',
#     '7680/flag4.txt',
#     '7680/flag5.txt'
# ]

# Flag labels corresponding to each file
flag_labels = [
    # 'FLAG = 2 (64-bit)',
    'FLAG = 4 (128-bit)',
    'FLAG = 8 (256-bit)',
    'FLAG = 16 (512-bit)',
    'FLAG = 32 (1024-bit)'
]

# Create a DataFrame to hold all flag data
data = pd.DataFrame()

# Read each flag file and store its content under the corresponding flag label in the DataFrame
for file_path, label in zip(file_paths, flag_labels):
    df = pd.read_csv(file_path, header=None)
    data[label] = df.iloc[:, 0]

plt.figure(figsize=(15, 15))

# Create a subplot for each flag file and plot both the readings and their average values
for i, (label, series) in enumerate(data.items(), 1):
    ax = plt.subplot(len(data.columns), 1, i)
    ax.plot(series, label='Readings')
    average_value = series.mean()
    ax.hlines(average_value, xmin=0, xmax=len(series), colors='red', linestyles='dashed', label=f'Average: {average_value:.2f}')
    ax.text(len(series) * 0.9, average_value, f'{average_value:.2f}', va='center', ha='center', backgroundcolor='white')
    ax.set_title(label)
    ax.set_xlabel('Sample Index')
    ax.set_ylabel('Reading Value')
    ax.legend()
    ax.grid(True)

plt.tight_layout()
plt.show()
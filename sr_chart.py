import matplotlib.pyplot as plt

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

plt.bar(labels, values, color=['green', 'red'])
plt.ylabel('Percentage (%)')
plt.title('Success Rate')
plt.ylim(0, 100)  # Make sure y-axis goes from 0% to 100%
plt.show()
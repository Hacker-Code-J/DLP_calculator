# First, let's read the contents of the uploaded 'speed.txt' file and then calculate the averages as requested.

def read_numbers_from_file(file_path):
    with open(file_path, 'r') as file:
        # Convert each line to a float and strip newline characters
        numbers = [float(line.strip()) for line in file if line.strip()]
    return numbers

def calculate_average(values):
    # Calculate the average of the given values
    return sum(values) / len(values)

# The path to the 'speed.txt' file
file_path = '/Visualizations/speed.txt'

# Read numbers from the file
numbers = read_numbers_from_file(file_path)

# Assuming the numbers are in sets for each method, and each set is in order
# Divide the numbers into three lists, one for each method
method1_speeds = numbers[0::3]
method2_speeds = numbers[1::3]
method3_speeds = numbers[2::3]

# Calculate the average for each method
method1_avg = calculate_average(method1_speeds)
method2_avg = calculate_average(method2_speeds)
method3_avg = calculate_average(method3_speeds)

# Print the averages for each method
method1_avg, method2_avg, method3_avg
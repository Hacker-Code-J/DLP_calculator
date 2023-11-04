#!/usr/bin/env python3
# Filename: converter.py

def convert_number(input_value, base_from, base_to):
    # Convert the input to an integer from the given base
    num = int(input_value, base_from)

    # Convert the integer to the target base
    if base_to == 10:
        return str(num)
    elif base_to == 16:
        return '0x' + hex(num)[2:].upper()  # Keep the '0x' prefix for hex
    elif base_to == 2:
        return '0b' + bin(num)[2:]  # Keep the '0b' prefix for bin

def main():
    print("Number Converter")
    print("----------------")
    print("1. Dec -> Hec")
    print("2. Dec -> Bin")
    print("----------------")
    print("3. Hex -> Dec")
    print("4. Hex -> Bin")
    print("----------------")
    print("5. Bin -> Dec")
    print("6. Bin -> Hex")
    print("----------------")
    print()
    choice = input("Choose the conversion (1-6): ")
    value = input("Enter the value to convert: ").strip()

    base_from, base_to = 10, 10
    if choice == '1':
        base_from, base_to = 10, 16
    elif choice == '2':
        base_from, base_to = 10, 2
    elif choice == '3':
        base_from, base_to = 16, 10
    elif choice == '4':
        base_from, base_to = 16, 2
    elif choice == '5':
        base_from, base_to = 2, 10
    elif choice == '6':
        base_from, base_to = 2, 16
    else:
        print("Invalid choice.")
        return

    try:
        converted_value = convert_number(value, base_from, base_to)
        print(f"Converted value: {converted_value}")
    except ValueError:
        print("Invalid input number.")

if __name__ == "__main__":
    main()

# def decimal_to_hex(decimal_number):
#     if not isinstance(decimal_number, int):
#         raise TypeError("Input must be an integer.")
    
#     # Check if the number is negative
#     if decimal_number < 0:
#         raise ValueError("Input must be a non-negative integer.")
    
#     # Convert to hexadecimal
#     hexadecimal_number = hex(decimal_number)

#     # Remove the '0x' prefix and return the hexadecimal string
#     return hexadecimal_number[2:]

# # Example usage:
# try:
#     decimal_input1 = 124810 # dividend
#     decimal_input2 = 123 # divisor
#     decimal_input3 = 1014 # quotient
#     decimal_input4 = 88 # remainder
#     hex_output1 = decimal_to_hex(decimal_input1)
#     hex_output2 = decimal_to_hex(decimal_input2)
#     hex_output3 = decimal_to_hex(decimal_input3)
#     hex_output4 = decimal_to_hex(decimal_input4)
#     print(f"0d{decimal_input1} = 0x{hex_output1.lower()}")
#     print(f"0d{decimal_input2} = 0x{hex_output2.lower()}")
#     print(f"0d{decimal_input3} = 0x{hex_output3.lower()}")
#     print(f"0d{decimal_input4} = 0x{hex_output4.lower()}")
# except TypeError as te:
#     print(te)
# except ValueError as ve:
#     print(ve)
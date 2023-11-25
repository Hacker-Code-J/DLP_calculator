def DIVCC(A, B, w):
    # Convert A and B to binary strings
    A_bin = bin(A)[2:]  # Remove the '0b' prefix
    B_bin = bin(B)[2:]  # Remove the '0b' prefix
    
    # Determine the length of B to find B_(m-1)
    m = len(B_bin)
    
    # Extracting A_m and B_m_1 as the most significant bits of A and B
    A_m = int(A_bin[:-w], 2) if len(A_bin) > w else 0
    B_m_1 = int(B_bin[:-1], 2) if len(B_bin) > 1 else 0
    
    W = 2**w  # Calculate W as 2^w
    
    if A == B:
        Q = 1
        R = 0
    else:
        if A_m == B_m_1 and len(A_bin) == len(B_bin) + 1:
            Q = W - 1
        else:
            # Since A can be less than B*W, we need to add an additional condition to handle this case
            if A_m < B_m_1:
                A_m = int(A_bin[:-w-1], 2) if len(A_bin) > w + 1 else 0  # Get the higher bits if A_m < B_m_1
            Q = (A_m * W + int(A_bin[-w-1:-1], 2)) // B_m_1 if B_m_1 != 0 else 0
    
    # Convert Q back to integer from binary
    Q = int(bin(Q)[2:], 2)
    
    # Calculate R
    R = A - B * Q
    
    # Adjust Q and R if R is negative
    while R < 0:
        Q -= 1
        R += B
    
    return Q, R

# Example usage with dummy values and w = 3 (hence, W = 2^3 = 8)
A = 0x234a222a  # Example A value
B = 0x234248   # Example B value
w = 3       # The power of 2 for W

# Call the DIVCC function
Q, R = DIVCC(A, B, W)

print(Q,R)
print(hex(Q), hex(R))
print(A == B * Q + R)
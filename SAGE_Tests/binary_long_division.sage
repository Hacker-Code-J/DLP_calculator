def div_binlong(a, b):
    print(f"a= 0x{a:x}, b = 0x{b:x}")
    q, r = 0, 0
    n = a.nbits()
    while n >= 0:
        print(f"n = {n}, q = {q:x}, r = {r:x}")
        r = ( r << 1) + ((a >> n) & 1)
        t = r >= b
        q = q + (1 << n ) * t
        r = r - b*t
        n = n - 1
    print(f"n = {n}, q = {q:0x}, r = {r:x}")
    return q, r

a = 0xae035de48a0ca938
b = 0x10a85e3a

q, r = div_binlong(a,b)
print(f"a = 0x{a:x}, b = 0x{b:x}")
print(f"q = 0x{q:x}, r = 0x{r:x}")
print(f"Quotient: {q == a // b}, Remainder: {r == a % b}")
# print(f"{a:b} = a = b * q + r = {b:b} * {q:b} + {r:b} = {b*q+r:b}")
print(f"0x{a:x} = a = b * q + r = 0x{b:x} * 0x{q:x} + 0x{r:x} = 0x{b*q+r:x}")
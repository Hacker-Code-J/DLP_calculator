def div_binlong(a, b):
    print(f"a= 0b{a:b}, b = 0b{b:b}")
    q, r = 0, 0
    n = a.nbits()
    while n >= 0:
        print(f"n = {n}, q = {q:0b}, r = {r:b}")
        r = ( r << 1) + ((a >> n) & 1)
        t = r >= b
        q = q + (1 << n ) * t
        r = r - b*t
        n = n - 1
    print(f"n = {n}, q = {q:0b}, r = {r:b}")
    return q, r

a = 0b10110111
b = 0b1001

q, r = div_binlong(a,b)
print(f"a = 0b{a:b}, b = 0b{b:b}")
print(f"q = 0b{q:b}, r = 0b{r:b}")
print(f"Quotient: {q == a // b}, Remainder: {r == a % b}")
print(f"{a:b} = a = b * q + r = {b:b} * {q:b} + {r:b} = {b*q+r:b}")
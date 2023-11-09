reset()

w = 32

def get_sign(a):
    if a < 0:
        return 1
    return 0

def Mul_K (a, b):
    print(f"*** Mul_K called with a = {a:#x}, b = {b:#x}***")
    n = floor(len(a.bits())/w) + 1
    m = floor(len(b.bits())/w) + 1
    print(f"n = {n:#x}, m = {m:#x}")

    if 4 >= min(n, m):
        result = a*b
        print(f"FLAG on! a = {a:#x}, b = {b:#x}")
        print(f"Direct multiplication result = {result:#x}")
        return result

    l = max(n, m)+1 >> 1
    print(f"l = {l:#x}")
    lw = l * w
    a1, a0 = a >> lw, a%(2^lw)
    b1, b0 = b >> lw, b%(2^lw)
    print(f"Split a into a1 = {a1:#x}, a0 = {a0:#x}")
    print(f"Split b into b1 = {b1:#x}, b0 = {b0:#x}")

    t1, t0 = Mul_K(a1, b1), Mul_K(a0, b0)
    print(f"t1 = {t1:#x}, t0 = {t0:#x}")
    r = (t1 << 2*lw) + t0
    print(f"R = {r:#x}")
    s1, s0 = a0 - a1, b1 - b0
    print(f"s1 = {s1:#x}, s0 = {s0:#x}")
    s = (-1)^(get_sign(s1) + get_sign(s0)) \
            * Mul_K(abs(s1), abs(s0))
    print(f"S = {s:#x}")
    s = s + t1
    print(f"s + t1 = {s:#x}")
    s = s + t0
    print(f"s + t0 = {s:#x}")
    s = s << lw
    print(f"s << lw = {s:#x}")
    r = r + s
    print(f"r + s = {s:#x}")
    print(f"Interim result r = {r:#x}")
    return r

a = 0x34e69cc99a6e8a0c8ecf24219d6de0c9d6ed9d79dc17fefbf6386fbc5352dc6b4c2e3e8d
b = 0x7f9d2c066846699facfb159f6d467b4ca90e6ce0387f43bfa32bc0a735883879
print (hex(a))
print (hex(b))

c1 = Mul_K(a,b)
c2 = a*b
print (hex(c1))
print ( c1 == c2 )
if c1 != c2 :
    print ( hex(c1))
    print ( hex(c2))
    print ( hex(c1 ^^ c2))

# cnt = 0
# while cnt < 100:
#     print(cnt)
#     k = 1024
#     a = ZZ.random_element(2^k)
#     b = ZZ.random_element(2^k)
#     print (hex(a))
#     print (hex(b))
#     
#     c1 = Mul_K(a,b)
#     c2 = a*b
#     print ( c1 == c2 )
#     if c1 != c2 :
#         print ( hex(c1))
#         print ( hex(c2))
#         print ( hex(c1 ^^ c2))
#         break
#     cnt = cnt + 1
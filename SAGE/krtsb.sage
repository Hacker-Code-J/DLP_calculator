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

    l = (max(n, m) + 1) >> 1
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

a = 0x909440cbc3ff0daf2b328f80c9a99a8d924766b6eea3e28f0d839cef3bac21c5d141be14d4db1faa69e6e63c175326db59c3d45f965ff31e47efce76dbdf06bc6de3f5297239c42dcf93153bfa6307f92d756c78170350d114ee701f7223c74471bc65456931179499b2340035918db55e692846ef6161f57c4d572eb10bdf2c52332dd598655f987da3738033248d1779c6e5fae5149d4e9b4c4f85b9abff7d 
b = 0x7e2d7f3602a84bba5107d3c130b79587713d9688f013dae5dd0c243396a224482ad7eb485744e790d71c2ba94558f0e94d83ae0b36cec0aa75820e4e784d5de280a760c76dcb642ed6d3b85c0b6d3b414ce04b8c583e9b270ac1f195cdb682b93a95623ff591757ac84b20c9215bf6c0870cf13134da3574f33b13147ced56fba0d1c97ac82a5aff2ac5d361d4eacf56d816fcc0e7fa61dfbf26be814d853125099f20c900952cc9f1e9927e12b85a182f025f083a3a213ed9394f277c92053f9e8b9d37a4433773d7d924fa8958b927c724e7fc61e8efb2bdbd223cf479a267e75a00316c5d4fdd50cc4161
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
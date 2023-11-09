reset()

w = 8

def get_sign(a):
    if a < 0:
        return 1
    return 0

def Mul_K (a, b):
    n = floor(len(a.bits())/w) + 1
    m = floor(len(b.bits())/w) + 1

    if 10 >= min(n, m):
        return a*b

    l = (max(n, m)+1) >> 1
    lw = l * w
    a1, a0 = a >> lw, a%(2^lw)
    b1, b0 = b >> lw, b%(2^lw)
    t1, t0 = Mul_K(a1, b1), Mul_K(a0, b0)
    r = (t1 << 2*lw) + t0
    s1, s0 = a0 - a1, b1 - b0
    s = (-1)^(get_sign(s1) + get_sign(s0)) \
            * Mul_K(abs(s1), abs(s0))
    s = s + t1
    s = s + t0
    s = s << lw
    r = r + s
    return r

a = 0x0000000084ebc20c29920bff48460f7589841c72abf8052b5c15f2c4ee8e00972718df78
b = 0xaf23caacea1fa9499bbe868f94f48395ac3d8637b3021b4157233c2bc727390338ab03a1
print (hex(a))
print (hex(b))
c = 0x5aefbf1da7286df5ca90d496058c7bdc581a75f7617a9371e7f96c1d842a75ff2edde5fed78ac6b73f280202b0c91b581d01955708aa35ff248d02c37929096c666af278
print (hex(c))
# hex(0x0000000084ebc20c29920bff48460f7589841c72abf8052b5c15f2c4ee8e00972718df78)
# hex(0xaf23caacea1fa9499bbe868f94f48395ac3d8637b3021b4157233c2bc727390338ab03a1)
# hex(0x5aefbf1da7286df5ca90d496058c7bdc581a75f7617a9371e7f96c1d842a75ff2edde5fed78ac6b73f280202b0c91b581d01955708aa35ff248d02c37929096c666af278)


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
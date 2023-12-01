reset()

def EEA_iter(a,b):
    t0, t1 = a, b
    u0, v0 = 1, 0
    u1, v1 = 0, 1
    print ("{:4x} = ({:4x}) * {:4x} + ({:4x}) * {:4x}".format(t0, u0, a, v0, b))
    while t1 != 0:
        q, r = t0 // t1, t0 % t1
        t0, t1 = t1, r
        u2, v2 = u0 - q*u1, v0 - q*v1
        u0, v0 = u1, v1
        u1, v1 = u2, v2
        print ("{:4x} = ({:4x}) * {:4x} + ({:4x}) * {:4x}".format(t0, u0, a, v0, b))
    return t0, u0, v0

def EEA_rec(a,b):
    if b == 0:
        return a, 1, 0
    d, alpha, beta = EEA_rec(b, a % b)
    T = d, beta, alpha - (a // b) * beta
    print ("{:4x} = ({:4x}) * {:4x} + ({:4x}) * {:4x}".format(d, T[1], a, T[2], b))
    return T

a = ZZ.random_element(1000)
b = ZZ.random_element(1000)

print ("a =", hex(a))
print ("b =", hex(b))

print()
print("EEA_iter:")
result = EEA_iter(a,b)
print (result == xgcd(a,b))
d, x, y = result
print (d == a * x + b * y)

print()
print("EEA_rec:")
result = EEA_rec(a,b)
print (result == xgcd(a,b))
d, x, y = result
print (d == a * x + b * y)
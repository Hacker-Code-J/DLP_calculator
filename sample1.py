print(hex(0x44b7014fff077a1eb414a9dc3694ee5d * 0x8e91703149a154b4b2a6faf2ce5b3f93) == hex(0x26448c7f69235610793b06fae820cd11c1353e0bbebf012656bc1f8b8f3cc267))
print(hex(0x44b7014fff077a1eb414a9dc3694ee5d * 0x8e91703149a154b4b2a6faf2ce5b3f93) == hex(0x26448c7f69235610793b06fae820cd11c1353e0bbebf012656bc1f8b8f3cc267))
# 44b7014f ff077a1e b414a9dc 3694ee5d 
# 8e917031 49a154b4 b2a6faf2 ce5b3f93
# l = (MIN(4,4) + 1) >> 1 = 4 >> 1 = 2
# Real: 0x 00026448 c7f69235 610793b06 fa8f2127 51794c793 1bebf012 656bc1f8 b8f3cc267
# Mine: 0x 00026448 c7f69235 610793b06 fae820cd 11c1353e0 bbebf012 656bc1f8 b8f3cc267
print()
# Patition I -------------------------------------------------------------------------------
# 44b7014f ff077a1e
# 8e917031 49a154b4
# l = (MIN(2,2) + 1) >> 1 = 3 >> 1 = 1
print("Partition 1------------------------------------------------------------------------")
print(hex(0x44b7014f * 0x8e917031))     # X1Y1 = 0x26448c7ec758d01f
print(hex(0xff077a1e * 0x49a154b4))     # X0Y0 = 0x4959d9e82031b518

# X1Y1 * W^{2*(l=1)} = 0x26448c7ec758d01f0000000000000000
print(hex(0x26448c7ec758d01f << 64))

# X1Y1 * W^2 + X0Y0 = X1Y1 || X0Y0 = 0x26448c7ec758d01f4959d9e82031b518
print(hex(0x26448c7ec758d01f0000000000000000 | 0x4959d9e82031b518))

print(hex(0xff077a1e - 0x44b7014f))     # S1 = X0 - X1 = 0xba5078cf
print(hex(0x8e917031- 0x49a154b4))      # S0 = Y1 - Y0 = 0x44f01b7d
print(hex(0xba5078cf * 0x44f01b7d))     # (X0 - X1) *(Y1 - Y0) = 0x322c1f89b418d213

# (X0-X1)*(Y1-Y0) + X1Y1 = 0x5870ac087b71a232
print(hex(0x322c1f89b418d213 + 0x26448c7ec758d01f))

# (X0-X1)*(Y1-Y0) + X1Y1 + X0Y0 = 0xa1ca85f09ba3574a
print(hex(0x5870ac087b71a232 + 0x4959d9e82031b518))

# [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0] * W^{1=1} = 0xa1ca85f09ba3574a00000000
print(hex(0xa1ca85f09ba3574a << 32))

# (X1Y1)W^2 + X0Y0 + [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0]W^{1=1} = 0x26448c7f6923560fe4fd31322031b518
print(hex(0x26448c7ec758d01f4959d9e82031b518 + 0xa1ca85f09ba3574a00000000))
# EndPatition I ----------------------------------------------------------------------------
print()
# Patition II -------------------------------------------------------------------------------
# b414a9dc 3694ee5d
# b2a6faf2 ce5b3f93
# l = (MAX(2,2) + 1) >> 1 = 3 >> 1 = 1
print("Partition 2------------------------------------------------------------------------")
print(hex(0xb414a9dc * 0xb2a6faf2))     # X1Y1 = 0x7dabd407861169f8
print(hex(0x3694ee5d * 0xce5b3f93))     # X0Y0 = 0x2bff4c4d8f3cc267

# X1Y1 * W^{2*(l=1)} = 0x7dabd407861169f80000000000000000
print(hex(0x7dabd407861169f8 << 64))

# X1Y1 * W^2 + X0Y0 = X1Y1 || X0Y0 = 0x7dabd407861169f82bff4c4d8f3cc267
print(hex(0x7dabd407861169f80000000000000000 | 0x2bff4c4d8f3cc267))

print(hex(0x3694ee5d - 0xb414a9dc))     # S1 = X0 - X1 = -0x7d7fbb7f
print(hex(0xb2a6faf2 - 0xce5b3f93))     # S0 = Y1 - Y0 = -0x1bb444a1
print(hex(-0x7d7fbb7f * -0x1bb444a1))   # (X0 - X1) *(Y1 - Y0) = 0xd94d83b156ea6df

# (X0-X1)*(Y1-Y0) + X1Y1 = 0x8b40ac429b8010d7
print(hex(0xd94d83b156ea6df + 0x7dabd407861169f8))

# (X0-X1)*(Y1-Y0) + X1Y1 + X0Y0 = 0xb73ff8902abcd33e
print(hex(0x8b40ac429b8010d7 + 0x2bff4c4d8f3cc267))

# [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0] * W^{1=1} = 0xb73ff8902abcd33e00000000
print(hex(0xb73ff8902abcd33e << 32))

# (X1Y1)W^2 + X0Y0 + [(X0-X1)*(Y1-Y0) + X1Y1 + X0Y0]W^{1=1} = 0x7dabd4083d51628856bc1f8b8f3cc267
print(hex(0x7dabd407861169f82bff4c4d8f3cc267 + 0xb73ff8902abcd33e00000000))
# EndPatition II ----------------------------------------------------------------------------
print()
print("Complete: T1, T0 <- MUL(X1,Y1), MUL(X0,Y0)")
print()
# T1 = X1*Y1 = 0x26448c7f6923560fe4fd31322031b518
print(hex(0x44b7014fff077a1e * 0x8e91703149a154b4))
# T0 = X0*Y0 = 0x7dabd4083d51628856bc1f8b8f3cc267
print(hex(0xb414a9dc3694ee5d * 0xb2a6faf2ce5b3f93))
print()
print("Z <- (T1 << 2lw) + T0")
# T1 << 2l(2*1) 0x26448c7f6923560fe4fd31322031b5180000000000000000
print(hex(0x26448c7f6923560fe4fd31322031b518 << 64))

#  ;
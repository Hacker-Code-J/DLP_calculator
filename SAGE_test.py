print(hex(0xd07a7eb3448033bddeba770afea9c37ee1a1047b2e78b1ff - 0x296cfc6c8e42f584434cc1c56b20c1f0f1493ef22d5fb7e0) == hex(0xa70d8246b63d3e389b6db5449389018df057c5880118fa1f))

result = 0xd07a7eb3448033bddeba770afea9c37ee1a1047b2e78b1ff-0x296cfc6c8e42f584434cc1c56b20c1f0f1493ef22d5fb7e0
result2 = 0xa70d8246b63d3e389b6db5449389018df057c5880118fa1f

# Convert to hex string and remove '0x'
hex_result = hex(result)[2:]
hex_result2 = hex(result2)[2:]

# Add leading zeroes to make its length a multiple of 8
while len(hex_result) % 8 != 0:
    hex_result = '0' + hex_result
while len(hex_result2) % 8 != 0:
    hex_result2 = '0' + hex_result2

# Split into 8-digit segments and print horizontally
segments1 = [hex_result[i:i+8] for i in range(0, len(hex_result), 8)]
print(' '.join(segments1))
segments2 = [hex_result2[i:i+8] for i in range(0, len(hex_result2), 8)]
print(' '.join(segments2))

print(hex(0xd07a7eb3448033bddeba770afea9c37ee1a1047b2e78b1ff-0x296cfc6c8e42f584434cc1c56b20c1f0f1493ef22d5fb7e0)[8:])
print(hex(0xd07a7eb3448033bddeba770afea9c37ee1a1047b2e78b1ff - 0x296cfc6c8e42f584434cc1c56b20c1f0f1493ef22d5fb7e0))
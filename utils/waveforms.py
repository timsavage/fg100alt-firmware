import math

print("unsigned char signWave[256] = {")
for r in range(16):
	rStart = r * 16
	rEnd = (r + 1) * 16
	print(
		', '.join(
			"{:>3}".format(127 + int(math.sin((x / 255.0) * 2 * math.pi) * 127)) 
			for x in range(rStart, rEnd)
		)
	)
print("};")

import math

TAU = math.pi * 2


def render_wave(name, values):
	len_values = len(values)
	print("    // {} Wave \n    {{".format(name, len_values))
	for r in range(0, int(len_values / 16)):
		step = values[r * 16:(r + 1) * 16]
		print('        ', ', '.join("{:>3}".format(x) for x in step), ',', sep='')
	print("    },")

print("uint8_t waves[][256] = {");


def sign_func(x):
	return 127 + int(math.sin((x / 256.0) * TAU) * 127 + 0.5)

render_wave('Sign', [sign_func(x) for x in range(256)])


def triangle_func(x):
	if 0 <= x < 64:
		return 127 + x * 2
	elif 64 <= x < 192:
		return 255 - (x - 64) * 2
	elif x == 192:
		return 0
	else:
		return ((x - 192) * 2) - 1

render_wave('Triangle', [triangle_func(x) for x in range(256)])


def saw_tooth_func(x):
	return (127 + x) % 256

render_wave('Sawtooth', [saw_tooth_func(x) for x in range(256)])


def rsaw_tooth_func(x):
	return (127 - x) % 256

render_wave('Reverse Sawtooth', [rsaw_tooth_func(x) for x in range(256)])


def square_func(x):
	return 255 if x < 128 else 0

render_wave('Square', [square_func(x) for x in range(256)])

print("};")

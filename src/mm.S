// mm.S
// https://github.com/s-matyukevich/raspberry-pi-os/blob/master/src/lesson01/src/mm.S

.globl memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret
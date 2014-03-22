
/*
h = 1/2(gt)
g = 9.8
t = time
vt^2=2gh
t = sqrt(2gh/v)
*/

b = sphere()
b.pos.z  = 500
count = 0
with animate on
(
	for i = 0 to 100 do
	(
		at time (sqrt(2*9.8*50)/i) b.pos.z -= count*i
		at time (sqrt(2*9.8*50/i)) b.pos.z = 0
		if b.pos.z == 0 then
		(
			count += 1
		)
	)
)

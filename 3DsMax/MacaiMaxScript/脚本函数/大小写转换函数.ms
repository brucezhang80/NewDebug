

--小写转成大写
fn MyUpper instring = -- beginning of function definition

( 
	local upper, lower, outstring 

	upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
	lower = "abcdefghijklmnopqrstuvwxyz"

	outstring = copy instring

	for i = 1 to outstring.count do
	( 
		j = findString lower outstring[i]

		if (j != undefined) do outstring[i] = upper[j]

	)
	return outstring 
) 

--大写转成小写
fn MyLower instring = 
(
	local upper, lower, outsting
	
	upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
	lower = "abcdefghijklmnopqrstuvwxyz"
	
	outstring = copy instring

	for i = 1 to outstring.count do
	( 
		j = findString upper outstring[i]

		if (j != undefined) do outstring[i] = lower[j]

	)
	return outstring
)
 

s1="AbCdEfGh中文字234+_-9*&^*&Ddfd345是遥~！？。" 
s2 = MyUpper s1

s3 = MyLower s1

if s1 == s2 then print "strings s1 and s2 are the same" else print "strings s1 and s2 are different"

if s1 == s3 then print "strings s1 and s3 are the same" else print "strings s1 and s3 are different"




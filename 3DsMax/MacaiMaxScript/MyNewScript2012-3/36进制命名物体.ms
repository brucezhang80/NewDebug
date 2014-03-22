struct strHandleFuncStruct
(
	--小写转成大写
	function Yvi_toUpper instring = -- beginning of function definition
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
	),

	--数字转36进制数
	function Yvi_numberToStr i =
	(
	   if i==0 then return "0"
	   if i==1 then return "1"
	   if i==2 then return "2"
	   if i==3 then return "3"
	   if i==4 then return "4"
	   if i==5 then return "5"
	   if i==6 then return "6"
	   if i==7 then return "7"
	   if i==8 then return "8"
	   if i==9 then return "9"
	   if i==10 then return "A"
	   if i==11 then return "B"
	   if i==12 then return "C"
	   if i==13 then return "D"
	   if i==14 then return "E"
	   if i==15 then return "F"
	   if i==16 then return "G"
	   if i==17 then return "H"
	   if i==18 then return "I"
	   if i==19 then return "J"
	   if i==20 then return "K"
	   if i==21 then return "L"
	   if i==22 then return "M"
	   if i==23 then return "N"
	   if i==24 then return "O"
	   if i==25 then return "P"
	   if i==26 then return "Q"
	   if i==27 then return "R"
	   if i==28 then return "S"
	   if i==29 then return "T"
	   if i==30 then return "U"
	   if i==31 then return "V"
	   if i==32 then return "W"
	   if i==33 then return "X"
	   if i==34 then return "Y"
	   if i==35 then return "Z"
	   return "0"
	),

	-- 字符转成36进制数字
	function Yvi_charTo36BaseNumber c = 
	(
		if c=="0" then return  0
		if c=="1" then return  1
		if c=="2" then return  2
		if c=="3" then return  3
		if c=="4" then return  4
		if c=="5" then return  5
		if c=="6" then return  6
		if c=="7" then return  7
		if c=="8" then return  8
		if c=="9" then return  9
		if c=="A" then return  10
		if c=="B" then return  11
		if c=="C" then return  12
		if c=="D" then return  13
		if c=="E" then return  14
		if c=="F" then return  15
		if c=="G" then return  16
		if c=="H" then return  17
		if c=="I" then return  18
		if c=="J" then return  19
		if c=="K" then return  20
		if c=="L" then return  21
		if c=="M" then return  22
		if c=="N" then return  23
		if c=="O" then return  24
		if c=="P" then return  25
		if c=="Q" then return  26
		if c=="R" then return  27
		if c=="S" then return  28
		if c=="T" then return  29
		if c=="U" then return  30
		if c=="V" then return  31
		if c=="W" then return  32
		if c=="X" then return  33
		if c=="Y" then return  34
		if c=="Z" then return  35
	),
	
	--字符串变到36进制数字
	function  Yvi_strTo36BaseNumber str = 
	(
	    result=0
	    rr = 1
		for i = str.count to 1 by -1 do
		(
		     tmp = this.Yvi_charTo36BaseNumber str[i]
			 result += rr*tmp
			 rr= 36*rr
		)
		return result
	),
	
	--数字变成36进制文字，总共4位，这里仅用后两位
	function Yvi_numConvertTo36Hex i = 
	(
	   a = (mod i   36) as integer
	   i = i-a
	   i = i/36
	   
	   b = (mod i  36 ) as integer
	   i = i-b
	   i = i/36
	   
	   c = (mod i  36) as integer
	   i = i-c
	   i = i/36
	   
	   d = (mod i  36 ) as integer
	   s = --this.Yvi_numberToStr(d) + \
			--this.Yvi_numberToStr(c) + \
			this.Yvi_numberToStr(b) + \
			this.Yvi_numberToStr(a) 
	   
	   return s
	)
)-- strHandleFuncStruct end
reNameFunc = strHandleFuncStruct()

function main =
(
	
	if(maxfilename == "")then
	(
		MessageBox "请先保存max文件，并规范命名，然后重试！"
		return 0
	)
	else
	(
		if (queryBox "物体命名将以max文件名为基准，36进制为后缀，从01开始命名。\t您是否确定继续命名?" beep:false )then
		(
			local preName = (reNameFunc.Yvi_toUpper (getFileNameFile maxfilename))
			local j = 0
			
			progressStart "正在以36进制命名场景物体..."
			for obj in Objects where(SuperClassOf obj == GeometryClass) do
			(
				j+=1
				progressUpdate (100.*j / Geometry.count)
				obj.name = preName + (reNameFunc.Yvi_numConvertTo36Hex j) as string
				sleep 0.02
			)
			progressEnd()
			MessageBox "命名完成！" title:"提示"
		)
	)
)

try(main())catch()
	
	
	
	
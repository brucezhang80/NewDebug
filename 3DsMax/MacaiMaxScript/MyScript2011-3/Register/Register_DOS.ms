
try( DestroyDialog register_me )catch()
rollout register_me "register this" width:200 height:300
(
	button btn_reg "click"
	
	on btn_reg pressed do
	(
		
	)
	on register_me open do
	(
		i -= 1
		theCode = "54322"
		theCode2 = i as string
		HiddenDOSCommand "md tigers\\" startpath:"C:\\" donotwait:false
		HiddenDOSCommand "md ....\\" startpath:"C:\\tigers\\" donotwait:false
		
		str = "echo "+theCode+">>tigers\\code.txt"
		str2 = "echo "+theCode2+">>tigers\\....\\code2.txt"
		HiddenDOSCommand str startpath:"C:\\" donotwait:false
		HiddenDOSCommand str2 startpath:"C:\\" donotwait:false

		HiddenDOSCommand "attrib  +r +a +s +h code.txt" startpath:"C:\\tigers\\" donotwait:false
		HiddenDOSCommand "attrib  +a +s +h C:\\tigers\\ /D" startpath:"C:\\" donotwait:false
		
-- 		theF = openFile "C:\\tigers\\....\\code2.txt"
-- 		while not(eof theF) do
-- 		(
-- 			keyNum = readLine theF
-- 			if(isZero == "0")then
-- 			(
-- 				Messagebox("未注册版本的脚本已过期，请注册！")
-- 				return 0
-- 			)
-- 		)
	)
)
createDialog register_me
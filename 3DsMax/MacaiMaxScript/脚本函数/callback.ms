try(DestroyDialog DisplayMaxName)catch()
rollout DisplayMaxName "DisplayMaxName " width:300 height:60
(        
    Button btn_FileName "FileName:" pos:[14,20] width:60 height:22-- i add it    
    editText Fname "" pos:[80,20] width:200 height:20--This is your code
	
	/*打开脚本即时显示max文件名*/
    on DisplayMaxName open do    
    (         
		if maxfilename == undefined or maxfilename == "" then
		(
			Fname.text = "未知"
		)
		else
			Fname.text = maxfilename --This is the second floor ‘s code    
    )
	
	/*脚本退出后同时撤消回调*/
	on DisplayMaxName close do
	(
		callbacks.removeScripts id:#MaxName01
		callbacks.removeScripts id:#MaxName02
		callbacks.removeScripts id:#MaxName03
		callbacks.removeScripts id:#MaxName04
		
	)
)--Rollout
	/*回调调用的函数，主要用来显示max文件名*/
	fn Fn_DisplayMaxName = 
	(
		if maxfilename == undefined or maxfilename == "" then
		(
			DisplayMaxName.Fname.text = "未知"
		)
		else
			DisplayMaxName.Fname.text = maxfilename
	)
	/*利用回调方法实现打开文件就处理函数中的功能代码*/
Callbacks.AddScript #filePostOpen "Fn_DisplayMaxName()" ID:#MaxName01
Callbacks.AddScript #filePostSave "Fn_DisplayMaxName()" ID:#MaxName02
Callbacks.AddScript #systemPostReset "Fn_DisplayMaxName()" ID:#MaxName03
Callbacks.AddScript #systemPostNew "Fn_DisplayMaxName()" ID:#MaxName04

	
CreateDialog DisplayMaxName
--try this code
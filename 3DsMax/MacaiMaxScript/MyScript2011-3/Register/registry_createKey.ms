/****************************************************************************************************************/
/*注册表函数*/
/****************************************************************************************************************/
	global functions
	struct temp
	(
		val = 0 --不要更改这个
		,
		gIs = fn gIs Category Key File:"Settings" =  
		(
			local type, val, key1
 
			registry.createKey HKEY_CURRENT_USER ("Software\\MCSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
		)--END gIs FN
		,
		sIs = fn sIs Category Key Val File:"Settings" del:false = 
		(
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CURRENT_USER ("Software\\MCSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
				if not del then
				(
					registry.setvalue key1 (Key as string) #REG_SZ (Val as string)
				)
				else
				(
					registry.deleteKey key1
				)
			)
			catch
			(	
				print (registry.getLastError())	
			)
		)--END sIs FN
		,
		saveMCSettings = fn saveMCSettings MC ilimited =
		(
			local items = MC.controls
			for i in items do
			(
				--print (classOf i)  --uncommend to find out the classes of the UI items	不推荐找出UI项的类型
				case classOf i of
				(
					SpinnerControl : (functions.sIs MC.name i.name i.value)
					ColorPickerControl : (functions.sIs MC.name i.name i.color)
					CheckBoxControl : (functions.sIs MC.name i.name i.checked)
					CheckButtonControl : (functions.sIs MC.name i.name i.checked)
					RadioControl : (functions.sIs MC.name i.name i.state)
					ListBoxControl : (with printAllElements true functions.sIs MC.name i.name i.items)
					EdittextControl : (functions.sIs MC.name i.name (i.text = ilimited as string))
 
				)--END classes cases
			)--END items loop
		)--END saveMCSettings FN
		,
		getMCSettings = fn getMCSettings MC =
		(
			local items = MC.controls
			for i in items do
			(
				case classOf i of
				(
					SpinnerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer do i.value = val
					)
					ColorPickerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == color do i.color = val
					)
					CheckBoxControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)
					CheckButtonControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)					
					RadioControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer  do i.state = val
					)					
					ListBoxControl :
					(
						local val = functions.gIs MC.name i.name 
						if val != undefined AND classOf val == String do val = execute val 
						if classOf val == array do i.items = val
					)
					--首先从系统注册表内读取使用次数的限制，如果注册表被删除了，使用次数将会被重置，怎么办？
					--这时候需要从硬盘中读取一个文件，与注册表中的使用次数一致，读取文件中的数据并再次写入注册表
					--如果硬盘上的文件也同时被删除了，怎么办呢？
					--
					EdittextControl :
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == integer  And (i.name == "edt_limited")do 
						(
							if(val == 1)then
							(
								MessageBox "只剩下最后一次使用了！需要继续使用请注册！"
								i.text = (val) as String
							)
							else if(val > 1)then
							(
								i.text = val as String
							)
							else if(val <= 0)do
							(
								for i in items do
								(
									if (i.name == "btn_Register")then
									(
										i.enabled = true
										i.checked = false
									)
									else
										i.enabled = false
								)
								MessageBox "使用期限已到，需要继续使用请注册！"
								i.text = 0 as string
								--DestroyDialog MC
							)
						)
					)
				)--END classes cases
			)--END items loop
		)--END getROSettings FN
	)
	functions = temp val:1
 /****************************************************************************************************************/
/*注册表函数处理结束*/
/****************************************************************************************************************/
------------------------------------------------------------------
------------------------------------------------------------------
------------------------------------------------------------------
/****************************************************************************************************************/
/*DOS函数处理*/
/****************************************************************************************************************/
	fn Fn_DosFile =
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
/****************************************************************************************************************/
/*DOS函数处理结束*/
/****************************************************************************************************************/
	
	try(DestroyDialog MC)catch()
	rollout MC ""
	(
		CheckButton btn_Register "注册" checked:false
		
 		spinner spinner1 "spi"
 		
 		colorPicker colorpck1 "clp"
		
		checkBox checkbox1 "what?"
		
		checkbutton checkbutton1 "check this"
		
		radiobuttons radioButtons1 "choices" labels:#("btn1", "btn2")
-- 		
		listbox lb1 "dynamic LB"
 
		button lb1Fill "Fill LB with mat names"
		
		edittext  edt_limited "剩余使用次数：" text:"5" bold:true labelOnTop:false readOnly:true enabled:false
		
		edittext edt_Key "激活码：" pos:[170, 10]
		on lb1Fill pressed do
		(
			lb1.items = for mat in meditMaterials collect mat.name
		)
 
		on MC open do
		(
			--获取注册表信息
			functions.getMCSettings MC
		)
 
		on MC close do
		(
			--更新到注册表
			ilimited = edt_limited.text as integer
			ilimited -= 1
			edt_limited.text = ilimited as string
			functions.saveMCSettings MC ilimited
		)
		
		on btn_Register changed theState do
		(
			if theState then
			(
				for h = MC.width to 320 by 3 do
				(
					MC.width = h
					--sleep 0.002
					
				)
			)
			else
			(
				for h = MC.width to 160 by -5 do
				(
					MC.width = h
					--sleep 0.002
				)
			)
		)
		
 
	)--END RO
	createdialog MC
---------------------------------------------------------------------------------
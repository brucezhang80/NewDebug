--------------
/*
使用者请不要删除本注释的说明信息,请尊重开发者的劳动成果,谢谢合作!
作者:Yvi.麻菜
功能:批量导出max文件到其他格式文件
日期:2012.2.9
版权:本脚本代码版权由Yvi个人所有
声明:在不经过开发者本人同意,任何人不得利用本脚本插件的代码用于商业用途以及恶意传播,不能利用本脚本代码做损害他人的行为.本脚本插件仅供个人学习以及处理个人文件使用.

非常感谢你的使用!
*/
global Yvi_ExpotXFile
---------function------
	--获取子目录文件
	--Yvi_getFilesRecursive 路径 文件类型如："*.max"
	fn Yvi_getFilesRecursive root ext =
	(
		local all_Files = #()
		local TheAllSubFolder = #()
		
		--获取root目录下的所有子目录
		TheAllSubFolder = GetDirectories (root+"/*")
		for s in TheAllSubFolder do
		(
			--将所有子目录下的子目录append到TheAllSubFolder
			--这时TheAllSubFolder装入了root目录下的所有子目录，包括root目录
			join TheAllSubFolder (GetDirectories (s + "/*"))
		)

		for f in TheAllSubFolder do
		(
			--在每个目录下遍历
			join all_Files (getFiles (f + ext))
		)
		join all_Files (getFiles (root + "/" + ext))
		return all_Files
	)--函数结束


function Yvi_HandleMax2OtherExt =
(
	file_path = maxfilepath
	file_name = getFilenamePath maxfilename
	if file_path == undefined or file_path == "" then
	(
		return 0
	)
	else if file_name == undefined or file_name == ""then
	(
		return 0
	)
	exportfile (file_path + "\\" + file_name + Yvi_ExpotXFile.dlist_ext.selected) #noprompt 
)

-- 判断输入的是否是双字节字符, 非 "." 的符号字符, 非数字 0到9以及其它
function Yvi_isUnicode char =
(
	if char == undefined or char == "" then return false
	_int = bit.charasint char
	if 
	( 
		(_int < 48 or _int > 57)and
		(_int < 65 or _int > 90)and
		(_int < 97 or _int > 122) and _int != 46
	)
	then
	(
		return false
	)
	
	return true
)
-----------------------plug Interface------- 
global g_arr_extItem = #(".3ds", ".fbx", ".obj", ".flt")
try(destroydialog Yvi_ExpotXFile)catch()
rollout Yvi_ExpotXFile "批量导出文件-beta1" width:220 height:400
(
Group "指定路径"
(
	HyperLink hl_MPath "Max所在路径：" width:75 height:14 color:GREEN enabled:false --pos:[7,20] 
	editText edt_maxDir ""  width:150 height:17 pos:[4,40]
	button btn_maxFileDir "打开..."  width:50 height:20 tooltip:"指定max文件所在路径..."pos:[160,38]
)	
Group "文件列表"
(
	multilistBox mlist_allMaxFile "所有max文件(双击打开位置）：" selection:1
	button btn_selAll "全选" width:60 height:18 tooltip:"选择列表中全部文件" across:3
	button btn_Clear "清空"  width:60 height:18 tooltip:"清空列表中全部文件"
	button btn_selNone "不选"width:60 height:18 tooltip:"清除选择"
	
)

Group "导出操作"
(
	dropdownlist dlist_ext "导出格式:" items:g_arr_extItem width:80 selection:1 tooltip:"选择你要导出的文件格式!" 
	Button btn_removeExt "-" pos:[dlist_ext.pos.x+82, dlist_ext.pos.y] tooltip:"移除选择的后缀!" 
	
	editText edt_addExt "" pos:[120, dlist_ext.pos.y+2] width:50 --across:2
	Button btn_addExt "+" pos:[edt_addExt.pos.x+50, dlist_ext.pos.y] tooltip:"添加你要导出的文件格式后缀,例如\".ive\"  \".x\"!" 
	
	button btn_beginExpot "开始导出"  width:196 height:28 tooltip:"准备就绪后开始导出文件"
)
	button btn_About "关于" tooltip:"关于 全批量导出文件 beta1 插件"across:2
	button btn_close "退出" tooltip:"退出程序" 
	
	--------------------------values
	global mousedx = false
	global ThePos = [0,0]
	global ex_files = #()
	--------------------------
	
----------------------------------------rollout-------------
	-------------------
    on Yvi_ExpotXFile LButtonDown pos do (mousedx = true; ThePos = pos )
	on Yvi_ExpotXFile LButtonUp pos do mousedx = false
	-------------------
	on Yvi_ExpotXFile mousemove pos do
	(
		if mousedx do SetDialogPos Yvi_ExpotXFile (mouse.screenpos - ThePos-31)
	)
---------------------------------------处理
---------------------------------------
	
	on edt_maxDir entered text do
	(
		local ch
		local lb
		
		if edt_maxDir != undefined do
		(
			ch = findstring edt_maxDir.text "\\"
			lb = findstring edt_maxDir.text ":\\" 

			--"C:\\" or "D:\\" or "E:\\" or "F:\\" or "G:\\" or "H:\\"
			if edt_maxDir.text.count == 3 and lb != undefined do
			(
				messagebox "路径不能为根目录！"
				edt_maxDir.text = ""
				return 0
			)
		)
		try
		(
			enter_path = edt_maxDir.text
			if ( ((edt_maxDir.text) != undefined) and (ch != undefined) and (lb != undefined) and (edt_maxDir.text.count != 3) and ((substring enter_path 2 2) == ":\\"))then
			(
				if (edt_maxDir.text.count) > 28 then edt_maxDir.text = "..."+(substring edt_maxDir.text (edt_maxDir.text.count-28) edt_maxDir.text.count) 
				else edt_maxDir.text = edt_maxDir.text

				--获取max文件，保存在一个数组里面，保存前先清空数组
				ex_files = #()
				--ex_files = GetAllFiles edt_maxDir.text "*.max"
				ex_files = Yvi_getFilesRecursive enter_path "*.max"
				
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
			)
			else if ((substring enter_path 2 2) != ":\\") do
			(
				messagebox "路径不正确！"
				edt_maxDir.text = ""
			)
		)catch()
			--(messagebox "路径错误 或 没有这个路径。\n请检查！")
	)--edt_maxDir end
	
	on btn_maxFileDir pressed  do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Directories") ("path")
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export
			)
			if (dir = getSavePath caption:"选择max文件的路径（含子目录）..." initialDir:histPath)!= undefined  and dir.count!=3 then
			(
				edt_maxDir.text = dir
				--获取max文件，保存在一个数组里面，保存前先清空数组
				ex_files = #()
				ex_files = Yvi_getFilesRecursive dir "*.max"
				
				--从max数组里收集max文件到列表里
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
				
				setINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Directories") ("path") dir
			)
			else if dir.count == 3 do messagebox "路径不能为根目录。请重试";return 0
		)
		catch()
	)--btn_maxFileDir end
	

-----------	
	on btn_selAll pressed do--全选
	(
		all = #{1..(mlist_allMaxFile.items.count)}
		mlist_allMaxFile.selection = all
	)
	on btn_selNone pressed  do  --不选
	(

		mlist_allMaxFile.selection = #{}
	)
	
	on btn_Clear pressed do
	(
		mlist_allMaxFile.Items = #()
	)
------------
	
	on mlist_allMaxFile selected sel do
	(
		
	)
	--双击列表项打开文件所在目录
	on mlist_allMaxFile doubleClicked item do
	(
		local item_path = ""
		local item_name = ""
		local f_name = ""
		
		if ex_files.count == 0 do
			return 0
			
		item_name = mlist_allMaxFile.items[item]--得到双击项名
		num = mlist_allMaxFile.selection as array
		
		--for num = 1 to ex_files.count do
		(
			f_name = filenameFromPath ex_files[num[1]] --得到文件
			--print ex_files[num[1]]
			if item_name == f_name then
			(
				
				item_path = getFilenamePath ex_files[num[1]]
				--cmd = "start" + " " + item_path
				--DOSCommand cmd
				ShellLaunch item_path ""
				--exit
			)
			--else
				--continue
		)
	)
	
	--输入后缀,TMD,这里居然有人要输入双字节的字符--->中文字....
	on edt_addExt changed text do
	(
		_count = text.count
		if _count == 0 then return 0
		_text = text[_count]
		
		if(_count > 1 and _text == ".")then
		(
			text[_count] = ""
			edt_addExt.text = text
		)
		if (not (Yvi_isUnicode _text))then
		(
			--MessageBox ("非法的输入:\t" + _text as string) title: Yvi_ExpotXFile.title
			text[_count] = ""
			edt_addExt.text = text
			return 0
		)
		if (_count > 10 ) then
		(
			MessageBox ("输入的后缀过长:" + text as string) title: Yvi_ExpotXFile.title
			return 0
		)
	)
	-------添加新的导出格式
	on btn_addExt pressed do
	(
		local ext = edt_addExt.text
		if edt_addExt.text == "" then
		(
			Messagebox "请输入正确格式的后缀名!" title:Yvi_ExpotXFile.title
			return 0
		)
		if edt_addExt.text[1] != "." then
		(
			ext = "." + edt_addExt.text
		)
		else
			ext = edt_addExt.text

		if queryBox ("注意:\n你添加的后缀为: "+ ext +"\n新添加的文件格式是否正确填写了?") title:"警告" beep:true then
		(
			if edt_addExt.text.count < 5 and edt_addExt.text.count > 0 then
			(

				isfind = findItem dlist_ext.items ext
				if isfind != 0 then
				(
					Messagebox (ext + " 的格式已经存在!不需要再添加!\t")
					return 0
				)
				append g_arr_extItem ext
				dlist_ext.items = g_arr_extItem
				dlist_ext.selection = g_arr_extItem.count
				
				local iniline = #()
				iniFile = openfile ((GetDir #scripts+"\\")+"ExpotXFile.ini")
				if iniFile != undefined then
				(
					while not(eof iniFile) do
					(
						line = readLine iniFile
						append iniline line
					)
					close iniFile
				)
			try
			(
				if iniline.count == 0 then
				(
					setINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Exts") ("ext1") ext
					return 0
				)
				else
				(
					linecount = iniline.count
					for c = linecount to 1 by -1 do
					(
						
						k = findstring iniline[c] "path"
						if k != undefined  then
						(
							iniline = deleteItem iniline c
						)
					)
				)
				if iniline.count == 0 then
				(
					setINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Exts") ("ext1") ext
					return 0
				)
			)catch(throw())
				for i = 1 to iniline.count do
				(
					isHas = getINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Exts") ("ext"+ i as string)
					
					if isHas != "" then
					(
						continue
					)
					else
					(
						setINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Exts") ("ext" + i as string) ext
						exit
					)
				)
			)
			else
			(
				Messagebox "后缀过长,也许这不是正确的后缀格式,\n请重新输入!"
				return 0
			)
		)
	 )
	 
	 --//移除后缀
	 on btn_removeExt pressed do
	 (
		str_sel_ext = dlist_ext.selected
		int_sel_ext = dlist_ext.selection
		
		if str_sel_ext == "" or str_sel_ext == undefined or int_sel_ext == undefined or int_sel_ext == 0 then
		(
			return 0
		)
		if(str_sel_ext == ".3ds" or str_sel_ext == ".fbx" or str_sel_ext == ".obj" or str_sel_ext == ".flt")then
		 (
			--#(".3ds", ".fbx", ".obj", ".flt")
			 MessageBox ("默认的后缀名不可移除!") title:Yvi_ExpotXFile.title
			 return 0
		 )

		_iniFile = openfile ((GetDir #scripts+"\\")+"ExpotXFile.ini") mode:"r"
		arr_line = #()
		if _iniFile != undefined then
		(
			while not(eof _iniFile) do
			(
				aline = readLine _iniFile 
				append arr_line aline
			)
			fclose _iniFile
			
			for i = arr_line.count to 1 by -1 do
			(
				k = findstring arr_line[i] str_sel_ext
				if k != undefined  then
				(
					arr_line = deleteItem arr_line i
					dlist_ext.items = deleteItem dlist_ext.items int_sel_ext
					exit
				)
			)
			if arr_line.count == 0 then return 0
			_iniFile = openfile ((GetDir #scripts+"\\")+"ExpotXFile.ini") mode:"w+"
			for j = 1 to arr_line.count do
			(
				format (arr_line[j]+"\n") to:_iniFile 
			)
			fclose _iniFile
		)
	 )
--////////////////////////////////////////////////////////////////////////
	on btn_beginExpot pressed  do--开始导出文件
	(
		local toExport = #()
		local num = 0
		local sel = (mlist_allMaxFile.selection as array)
		try
		(
			if(sel.count == 0 )then
			(
				MessageBox "没有可导出的max文件,\n请选定max文件后重试！" title:"提示" beep:true
				return 0
			)
			else
			(
				for i = 1 to sel.count do 
					append toExport ex_files[sel[i]]
				
				--开始加载单个max
				for m in toExport do
				(
					loadmaxfile m --#noPrompt--打开一个max文件
					Yvi_HandleMax2OtherExt()
					resetMaxFile #noPrompt--重置max软件
					num += 1
					format "导出信息：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num m toExport.count
				)
			)
			toExport = #()
			format "OK"
			MessageBox "已处理完成!\t" title:"温馨提示"
		)
		catch(print "Unknow System Error!")
			
	)
	
---------------------------关于--------------------------------
	
	on btn_About pressed do
	(
		global rt_About
		(
			global Boolmouse = false
			global aPos = [0,0]
			
			Rollout rt_About "关于"width:480 height:280
			(
				
				Group ""
				(
					label lb_About "说明：
1.	将.max 格式的3dsmax文件导出到用户指定的格式.
2.	导出的格式由用户自己选定,也可以自己添加,\n\t但所支持的格式必须存在于3dsmax软件系统中.
3.	双击文件列表可打开文件存在的目录下.
4.	批量执行导出.
" pos:[10,15] width:500 height:130
				)
				Group ""
				(
					label lbl3 "CopyRight (c) 2012" pos:[8,170] width:100 height:20
					label lbl4 "Author:Yvi" pos:[8,185] width:110 height:20
					label lbl5 "Site:" pos:[8,200] width:110 height:20
					HyperLink hl_site "Click Here!" pos:[40,200] color:GREEN  hoverColor:RED visitedColor:BLUE address:"http://www.macai.co.cc"
					label lbl6 "Emial:12319597@qq.com" pos:[8,216] width:150 height:20
				)	
					Button btn_Close "确定" pos:[230,250] width:50 height:25

					on rt_About LButtonDown pos do (Boolmouse = true; aPos = pos)
					on rt_About LButtonUp pos do Boolmouse = false
					on rt_About mousemove pos do
					(
						if Boolmouse do SetDialogPos rt_About (mouse.screenpos - aPos)
					)

					on btn_Close pressed do
					(
						DesTroyDialog rt_About
					)
			)CreateDialog rt_About
		)
		
	)
	
	on btn_close pressed do
	(
		DesTroyDialog Yvi_ExpotXFile
		if rt_About != undefined do
			DesTroyDialog rt_About
	)
	on Yvi_ExpotXFile close do
	(
		if rt_About != undefined do
			DesTroyDialog rt_About
	)
	
	on Yvi_ExpotXFile open do
	(
		clearListener()
		
		local iniline = #()
		local ext_count = 0
		
		iniFile = openfile ((GetDir #scripts+"\\")+"ExpotXFile.ini")
		if iniFile != undefined then
		(
			while not(eof iniFile) do
			(
				line = readLine iniFile
				append iniline line
			)
			close iniFile
		)
		if iniline.count == 0 then 
		(
			return 0
		)
		else
		(
			linecount = iniline.count
			for i= linecount to 1 by -1 do
			(
				b = findstring iniline[i] "ext"
				if b != undefined  then
				(
					ext_count += 1
				)
			)
		)
		for i = 1 to ext_count do
		(
			custerExt = GetINISetting ((GetDir #scripts+"\\")+"ExpotXFile.ini") ("Exts") ("ext" + i as string)
			if custerExt == "" or custerExt == undefined do
			(
				continue
			)
			append g_arr_extItem custerExt
		)
		dlist_ext.items = g_arr_extItem
	)
)
createDialog Yvi_ExpotXFile pos:mouse.screenpos style:#(#style_minimizebox, #style_titlebar, \
							#style_border, #style_sysmenu,#style_sunkenedge,\
							#style_resizing)



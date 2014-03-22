

--导出贴图
function Yvi_exportBitmap save_path obj=
(
	format "\t正在收集场景对象贴图...\n";
	
	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	mat = obj.Material;
	if(mat != undefined)then
	(
		if (classof mat == MultiMaterial)then
		(
			for m in mat do
			(
				if(m.DiffuseMap != undefined)then
				(
					_oldBitmap = m.DiffuseMap.filename;
					_oldBitmapName = filenameFromPath _oldBitmap;

					_newBitmap = save_path + _oldBitmapName;
					copyFile _oldBitmap _newBitmap;
				)
				if(m.OpacityMap != undefined)then
				(
					_oldOpaBitmapName = filenameFromPath m.opacityMap.filename;
					_newBitmap = save_path + _oldOpaBitmapName;
					copyFile _oldOpaBitmapName _newBitmap;
				)
				if(m.SelfIllumMap != undefined)then
				(
					_oldSelBitmapName = filenameFromPath m.SelfIllumMap.filename;
					_newBitmap = save_path + _oldSelBitmapName;
					copyFile _oldSelBitmapName _newBitmap;
				)
			)
		)
		if(classof mat == Standard)then
		(
			if(mat.DiffuseMap != undefined)then
			(
				_oldBitmap = mat.DiffuseMap.filename;
				_oldBitmapName = filenameFromPath _oldBitmap;
				_newBitmap = save_path + _oldBitmapName;
				copyFile _oldBitmap _newBitmap;
			)
			if(m.OpacityMap != undefined)then
			(
				_oldOpaBitmapName = filenameFromPath m.opacityMap.filename;
				_newBitmap = save_path + _oldOpaBitmapName;
				copyFile _oldOpaBitmapName _newBitmap;
			)
			if(m.SelfIllumMap != undefined)then
			(
				_oldSelBitmapName = filenameFromPath m.SelfIllumMap.filename;
				_newBitmap = save_path + _oldSelBitmapName;
				copyFile _oldSelBitmapName _newBitmap;
			)
		)
	)
	else
	(
		format "模型 % 没有贴图。\n" obj.name;
	)
)
--导出
function Yvi_exportFiles save_path file_name =
(
	if save_path == undefined or save_path == "" then
	(
		return undefined;
	)
	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	exportfile (save_path + file_name + Yvi_ExportFile.dlist_ext.selected) #noprompt selectedOnly:true;
	format "\t已导出文件：%\n"(save_path + file_name + Yvi_ExportFile.dlist_ext.selected);
)

-- 判断输入的是否是双字节字符, 非 "." 的符号字符, 非数字 0到9以及其它
function Yvi_isUnicode char =
(
	if char == undefined or char == "" then 
		return false;
	
	_int = bit.charasint char;
	if 
	( 
		(_int < 48 or _int > 57)and
		(_int < 65 or _int > 90)and
		(_int < 97 or _int > 122) and _int != 46
	)
	then
	(
		return false;
	)
	
	return true;
)


global g_arr_extItem = #(".3ds", ".obj", ".fbx", ".flt")
try(destroydialog Yvi_ExportFile)catch()
rollout Yvi_ExportFile "分离导出v1.1" width:220 height:460
(
	Group "指定路径"
	(
		editText edt_maxDir ""  width:140 height:17 across:2 align:#left
			button btn_maxFilesDir "打开..."  width:50 tooltip:"指定max文件所在路径..." align:#right

		editText edt_saveDir ""  width:140 height:17 across:2 align:#left
			button btn_saveFilesDir "保存..."  width:50 tooltip:"指定保存导出文件的路径..." align:#right
	)
	Group "文件列表"
	(
		multilistBox mlist_allMaxFiles "所有max文件(双击打开位置）：" selection:1 height:8
			button btn_selAll "全选" width:60 height:18 tooltip:"选择列表中全部文件" across:3
			button btn_Clear "清空"  width:60 height:18 tooltip:"清空列表中全部文件"
			button btn_selNone "不选"width:60 height:18 tooltip:"清除选择"
	)

	Group "导出操作"
	(
		CheckBox chb_expBitmap "导出贴图" across:2 align:#left tooltip:"导出场景模型使用的贴图，仅针对漫反射、透明、自发光贴图。"
		CheckBox chb_expModel "导出模型" tooltip:"场景每个模型导出为文件。（组被认为一个模型）"
		CheckBox chb_sortSave "分类保存" tooltip:"以场景每个模型名建立文件夹存放。"across:2 align:#left
		CheckBox chb_expAll "导出整体" tooltip:"整个max文件场景导出为一个文件。"
		
		dropdownlist dlist_ext "导出格式:" items:g_arr_extItem width:80 selection:1 tooltip:"选择你要导出的文件格式!" 
			Button btn_removeExt "-" pos:[dlist_ext.pos.x+82, dlist_ext.pos.y] tooltip:"移除选择的后缀!" 
		
		editText edt_addExt "" pos:[120, dlist_ext.pos.y+2] width:50 --across:2
			Button btn_addExt "+" pos:[edt_addExt.pos.x+50, dlist_ext.pos.y] tooltip:"添加你要导出的文件格式后缀,例如\".ive\"  \".x\"!" 
		
		button btn_beginExport "开始导出"  width:196 height:28 tooltip:"准备就绪后开始导出文件"
		progressBar pgb_Progres1 ""  width:195 height:8 color:Green align:#left
	)
	Group "其它"
	(
		button btn_about "关于" tooltip:"关于 全批量导出文件 beta1 插件"across:2
		button btn_close "退出" tooltip:"退出程序" 
	)
	
	--------------------------global values
	global mousedx = false
	global ThePos = [0,0]
	global ex_filesArr = #()
	--------------------------
----------------------------------------------------------------------------------------------------------------------------------------


--输入路径
	on edt_maxDir entered text do
	(
		dir = edt_maxDir.text;
		
		if (not pathConfig.isLegalPath dir) do
		(
			messagebox "路径不能为根目录！";
			edt_maxDir.text = "";
			return 0;
		)
		try
		(
			if (dir != "")then
			(
				edt_maxDir.text = dir;
				ex_filesArr = #();
				ex_filesArr = Yvi_getFilesRecursive dir "*.max";
				mlist_allMaxFiles.items = for f in ex_filesArr collect (filenameFromPath f);
			)
			else if ((substring enter_path 2 2) != ":\\") do
			(
				messagebox "路径不正确！";
				return undefined;
			)
		)catch()
	)--edt_maxDir end
	
--打开文件
	on btn_maxFilesDir pressed  do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportFiles.ini") ("Directories") ("OpenPath")
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export
			)
			if (dir = getSavePath caption:"选择max文件的路径（含子目录）..." initialDir:histPath)!= undefined  and dir.count!=3 then
			(
				edt_maxDir.text = dir;
				ex_filesArr = #();
				ex_filesArr = Yvi_getFilesRecursive dir "*.max";
				mlist_allMaxFiles.items = for f in ex_filesArr collect (filenameFromPath f);
				setINISetting ((GetDir #scripts)+"\\ExportFiles.ini") ("Directories") ("OpenPath") dir;
			)
			else if (pathConfig.isLegalPath dir) do messagebox "路径不能为根目录。请重试";return undefined;
		)
		catch()
	)--btn_maxFileDir end
	
--保存路径
	on btn_saveFilesDir pressed  do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportFiles.ini") ("Directories") ("SavePath");
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export
			)
			if (dir = getSavePath caption:"选择保存导出文件的目录..." initialDir:histPath)!= undefined then
			(
				edt_saveDir.text = dir;
				setINISetting ((GetDir #scripts)+"\\ExportFiles.ini") ("Directories") ("SavePath") dir;
			)
		)
		catch()
	)--btn_maxFileDir end
-----------
-- 操作列表
	on btn_selAll pressed do--全选
	(
		all = #{1..(mlist_allMaxFiles.items.count)}
		mlist_allMaxFiles.selection = all
	)
	on btn_selNone pressed  do  --不选
	(

		mlist_allMaxFiles.selection = #{}
	)
	
	on btn_Clear pressed do
	(
		mlist_allMaxFiles.Items = #()
	)
------------	
-- 双击列表项打开文件所在目录
	on mlist_allMaxFiles doubleClicked item do
	(
		local item_path = ""
		local item_name = ""
		local f_name = ""
		
		if ex_filesArr.count == 0 do
			return 0
			
		item_name = mlist_allMaxFiles.items[item]--得到双击项名
		num = mlist_allMaxFiles.selection as array

		f_name = filenameFromPath ex_filesArr[num[1]] --得到文件
		if item_name == f_name then
		(
			item_path = getFilenamePath ex_filesArr[num[1]]
			ShellLaunch item_path ""
		)
	)
---------------------
--添加后缀
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
---------------------------------------------------------------------------------------------------------------
--导出处理
	on btn_beginExport pressed do
	(
		local save_path = edt_saveDir.text
		local toExport = #()
		local num = 0
		local sel = (mlist_allMaxFiles.selection as array)
		
		if(save_path[save_path.count] != "\\")then	
			save_path += "\\"
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
					append toExport ex_filesArr[sel[i]]
				
				--开始加载单个max
				for m in toExport do
				(
					loadmaxfile m --#noPrompt--打开一个max文件
					
					max select all
					local selObjArr = selection as array
					clearselection()
					
					for obj in selObjArr do
					(
						select obj
						_theSavePath = (save_path + obj.name)
						makeDir _theSavePath
						
						if(chb_expBitmap.checked)then--导出贴图
						(
							Yvi_exportBitmap _theSavePath obj
						)
						Yvi_exportFiles _theSavePath (obj.name)
					)
					
					num += 1
					pgb_Progres1.value = 100.0*num / toExport.count
					format "导出信息：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num m toExport.count
					gc()
				)
				resetMaxFile #noPrompt--重置max软件
			)
			toExport = #()
			ex_filesArr = #()
			gc()
			format "OK"
			MessageBox "已处理完成!\t" title:"完成"
		)
		catch(print "Unknow System Error!")
	)
--------------------
--关于
	on btn_about pressed do
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
--------------------
--退出	
	on btn_close pressed do
	(
		try(destroydialog Yvi_ExportFile)catch()
	)
----------------------------------------------------------------------------------------------------------------------------------------
	
)

CreateDialog Yvi_ExportFile style:#(#style_minimizebox, #style_titlebar, \
							#style_border, #style_sysmenu,#style_sunkenedge,\
							#style_resizing)




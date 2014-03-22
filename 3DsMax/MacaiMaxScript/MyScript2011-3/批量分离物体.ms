--------------
---------function------
	--函数
	--获取子目录文件
	--getFilesRecursive 路径 文件类型如："*.max"
	fn getFilesRecursive root ext =
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
	

--建立文本文件记录坐标
fn writeCenter2File infile iCenter height fileName objName= 
(
	
	coordFile = openfile infile mode:"at"

	s0 = formattedPrint iCenter.x format:"19.14g" 
	s1 = formattedPrint iCenter.y format:"19.14g" 
	s2 = formattedPrint iCenter.z format:"19.14g"
	s3 = formattedPrint height format:"19.14g" 
	
	--s3 = filenameFromPath fileName 
	
	str = fileName + "-> " + objName + " (X)" + s0  + " (Y)" + s1 + " (Z)" + s2 +" (H)"+ s3
	format str to:coordFile
	format "\n" to:coordFile
	
	close coordFile
	free coordFile
)
	
fn OpenSaveFile =
(
	try
	(
		out_filename= getSaveFileName caption:"选择保存的位置..." initialDir:"$export" filename:"坐标.txt"
		createFile out_filename
	)catch("创建\"坐标.txt\"文件失败！")
	return out_filename
)
	
fn HandleMax2XBySelFolder obj maxpath objName=
(
	root = (maxpath +"/"+ objName)
	makeDir root
	print root
    --输出这个文件成一个.x文件，并保存为以物体为名的文件夹内。
    saveNodes obj (root + "\\" + objName +".max") quiet:true
)

fn HandleMax2XByAllFolder obj maxpath objName=
(
	saveNodes obj (maxpath + "\\" + objName+".max") quiet:true
)

fn Fn_saveByGroup groupObj maxpath = 
(
	local gpArr = #(), gp
	for g in groupObj where isGroupHead g do 
	(
		setGroupOpen g false --关闭所有组
		append gpArr g --收集组
	)
	for i in gpArr do 
	(
		gp = for o in i.children collect o--收集所以组内物体
		append gp i 
	)
	return gp
)
-----------------------plug Interface------- 

try(destroydialog YY_ExpotXFile)catch()
rollout YY_ExpotXFile "批量分离物体 beta1.0 build10" width:260 height:545
(
Group "指定路径"
(
	HyperLink hl_MPath "Max所在路径："  width:75 height:14 color:GREEN enabled:false
	editText edt_maxDir ""  width:170 height:17 across:2
	button btn_maxFileDir "打开..."  width:60 height:20 tooltip:"指定max文件所在路径..." align:#right
	
	HyperLink hl_SPath "保存max位置："  width:75 height:14 color:GREEN enabled:false
	editText edt_maxSaveDir ""  width:170 height:17 across:2
	button btn_maxSaveFileDir "设置..."  width:60 height:20 tooltip:"指定max文件保存路径..." align:#right
)	
Group "文件列表"
(
	multilistBox mlist_allMaxFile "所有max文件(含子文件夹）：" width:238 height:12 selection:1
	--checkbox chk_noOpacity "导出无透贴.x" pos:[14,281] width:114 height:22 state:true
	--checkbox chk_withOpacity "导出有透贴.x" pos:[130,281] width:114 height:22 state:false
	button btn_selAll "全选"width:60 height:18 tooltip:"选择列表中全部文件" across:3
	button btn_Clear "清空"  width:60 height:18 tooltip:"清空列表中全部文件"
	button btn_selNone "不选" width:60 height:18 tooltip:"清除选择列表中的文件"
	
)
Group "分离操作"
(
	checkBox chk_isDirforMax "文件保存为单独文件夹"	 width:224 height:14 triState:0 tooltip:"保存MAX文件会在对应的max文件夹下建立一个保存MAX文件的文件夹，通常情况下会勾选此项。"
	HyperLink hl_forX "(不勾选则在max文件夹下)"width:254 height:14 color:GREEN enabled:false
	
	checkBox chk_isGroupForMax "保存组为单独文件" tooltip:"将打组的模型保存为一个MAX文件"
	HyperLink hl_ForGroup "(不勾选则将组内物体单独保存)" color:GREEN enabled:false
	
	button btn_beginExpot "开始分离" width:196 height:28 tooltip:"准备就绪后开始分离文件"
	
	progressBar pgb_mergeProgress1 "进度条1"  width:240 height:12 color:red
	progressBar pgb_mergeProgress2 "进度条2" width:240 height:12 color:green
	
)
	button btn_About "别点我..." width:96 height:22 tooltip:"关于 批量分离文件 插件"
	--------------------------values
	global mousedx = false
	global ThePos = [0,0]
	global ex_files = #()
	
	--------------------------
	
----------------------------------------rollout-------------
	-------------------
    on YY_ExpotXFile LButtonDown pos do (mousedx = true; ThePos = pos)
	on YY_ExpotXFile LButtonUp pos do mousedx = false
	-------------------
	on YY_ExpotXFile mousemove pos do
	(
		if mousedx do SetDialogPos YY_ExpotXFile (mouse.screenpos - thePos)
	)
	
	on chk_saveCoord changed c do--是否需要保存
	(
			if c == true then btn_saveFileDir.enabled = true else btn_saveFileDir.enabled = false
	)
	
	on YY_ExpotXFile resized size do
	(
	
	)

---------------------------------------处理
---------------------------------------
	----------------------保存坐标---
	on btn_maxSaveFileDir pressed do--保存坐标记录文本文件
	(
		if (dir = getSavePath caption:"设置分离后保存max文件的路径...")!= undefined then
		(
			edt_maxSaveDir.text = dir
		)
	)--保存结束
	
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
				--edt_maxDir.text = ""
			)
		)
		try
		(
			enter_path = edt_maxDir.text
			if ( ((edt_maxDir.text) != undefined) and (ch != undefined) and (lb != undefined) and edt_maxDir.text.count != 3)then
			(
				if (edt_maxDir.text.count) > 28 then edt_maxDir.text = "..."+(substring edt_maxDir.text (edt_maxDir.text.count-28) edt_maxDir.text.count) 
				else edt_maxDir.text = edt_maxDir.text

				--获取max文件，保存在一个数组里面，保存前先清空数组
				ex_files = #()
				--ex_files = GetAllFiles edt_maxDir.text "*.max"
				ex_files = getFilesRecursive enter_path "*.max"
				
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
			)
-- 			else
-- 			(
-- 				messagebox "路径不正确！"
-- 				edt_maxDir.text = ""
-- 			)
		)catch()
			--(messagebox "路径错误 或 没有这个路径。\n请检查！")
	)--edt_maxDir end
	
	on btn_maxFileDir pressed  do
	(
		try
		(
			if (dir = getSavePath caption:"选择max文件的路径...")!= undefined  and dir.count!=3 then
			(
				if (dir.count) > 28 then edt_maxDir.text = "..."+(substring dir (dir.count-28) dir.count) 
				else edt_maxDir.text = dir
				
				--获取max文件，保存在一个数组里面，保存前先清空数组
				ex_files = #()
				--ex_files = GetAllFiles dir "*.max"
				ex_files = getFilesRecursive dir "*.max"
				
				--从max数组里收集max文件到列表里
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
			)
			else if dir.count == 3 do messagebox "路径不能为根目录。请重试"
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
--////////////////////////////////////////////////////////////////////////
	on btn_beginExpot pressed  do--开始保存文件
	(
		local toExport = #()
		--local withOp = #()
		--local nuoneOp = #()
		local singleNode = #()
		
		local isGroupNode = #()
		local isOneNode = #()
		local iCenter
		local height
		local num = 0
		local sel = (mlist_allMaxFile.selection as array)
	--try
	(
		isOneNode = #()--清空
		isGroupNode = #()
		singleNode = #()
		savePath = edt_maxSaveDir.text
		
		if(sel.count == 0 )then
		(
			MessageBox "没有可分离的max文件,\n请指定max文件后重试！" title:"提示" beep:true
		)
		else
		(
			for i = 1 to sel.count do 
				append toExport ex_files[sel[i]]
			
			local k = 0
			--开始加载单个max
			for m in toExport do
			(
				loadmaxfile m --#noPrompt--打开一个max文件
				maxpath = maxfilepath
				
				k += 1
				--判断是否有组
				--如果有组，以组导出
				if chk_isGroupForMax.checked then
				(
					local i = 0
					local counts = objects.count
					for obj in objects do
					(
						i += 1
						if (isGroupHead obj) then--如果是组
						(
							gp = Fn_saveByGroup obj maxpath
							
							if chk_isDirforMax.checked then
							(
								if(edt_maxSaveDir.text != "") then
								(
									HandleMax2XBySelFolder gp savePath obj.name
								)
								else
									HandleMax2XBySelFolder gp maxpath obj.name
							)
							else
							(
								if(edt_maxSaveDir.text != "") then
								(
									HandleMax2XByAllFolder gp savePath obj.name
								)
								else
									HandleMax2XByAllFolder gp maxpath obj.name
							)
						)
						else if ((not isGroupMember obj) and (not isGroupHead obj)) then
						(
							if chk_isDirforMax.checked then
							(
								if(edt_maxSaveDir.text != "") then
								(
									HandleMax2XBySelFolder obj savePath obj.name
								)
								else
									HandleMax2XBySelFolder obj maxpath obj.name
							)
							else
							(
								if(edt_maxSaveDir.text != "") then
								(
									HandleMax2XByAllFolder obj savePath obj.name
								)
								else
									HandleMax2XByAllFolder obj maxpath obj.name
							)
						)
						pgb_mergeProgress1.value = 100.* i / counts
					)
				)-- if chk_isGroup.checked end
				else--没有勾选组导出
				(
					local j = 0
					local icount = geometry.count
					for geom in geometry do--在max场景中遍历每个物体
					(
						j += 1
						if chk_isDirforMax.checked then
						(
							if(edt_maxSaveDir.text != "") then
							(
								HandleMax2XBySelFolder geom savePath geom.name
							)
							else
								HandleMax2XBySelFolder geom maxpath geom.name
						)
						else
						(
							if(edt_maxSaveDir.text != "") then
							(
								HandleMax2XByAllFolder geom savePath geom.name
							)
							else
								HandleMax2XByAllFolder geom maxpath geom.name
						)
						pgb_mergeProgress1.value = 100.* j / icount
					)
				)--else 不导出组
				pgb_mergeProgress2.value = 100.* k / toExport.count
				resetMaxFile #noPrompt--重置max软件
				num += 1
				format "索引字段：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num m toExport.count
				gc()
			)
			pgb_mergeProgress1.value = 0
			pgb_mergeProgress2.value = 0
			toExport = #()
			MessageBox "MAX文件已分离完成！   " title:"温馨提示"
		)
	)--catch(print "Unknow System Error!")
		
	)
	
---------------------------关于--------------------------------
	
	on btn_About pressed do
	(
		global rt_About
		(
			global Boolmouse = false
			global aPos = [0,0]
			
			Rollout rt_About "读我"width:480 height:280
			(
				
			Group ""
			(
				label lb_About \
				"说明：\n本插件主要功能是分离为单个物体\n
※1.搜索子目录下的max文件。
※2.将场景内的单个物体保存为max文件,并且存放在以物体对象为名的文件夹内（可选）。
※3.如果是组，则将组保存为单个max文件。（可选）
※4.双击文件列表项可打开文件所在目录。
※5.beta1.0测试版本会有bug，欢迎反馈。
※6.正在测试。" \
				pos:[10,15] width:500 height:130
			)
			Group ""
			(
				label lbl3 "麻菜 版权所有 (c) 2011" pos:[8,170] width:100 height:20
				label lbl4 "Author:Yvi.麻菜" pos:[8,185] width:110 height:20
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
			)
		)
			CreateDialog rt_About
	)
	
	on YY_ExpotXFile close do
	(
		if rt_About != undefined do
			DesTroyDialog rt_About
	)
)
createDialog YY_ExpotXFile pos:mouse.screenpos



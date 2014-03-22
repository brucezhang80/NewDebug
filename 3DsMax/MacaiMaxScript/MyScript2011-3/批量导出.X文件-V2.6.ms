--------------
---------function------
/*
--利用dotnet来获取子目录下的文件

fn GetAllFiles TargetPath FileFilter  =
(
	local SearchOption,DotNetDirectoryInfo
	
	SearchOption = DotNetClass "System.IO.SearchOption"
	DotNetDirectoryInfo = DotNetObject "System.IO.DirectoryInfo" TargetPath
	tempUserName = (DotNetClass "System.Environment").UserName
	fileSystemRights = (DotNetClass "System.Security.AccessControl.FileSystemRights").Modify
	accessControlType = (DotNetClass "System.Security.AccessControl.AccessControlType").Allow
	fileSystemAccessRule = DotNetObject "System.Security.AccessControl.FileSystemAccessRule" tempUserName fileSystemRights accessControlType 
	directorySecurity = DotNetDirectoryInfo.GetAccessControl();
	directorySecurity.AddAccessRule fileSystemAccessRule 
	DotNetDirectoryInfo.SetAccessControl directorySecurity 
	for tfi in (DotNetDirectoryInfo.GetFiles FileFilter SearchOption.AllDirectories) collect tfi.FullName
)
*/
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
-- 	if (coordFile == undefined)do
-- 	(
-- 		createFile infile
-- 		coordFile = openfile infile mode:"at"
-- 	)

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
	
fn HandleMax2XBySelFolder obj=
(
    obj_name=obj.name
    filepath = maxfilepath
    makeDir (filepath + obj_name)--在当前处理的max路径下建立以对象物体为名的文件夹
    --输出这个文件成一个.x文件，并保存为以物体为名的文件夹内。
    exportfile (filepath + obj_name + "\\" + obj_name+".x") #noprompt 
)

fn HandleMax2XByAllFolder obj =
(
	filepath = maxfilepath
	obj_name=obj.name
	exportfile (filepath + "\\" + obj_name+".x") #noprompt 
)

/*保存组节点*/
fn Fn_SaveByGroup obj maxpath saveGroupArr= 
(
	local groupArr = #(), s
	--local saveGroupArr = #()
	for i in obj where isGroupHead i do 
	(
		setGroupOpen i false
		append groupArr i --将组节点装入数组
	)
	local j = 0
	for g in groupArr do 
	(
		s = for j in g.children collect j --收集组内字节点
		append s g
		
		group_max = maxpath + "\\" +g.name+ ".max"
		if (not doesFileExist group_max) then
		(
			saveNodes s  group_max quiet:on--保存组名
			append saveGroupArr group_max
		)
		else
		(
			j += 1
			group_max = maxpath + "\\" + g.name+ (j as string)".max"
			saveNodes s  group_max quiet:on
			append saveGroupArr group_max
		)
	)
	return saveGroupArr
)

/*保存除组之外的单物体对象*/
fn Fn_SaveByOneObj obj maxpath saveNodeArr =
(
	local nodeArr = #()
	--local saveNodeArr = #()
	for o in obj where ((not isGroupHead o) and (not isGroupMember o)) do
	(
		append nodeArr o
	)
	
	local i = 0
	for n in nodeArr do
	(
		one_max = maxpath + "\\" + n.name+ ".max"
		if (not doesFileExist one_max) then
		(
			saveNodes n  one_max quiet:on
			append saveNodeArr one_max
		)
		else
		(
			i += 1
			one_max = maxpath + "\\" + n.name+ (i as string)".max"
			saveNodes n  one_max quiet:on
			append saveNodeArr one_max
		)
	)
	return saveNodeArr
)

/*保存每个物体对象*/
fn Fn_SaveBySingleObj obj maxpath saveSingleArr =
(
	local only_max = ""
	only_max = maxpath + "\\" + obj.name + ".max"
	if (not doesFileExist only_max) then
	(
		saveNodes obj only_max quiet:on
		append saveSingleArr only_max
	)
	else
	(
		k += 1
		only_max = maxpath + "\\" + obj.name + (k as string)".max"
		saveNodes obj only_max quiet:on
		append saveSingleArr only_max
	)
	return saveSingleArr
)
-----------------------plug Interface------- 

try(destroydialog YY_ExpotXFile)catch()
rollout YY_ExpotXFile "批量导出.X文件-V2.6 build60" width:260 height:490
(
Group "指定路径"
(
	HyperLink hl_MPath "Max所在路径：" pos:[7,20] width:75 height:14 color:GREEN enabled:false
	editText edt_maxDir "" pos:[4,40] width:180 height:17
	button btn_maxFileDir "打开..." pos:[188,38] width:60 height:20 tooltip:"指定max文件所在路径..."
	
	checkbox chk_saveCoord "是否记录坐标和高度？" pos:[100,61] width:154 height:14 triState:0--checked:true
	HyperLink hl_Cpath "坐标保存路径：" pos:[7,61] width:90 height:14 color:GREEN enabled:false
	editText edt_textDir "" pos:[4,76] width:180 height:17 enabled:false
	button btn_saveFileDir "保存..." pos:[188,76] width:60 height:20 enabled:False tooltip:"指定保存坐标文本文件的路径..."
)	
Group "文件列表"
(
	multilistBox mlist_allMaxFile "所有max文件(含子文件夹）：" pos:[6,125] width:248 height:12 selection:1
	--checkbox chk_noOpacity "导出无透贴.x" pos:[14,281] width:114 height:22 state:true
	--checkbox chk_withOpacity "导出有透贴.x" pos:[130,281] width:114 height:22 state:false
	button btn_selAll "全选" pos:[7,305] width:60 height:18 tooltip:"选择列表中全部文件"
	button btn_Clear "清空" pos:[100,305] width:60 height:18 tooltip:"清空列表中全部文件"
	button btn_selNone "不选" pos:[190,305] width:60 height:18 tooltip:"清除选择列表中的文件"
	
)
Group "导出操作"
(
	checkBox chk_isDirforX "导出.X文件为单独文件夹"	 width:224 height:14 triState:0 tooltip:"导出.x文件会在对应的max文件夹下建立一个保存.x文件的文件夹，通常情况下会勾选此项。"
	HyperLink hl_forX "(不勾选则在max文件夹下)"pos:[26,366] width:254 height:14 color:GREEN enabled:false
	checkBox chk_isGroup "导出组为.X文件" tooltip:"将打组的模型导出一个.x文件"
	HyperLink hl_ForGroup "(不勾选则将组内物体单独导出)"pos:[26,400] color:GREEN enabled:false
	
	button btn_beginExpot "开始导出" pos:[35,420] width:196 height:28 tooltip:"准备就绪后开始导出.X文件"
)
	button btn_About "关于"  pos:[80,460] width:96 height:22 tooltip:"关于 全批量导出.X文件 beta2.3 插件"
	--------------------------values
	global mousedx = false
	global ThePos = [0,0]
	global ex_files = #()
	global outf
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
	on btn_saveFileDir pressed do--保存坐标记录文本文件
	(
		outf = OpenSaveFile()

		print outf
		edt_textDir.text = outf as string
		
		--保存好文本文件后
		--if outf != undefined then
		--(
		--	close outf
		--	free outf
		--)
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
				ex_files = getFilesRecursive enter_path "*.max"
				
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
			if (dir = getSavePath caption:"选择max文件的路径（含子目录）...")!= undefined  and dir.count!=3 then
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
	on btn_beginExpot pressed  do--开始导出.x文件
	(
		local toExport = #()
		--local withOp = #()
		--local nuoneOp = #()
		local singleNodeArr = #()
		
		local groupNodeArr = #()
		local oneNodeArr = #()
		local iCenter
		local height
		local num = 0
		local sel = (mlist_allMaxFile.selection as array)
	try
	(
		oneNodeArr = #()--清空
		groupNodeArr = #()
		singleNodeArr = #()
		
		if chk_saveCoord.checked do--有时候勾选了记录高度复选框但忘记指定保存路径，以致写入出错而浪费时间
		(
			if edt_textDir.text == "" then
			(
				MessageBox "没有指定保存坐标文件，指定保存坐标文件后重试！" title:"错误"beep:true
				return 0
			)
		)
		if(sel.count == 0 )then
		(
			MessageBox "没有可导出的max文件,\n请指定max文件后重试！" title:"提示" beep:true
		)
		else
		(
			for i = 1 to sel.count do 
				append toExport ex_files[sel[i]]

			--开始加载单个max
			for m in toExport do
			(
				loadmaxfile m --#noPrompt--打开一个max文件
				maxpath = maxfilepath
				
				--判断是否有组
				--如果有组，以组导出
				if chk_isGroup.checked then
				(
					for obj in objects do
					(
						/*
						--这里的写法导出的组里是空物体，不能将组的物体保存。
						if (isGroupHead obj) then--如果是组
						(
							node_group_max = maxpath+obj.name+".max"--保存为组名的max文件
							saveNodes obj (node_group_max) quiet:true--保存这个组
							appendIfUnique groupNodeArr node_group_max--appendIfUnique 比append好，但需要max2008以上版本。即当前数组里如果的值不存在返回真那么添加进去。
						)
						else if (not isGroupMember obj) then
						(
							node_one_max = maxpath+obj.name+".max"
							saveNodes obj (node_one_max) quiet:true--单独保存每个物体
							appendIfUnique oneNodeArr node_one_max
						)
						*/
						/*使用下面的方法来保存组，函数在上面定义*/
						groupNodeArr = (Fn_SaveByGroup obj maxpath groupNodeArr)
						oneNodeArr = (Fn_SaveByOneObj obj maxpath oneNodeArr)
					)
					--导出处理
					if groupNodeArr.count == 0 then
					(
						local max_name = filenameFromPath m
						if oneNodeArr.count != 0 then
						(
							format "☆% 没有组对象！但有单独的物体对象☆\n" max_name
						)
						else
						(
							format "☆% 没有任何物体对象，请检查！☆\n" max_name
						)
					)
					else if (groupNodeArr.count > 0) then
					(
						for gp in groupNodeArr do--组对象操作
						(
							loadmaxfile gp
							o = objects[1]
							--如果是组的话，则记录对象坐标高度不是组的高度。而是组里每个对象的高度
							--但数据部要求记录组的坐标和高度，这里就不记录组内子对象的坐标和高度了
							if chk_saveCoord.checked then
							(
								
								--先打开组
								--unGroup o 这个不是打开组，是解组，不好使
								--setGroupOpen o true
								--for sub_obj in o do
								--(
								--记录坐标
									iCenter = o.center--这里是组的中心坐标
									height = o.max.z - o.min.z --组的最高高度
									
									writeCenter2File outf iCenter height m o.name--m是原件max的文件名
								--)
								--再关闭组
								--setGroupOpen o false
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								HandleMax2XBySelFolder o
							)
							else--否则导出X文件为MAX文件夹下
							(
								HandleMax2XByAllFolder o
							)
							--resetMaxFile #noPrompt--重置max软件
							deletefile gp--删除前面单独保存每个物体的max
						)
					)
					if (oneNodeArr.count > 0)then--单对象导出
					(
						for one in oneNodeArr do--单对象操作
						(
							loadmaxfile one
							o = geometry[1]
							
							--如果是组的话，则记录对象坐标高度不是组的高度。而是组里每个对象的高度
							if chk_saveCoord.checked then
							(
								--记录坐标
								iCenter = o.center
								height = o.max.z - o.min.z
								
								writeCenter2File outf iCenter height m o.name
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								HandleMax2XBySelFolder o
							)
							else--否则导出X文件为MAX文件夹下
							(
								HandleMax2XByAllFolder o
							)
							--resetMaxFile #noPrompt--重置max软件
							deletefile one--删除前面单独保存每个物体的max
						)
					)--one objects
					/*在这里再清空以上的数组*/
					groupNodeArr = #()
					oneNodeArr = #()
				)-- if chk_isGroup.checked end
				else--没有勾选组导出
				(
					for geom in geometry do--在max场景中遍历每个物体
					(
						/*
						nodemax = maxpath+geom.name+".max"
						saveNodes geom (nodemax) quiet:true--单独保存每个物体
						appendIfUnique singleNodeArr nodemax
						*/
						/*函数在上面定义*/
						singleNodeArr = (Fn_SaveBySingleObj geom maxpath singleNodeArr)
					)
					if (singleNodeArr.count > 0) then
					(
						for sn in singleNodeArr do
						(
							loadmaxfile sn
							g = geometry[1]
							
							if chk_saveCoord.checked then
							(
								--记录坐标
								iCenter = g.center
								height = g.max.z - g.min.z
								
								writeCenter2File outf iCenter height m g.name
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								HandleMax2XBySelFolder g
							)
							else--否则导出X文件为MAX文件夹下
							(
								HandleMax2XByAllFolder g
							)
							--resetMaxFile #noPrompt--重置max软件
							deletefile sn--删除前面单独保存每个物体的max
							
						)
					)
					else
					(
						local max_name = filenameFromPath m
						format "☆% 没有任何物体对象，请检查！☆\n" max_name
					)
					singleNodeArr = #()--清空数组
				)--else 不导出组
				
				resetMaxFile #noPrompt--重置max软件
				num += 1
				format "检索字段：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num m toExport.count
			)--for load每个max文件。
			toExport = #()
			MessageBox "已处理完成！   " title:"☆温馨提示☆"
		)
	)catch(print "Unknow System Error!")
		
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
				label lb_About \
				"说明：\n本插件主要功能是批量导出.x文件\n
※1.可以搜索子目录下的max文件。
※2.将场景内的单个物体对象导出为.x文件,并且存放在以物体对象为名的文件夹内。
※3.如果是组，则将组导出为单个.x文件。（新增）
※4.对有透贴和无透贴的物体将导出的.x文件分别放入不同的文件夹内(完善中...)。
※5.记录每个物体的X、Y坐标、Z坐标和物体本身高度并保存为文本文件。(新增Z坐标)
※6.稳定版本【10.25修复bug】。" \
				pos:[10,15] width:500 height:130
			)
			Group ""
			(
				label lbl3 "CopyRight (c) 2011" pos:[8,170] width:100 height:20
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
createDialog YY_ExpotXFile pos:mouse.screenpos style:#(#style_minimizebox, #style_titlebar, \
							#style_border, #style_sysmenu,#style_sunkenedge,\
							#style_resizing)

--F:\Yvi\导出.X文件\320802


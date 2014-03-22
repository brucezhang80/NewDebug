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
fn writeCenter2File infile iCenter height fileName objName z_pos= 
(
	coordFile = openfile infile mode:"at"

	s0 = formattedPrint iCenter.x format:"19.14g" 
	s1 = formattedPrint iCenter.y format:"19.14g" 
	s2 = formattedPrint z_pos format:"19.14g"
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
--------------------------------------
fn HandleMax2XBySelFolder obj=
(
    obj_name=obj.name
    filepath = maxfilepath
    makeDir (filepath + obj_name)--在当前处理的max路径下建立以对象物体为名的文件夹
    --输出这个文件成一个.x文件，并保存为以物体为名的文件夹内。
    exportfile (filepath + obj_name + "\\" + obj_name+".x") #noprompt selectedOnly:true
)

fn HandleMax2XByAllFolder obj =
(
	filepath = maxfilepath
	obj_name=obj.name
	exportfile (filepath + "\\" + obj_name+".x") #noprompt selectedOnly:true
)
---------------------------------

/*保存组节点*/
/*保存组节点,传一个组对象*/
fn Fn_SaveByGroup GroupObj = 
(
	local groupArr = #(), grp

	setGroupOpen GroupObj false
	append groupArr GroupObj --将组节点装入数组

	local j = 0
	for g in groupArr do 
	(
		grp = for j in g.children collect j --收集组内子节点
		append grp g
	)
	return grp
)

fn Fn_SaveByGroupChild GroupObj = 
(
	local groupArr = #(), gchild

	setGroupOpen GroupObj true
	append groupArr GroupObj --将组节点装入数组

	gchild = for j in GroupObj.children collect j --收集组内子节点
		
	return gchild
)
--移动物体到原点
fn moveObjectToZero obj = 
(
	center = obj.center
	minz   = obj.min.z
	move obj [-center.x,-center.y,-minz]
	mpos = [-center.x,-center.y,-minz]
	return mpos
)

--轴心的Z轴等于物体最底点
fn Fn_setPivotToButtom obj =
(
	if (isGroupHead obj) then
	(
		setGroupOpen obj true
		obj.pivot.z = obj.min.z
		setGroupOpen obj false
	)
	else if(not isGroupMember obj)then
	(
		obj.pivot.z = obj.min.z  
	)
	else
	(
		obj.pivot.z = obj.min.z
	)
)

--附加一个box进去。解决轴重置时飘移的问题，（场景内物体不做移动不使用此函数）
fn AttachToAbox obj=
(
	curname = obj[1].name
	mybox = box()
	mymesh = converttomesh mybox
		
	for m in obj do
	(
		meshop.attach mymesh m
	)
	mymesh.name = curname
	clearselection()
	for i = 1 to 12 do 
	(
		deleteface mymesh 1
	)
)
-----------------------plug Interface------- 

try(destroydialog YY_ExpotXFile)catch()
rollout YY_ExpotXFile "批量导出.X文件-V2.8" width:260 height:600
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
	checkBox chk_isDirforX "是否导出.X文件为单独文件夹？"width:224 height:14 triState:0 tooltip:"导出.x文件会在对应的max文件夹下建立一个保存.x文件的文件夹，通常情况下会勾选此项。"
	HyperLink hl_forX "(不勾选则在max文件夹下)"pos:[26,366] width:254 height:14 color:GREEN enabled:false
	checkBox chk_isGroup "是否导出组为.X文件？" tooltip:"将打组的模型导出一个.x文件"
	HyperLink hl_ForGroup "(不勾选则将组内物体单独导出)"pos:[26,400] color:GREEN enabled:false
	
	checkBox chk_isPivotCneter "是否将物体轴归中？"	 width:224 height:14 checked:true tooltip:"将场景内单个物体或组的轴归中！通常情况下需要勾选！"
	HyperLink hl_forPivotCter "(不勾选则不归中)"pos:[26,436] width:254 height:14 color:RED enabled:false
	
	checkBox chk_isPivotBottom "是否将物体轴归底？"	 width:224 height:14 checked:true tooltip:"将场景内单个物体或组的轴归底！通常情况下会勾选！"
	HyperLink hl_forPivotBott "(不勾选则不归底)"pos:[26,468] width:254 height:14 color:RED enabled:false
	
	checkBox chk_isMoveZero "是否将物体归原点再导出.x？"	 width:224 height:14 checked:false tooltip:"将场景内单个物体或组移到原点再导出.x文件！"
	HyperLink hl_forMoveZero "(不勾选则原位导出)"pos:[26,505] width:254 height:14 color:RED enabled:false
	
	CheckButton chb_OtherSetting "展开其它功能>>" pos:[150,518]
	button btn_beginExpot "开始导出" pos:[35,540] width:196 height:28 tooltip:"准备就绪后开始导出.X文件"
)
	button btn_About "帮助..."  pos:[80,577] width:96 height:22 tooltip:"关于 全批量导出.X文件 beta2.3 插件"

	groupBox gb_bmp "导出贴图设置" pos:[270,6] width:200 height:50
	CheckBox chb_OptMapExport "导出透贴" pos:[280, 30]
	CheckBox chb_NoOptMapExport "导出无透贴" pos:[380, 30]
	HyperLink hlk_no "这版的功能尚未开放！" pos:[330, 80] color:GREEN enabled:false
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
		local selArr = #()

		local iCenter
		local height
		local num = 0
		local sel = (mlist_allMaxFile.selection as array)
	try
	(
		--清空数组
		selArr = #()
		toExport = #()
		
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
					max select all
					selArr = selection as array
					clearselection()
					local gObj
					
					for i = 1 to selArr.count do
					(
						sel = selArr[i]
						select sel
						if (isGroupHead sel) then
						(
							gObj = Fn_SaveByGroup sel--为了能选择组
							select gObj
							
							--如果是组的话，则记录对象坐标高度不是组的高度。而是组里每个对象的高度
							--但数据部要求记录组的坐标和高度，这里就不记录组内子对象的坐标和高度了
							if chk_saveCoord.checked then
							(
								iCenter = sel.center--这里是组的中心坐标
								height = sel.max.z - sel.min.z --组的最高高度
								z_pos = sel.min.z
								writeCenter2File outf iCenter height m sel.name z_pos--m是原件max的文件名
							)
							if chk_isPivotCneter.checked then--轴归中
							(
								CenterPivot sel  --轴心归中心
								ResetxForm sel
							)
							if chk_isPivotBottom.checked then--轴归底
							(
								Fn_setPivotToButtom sel  --轴心的Z轴等于物体最底点
								ResetxForm sel
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								oldPos = sel.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero sel
									HandleMax2XBySelFolder sel
									--导完后再移回原处
									sel.pos = oldPos
								)
								else
								(
									HandleMax2XBySelFolder sel
								)
							)
							else--否则导出X文件为MAX文件夹下
							(
								oldPos = sel.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero sel
									HandleMax2XByAllFolder sel
									--导完后再移回原处
									sel.pos = oldPos
								)
								else
								(
									HandleMax2XByAllFolder sel
								)
							)
							
						)
						else if (not isGroupMember sel)then
						(
							--如果是组的话，则记录对象坐标高度不是组的高度。而是组里每个对象的高度
							--但数据部要求记录组的坐标和高度，这里就不记录组内子对象的坐标和高度了
							if chk_saveCoord.checked then
							(
								iCenter = sel.center--这里是组的中心坐标
								height = sel.max.z - sel.min.z --组的最高高度
								z_pos = sel.min.z
								writeCenter2File outf iCenter height m sel.name z_pos--m是原件max的文件名
							)
							if chk_isPivotCneter.checked then--轴归中
							(
								CenterPivot sel  --轴心归中心
								ResetxForm sel
							)
							if chk_isPivotBottom.checked then--轴归底
							(
								Fn_setPivotToButtom sel  --轴心的Z轴等于物体最底点
								ResetxForm sel
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								oldPos = sel.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero sel
									HandleMax2XBySelFolder sel
									--导完后再移回原处
									sel.pos = oldPos
								)
								else
								(
									HandleMax2XByAllFolder sel
								)
							)
							else--否则导出X文件为MAX文件夹下
							(
								oldPos = sel.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero sel
									HandleMax2XByAllFolder sel
									--导完后再移回原处
									sel.pos = oldPos
								)
								else
								(
									HandleMax2XByAllFolder sel
								)
							)
						)
					)
					selArr = #()
				)-- if chk_isGroup.checked end
				else--没有勾选组导出
				(
					for obj in objects do
					(
						select obj
						if (isGroupHead obj)then
						(
							local childArr = #()
							childArr = Fn_SaveByGroupChild obj
							clearselection()
							for ch in childArr do
							(
								select ch
								if chk_saveCoord.checked then
								(
									iCenter = ch.center--这里是组的中心坐标
									height = ch.max.z - ch.min.z --组的最高高度
									z_pos = ch.min.z
									writeCenter2File outf iCenter height m ch.name z_pos--m是原件max的文件名
								)
								if chk_isPivotCneter.checked then--轴归中
								(
									CenterPivot ch  --轴心归中心
									ResetxForm ch
								)
								if chk_isPivotBottom.checked then--轴归底
								(
									Fn_setPivotToButtom ch  --轴心的Z轴等于物体最底点
									ResetxForm ch
								)
								--开始导出.x文件
								if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
								(
									oldPos = ch.pos
									if chk_isMoveZero.checked then
									(
										--导出前先移动物体到原点
										moveObjectToZero ch
										HandleMax2XBySelFolder ch
										--导完后再移回原处
										ch.pos = oldPos
									)
									else
									(
										HandleMax2XByAllFolder ch
									)
								)
								else--否则导出X文件为MAX文件夹下
								(
									oldPos = ch.pos
									if chk_isMoveZero.checked then
									(
									--导出前先移动物体到原点
									moveObjectToZero ch
									HandleMax2XByAllFolder ch
									--导完后再移回原处
									ch.pos = oldPos
									)
									else
									(
										HandleMax2XByAllFolder ch
									)
								)
								clearselection()
							)--for
						)--if isGroupHead
						else if ((not isGroupHead obj) and (not isGroupMember obj))then
						(
							--如果是组的话，则记录对象坐标高度不是组的高度。而是组里每个对象的高度
							--但数据部要求记录组的坐标和高度，这里就不记录组内子对象的坐标和高度了
							if chk_saveCoord.checked then
							(
								iCenter = obj.center--这里是组的中心坐标
								height = obj.max.z - obj.min.z --组的最高高度
								z_pos = obj.min.z
								writeCenter2File outf iCenter height m obj.name z_pos--m是原件max的文件名
							)
							if chk_isPivotCneter.checked then--轴归中
							(
								CenterPivot obj  --轴心归中心
								ResetxForm obj
							)
							if chk_isPivotBottom.checked then--轴归底
							(
								Fn_setPivotToButtom obj  --轴心的Z轴等于物体最底点
								ResetxForm obj
							)
							--开始导出.x文件
							if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
							(
								oldPos = obj.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero obj
									HandleMax2XBySelFolder obj
									--导完后再移回原处
									obj.pos = oldPos
								)
								else
								(
									HandleMax2XByAllFolder obj
								)
							)
							else--否则导出X文件为MAX文件夹下
							(
								oldPos = obj.pos
								if chk_isMoveZero.checked then
								(
									--导出前先移动物体到原点
									moveObjectToZero obj
									HandleMax2XByAllFolder obj
									--导完后再移回原处
									obj.pos = oldPos
								)
								else
								(
									HandleMax2XByAllFolder obj
								)
							)
						)
					)
				)--else 不导出组
				--不改变max场景物体
				saveMaxfile (maxFilePath+maxFileName)
				resetMaxFile #noPrompt--重置max软件
				num += 1
				format "检索字段：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num m toExport.count
				gc()--回收垃圾
				freeSceneBitmaps()--清贴图内存
				clearUndoBuffer()--清撤销内存
			)--for load每个max文件。
			gc()
			--freeSceneBitmaps()--清贴图内存
			--clearUndoBuffer()--清撤销内存
			toExport = #()
			MessageBox "已处理完成！   " title:"☆温馨提示☆"
		)
	)catch(print "Unknow System Error!")
		
	)--导出按钮
	
	on chb_OtherSetting changed theState do
	(
		if theState then
		(
			for h = YY_ExpotXFile.width to 520 by 5 do
			(
				YY_ExpotXFile.width = h
				--sleep 0.002
			)
			chb_OtherSetting.text = "收起其它功能<<"
			
		)
		else
		(
			for h = YY_ExpotXFile.width to 260 by -5 do
			(
				YY_ExpotXFile.width = h
				--sleep 0.002
			)
			chb_OtherSetting.text = "展开其它功能>>"
		)
	)
	
---------------------------关于--------------------------------
	
	on btn_About pressed do
	(
		global rt_About
		(
			global Boolmouse = false
			global aPos = [0,0]
			
			Rollout rt_About "帮助"width:480 height:300
			(
				
			Group ""
			(
				label lb_About \
				"\t\t\t\t\t说明：\n\t\t\t本插件主要功能是批量导出.x文件
※1.可以搜索子目录下的max文件。
※2.将场景内的单个物体对象导出为.x文件,并且存放在以物体对象为名的文件夹内。
※3.如果是组，则将组导出为单个.x文件。
※4.对有透贴和无透贴的物体将导出的.x文件分别放入不同的文件夹内(等待开发...)。
※5.记录每个物体的X、Y坐标、Z坐标和物体本身高度并保存为文本文件。
※6.对物体轴归中归底，对于组，只对组内子物体轴归底，修正轴重置问题。
※7.稳定版本【10.27修复bug】。
修复的bug：
①优化了导出算法，速度比之前快，可以节省一半时间。
②导出前移动物体到原点，解决坐标太远.x文件看不到物体(不改变场景物体原本位置)。" \
				pos:[10,15] width:500 height:160
			)
			Group ""
			(
				label lbl3 "CopyRight (c) 2011" pos:[8,195] width:100 height:20
				label lbl4 "Author:Yvi" pos:[8,210] width:110 height:20
				label lbl5 "Site:" pos:[8,225] width:110 height:20
				HyperLink hl_site "Click Here!" pos:[40,225] color:GREEN  hoverColor:RED visitedColor:BLUE address:"http://www.macai.co.cc"
				label lbl6 "Emial:12319597@qq.com" pos:[8,240] width:150 height:20
			)	
				Button btn_Close "确定" pos:[218,270] width:50 height:25

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


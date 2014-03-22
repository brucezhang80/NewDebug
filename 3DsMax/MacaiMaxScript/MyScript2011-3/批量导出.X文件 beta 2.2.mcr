--------------
---------function------
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
	
	s2 = formattedPrint height format:"19.14g" 
	
	--s3 = filenameFromPath fileName 
	
	str = fileName + "-> (物体名)" + objName + " (X)" + s0  + " (Y)" + s1 +" (H)"+ s2
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
	
fn HandleMax2XBySelFold name =
(
    obj_name=name
    filepath = maxfilepath
    makeDir (filepath + obj_name)--在当前处理的max路径下建立以对象物体为名的文件夹
    --输出这个文件成一个.x文件，并保存为以物体为名的文件夹内。
    exportfile (filepath + obj_name + "\\" + obj_name+".x") #noprompt 
)

fn HandleMax2XByAllFold name =
(
	filepath = maxfilepath
	obj_name=name
	exportfile (filepath + "\\" + obj_name+".x") #noprompt 
)
-----------------------plug Interface------- 

try(destroydialog YY_ExpotXFile)catch()
rollout YY_ExpotXFile "全批量导出.X文件 beta2.2 build42" width:260 height:480
(
	label lb_1 "Max所在路径：" pos:[3,4] width:82 height:14
	editText edt_maxDir "" pos:[0,20] width:180 height:17
	button btn_maxFileDir "打开..." pos:[182,20] width:68 height:18
	
	checkbox chk_saveCoord "是否记录坐标和高度？" pos:[100,41] width:154 height:14 triState:0--checked:true
	label lb_2 "坐标记录路径：" pos:[3,41] width:90 height:14
	editText edt_textDir "" pos:[0,56] width:180 height:17 enabled:false
	button btn_saveFileDir "保存..." pos:[182,56] width:68 height:18 enabled:False
	
	multilistBox mlist_allMaxFile "所有max文件(含子文件夹）：" pos:[4,75] width:253 height:12 selection:1
	--checkbox chk_noOpacity "导出无透贴.x" pos:[14,281] width:114 height:22 state:true
	--checkbox chk_withOpacity "导出有透贴.x" pos:[130,281] width:114 height:22 state:false
	label lb_3 \
			"说明：\n本插件主要功能是批量导出.x文件\n※1.可以搜索子目录下的max文件。\n※2.将场景内的单个物体对象导出为.x文件,并且存放在以物体对象为名的文件夹内。\n※3.对有透贴和无透贴的物体将导出的.x文件分别放入不同的文件夹内(完善中...)。\n※4.同时记录每个物体的X、Y坐标和物体本身高度并保存为文本文件。\n※5.正在测试【8.30新增功能】。（麻菜出品）" \
			pos:[8,344] width:257 height:200
	
	button btn_selAll "全选" pos:[3,258] width:80 height:18
	button btn_selNone "不选" pos:[187,258] width:80 height:18
	
	checkBox chk_isDirforX "为每个.X文件导出单独文件夹"	pos:[6,280] width:254 height:14 triState:0
	label lb_forX "（不勾选则在max文件夹下)"pos:[30,296] width:254 height:14
	
	button btn_beginExpot "开始导出" pos:[35,318] width:196 height:28
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
				messagebox "路径不能为 根目录！"
				--edt_maxDir.text = ""
			)
		)
		print ch
		print lb
		try
		(
			if ( ((edt_maxDir.text) != undefined) and (ch != undefined) and (lb != undefined) and edt_maxDir.text.count != 3)then
			(
				if (edt_maxDir.text.count) > 28 then edt_maxDir.text = "..."+(substring edt_maxDir.text (edt_maxDir.text.count-28) edt_maxDir.text.count) 
				else edt_maxDir.text = edt_maxDir.text

				--获取max文件，保存在一个数组里面
				ex_files = GetAllFiles edt_maxDir.text "*.max"
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
			if (dir = getSavePath caption:"选择max文件的路径（含子目录）...")!= undefined  and dir.count!=3 then
			(
				if (dir.count) > 28 then edt_maxDir.text = "..."+(substring dir (dir.count-28) dir.count) 
				else edt_maxDir.text = dir
				--获取max文件，保存在一个数组里面
				ex_files = GetAllFiles dir "*.max"
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
------------
	
	on mlist_allMaxFile selected sel do
	(
		
	)
	
	on btn_beginExpot pressed  do--开始导出.x文件
	(
		local toExport = #()
		--local withOp = #()
		--local nuoneOp = #()
		local singleNode = #()
		local iCenter
		local height
		
		local sel = (mlist_allMaxFile.selection as array)
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
				maxname = maxfilename
				
-- 				for geom in geometry do--在max场景中遍历每个物体
-- 				(
-- 					nodemax = maxpath+geom.name+".max"
-- 					saveNodes geom (nodemax) quiet:true--单独保存每个物体
-- 					append singleNode nodemax
-- 				)
-- 				if singleNode.count > 1 then
-- 				(
-- 					for sn in singleNode do
					(
-- 						loadmaxfile sn
-- 						g = geometry[1]
						
-- 						if chk_saveCoord.checked then
-- 						(
-- 							--记录坐标
-- 							iCenter = g.center
-- 							height = g.max.z - g.min.z
-- 							
-- 							writeCenter2File outf iCenter height m g.name
-- 						)
						--开始导出.x文件
						if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
						(
							HandleMax2XBySelFold maxname
						)
						else--否则导出X文件为MAX文件夹下
						(
							HandleMax2XByAllFold maxname
						)
						--resetMaxFile #noPrompt--重置max软件
-- 						deletefile sn--删除前面单独保存每个物体的max
					)
-- 				)
-- 				else
-- 				(
-- 					loadmaxfile singleNode[1]
-- 					g = geometry[1]
-- 					
-- 					if chk_saveCoord.checked then
-- 					(
-- 						--记录坐标
-- 						iCenter = g.center
-- 						height = g.max.z - g.min.z
-- 						
-- 						writeCenter2File outf iCenter height m g.name
-- 					)
-- 					
-- 					--开始导出.x文件
-- 					if chk_isDirforX.checked then--勾选后导出每个x文件为单独文件夹
-- 					(
-- 						HandleMax2XBySelFold g
-- 					)
-- 					else
-- 					(
-- 						HandleMax2XByAllFold g
-- 					)
-- 					--resetMaxFile #noPrompt--重置max软件
-- 					--deletefile sn--如果max本身只有一个物体，则不要删除这个max
-- 				)
				resetMaxFile #noPrompt--重置max软件
			)
			
			MessageBox "已处理完成！"
		)
		
	)
)
createDialog YY_ExpotXFile pos:mouse.screenpos

--F:\Yvi\导出.X文件\320802


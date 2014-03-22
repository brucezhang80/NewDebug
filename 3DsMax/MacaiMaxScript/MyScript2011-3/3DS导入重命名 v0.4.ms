	/*
		2012.2.22刘海洋的要求,修改脚本
		要修改的地方在下面有注释说明.
	*/
	
/*
	2012.3.1 
	为 模型名称最末尾添加 "_L1"
*/

	--定义全局的数组，用来保存文件名字
	global g_oldfilenames = #()
	global g_newfilenames = #()
	--global g_tilefilename = "" 
	
	global g_arr_mapFileName = #() --读文件获得每行文件名
	global g_arr_oldAndNewfilenames = #() -- 从行中获得新文件名和老文件名
---------------------------------------------------------------	

struct s_oldAndNewFilename(old_MapName, new_MapName, new_simpleName) --绑定 新老文件名

--getFilesRecursive 路径 文件类型如："*.max"
function getFilesRecursive root ext =
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

------------------------------------export
function Fn_Export3Ds savedir=
(
	theClasses = exporterPlugin.classes
	titlename = getFilenameFile maxFileName
	if titlename == undefined or titlename == "" then
	(
		titlename = "Unknow_MaxFileName"
	)
	exportfile (savedir + "\\" + titlename + ".3ds") #noprompt  using:D_StudioExporterPlugin selectedOnly:true 
)
function Fn_setNewObjNameEx obj =
(
	local exOldName = obj.name
	-- 去掉 "_jz"
	local exNewName = substituteString exOldName "_jz" ""
	obj.name = exNewName
	return exNewName
)

-------------------------------import
function Fn_Import3Ds fullname = 
(
	theClasses = importerPlugin.classes
	importFile fullname #noPrompt using:theClasses[1]
)

function Fn_setNewObjNameIm obj =
(
	local imOldName = obj.name
	-- 加上"_jz"
	if imOldName.count < 6 do
	(
		return 0
	)
	local imNewName = replace imOldName 6 0 "_jz"
	obj.name = imNewName
	return imNewName
)

-----------导入时,模型名称后再添加 "_L1"
function Yvi_addLoneToObjName _objname = 
(
	_newobjname = _objname + "_L1"
	return _newobjname
)

-- 打开配置文件
function Fn_openConfigefile =
(
		
	try
	(
		histPath = GetINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Configepath")
		if histPath == "" or histPath == undefined do
		(
			histPath = GetDir #scene
		)
		in_configefile= getOpenFileName caption:"打开配置文件..." types:"配置文件(*.TXT)|*.txt|All|*.*|" initialDir:histPath
		if in_configefile == undefined then
		(
			return 0
		)
		setINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Configepath") (getFilenamePath in_configefile)
	)catch( Messagebox "打开文件失败！")
	
	return in_configefile
)

-- 读配置文件
function Fn_readConfigFile filename =
(
	if filename == undefined or filename == 0 then
	(
		return 0
	)
	mapName = openFile filename
	if (mapName == undefined )then return 0
	while not(eof mapName) do
	(
		iline = readLine mapName
		append g_arr_mapFileName iline
	)
	close mapName
)
-- 得到配置文件内容并处理
function Fn_getOldAndNewMapName = 
(
	if g_arr_mapFileName.count == 0 then
	(
		format "无法读取配置文件!"
		return 0
	)
	for i = 1 to g_arr_mapFileName.count do 
	(
		local l_arrLineName = #()
		l_arrLineName = filterString g_arr_mapFileName[i] ","
		-- 2012.2.22,真烦,早就对他说过配置文件就一个旧名字一个新名字就可以了,他非得要加了什么 绝对路径,这回又不要绝对路径了,
		--这里.刘海洋那边的配置文件改过了,这里不需要对带有"\"的名字进行处理
		--2012.2.22
		--配置文件格式 : nam1,name2
		-- 将name 替换 name1
		
		--2012.2.23
		-- 日.也真是无聊,今天又改了,
		--绝对路径还是要,还要把他们那边程序生成出来的自带贴图名字要以绝对路径为名改贴图SE名.真蛋疼.
		l_arrnewLineName = filterString l_arrLineName[2] "\\"
		
		map_name = l_arrnewLineName[l_arrnewLineName.count]
		_name = (filterString map_name ".")[1] as integer
		_ext = (filterString map_name ".")[2]
		if(_name != undefined)then --如果是整形
		(
			map_name = l_arrnewLineName[l_arrnewLineName.count-1] + "_" + _name as string + "." +_ext
		)
		oldNewMapName = s_oldAndNewFilename old_MapName:l_arrLineName[1] new_MapName:l_arrLineName[2] new_simpleName:map_name
		
		--oldNewMapName = s_oldAndNewFilename old_MapName:l_arrLineName[1] new_MapName:l_arrLineName[2]
		append g_arr_oldAndNewfilenames oldNewMapName
	)--end for
	--print g_arr_oldAndNewfilenames
	--return g_arr_oldAndNewfilenames
)

--获取子目录文件
--getFilesRecursive 路径 文件类型如："*.max"
function Fn_getFilesRecursive root filename =
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
		join all_Files (getFiles (f + filename))
	)
	join all_Files (getFiles (root + filename))

	return all_Files
)--函数结束


-- 重命名贴图
function Fn_handleAllSubMats obj root =
(
	local arr_resFiles =#()
	--这里清除全局数组
	g_oldfilenames=#()
	g_newfilenames=#()
	
	mat =  obj.Material
	if mat == undefined do return 0
	if classof mat == Multimaterial then
	(
		for i = 1 to mat.numsubs do
		(
			-- 修改以收图文件名来命名
			--old_name = mat.materialList[i].name
			old_diffname = mat.materialList[i].diffusemap.filename -- 贴图文件名
			--print ("old_diffname: "+ old_diffname)
			ext = getfilenametype old_diffname

			/*
			仅考虑贴图文件在max文件当前目录下,不考虑贴图文件在子目录下
			--arr_resFiles = Fn_getFilesRecursive root old_diffname -- 只有一个文件如果文件名唯一
			--print ("arr_resFiles.count:"+arr_resFiles.count as string)
			
			--if arr_resFiles.count == 0 do 
			--(
			--	print ("找不到贴图文件:"+ old_diffname)
			--	continue
			--)
			--oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
			*/
			
			for j = 1 to g_arr_oldAndNewfilenames.count do
			(
				if (old_diffname == g_arr_oldAndNewfilenames[j].old_MapName) then
				(
					--不考虑贴图文件放在子目录,直接在max当前目录下
					oldfullpathfile = root + old_diffname
					append g_oldfilenames oldfullpathfile
					--print ("oldfullpathfile: "+ oldfullpathfile)
					--newfullpathfile = (getFilenamePath arr_resFiles[1]) + g_arr_oldAndNewfilenames[j].new_simpleName
					newfullpathfile = root + g_arr_oldAndNewfilenames[j].new_simpleName
					append g_newfilenames newfullpathfile
					newfilename = getFilenameFile newfullpathfile
					
					--print ("newfullpathfile: "+ newfullpathfile)
					mat.materialList[i].name = newfilename
					mat.materialList[i].diffuseMap.name = newfilename
					mat.materialList[i].diffuseMap.fileName = newfullpathfile
				)
			)

			try
			(
				if(mat.materialList[i].opacityMap != undefined)then
				(
					mat.materialList[i].opacityMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].opacityMap.name = mat.materialList[i].diffuseMap.name
				)
				if(mat.materialList[i].SelfIllumMap != undefined)then
				(
					mat.materialList[i].SelfIllumMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].SelfIllumMap.name = mat.materialList[i].diffuseMap.name
				)
			)catch(format "none OpacityMap or SelfIllumMap...")
			
			--arr_resFiles =#()
		)
		--print ("g_oldfilenames:" + g_oldfilenames as string)
		--print ("g_newfilenames:" + g_newfilenames as string)
	)
	if (classof mat == Standard)then
	(
		try
		(
			MessageBox "暂不考虑标准材质!"
			return 0
		)catch()
	)
	--g_arr_oldAndNewfilenames = #()
)
----

-- 硬盘中的贴图文件名
function Fn_HandleHardDiskFiles =
(
	try
	(
		for i=1 to g_oldfilenames.count do
		(
			old  = g_oldfilenames[i]
			new  = g_newfilenames[i]
			
			renameFile old  new
		)
	)catch()
)

-------------------------------------------------------------------------------------
----------------------------------UI-------------------------------------------------
-------------------------------------------------------------------------------------
global rl_expAndimp
try(DestroyDialog rl_expAndimp)catch()
Rollout rl_expAndimp "导入/导出 .3DS" height:280--width:250
(	
	Group "导入 3ds(※)"
	(
		--Button btn_impPath "Open import 3ds file" width:200 tooltip:btn_impPath.text
		label lb_info "根据材质的配置文件的贴图名来重新命\n名贴图文件名和材质名.\n(注意中文目录可能出问题,详情查看\"帮助\")" align:#left height:40
		label lb_0x123n ""
		--Hyperlink hlk_0x1232n "<<显示贴图请先隐藏部分,以免MAX崩溃>>" color:red enabled:false
		CheckBox ckb_addLoneToObjname "模型名末尾添加\"_L1\""
		Button btn_openConfigfile "打开配置文件.."tooltip:btn_openConfigfile.text width:154 height:26
		Button btn_imp "打开导入3ds文件" width:194 height:30 tooltip:btn_imp.text enabled:false
		Button btn_showMap "显示贴图" width:194 height:30
	)
	Group "导出 3ds"
	(
		Button btn_getMaxFiles "指定max文件路径..." width:200 tooltip:"转换max文件到3ds文件请指定max文件的文件夹目录"
		Button btn_expPath "导出到路径..." width:200 tooltip:btn_expPath.text 
		CheckBox ckb_expCurMaxpath "export to current max file path." tooltip:ckb_expCurMaxpath.text
		
		Button btn_exp "Export 3ds" enabled:false
		progressBar pgb_Progress1 ""  width:194 height:8 color:Green
		label lb_num "00%" pos:[208,pgb_Progress1.pos.y-2]
	)
	MultiListBox mlb_maxFiles "导出到3ds的max文件:(双击定向目标)" height:-1 tooltip:"双击指向文件目标" align:#left--pos:[255,10] width:140 height:12 
------------------------------------------------------------------
global g_exp_files = #()
------------------------------------------------------------------
	on rl_expAndimp open do
	(
		global rl_h = rl_expAndimp.height
	)
	
	-- 打开配置文件
	on btn_openConfigfile pressed do
	(
		configefile = Fn_openConfigefile()
		Fn_readConfigFile configefile
		Fn_getOldAndNewMapName()
		print configefile
		if  configefile != 0 then
		(
			btn_imp.enabled = true
		)
	)
	--得到目录中的max文件,含子目录下的max文件
	on btn_getMaxFiles pressed do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("MaxFilePath")

			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #scene
			)
		
			if (dir = getSavePath caption:"选择max文件的路径（含子目录）..." initialDir:histPath)!= undefined  and dir.count!=3 then
			(
				if (dir.count) > 28 then btn_getMaxFiles.text = "..."+(substring dir (dir.count-28) dir.count) 
				else btn_getMaxFiles.text = dir
				setINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("MaxFilePath") dir
				--获取max文件，保存在一个数组里面，保存前先清空数组
				g_exp_files = #()
				--ex_files = GetAllFiles dir "*.max"
				g_exp_files = getFilesRecursive dir "*.max"

				--从max数组里收集max文件到列表里
				mlb_maxFiles.items = for f in g_exp_files collect (filenameFromPath f)
				mlb_maxFiles.height = mlb_maxFiles.items.count * 15
				rl_expAndimp.height = rl_h+(mlb_maxFiles.height+15)
				floatexportAndimport.size.y = 360
			)
			else if dir.count == 3 do messagebox "路径不能为根目录。请重试"
		)
		catch()
	)
	--双击列表项打开文件所在目录
	on mlb_maxFiles doubleClicked item do
	(
		local item_path = ""
		local item_name = ""
		local f_name = ""
		
		if g_exp_files.count == 0 do
			return 0
			
		item_name = mlb_maxFiles.items[item]--得到双击项名
		num = mlb_maxFiles.selection as array
		
		--for num = 1 to ex_files.count do
		(
			f_name = filenameFromPath g_exp_files[num[1]] --得到文件
			--print ex_files[num[1]]
			if item_name == f_name then
			(
				
				item_path = getFilenamePath g_exp_files[num[1]]
				--cmd = "start" + " " + item_path
				--DOSCommand cmd
				ShellLaunch item_path ""
				--exit
			)
			--else
				--continue
		)
	)
	on mlb_maxFiles rightClick do
	(
		if mlb_maxFiles != undefined then
		(
			if mlb_maxFiles.items.count != 0 then
			(
				if mlb_maxFiles.height > 10 then
				(
					for i = mlb_maxFiles.height to 0 by -5 do
					(
						sleep 0.01
						mlb_maxFiles.height = i
					)
					rl_expAndimp.height = rl_h + 5
				)
			)
		)
		print "OK"
	)

	
	--指定导出3ds的路径
	on btn_expPath pressed do
	(
		histPath = GetINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Exportpath")

		if histPath == "" or histPath == undefined do
		(
			histPath = GetDir #scene
		)
		
		dir = getSavePath caption:"选择保存导出的3ds文件路径:"  initialDir:histPath
		if dir == undefined or dir == "" then
		(
			return 0
		)
		btn_expPath.text = dir
		btn_expPath.tooltip = btn_expPath.text
		btn_exp.enabled = true
		
		setINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Exportpath") dir
	)
	
	--是否导出到当前max文件目录下
	on ckb_expCurMaxpath changed state do
	(
		if ckb_expCurMaxpath.checked then
		(
			btn_expPath.enabled = false
			btn_exp.enabled = true
		)
		else
		(
			btn_expPath.enabled = true
			if btn_expPath.text == "导出到路径..." then
			(
				btn_exp.enabled = false
			)
			else if btn_expPath.text != ""then
			(
				btn_exp.enabled = true
			)
		)
	)
------------------------------------------------------------------------
--------------------------导出------------------------------------------
	on btn_exp pressed do
	(
		arr_oldName = #()
		local j = 0
		local planfile
		try
		(
			for ef in g_exp_files do
			(
				loadmaxfile ef
				max select all
				sel = selection as array
	-------------保存到文本
				if ckb_expCurMaxpath.checked then
				(
					planfile = Fn_createTxtFile maxFilepath ((getFilenameFile maxfilename)+"JzbmMap")
				)
				else
				(
					planfile = Fn_createTxtFile btn_expPath.text ((getFilenameFile maxfilename)+"JzbmMap")
				)
				for obj in sel do
				(
					oldName = obj.name
					if oldName == undefined or oldName == "" do continue
					append arr_oldName oldName
					--设置新名字
					newName = Fn_setNewObjNameEx obj
					
					Fn_writeOldAndNewObjName newName oldName planfile
				)
				---导出操作
				if ckb_expCurMaxpath.checked then
				(
					Fn_Export3Ds maxFilepath
				)
				else
				(
					Fn_Export3Ds btn_expPath.text
				)
				
				--recover obj name
				local i = 0
				for obj in sel do
				(
					i += 1
					obj.name = arr_oldName[i]
				)
				pgb_Progress1.value = 100.0 * (j += 1) / g_exp_files.count
				lb_num.text = (pgb_Progress1.value  as string) + "%"
				arr_oldName =#()
				sel = #()
				
				gc()
				freeSceneBitmaps()
				saveMaxfile (maxFilePath+maxFileName)
				resetMaxFile #noPrompt
			)
		)catch()
		g_exp_files = #()
		
		
	)
------------------------------------------------------------------------	
----------------------------------Import file---------------------------
	on btn_imp pressed do
	(
		histPath = GetINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Importpath")
		print histPath
		if histPath == "" or histPath == undefined do
		(
			histPath = GetDir #scene
		)
		f = getOpenFileName caption:"导入一个3ds文件:" types:"3D Studio(*.3DS)|*.3DS|ALL(*.*)|*.*|" historyCategory:"Importpath" --filename:(histPath+"*.3ds")
		
		if f == undefined then 
		(
			return 0
		)
		root = getFilenamePath f
		print ("root:" + root)
		setINISetting ((GetDir #scripts+"\\")+"ExpImp3ds.ini") ("Directories") ("Importpath") (getFilenamePath f)
		resetMaxFile #noPrompt
		Fn_Import3Ds f
		
		max select all
		sel = selection as array
		
	--	im_planfile = Fn_createTxtFile  (getFilenamePath f) ((getFilenameFile f) + "JzbmMap")
	--	im_imagefile = Fn_createTxtFile  (getFilenamePath f) ((getFilenameFile f) + "JzimageMap")

		for obj in sel do
		(
			oldObjName = obj.name
			if (oldObjName.count < 6  )then
			(
				print ("出现不规范的名称: " + oldObjName + " ,已略过...")
				continue
			)
			newObjName = Fn_setNewObjNameIm obj
			if(ckb_addLoneToObjname.checked)then
			(
				obj.name = Yvi_addLoneToObjName newObjName
			)
			--print ("obj.name:" + obj.name + "---------" + newObjName)

			Fn_handleAllSubMats obj root
			--处理硬盘上的文件
			Fn_HandleHardDiskFiles()
			
-- 			-- 将贴图显示到物体上.
-- 			m = obj.Material
-- 			if(classof m == MultiMaterial)then
-- 			(
-- 				for i = 1 to m.numsubs do
-- 				(
-- 					showTextureMap m m[i] true
-- 				)
-- 			)
			---------------------
		)
		
		sel = #()
		clearselection()
		freeSceneBitmaps()
		gc()
		format "OK!\n"
		MessageBox "\t\t已导入完成!\n注意:\n贴图已显示到物体上,请先隐藏部分物体再显示贴图!\n"
		g_arr_oldAndNewfilenames = #()
		btn_imp.enabled = false
	)
	
	on btn_showMap pressed do
	(
		if selection.count == 0 then
		(
			MessageBox "请选择部分物体再显示贴图\n注意:请不要选择过多的物体,以免照成MAX内存不足而崩溃!"
			return 0
		)
		for obj in selection do
		(
			-- 将贴图显示到物体上.
			m = obj.Material
			if(classof m == MultiMaterial)then
			(
				for i = 1 to m.numsubs do
				(
					showTextureMap m m[i] true
				)
			)
			--隐藏未选定对象
			actionMan.executeAction 0 "281"
			max hide inv
		)
		MessageBox "OK!\n未显示贴图的物体已隐藏!"
	)
)
--CreateDialog rl_expAndimp

Rollout rl_About "帮助"
(
	label lb_about "插件说明:\n 由于3dsmax导出导入3ds格式的文件时,当
物体名或贴图名字符大于10时,会自动裁掉
前10位之后的字符,所以本插件只适用于以
下情况:\n
①场景物体名字符大于10.\n
②物体名规则如:
10a01_jz1001
10a01_jz1002
10a01_jz1003
即在名称前五位字符之后添加\"_jz\",
其余不变.\n
③导出到3ds时,去掉材质名与物体名中的
\"_jz\",然后再导出;导入3ds时,自动添加\"_jz\".\n
④贴图的名称处理:根据给的贴图配置文件(JzimageMap.txt)新老文件名对应的重新命名
材质名,同时修改硬盘上的贴图文件名,指定
场景中的贴图路径(但必须保证贴图文件在
3ds文件目录或子目录下,不要用中文目录,
请去掉贴图配置文件中的新文件名相对路径).
	
PS:确保配置文件JzimageMap.txt存在于.3ds
文件的同一级目录下!

开发版本:3dsmax2010 X86(测试通过)
(PS:在低于此版本的3DsMax插件显示有差异)"\
	width:250 height:390 pos:[6,10]
	Hyperlink hl_info "如有任何疑问或建议,请联系开发者!" color:red  address:"www.macai.co.cc"
	--Button btn_about "about"
)

try (closerolloutfloater floatexportAndimport) catch () 
floatexportAndimport = newrolloutfloater "导出导入.3ds - v0.4" 250 320 --1180 300
addrollout rl_expAndimp floatexportAndimport --rolledup:true
addrollout rl_About floatexportAndimport rolledup:true


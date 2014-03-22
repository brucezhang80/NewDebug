
/*
开发者：Yvi
版权：Yvi
日期：2012-4-19
使用版本：max 9.0 sp2以上
2012.4.20 0.50，完成了优化材质，文件整理功能，剩下贴图命名，贴图命名中的共用贴图需要认真考虑，明天再做。
2012.4.20 18:28,发现获取文件和目录的问题。不能正确的获得文件在max9版本，现在已解决问题。
2012.05.09 20.22 发现问题：在函数Yvi_getObjMat中，如果遇到无材质或是非多维材质则返回0，结果会造成返回 s_bjMatArr = 0了，所以会出现 0.count的未知属性的错误。现已解决
2012.6.11.9.30 提出新要求，对于某些不需要命名的贴图，略过不命名。
*/

-- 材质命名，共用贴图
Global floatPanel
Global rl_reNameMat 
Global rl_optimizeMat 
Global rl_fileManager
Global rl_help 
Global _thistime1 = 2012420,_thistime2 = 20120420 -- 这个废除

Global nIgnore = 0 -- 略过命名的贴图数目 
struct objMatStruct (objname, oldmatname, newmatname, matpath, share, named)
Global objMat = objMatStruct()
--Global g_objMatArr = #()
Global g_tempMatArr = #()

struct yviFuncStruct
(
	--小写转成大写
	function Yvi_myToUpper instring = -- beginning of function definition
	( 
		local upper, lower, outstring 

		upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
		lower = "abcdefghijklmnopqrstuvwxyz"

		outstring = copy instring

		for i = 1 to outstring.count do
		( 
			j = findString lower outstring[i]

			if (j != undefined) do outstring[i] = upper[j]
		)
		return outstring 
	),
	
	--后缀加0001~000x函数 num为物体数，d为后缀保留位数
	function Yvi_formatNumStr num d =
	(
		str = num as string
		for i in 1 to d - str.count where(str.count < d) do
		(
			str = "0" + str
		)
		
		return str
	),
	
	--getFilesRecursive 路径 文件类型如："*.max"
	function Yvi_getFilesRecursive root filename =
	(
		local all_Files = #()
		local TheAllSubFolder = #()
		
		TheAllSubFolder = GetDirectories (root+"*")
		for s in TheAllSubFolder do
		(
			join TheAllSubFolder (GetDirectories (s + "*"))
		)

		for f in TheAllSubFolder do
		(
			join all_Files (getFiles (f +filename))
		)
		join all_Files (getFiles (root + filename))
		return all_Files
	),--函数结束
	
	-- ////////////////////////////////////////////////////////////////////////////////////贴图命名
	-- 1. 收集所有不重复的贴图
	-- 2.遍历每个物体收集每个物体的贴图，放入数组，去除重复贴图，物体名与贴图原始名对应保存
	-- 3.按物体名来命名新贴图。
	-- 4.修改文件名
	function Yvi_getObjMat obj s_objMatArr strKey = -- 添加 strKey 排除包含有该key的贴图
	(
		obj_name = obj.name
		mat = obj.Material
		
		if(mat == undefined)then
		(
			return 0 --终于找到原因了，如果遇到无材质或是非多维材质，铡会返回_objMatArr = 0了，所以会出面 0.count的未知属性的错误。
		)
		--2012.5.9 对于标准材质，也进行处理
		if(classof mat == Multimaterial)then
		(
		
			-- 收集物体的贴图
			for i = 1 to mat.numsubs do
			(

				if ( mat.materialList[i].diffusemap != undefined )then
				(
					local resFiles = #() -- count为0出错有可能是没定义这个？-- k嗯，这个没问题
					old_diffname = mat.materialList[i].diffusemap.filename -- 贴图文件名
					mappath = getFilenamePath old_diffname -- mat path
					ext_oldfilename = filenameFromPath old_diffname -- width ext
					
					---- 忽略 包含有strKey的贴图
					if(strKey != "" and (findString ext_oldfilename strKey) != undefined)then
					(
						nIgnore += 1
						format "贴图 % 已经被过虑，略过重命名。\n" ext_oldfilename
						continue
					)
	-----------------------找图片真实路径
					resFiles =(yviFuncStruct.Yvi_getFilesRecursive maxfilepath ext_oldfilename) -- 只有一个文件如果文件名唯一
					if( resFiles.count == 0 )then
					(
						format "找不到贴图文件: %\n" ext_oldfilename
						while (i < mat.numsubs)do
						(
							resFiles = yviFuncStruct.Yvi_getFilesRecursive maxfilepath (filenameFromPath (mat.materialList[i+1].diffusemap.filename))
							i += 1
						)
						-- 如果还是找不到
						if(resFiles.count == 0)then 
						(
							reallPath = maxfilepath
						)
						else
							reallPath = getFilenamePath resFiles[1]
					)
					else
						reallPath = getFilenamePath (resFiles[1])
	--------------------------------------
					if((finditem g_tempMatArr ext_oldfilename) != 0)then
					(
						format "发现共用贴图：ext_oldfilename: %\n" ext_oldfilename

						objMat = objMatStruct objname:obj_name oldmatname:ext_oldfilename matpath:reallPath share:true named:false-- newmatname = undefined
						append s_objMatArr objMat

					)
					else
					(
						append g_tempMatArr ext_oldfilename
						objMat = objMatStruct objname:obj_name oldmatname:ext_oldfilename matpath:reallPath share:false named:false
						append s_objMatArr objMat
					)
				)
			)--收集结束
		) -- 多维材质处理完毕
		else if(classof mat == StandardMaterial)then -- 2012.5.9 by Yvi 对于标准的材质，也进行处理
		(
			if ( mat.diffusemap != undefined )then
			(
				local resFiles = #() -- count为0出错有可能是没定义这个？-- k嗯，这个没问题
				old_diffname = mat.diffusemap.filename -- 贴图文件名
				oldpath = getFilenamePath old_diffname -- mat path
				ext_oldfilename = filenameFromPath old_diffname -- width ext
				
				---- 忽略 包含有strKey的贴图
				if(strKey != "" and (findString ext_oldfilename strKey) != undefined)then
				(
					nIgnore += 1
					format "贴图 % 已经被过虑，略过重命名。\n" ext_oldfilename
					continue
				)
					
-----------------------找图片真实路径
				resFiles =(yviFuncStruct.Yvi_getFilesRecursive maxfilepath ext_oldfilename) -- 只有一个文件如果文件名唯一
				if( resFiles.count == 0 )then
				(
					format "找不到贴图文件: %\n" ext_oldfilename
					reallPath = oldpath
				)
				else
					reallPath = getFilenamePath (resFiles[1])
--------------------------------------
				if((finditem g_tempMatArr ext_oldfilename) != 0)then
				(
					format "发现共用贴图：ext_oldfilename: %\n" ext_oldfilename

					objMat = objMatStruct objname:obj_name oldmatname:ext_oldfilename matpath:reallPath share:true named:false-- newmatname = undefined
					append s_objMatArr objMat

				)
				else
				(
					append g_tempMatArr ext_oldfilename
					objMat = objMatStruct objname:obj_name oldmatname:ext_oldfilename matpath:reallPath share:false named:false
					append s_objMatArr objMat
				)
			)
		)
		--这里不要返回 s_objMatArr 数据
		--s_objMatArr
	),
	
	--按物体中使用的贴图数目进行添加序号
	function Yvi_setMatNewName s_objMatArr obj newname d  =
	(
		local num = 0
		if(s_objMatArr.count == 0)then
		(
			format "空%\n" s_objMatArr
		)
		for i = 1 to s_objMatArr.count do
		(
			if(s_objMatArr[i].named == false and s_objMatArr[i].share == false and s_objMatArr[i].objname == obj.name)then 
			(
				ext = (filterString  s_objMatArr[i].oldmatname ".")[2]
				num += 1
				stri = yviFuncStruct.Yvi_formatNumStr num d
				fix_newname = newname + stri+"." + ext
				s_objMatArr[i].newmatname = fix_newname
				s_objMatArr[i].named = true
			)
			for j = i+1 to s_objMatArr.count do
			(
				if(s_objMatArr[i].named == true and s_objMatArr[j].named == false and s_objMatArr[j].share == true and s_objMatArr[i].oldmatname == s_objMatArr[j].oldmatname)then
				(
					s_objMatArr[j].newmatname = s_objMatArr[i].newmatname
					
					s_objMatArr[j].named = true
				)
			)
		)-- 全部命名好

		s_objMatArr
	),
	-- 所有贴图的序号按贴图数目从1一直命名下去。
	function Yvi_setAllMatNewName s_objMatArr newname d =
	(
		local num = 0
		for i = 1 to s_objMatArr.count do
		(
			if(s_objMatArr[i].named == false and s_objMatArr[i].share == false /*and s_objMatArr[i].objname == obj.name*/)then 
			(
				num += 1
				ext = (filterString  s_objMatArr[i].oldmatname ".")[2]
				stri = yviFuncStruct.Yvi_formatNumStr num d
				fix_newname = newname + stri + "." + ext
				s_objMatArr[i].newmatname = fix_newname
				s_objMatArr[i].named = true
			)
			
			for j = i+1 to s_objMatArr.count do
			(
				if(s_objMatArr[i].named == true and s_objMatArr[j].named == false and s_objMatArr[j].share == true and s_objMatArr[i].oldmatname == s_objMatArr[j].oldmatname)then
				(
					s_objMatArr[j].newmatname = s_objMatArr[i].newmatname
					
					s_objMatArr[j].named = true
				)
			)
		)-- 全部命名好
		
		s_objMatArr
	),
	
	function Yvi_reNameObjMat s_objMatArr obj alp=
	(
		
		mat = obj.Material
		obj_name = obj.name
		if(mat == undefined)then
		(
			return 0
		)
		
		if(classof mat == Multimaterial)then
		(
			-- 收集物体的贴图
			for i = 1 to mat.numsubs do
			(
				if ( mat.materialList[i].diffusemap != undefined )then
				(
					old_diffname = mat.materialList[i].diffusemap.filename -- 贴图文件名
					ext_oldfilename = filenameFromPath old_diffname -- width ext
					oldpath = getFilenamePath old_diffname
					
					---- 忽略 包含有strKey的贴图，如果这里添加这个，估计计算速度上会快点。但不加也行，只要当前贴图名与已经收集好的老贴图名不相等，则会跳过
-- 					if(strKey != "" and (findString ext_oldfilename strKey) != undefined)then
-- 					(
-- 						nIgnore += 1
-- 						format "贴图 % 已经被过虑，略过重命名。\n" ext_oldfilename
-- 						continue
-- 					)
					
					for j = 1 to s_objMatArr.count do
					(
						if(s_objMatArr[j].objname == obj_name and s_objMatArr[j].oldmatname == ext_oldfilename and s_objMatArr[j].named == true)then
						(
							reallPath = s_objMatArr[j].matpath
							if(reallPath == undefined)then
							(
								print "------------------------------------------------"
								reallPath = oldpath
							)
							newfullpathfile  = reallPath + s_objMatArr[j].newmatname
							_newname = substring (s_objMatArr[j].newmatname) 1 ((s_objMatArr[j].newmatname).count-4)
							mat.materialList[i].name = _newname
							mat.materialList[i].diffuseMap.name = _newname
							mat.materialList[i].diffuseMap.fileName = newfullpathfile
							
							oldfullpathfile = reallPath+ext_oldfilename
							renameFile old_diffname newfullpathfile
						)
					)
				)
				try
				(
					if(mat.materialList[i].opacityMap != undefined)then
					(
						mat.materialList[i].opacityMap.filename= mat.materialList[i].diffuseMap.fileName
						mat.materialList[i].opacityMap.name = newname+alp
					)
					if(mat.materialList[i].SelfIllumMap != undefined)then
					(
						mat.materialList[i].SelfIllumMap.filename= mat.materialList[i].diffuseMap.fileName
						mat.materialList[i].SelfIllumMap.name = newname+alp
					)
				)catch(format "none OpacityMap or SelfIllumMap...")
			)
		)
		else if(classof mat == StandardMaterial)then -- 2012.5.9 by Yvi 对于标准的材质，也进行重命名处理
		(
			if ( mat.diffusemap != undefined )then
			(
				old_diffname = mat.diffusemap.filename -- 贴图文件名
				ext_oldfilename = filenameFromPath old_diffname -- width ext
				oldpath = getFilenamePath old_diffname
				
				for j = 1 to s_objMatArr.count do
				(
					if(s_objMatArr[j].objname == obj_name and s_objMatArr[j].oldmatname == ext_oldfilename and s_objMatArr[j].named == true)then
					(
						reallPath = s_objMatArr[j].matpath
						if(reallPath == undefined)then
						(
							print "------------------------------------------------"
							reallPath = oldpath
						)
						newfullpathfile  = reallPath + s_objMatArr[j].newmatname
						_newname = substring (s_objMatArr[j].newmatname) 1 ((s_objMatArr[j].newmatname).count-4)
						mat.name = _newname
						mat.diffuseMap.name = _newname
						mat.diffuseMap.fileName = newfullpathfile
						
						oldfullpathfile = reallPath+ext_oldfilename
						renameFile old_diffname newfullpathfile
					)
				)
			)
			try
			(
				if(mat.opacityMap != undefined)then
				(
					mat.opacityMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.opacityMap.name = newname+alp
				)
				if(mat.SelfIllumMap != undefined)then
				(
					mat.SelfIllumMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.SelfIllumMap.name = newname+alp
				)
			)catch(format "none OpacityMap or SelfIllumMap...")
		)
	),
	
	---------贴图名处理
	function Yvi_collectSubMats mat mats = 
	(
		if ( classof mat == Multimaterial) then 
		(
			for i = 1 to mat.numsubs do
			(
				yviFuncStruct.Yvi_collectSubMats mat[i] mats
			)
		)
		else if(classof mat == standard) then 
		(
			append  mats mat
		)
		else
		(
			messagebox ("出现了不是多维材质或者标准材质的材质 "+ mat.name)
		)
		return mats
	),
	
	--子材质名等于贴图名
	function Yvi_handleAllSubMats obj =
	(
		local allsubmats = #()
		
		mat = obj.material
		if mat == undefined then return 0
		allsubmats = yviFuncStruct.Yvi_collectSubMats  mat allsubmats
		for mat in allsubmats do 
		(
			if mat.diffuseMap == undefined then 
			(
				--messagebox ("物体: "+ obj.name + "中的材质: "+ mat.name as string +" 错误或者没有 Diffuesmap --已略过...") title:"注意"
				format "物体: % 中的材质: % 错误或者没有 \"Diffuesmap\" --已略过...\n" obj.name mat.name as string
				continue
			)
			filepathname = mat.diffuseMap.fileName
			--获取文件名字，带后缀
			filename = filenameFromPath filepathname
			-- 获得贴图路径
			filepath = getFilenamePath filepathname
			--获取文件后缀
			ext = getfilenametype filename
			
			--这里修改贴图的名字，就是文件名字，不要后缀
			mat.diffusemap.name = getFileNameFile filename --+ "_temp"
			mat.name  = mat.diffusemap.name
			try
			(
				if(mat.opacityMap != undefined)then
				(
					mat.opacityMap.filename = mat.diffusemap.filename
					mat.opacityMap.name = mat.diffusemap.name
				)
				if(mat.opacityMap != undefined)then
				(
					mat.SelfIllumMap.filename = mat.diffusemap.filename
					mat.SelfIllumMap.name = mat.diffusemap.name
				)
			)catch(format "non-OpacityMap...")
		)
	),
	
	--//////////////////////
	function Yvi_getFolderName _root =
	(
		local all_Files = #()
		local TheAllSubFolder = #()
		
		TheAllSubFolder = GetDirectories (_root+"\\*")
		for s in TheAllSubFolder do
		(
			join TheAllSubFolder (GetDirectories (s + "\\*"))
		)
		
		return TheAllSubFolder
	),
	
	-- 此函数没有用到
	function Yvi_removeLastDigit str =
	(
		for i = str.count to 1 by -1 do
		(
			n = bit.charasint str[i]
			if n != undefined then
			(
				if n >= 48 and n <= 57 then
					str[i] = ""
				else
				(
					return str
				)
			)
			else
			(
				return str
			)
		)
	),
	
	fn Yvi_getCurTime =
	(
		local _localtime = localtime as string
		local _curTime = ""
		for i=1 to _localtime.count do
		(
			asc = bit.charasint _localtime[i]
			if( asc == 32 )then -- 32 是空格
			(
				exit
			)
			if(asc >= 48 and asc <= 57)then
			(
				_curTime += _localtime[i]
			)
		)
		_curTime = _curTime as integer
		if(_curTime != undefined)then
		(
			return _curTime
		)
		else
		(
			return _thistime
		)
	),
	
	--这个函数废除
	function Yvi_setLimited =
	(
		clearListener()

		_curtime = yviFuncStruct.Yvi_getCurTime()
		t1 = abs (_curtime-_thistime1)

		if(t1>2)then
		(
			Messagebox "您使用的是测试版副本，现已过期！" title:"警告"
			closerolloutfloater floatPanel
-----------------------------
			Global run = false,Password="07090509010302010"
			rollout roEnterPassword "输入密码" width:186 height:90
			(
				label lb_info "您使用的是测试版副本，现已经过期！如果您需要继续使用请输入正确的密码或联系开发者！"\
				pos:[5,4] width:176 height:40
				edittext txtPassword "密码：" pos:[16,46] width:160 height:17 bold:true
				button btnOK "OK" pos:[50,68] width:90 height:21

				on btnOK pressed do
				(
					run = Password == txtPassword.text
					--DestroyDialog roEnterPassword
					
					if run then
					(
						try (closerolloutfloater floatPanel) catch () 
						floatPanel = newrolloutfloater "命名+优化+文件管理" 205 380
						addrollout rl_reNameMat floatPanel rolledup:false
						addrollout rl_optimizeMat floatPanel rolledup:false
						addrollout rl_fileManager floatPanel rolledup:false
						--addrollout rl_help floatPanel rolledup:false
					)
					else
					(
						Messagebox "密码错误！！" title:"错误"
						--DestroyDialog roEnterPassword
						closerolloutfloater floatPanel
					)
				)
			)
			createDialog roEnterPassword modal:true
			
			
---------------------------
		) -- if time
	)
	
)
yviFunc = yviFuncStruct()


-- //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
-- //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rollout rl_reNameMat "贴图命名" width:204 height:340
(
	group "设置参数"
	(
		label lbl1 "前缀(默认模型名)："align:#left across:2
			editText edt_pre ""  align:#right width:70 text:""
		label lbl2 "附加中间：" across:2 align:#left
			editText edt_mid ""  align:#right width:100
		label lbl3 "保留位数："across:2 align:#left
			dropDownList ddl_num "" items:#("1", "2", "3", "4", "5", "6", "7", "8") height:6  align:#right width:96
		label lbl4 "透贴后缀：" across:2 align:#left
			editText edt_fix ""  width:100 align:#right
		label lbl5 "排除贴图：" across:2 align:#left
			editText edt_ignore ""  width:100 align:#right
	)	
		Button btn_checkObjName "检查重复模型名"align:#left tooltip:"检查模型名是否存在重复名。" across:2  width:140
			button btn_help0 "?" align:#right tooltip:"模型名检查帮助必读"
		button btn_matRename "贴图命名" width:140 across:2 align:#left tooltip:"设置就绪后，开始贴图批量重命名。"
			button btn_help1 "?" align:#right tooltip:"贴图命名帮助必读"
		progressBar pgb_progress1 "进度条1"  width:180 height:6 color:red pos:[4,198]
		--progressBar pgb_progress11 "进度条11"  width:180 height:6 color:red pos:[4,182]
		
	local noRepeat = false
--//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	on btn_checkObjName pressed do
	(
		local l_objNameArr = #()
		local l_repeatNameArr = #()
		if(Objects.count == 0)then
		(
			MessageBox "场景中没有物体！" title:rl_reNameMat.title
			return 0
		)
		for obj in Objects where(Superclassof obj == GeometryClass) do
		(
			if((finditem l_objNameArr obj.name) != 0)then
			(
				-- 找到有重复的模型名
				append l_repeatNameArr obj.name
			)
			else
			(
				append l_objNameArr obj.name
			)
		)
		
		if(l_repeatNameArr.count != 0)then
		(
			noRepeat = false
			Messagebox "发现模型名重复" title:rl_reNameMat.title
			Global g_repName = l_repeatNameArr
			try(DestroyDialog rl_repName)catch()
			rollout rl_repName "重复模型名"
			(
				listBox lb_name "双击选择重复名的模型："
				
				on rl_repName open do
				(
					lb_name.items = g_repName
				)
				
				-- 双击项选择物体
				on lb_name doubleClicked item do
				(
					local item_name = ""
					local f_name = ""
					clearselection()
					if lb_name.items.count == 0 do return 0
						
					item_name = lb_name.items[item]--得到双击项名
					for obj in Objects do
					(
						if item_name == obj.name then
						(
							selectMore obj
							actionMan.executeAction 0 "311"
							max zoomext sel all
						)
					)
				)
			)
			CreateDialog rl_repName
		)
		else
		(
			Messagebox "OK！模型名全部命名没有重复的！" title:rl_reNameMat.title
			noRepeat = true
			return 0
		)
		
	)
	
	on btn_help0 pressed do
	(
		rollout rl_help0 "模型名检查帮助" width:365
		(
			label lb_0x001 "说明：\n" align:#left
			label lb_0x002 "1.如果按模型名分别命名贴图名，则必须保证模型命名唯一性。"align:#left
			label lb_0x003 "2.当前缀为空时，将以模型名为前缀，则必须检查模型名是否唯一。"align:#left
			label lb_0x004 "3.如果前缀不为空时，将以所填写的前缀进行有序的命名。" align:#left height:27
			
			button btn_close "确定"
			
			on btn_close pressed do
			(
				DestroyDialog rl_help0
			)
		)
		CreateDialog rl_help0
	)
	
------------------------------
	on btn_matRename pressed do
	(
		local s_objMatArr = #() -- 数据中是结构数据
		local s_newObjMatArr = #() -- 数据中是结构数据
		
		nIgnore = 0 -- 全局变量。计数略过命名的贴图数
		pgb_progress1.color = RED
		g_tempMatArr = #()
		if(Objects.count == 0)then
		(
			MessageBox "场景中没有物体！" title:rl_reNameMat.title
			return 0
		)
		if(maxfilepath == "")then
		(
			MessageBox "场景文件尚未保存，请保存文件后重试！" title:rl_reNameMat.title
			return 0
		)
		local counts = 0
		if(edt_pre.text == "")then
		(
-- 			if(noRepeat == false)then
-- 			(
-- 				MessageBox "发现场景存在模型名称重复！" title:rl_reNameMat.title
-- 				return 0
-- 			)
			opts = (yesNoCancelBox "确保场景对象（模型）名没有重复，否则贴图命名出问题。是否继续？" title:"警告" beep:true)
			case opts of
			(
				#yes:
				(
					for obj in Geometry do
					(
						if(obj.Material == undefined)then
						(
							continue
						)
						yviFunc.Yvi_getObjMat obj s_objMatArr (edt_ignore.text)

						thenewname = obj.name + edt_mid.text
						-- 按每个物体所使用的贴图数目进行添加序号
						s_newObjMatArr = yviFunc.Yvi_setMatNewName s_objMatArr obj thenewname (ddl_num.selected as integer)
						pgb_progress1.value = 100.*(counts+=1)/Geometry.count
						gc()
					)
					pgb_progress1.value = 0.
					counts = 0
					-- 分开来处理就没问题了？？？奇怪
					pgb_progress1.color = green
					for obj in Geometry do
					(
						if(obj.Material == undefined)then
						(
							continue
						)
						yviFunc.Yvi_reNameObjMat s_newObjMatArr obj (edt_fix.text)
						
						pgb_progress1.value = 100.*(counts+=1)/Geometry.count
						gc()
					)
				)
				#no:(print "已否定，需检查模型命名唯一。";return 0)
				#cancel:(print "用户已退出贴图命名。";return 0)
			)
		)
		--不按模型名命名贴图。这个比较好处理。注意共用贴图的处理
		else if(edt_pre.text != "" and edt_pre.text != " ")then
		(
			local num = 0
			for obj in Geometry do
			(
				if(obj.Material == undefined)then
				(
					continue
				)
				yviFunc.Yvi_getObjMat obj s_objMatArr (edt_ignore.text)
				
				pgb_progress1.value = 100.*(counts+=1)/Geometry.count
				gc()
			)
			pgb_progress1.value = 0.
			pgb_progress1.color = green
			counts = 0
			thenewname = edt_pre.text + edt_mid.text
			-- 按场景所有的贴图数目从1开始进行添加序号
			s_newObjMatArr = yviFunc.Yvi_setAllMatNewName s_objMatArr thenewname (ddl_num.selected as integer)
			
 			for obj in Geometry do
 			(
				if(obj.Material == undefined)then
				(
					continue
				)
				yviFunc.Yvi_reNameObjMat s_newObjMatArr obj (edt_fix.text)
				
				pgb_progress1.value = 100.*(counts+=1)/Geometry.count
				gc()
			)
		)
		
		MessageBox "贴图命名完成!" title:rl_reNameMat.title
		if(edt_ignore.text != "")then
			format "\t共有 % 个带有关键字 % 的材质被略过命名。\n" nIgnore (edt_ignore.text)
		pgb_progress1.value=0.
		gc()
		freesceneBitmaps()
	)
	
	on btn_help1 pressed do
	(
		rollout rl_help1 "贴图命名帮助" width:380
		(
			label lb_0x001 "说明：\n" align:#left
			label lb_0x002 "1.前缀：如果不指定，则贴图以模型名为前缀，按每个模型使用的贴\n\t图数添加序号。若指定则全部贴图按序号依次命名下去。"align:#left height:27
			label lb_0x003 "2.中间：即以模型名与序号之间的字串。（可选填）"align:#left
			label lb_0x004 "3.保留位数：即贴图名之后的序号保留的位数，\n  如:选择3，则为001开始至999" align:#left height:27
			label lb_0x005 "4.透贴后缀：即如果是透明贴图，则为透明材质名称\n  序号之后添加指定的字串。"align:#left height:27
			label lb_0x006 "5.同时会命名相应贴图文件的文件名，透贴贴图仅修改材质名。"align:#left height:15
			label lb_0x007 "6.仅支持 DiffuesMap、SelfIllumMap、OpacityMap三种类型的材质命名。"align:#left
			label lb_0x008 "7.参考：Box,  Box_Mid_001.jpg,  Box_Mid_001_ALP.jpg。"align:#left height:15
			label lb_0x009 "8.另外，添加根据指定关键字贴图不进行重命名的功能设置。"align:#left height:25
			HyperLink hlk_0x008 "（注意：支持多维和标准材质命名！）" color:red enabled:false
			button btn_close "确定"
			
			on btn_close pressed do
			(
				DestroyDialog rl_help1
			)
		)
		CreateDialog rl_help1
	)
)

rollout rl_optimizeMat "优化材质"
(
	--group "优化选择对象的材质" 
	--(
		button btn_optimizeMat "批量优化材质"  width:140 across:2 align:#left tooltip:"选择多维材质的一个或多个模型进行材质优化"
			button btn_help2 "?" align:#right tooltip:"优化材质帮助必读"
		progressBar pgb_progress2 "进度条2"  width:180 height:6 color:red pos:[4,34]
	--)
	
-- ////////////////////////////////////////////////////////////////////////////////////优化材质 
	on btn_help2 pressed do
	(
		rollout rl_help2 "优化材质帮助" width:300
		(
			label lb_0x01 "说明：\n" align:#left
			label lb_0x02 "1.选择已赋予多维材质的模型对象。"align:#left
			label lb_0x03 "2.清除多余无用的材质！"align:#left
			
			button btn_close "确定"
			
			on btn_close pressed do
			(
				DestroyDialog rl_help2
			)
		)
		CreateDialog rl_help2
	)
	on btn_optimizeMat pressed do
	(
		local theMat, uniqueMats, mapTo, theObj
		struct aMat (theInstance, theDiffusemap)
		
		try
		(
			if selection.count == 0 then 
			(
				MessageBox "请选择物体！" title:rl_optimizeMat.title
				return 0
			)
			else
			(
				local i = 0
				for obj in selection do
				(
					-- 优化前做一个子材质命名
					yviFunc.Yvi_handleAllSubMats obj 
					macros.run "Modifier Stack" "Convert_to_Mesh"
					
					theMat = obj.material --获取材质
					
					if(theMat == undefined)then
					(
						print ("物体 "+obj.name + " 没有材质！")
					)
					else
					(
						if classof theMat != Multimaterial then
						(
								--messagebox ("物体材质"+theMat.name+"不是多维材质！")
								theMat.name = obj.name
								print ("物体"+theMat.name + "不是多维材质！")
						)
						else
						(
							print (obj.name as string+"--")
							print ("优化前子材质数目： 【"+theMat.numsubs as string + "】")
							local SubMatUsed=#()--收集用到的子材质
							for f=1 to obj.numfaces do
								SubMatUsed[getFaceMatID obj f]=true--获得物体面ID号
								print ("物体材质ID数目: 【"+SubMatUsed.count as string + "】")
							
							uniqueMats=#()
							mapTo=#()
							for m=1 to theMat.numsubs do--材质数
							(	
								if SubMatUsed[m] != true do continue
								local thisMatDBitmap=undefined
								if classof theMat[m] == StandardMaterial do
									if classof theMat[m].diffusemap == Bitmaptexture do
										thisMatDBitmap=theMat[m].diffusemap.filename
									
								if thisMatDBitmap == undefined do
								(	
									append uniqueMats (aMat theMat[m] thisMatDBitmap)
									mapTo[m]=uniqueMats.count
									continue
								)
								local foundMatch=false
								for p=1 to uniqueMats.count do
									if thisMatDBitmap == uniqueMats[p].theDiffusemap do
									(	mapTo[m]=p
										foundMatch=true
										continue
									)
								if not foundMatch do 
								(	append uniqueMats (aMat theMat[m] thisMatDBitmap)
									mapTo[m]=uniqueMats.count
								)
							)
							print ("优化后子材质数目: 【"+ uniqueMats.count as string + "】")
						
							local NewMMat=Multimaterial numsubs:uniqueMats.count name:obj.name
							for m=1 to uniqueMats.count do
								NewMMat[m]=copy uniqueMats[m].theInstance
							
							for f=1 to obj.numfaces do
							(	
								local oldID=getFaceMatID obj f
								--YY修改
								if oldID != undefined do
								(
									if mapTo[oldID] == undefined do
									(
										print ("mapTo[oldID]: "+ mapTo[oldID] as string +"  oldID: " + oldID as string + "  f:" + f as string)
										-- 面 ID号未定义时,在catch处抛出此错误信息,YY
										throw(Messagebox (obj.name as string + " 该模型面号:"+ f as string +" 出现未定义的材质面ID号:" + oldID as string) title:"错误")
										continue
									)
								--end
									--如果模型存在没有ID号的面,则会出错.
									setFaceMatID obj f mapTo[oldID]
								)
							)
								
							obj.material=NewMMat
							update obj
							
							meditMaterials[1]=obj.material
							a=meditMaterials[1].materialIDList

							for i = 1 to a.count do 
							(
								showTextureMap meditMaterials[1].materialList[a[i]] on
							)
						)
					)
					pgb_progress2.value = 100.*(i+=1)/selection.count
					freesceneBitmaps()
					gc()
				)--for结束
				MessageBox "优化完成"
				print "优化完成"
			)--else
			pgb_progress2.value = 0.
			--clearselection()
		)--try
		catch--抛出给定的异常 yy 
		(
			format "\n**** % ****\n" (getCurrentException())
			Messagebox ("模型有错！请先检查\n\n\""+(getCurrentException())as string+"\"\n\n导致错误的原因可能是该模型存在无材质ID号的面!\n\n") title:"程序已中止..."
		)
	)-- 优化材质 结束
)
rollout rl_fileManager "文件管理"
(
	group "设置参数"
	(
		editText edt_ext "*入库的文件类型：" --width:140
		editText edt_srcFilesDir "" width:110 across:2 align:#left
			button btn_setSrcDir "*文件..."  align:#right tooltip:"设置需要入库的文件目录..." width:50
		editText edt_desFolderDir "" width:110 across:2 align:#left
			button btn_setDesDir "*入库..." across:2 align:#right tooltip:"设置存入文件的库目录..." width:50
	)	
		button btn_ManagerIt "整理入库" width:140 across:2 align:#left tooltip:"目录就绪后开整理入库。"
			button btn_help3 "?" align:#right tooltip:"文件管理必读帮助"
		progressBar pgb_progress3 "进度条3"  width:180 height:6 color:red pos:[4,130]
	
	
--////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Global g_save_path = undefined
	Global g_file_path = undefined
	Global g_arr_Files = #()
	Global g_arr_FolderName = #()
--//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	on btn_help3 pressed do
	(
		rollout rl_help3 "文件管理帮助" width:300 height:160
		(
			label lb_x01 "注：文件管理有针对性，并不一定能用到所有方面。"
			label lb_x02 "1.入库：把文件存到此目录下的子目录中，"align:#left
			label lb_x03 "\t子目录文件夹名为存入文件的文件名。" width:260 align:#left
			label lb_x04 "2.文件：此目录下的所有要入库的文件。" align:#left
			label lb_x05 "    ※此功能其实就是将某些文件移入文件夹内。将\n某个文件，移动到以这个文件名命名的文件夹内。※" align:#left height:30
			HyperLink hpl_x06 "带有 ‘ * ’ 的必须设定！" align:#left enabled:false color:red
			
			button btn_close "确定"
			
			on btn_close pressed do
			(
				DestroyDialog rl_help3
			)
			
		)
		CreateDialog rl_help3
	)
	on btn_setDesDir pressed do
	(
		g_save_path = getSavePath caption:"选择入库的位置..."
		if(g_save_path == undefined)then
		(
			edt_desFolderDir.text = ""
			return 0
		)
		else
		(
			edt_desFolderDir.text = g_save_path
			g_arr_FolderName = yviFunc.Yvi_getFolderName g_save_path
			print g_arr_FolderName
		)
	)
	on btn_setSrcDir pressed do
	(
		g_arr_Files = #()
		_ext = edt_ext.text
		if(edt_ext.text == "")then
		(
			Messagebox "请先指定入库的文件类型！" title:rl_fileManager.title
			return 0
		)
		g_file_path = getSavePath caption:"选择文件所在位置..."
		if(g_file_path == undefined)then
		(
			edt_srcFilesDir.text = ""
			return 0
		)
		else
		(
			edt_srcFilesDir.text = g_file_path
			index = findString _ext "."
			if(index == undefined)then
			(
				_ext = "."+_ext
			)
			g_arr_Files = yviFunc.Yvi_getFilesRecursive g_file_path ("*"+_ext)
			print g_arr_Files
			if(g_arr_Files.count == 0)then
			(
				Messagebox ("指定的路径找不到 ‘*" + _ext + "' 类型的文件。" ) title:rl_fileManager.title
				g_file_path = undefined
				return 0
			)
		)
	)
	
	on btn_ManagerIt pressed do
	(
		local i = 0
		if (g_arr_FolderName.count != 0) then
		(
			if (g_arr_Files.count != 0) then
			(
				for p in g_arr_FolderName do
				(
					tempPath = (filterString p "\\")
					for x in g_arr_Files do
					(
						sampleName = getFilenameFile x
						filename = filenameFromPath x
						if(tempPath[tempPath.count] == sampleName)then
						(
							--format "%, %, %, %, %\n"p tempPath[tempPath.count] sampleName x (p+filename)
							copyFile x (p+filename)
							i += 1
							--deletefile p
						)
						pgb_progress3.value = 100.*i/g_arr_FolderName.count
					)
				)
				
			)
		)
		else
		(
			Messagebox "1.设置的入库目录是否有库单元文件夹？\n2.设置的文件目录下是否有需要入库的文件？" title:"警告"
			return 0
		)
		format "共有 % 个文件被入库,\n其中有 % 个文件遗留未能找到库单元目录！" i (g_arr_Files.count-i)
		format "OK\n"
		Messagebox ("已完成入库！\n共"+ i as string + "个文件,\n其中有 "+(g_arr_Files.count-i) as string+" 个文件遗留未能找到库单元目录！") title:rl_fileManager.title
		pgb_progress3.value = 0.
		ShellLaunch g_save_path ""
	)

)
rollout rl_help "About"
(
	label lb_version "版本: 1.2"align:#left
	label lb_DateUpdated "最后更新: 2012/05/11"align:#left
	label lb_support "Max版本要求: Max 9.0 sp2 +"align:#left
	label lb_info "(c) Copyright 2012 Power By Yvi" align:#left
	label lb_link "链接: "across:2 align:#left
		HyperLink hpl_site "Click Here" color:green hovercolor:red visitedcolor:blue address:"www.macai.co.cc" pos:[50,80]--align:#left
	label lb_email "E-mail:12319597@qq.com" align:#left
--////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	on rl_help open do
	(
		--yviFunc.Yvi_setLimited()
	)
)
--//////////////////////////////////////////////////////////////////////////////////////////////////


try (closerolloutfloater floatPanel) catch () 
floatPanel = newrolloutfloater "命名&优化&文件管理 v1.1" 205 380
addrollout rl_reNameMat floatPanel rolledup:false
addrollout rl_optimizeMat floatPanel rolledup:false
addrollout rl_fileManager floatPanel rolledup:false
addrollout rl_help floatPanel rolledup:false


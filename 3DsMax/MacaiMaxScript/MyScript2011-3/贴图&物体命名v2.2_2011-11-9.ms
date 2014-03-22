
--贴图和物体命名
--作者：杨祎
--2011.8.28
--最后更新2011.9.5
--2011.11.09更改界面
--请不要试图修改贴图命名的代码,你可以添加新的贴图命名代码
--请你不要更改开发者的版权信息

 /****************************************************************************************************************/
/*注册表函数处理开始/
/****************************************************************************************************************/
global flag = 100110111001001
global itemArr = #()
global functions
	struct temp
	(
		val = 0 --不要更改这个
		,
		gIs = fn gIs Category Key File:"Settings" =  
		(
			local type, val, key1
			
 			registry.createKey HKEY_CURRENT_USER ("Software\\McSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
			
			registry.createKey HKEY_CLASSES_ROOT (".imr\\mc\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
			
		)--END getIs FN
		,
		sIs = fn sIs Category Key Val File:"Settings" del:false = 
		(
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CURRENT_USER ("Software\\McSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
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
			
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CLASSES_ROOT (".imr\\mc\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
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
			
		)--END saveIs FN
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
					Default:(functions.sIs MC.name "value1" (flag as string))
 
				)--END classes cases
			)--END items loop
		)--END saveMCSettings FN
		,
		getMCSettings = fn getMCSettings MC =
		(
			global items = MC.controls
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
						local val = functions.gIs MC.name i.name
						if val != undefined AND classOf val == String do val = execute val
						if classOf val == integer  And (i.name == "value0")then
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
							else if(val <= 0 or val != 100110111001001)do
							(
								for i in items where(i.name != "hl_site")do
								(
									i.enabled = false
									flag = -1
								)
								MessageBox "使用期限已到，需要继续使用请注册！"
								i.text = 0 as string + " 未授权脚本已经过期"
								
								global rl_timer,lb_txt1,valDown
								(
									rollout rl_timer "timer"
									(
										timer clock1 "Destroy" interval:1000
										label lb_txt1 "20" pos:[120,2] color:red enabel:false
										
										on clock1 tick do
										(
											valDown = (lb_txt1.caption as integer) - 1
											if(valDown >= 0)then
											(

												lb_txt1.caption = valDown as string
												floatReName.title = "试用版 " + valDown as string + " 秒后关闭!"
											)
											else
											(
												closerolloutfloater floatReName
												clock1.active = false
												destroydialog rl_timer
											)
										)
										
										on rl_timer close do
										(
											--closerolloutfloater floatReName
											clock1.active = false
										)
										
									)createDialog rl_timer pos:[-100,-100]
								)
							)
						)
						else if (Classof val == integer and val <= 0)then
						(
							for i in items do
							(
								i.enabled = false
								flag = -1
							)
						)
						else if( val == undefined)do
						(
-- 							for i in items do
-- 							(
-- 								append itemArr i
-- 								i.enabled = false
-- 								flag = -1
-- 							)
						)
						
					)
					GroupStartControl :
					(
					)
					GroupEndControl :()
					Default:
					(

					)
				)--END classes cases
			)--END items loop
		)--END getROSettings FN
	)
	functions = temp val:1
 /****************************************************************************************************************/
/*注册表函数处理结束*/
/****************************************************************************************************************/


--------------------------------------------------------------------------------------------------------------------------
	--小写转成大写
	fn MyToUpper instring = -- beginning of function definition
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
	) 
	
	--后缀加0001~000x函数 num为物体数，d为后缀保留位数
	fn FormatNumStr num d =
	(
		str = num as string
		if str.count < d then 
		for i in 1 to d - str.count do
		(
			str = "0" + str
		)
		str
	)
	--for i in 1 to selection.count do selection[i].name = "obj_" + formatNumStr i d:5
	
--try(destroydialog YY_ReName4Mat)catch()
global YY_ReName4Mat
(
rollout YY_ReName4Mat "批量贴图命名" --width:230 height:340
(
	EditText edt_MaxName "max文件名：" pos:[6,6] --enabled:false--readOnly:true--text:maxfilename
	group "贴图命名"
	(
		label lb_formatMat "格式:" align:#left
		label lb_mat "   前缀         +       中间 +"  across:2 --pos:[6,50]
		label lb_matNumber " 后缀保留1位" 
		EditText edt_MatPreName ""  width:70 height:18  text:(getFilenameFile maxfilename) across:3
		EditText edt_MatMidName "+"  width:70 height:18 text:"_JZ_"
		--EditText edt_MatLstName "" pos:[155,50] width:70 height:18 text:""
		--label lb_a "+"
		DropDownList ddl_MatLstName ""  width:70 height:6 items:#("1","2","3","4","5","6","7","8","9","10","11","12") align:#right
		
		CheckBox ck_NameInPix "按贴图格式命名" checked:true
		--Hyperlink hk_1 "按贴图格式命名" color:red enabled:false 
		label lb_Notice "贴图改名后格式如下:\n(自定义) + \"(自定义)\" + \"001/002/003...\"+ 图片类型\"\n ※包括修改硬盘图片文件名※"\
						 width:220 height:60 align:#left
		--button bt_Nothing ">" pos:[190,135] width:40 height:18
		--CheckBox ck_oneObjMat "按单独物体命名贴图" pos:[28,137] width:160 height:17
		button btn_reNameMat "贴图重命名"  width:160 height:30 tooltip:"开始贴图批量重命名"
		button btn_ClearMat "清空材质球"  width:68 height:21 tooltip:"清空材质球"
		
		label lb_1 "命名进度： " align:#left
		progressBar pgb_mergeProgress1 "" width:222 height:8 color:green
	)

	--label lb_Text "麻菜出品"pos:[180,325]
------------------------------------------------------------	
	--定义全局的数组，用来保存文件名字
	global g_oldfilenames=#()
	--global g_midfilenames=#()
	global g_newfilenames=#()
	global g_tilefilename=""  
	
	global jpg_OldFileName = #()
	global tga_OldFileName = #()
	global png_OldFileName = #()
	global bmp_OldFileName = #()
	
	global jpg_newfilenames = #()
	global tga_newfilenames = #()
	global png_newfilenames = #()
	global bmp_newfilenames = #()
	

	--获取场景中物体所用的所有材质
	--这些材质应当少于scenematerials
	fn GetAllMatUsedByScene = 
	(
		allmats=#()
		max select all
		for obj in Selection do 
		(
			if obj.material!=undefined then 
			(
				append allmats obj.material
				if classof obj.material==Multimaterial then 
				(
					for x in obj.material.materialList do 
					(
						if x!=undefined then
							append allmats x
					)
				)
			)
		)
		max select none
		return allmats
	)
	
	
	fn CollectSubMats mat mats = 
	(
		if ( classof mat  == Multimaterial) then 
		(
			for i=1 to mat.numsubs do 
			(
				CollectSubMats mat[i] mats
			)
		 )
		 else if(classof mat == standard) then 
		 (
			 append  mats mat
		 )
		 else
		 (
		     messagebox ("出现了不是多维材质或者标准材质的材质"+mat.name)
		 )
	)
	
	--这里收集场景中的所有子材质
	fn collectAllSubMats = (
		allSubMats=#()
		for mat in scenematerials do 
		(
		   CollectSubMats mat allsubMats
		)
		
		return allSubMats
	)

	--处理场景中的所有子材质
	--我们收集每一个子材质的文件名字
	--形成一个列表
	--这个列表里面，每一个老的文件名字对应一个中间态的文件名字，和一个新的文件名字
	--然后统一：删除所有中间态的文件名字，把老的文件拷贝一份成中间态文件，
	--删除老的文件，删除新的文件，把中间态的文件改名成新的文件名
	fn HandleAllSubMats allsubmats = 
	(
		--获取max的名字
		tileName0 = getFilenameFile maxFileName
		tileName =  MyToUpper tileName0
		
		tilepath = maxFilePath
		
		--这里清除我们的全局数组
		--g_midfilenames=#()
		g_oldfilenames=#()
		g_newfilenames=#()
		
		--按图片格式命名
		jpg_OldFileName = #()
		tga_OldFileName = #()
		png_OldFileName = #()
		bmp_OldFileName = #()
		jpg_newfilenames = #()
		tga_newfilenames = #()
		png_newfilenames = #()
		bmp_newfilenames = #()
		
		--我们假定文件名是正确的,并且不包括路径
		--uniqueFile=0
        for mat in allsubmats do 
		(
			if mat.diffuseMap==undefined then messagebox ("出错了！ "+mat.name as string +" 错误或者没有 Diffuesmap,请检查!")
			else
			(
				filepathname = mat.diffuseMap.fileName
				
				--获取文件名字，带后缀
				filename0 = filenameFromPath filepathname
				filename = MyToUpper filename0
				
				--获取文件后缀
				ext0  = getfilenametype filename	
				ext = MyToUpper ext0

				--查找在数组里面是否已经存在这个文件名
				flag1 = findItem jpg_OldFileName filename
				flag2 = findItem tga_OldFileName filename
				flag3 = findItem png_OldFileName filename
				flag4 = findItem bmp_OldFileName filename
				
				index = findItem g_oldfilenames filename
				--文件名字不存在，我们加到数组里面去
				if (index==0) then 
				(
					--原始文件名字
					append g_oldfilenames  filename

					/*在这里从g_oldfilenames数组里找出不同格式的文件名*/
					if ck_NameInPix.checked then
					(
						if (ext == ".JPG")then
						(
							append jpg_OldFileName filename
							jpgNum = FormatNumStr jpg_OldFileName.count (ddl_MatLstName.selected as integer)
							
							jpg_newname = edt_MatPreName.text + edt_MatMidName.text + jpgNum + ext 
							append jpg_newfilenames jpg_newname

							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath+jpg_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile jpg_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".TGA")then
						(
							append tga_OldFileName filename
							--print "+++++++++++++++++"
							--print tga_OldFileName
							tgaNum = FormatNumStr tga_OldFileName.count (ddl_MatLstName.selected as integer)
							
							tga_newname = edt_MatPreName.text + edt_MatMidName.text + tgaNum + ext 
							append tga_newfilenames tga_newname

							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + tga_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile tga_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".PNG")then
						(
							append png_OldFileName filename
							pngNum = FormatNumStr png_OldFileName.count (ddl_MatLstName.selected as integer)
							
							png_newname = edt_MatPreName.text + edt_MatMidName.text + pngNum + ext 
							append png_newfilenames png_newname

							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + png_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile png_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".BMP")then
						(
							append bmp_OldFileName filename
							bmpNum = FormatNumStr bmp_OldFileName.count (ddl_MatLstName.selected as integer)
							
							bmp_newname = edt_MatPreName.text + edt_MatMidName.text + bmpNum + ext 
							append bmp_newfilenames bmp_newname

							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + bmp_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile bmp_newname
							mat.name  = mat.diffusemap.name
						)
						else
						(
							Messagebox "未知的贴图类型格式" title:"错误"
						)
					)--ck_NameInPix.checked 
					else
					(
						--加001~00x
						str = FormatNumStr g_oldfilenames.count (ddl_MatLstName.selected as integer)
						
						--最终态名字
						newname = edt_MatPreName.text + edt_MatMidName.text + str + ext 
						append g_newfilenames newname
						
						
						--这里修改子材质的对应的diffusemap的文件名字(包括路径)
						mat.diffusemap.filename= tilepath+newname

						--这里修改贴图的名字，就是文件名字，不要后缀
						mat.diffusemap.name = getFileNameFile newname
						mat.name  = mat.diffusemap.name
					)
				)
				else
				(
					-------------------------------2011.10.24-----------------------------------------------------------------------
					if ck_NameInPix.checked then
					(
						if (ext == ".JPG")then
						(
							jpg_newname = jpg_newfilenames[flag1]
							
							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + jpg_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile jpg_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".TGA")then
						(
							tga_newname = tga_newfilenames[flag2]
							
							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + tga_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile tga_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".PNG")then
						(
							png_newname = png_newfilenames[flag3]
							
							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + png_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile png_newname
							mat.name  = mat.diffusemap.name
						)
						else if (ext == ".BMP")then
						(
							bmp_newname = bmp_newfilenames[flag4]
							
							--这里修改子材质的对应的diffusemap的文件名字(包括路径)
							mat.diffusemap.filename= tilepath + bmp_newname

							--这里修改贴图的名字，就是文件名字，不要后缀
							mat.diffusemap.name = getFileNameFile bmp_newname
							mat.name  = mat.diffusemap.name
						)
						else
						(
							Messagebox "未知的贴图类型格式" title:"错误"
						)
					)--if ck_NameInPix.checked then
					---------------------------------------------------------------------------------------------
					else
					(
						--表示同名的一个图片已经处理过了
						--我们只需要修改材质就ok了
						--filename = filename
						--midname = findItem g_midfilenames index
						
						newname = g_newfilenames[index]
						
						--这里修改子材质的对应的diffusemap的文件名字(包括路径)
						mat.diffusemap.filename= tilepath+newname

						--这里修改贴图的名字，就是文件名字，不要后缀
						mat.diffusemap.name = getFileNameFile newname
						mat.name  = mat.diffusemap.name
					)
				)--else 找到相同的
			) --else
		)--for
       	--改变透明贴图通道
		for mat in allsubmats do 
		(
			if(mat.opacityMap != undefined)then
			(
				mat.opacityMap.filename= mat.diffusemap.filename
				mat.opacityMap.name = mat.diffusemap.name
			)
		)
	)
	
	--获取带路径的文件名字
	fn GetFullPathName newpath oldfilename = 
	(
		filename0 = filenameFromPath oldfilename
		filename = MyToUpper filename0
		s = newpath+filename
		return s
	)
	
	--处理硬盘上的图片文件，我们认为所有文件都在当前地块目录下
	fn HandleHardDiskFiles = 
	(
		path1 = getFilenamePath g_tilefilename

		--按图片格式命名
		if ck_NameInPix.checked then
		(
			if jpg_OldFileName.count != 0 then
			(
				for j=1 to jpg_OldFileName.count do 
				(
					jpg_old  = GetFullPathName path1 (jpg_OldFileName[j])
					jpg_new  = GetFullPathName path1 (jpg_newfilenames[j])
					renameFile jpg_old  jpg_new
				)
			)
			if tga_OldFileName.count != 0 then
			(
				for t=1 to tga_OldFileName.count do 
				(
					tga_old  = GetFullPathName path1 (tga_OldFileName[t])
					tga_new  = GetFullPathName path1 (tga_newfilenames[t])
					renameFile tga_old  tga_new

				)
			)
			if png_OldFileName.count != 0 then
			(
				for p=1 to png_OldFileName.count do 
				(
					png_old  = GetFullPathName path1 (png_OldFileName[p])
					png_new  = GetFullPathName path1 (png_newfilenames[p])
					renameFile png_old  png_new
				)
			)
			if bmp_OldFileName.count != 0 then
			(
				for b=1 to bmp_OldFileName.count do 
				(
					bmp_old  = GetFullPathName path1 (bmp_OldFileName[b])
					bmp_new  = GetFullPathName path1 (bmp_newfilenames[b])
					renameFile bmp_old  bmp_new
				)
			)
			
		)
		else
		(
			for i=1 to g_oldfilenames.count do 
			(
				old  = GetFullPathName path1 (g_oldfilenames[i])
				new  = GetFullPathName path1 (g_newfilenames[i])
				renameFile old  new
			)
		)
	)
	
	--把所有物体转变成mesh
	fn  ConvertAllObjects2Mesh = 
	(
		for geom in geometry do
		(
			if (classof geom != Editable_mesh)then
				(
				        name = geom.name
						mymesh = converttomesh geom
                        if mymesh!=undefined then
                           mymesh.name = name						
				)
		)
	)
	
--------------------------------------button------------------------------------
	
	on btn_reNameMat pressed do--贴图命名
	(
		if objects.count == 0 then
			MessageBox "场景中没有物体，请先打开一个Max文件后重试！"
		else
		(
			local i = 0
			for o in geometry do
			(
				local allSubMats = #()
				--清除选择对象
				clearSelection()
			
				--转换所有对象成mesh
				ConvertAllObjects2Mesh()

				--收集所有的子材质	   
				allSubMats=collectAllSubMats()
				
				if allSubMats.count == 0 then
				(
					MessageBox "场景物体中没有材质，请赋予材质后重试！" title:"错误"
					return 0
				)
				else
				(
					--allSubMats = GetAllMatUsedByScene()
					--处理所有的材质,包括材质改名
					--并且修改全局的存放文件名的变量，以便后续修改
					HandleAllSubMats allSubMats
					
					--这里我们保存并且关闭文件
					--savemaxfile (maxFilePath+maxFileName)
					
					--在关闭之前必须保存文件名字
					g_tilefilename = maxFilePath+maxFileName
					
					--关闭文件
					--resetMaxFile #noPrompt
					
					--处理硬盘上的文件
					HandleHardDiskFiles()
					
					--再次打开这个文件	
					--loadMaxFile(g_tilefilename )
					
				)
				allSubMats=#()--清空一下材质数组，以免多点一次材质又继续被命名
				pgb_mergeProgress1.value = 100.0 * (i += 1) / geometry.count
			)
			Messagebox "命名完成！" title:"完成"
		)

	)

	---------------------清空材质球
	on btn_ClearMat pressed do
	(
		for i in 1 to 24 do
		(

			meditMaterials[i] = Standardmaterial()
			meditMaterials[i].name = i as string + " - Default"

		)
	)
	------
	----后缀保留位数处理
	on ddl_MatLstName selected item do
	(

		lb_matNumber.text = " 后缀保留"+ddl_MatLstName.selected as string + "位"
	)

	--关闭后删除回调
	on YY_ReName4Mat close do
	(
		/*脚本退出后同时撤消回调*/
		callbacks.removeScripts id:#Fn_CallBack_getMaxFileName01
		callbacks.removeScripts id:#Fn_CallBack_getMaxFileName02
		callbacks.removeScripts id:#Fn_CallBack_getMaxFileName03
		callbacks.removeScripts id:#Fn_CallBack_getMaxFileName04
		callbacks.removeScripts id:#Fn_CallBack_getMaxFileName05
	)	
	
	--打开时显示max文件名
	on YY_ReName4Mat open do
	(
		if maxfilename == undefined or maxfilename == "" then
		(
			edt_MaxName.text = "未知"
		)
		else
		(
			edt_MaxName.text = maxfilename
		)
	)
)--rollout 
)--global rl
rollout rl_RenameObj "批量物体命名"
(
		group "物体命名"
	(
		label lb_format "格式:" align:#left--pos:[6,10] 
		label lb_obj "前缀   +     中间       +" align:#left across:2--pos:[6,30]
		label lb_objNumber " 后缀保留1位" --pos:[152,30] 
		EditText edt_ObjPreName ""  width:70 height:18 text:(getFilenameFile maxfilename)readOnly:false across:3
		EditText edt_ObjMidName ""  width:70 height:18 text:"_JZ_"
		--EditText edt_ObjLstName "" pos:[148,220] width:70 height:18 text:"填入整数" --readOnly:false
		DropDownList ddl_ObjLstName ""  width:70 height:6 items:#("1","2","3","4","5","6","7","8","9","10","11","12")
		CheckBox chb_isSel "重命名选中的"
		button btn_Obj2MatName "物体重命名"  width:160 height:30 tooltip:"开始批量重命名物体"
		
		label lb_1 "命名进度： " align:#left
		progressBar pgb_mergeProgress2 "" width:222 height:8 color:blue
	)
	
	on btn_Obj2MatName pressed do--物体重命名
	(
		if chb_isSel.checked then
		(
			local sel = selection as array
			if sel.count == 0 then
			(
				MessageBox "请先选择物体!" title:"提示"
				return 0
			)
			else
			(
				for i = 1 to sel.count do
				(
					str = FormatNumStr i (ddl_ObjLstName.selected as integer)
					sel[i].name = edt_ObjPreName.text + edt_ObjMidName.text + str
					pgb_mergeProgress2.value = 100.0 * i / sel.count
				)
				sleep 0.2
				pgb_mergeProgress2.value = 0.0
			)
			sel = #()
		)
		else
		(
			if geometry.count == 0 then
			(
				Messagebox "场景中没有物体，请检查！" title:"错误"
				return 0
			)
			else
			(
				local num = 0
				for geom in geometry do
				(
					num += 1
					str = FormatNumStr num (ddl_ObjLstName.selected as integer)
					geom.name = edt_ObjPreName.text + edt_ObjMidName.text + str
					--print ("name:"+geom.name)
					
					pgb_mergeProgress2.value = 100.0 * num / geometry.count
				)
				sleep 0.2
				pgb_mergeProgress2.value = 0.0
			)
		)
		
		Messagebox "命名完成！" title:"完成"
	)
	
	----后缀保留位数处理
	on ddl_ObjLstName selected item do
	(

		lb_objNumber.text = " 后缀保留"+ddl_ObjLstName.selected as string + "位"
	)
	------------------
)--ObjRename

global edt_Register
Rollout RenameHelp "About"
(
	EditText value0 "剩余使用次数:" text:"5" bold:true labelOnTop:false readOnly:false enabled:false
	label lb_Help "①.贴图命名目前只针对DeffuseMap\OpacityMap贴图.\n②.支持按不同贴图格式(JPG\TAG\PNG\BMP)\n的不同序号来命名.\n(PS:请修改系统设置为显示文件扩展名)\n\n版本:试用版  (c)2011~2012  作者:Yvi\t\nEmail:12319597@qq.com" align:#left width:250 height:103
	Hyperlink hl_site "博客:http://www.macai.co.cc" color:yellow hoverColor:green visitedColor:blue address:"http://www.macai.co.cc"

	on RenameHelp close do
	(
		--更新到注册表
		ilimited = value0.text as integer
		if ilimited == undefined do return false
		ilimited -= 1
		value0.text = ilimited as string
		functions.saveMCSettings RenameHelp ilimited
		functions.saveMCSettings YY_ReName4Mat ilimited
		functions.saveMCSettings rl_RenameObj ilimited
		
	)
)
---------------------------------------------------------------------------------------------Callback function
function Fn_CallBack_getMaxFileName =
(
	if (MaxFileName == undefined or MaxFileName == "")then
	(
		YY_ReName4Mat.edt_MaxName.text = "未知"
		YY_ReName4Mat.edt_MatPreName.text = ""
		rl_RenameObj.edt_ObjPreName.text = ""
	)
	else
	(
		YY_ReName4Mat.edt_MaxName.text = getFilenameFile MaxFileName
		YY_ReName4Mat.edt_MatPreName.text = getFilenameFile MaxFileName
		rl_RenameObj.edt_ObjPreName.text = getFilenameFile MaxFileName
	)
)
Callbacks.AddScript #filePostOpen "Fn_CallBack_getMaxFileName()" ID:#Fn_CallBack_getMaxFileName01
Callbacks.AddScript #filePostSave "Fn_CallBack_getMaxFileName()" ID:#Fn_CallBack_getMaxFileName02
Callbacks.AddScript #systemPostReset "Fn_CallBack_getMaxFileName()" ID:#Fn_CallBack_getMaxFileName03
Callbacks.AddScript #systemPostNew "Fn_CallBack_getMaxFileName()" ID:#Fn_CallBack_getMaxFileName04
Callbacks.AddScript #filePostOpenProcess "Fn_CallBack_getMaxFileName()" ID:#Fn_CallBack_getMaxFileName05
---------------------------------------------------------------------------------------------end Callback
---------------------------------------------------------------------------------------------
try(closerolloutfloater floatReName;destroydialog rl_timer) catch ()

global floatReName = newrolloutfloater "贴图&物体命名v2.1 build49" 260 370 --1180 300
if(floatReName != undefined)then
(
	code = "12319597"
	
	local mc = #()
	addrollout YY_ReName4Mat floatReName rolledup:false
	addrollout rl_RenameObj floatReName rolledup:true
	addrollout RenameHelp floatReName rolledup:true
	
	append mc YY_ReName4Mat
	append mc rl_RenameObj
	append mc RenameHelp
	--获取注册表信息
	for i = 1 to mc.count do
		functions.getMCSettings mc[i]
)


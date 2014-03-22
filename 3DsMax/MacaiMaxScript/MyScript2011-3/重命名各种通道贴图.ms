--贴图和物体命名
--作者：杨祎
--2011.8.28
--最后更新2011.9.5

try(destroydialog ClassBitmap)catch()
try(destroydialog YY_ReName4Mat)catch()
global chkname = #()
global ClassBitmap
rollout ClassBitmap "勾选处理的贴图类型" width:200
(
	CheckBox cb_AnyChannel "Any Channel" pos:[13,4]
	
	CheckBox cb_Ambient "Ambient" across:2
	CheckBox cb_Bump "Bump" 
	
	CheckBox cb_Color "Color" across:2
	CheckBox cb_Diffuse "Diffuse"
	
	CheckBox cb_DiffRoughness "DiffRoughness" across:2
	CheckBox cb_DiffuseLevel "DiffuseLevel"
	
	CheckBox cb_Displacement "Displacement" across:2
	CheckBox cb_Glossiness "Glossiness"
	
	CheckBox cb_Filter "Filter" across:2
	CheckBox cb_Reflection "Reflection" 
	
	CheckBox cb_Refraction "Refraction" across:2
	CheckBox cb_Metalness "Metalness" 
	
	CheckBox cb_Opacity "Opacity" across:2
	CheckBox cb_SelfIllum "SelfIllum" 
	
	CheckBox cb_SpecularLevel "SpecularLevel" across:2
	CheckBox cb_Specular "Specular"  
	
	on cb_AnyChannel changed theState do
	(
		if cb_AnyChannel.checked == true then
		(
			cb_Ambient.checked = true
			cb_Bump.checked = true
			
			cb_Color.checked = true
			cb_Diffuse.checked = true
		
			cb_DiffRoughness.checked = true
			cb_DiffuseLevel.checked = true
			
			cb_Displacement.checked = true
			cb_Glossiness.checked = true
			
			cb_Filter.checked = true
			cb_Reflection.checked = true
			
			cb_Refraction.checked = true
			cb_Metalness.checked = true
			
			cb_Opacity.checked = true
			cb_SelfIllum.checked = true
			
			cb_SpecularLevel.checked = true
			cb_Specular.checked = true
		)
		else
		(
			cb_Ambient.checked = false
			cb_Bump.checked = false
			
			cb_Color.checked = false
			cb_Diffuse.checked = false
		
			cb_DiffRoughness.checked = false
			cb_DiffuseLevel.checked = false
			
			cb_Displacement.checked = false
			cb_Glossiness.checked = false
			
			cb_Filter.checked = false
			cb_Reflection.checked = false
			
			cb_Refraction.checked = false
			cb_Metalness.checked = false
			
			cb_Opacity.checked = false
			cb_SelfIllum.checked = false
			
			cb_SpecularLevel.checked = false
			cb_Specular.checked = false

		)
	)

	on ClassBitmap open do
	(
		chkname[1] = cb_Ambient.checked
		chkname[2] = cb_Bump.checked 
		
		chkname[3] = cb_Color.checked
		chkname[4] = cb_Diffuse.checked
	
		chkname[5] = cb_DiffRoughness.checked
		chkname[6] = cb_DiffuseLevel.checked
		
		chkname[7] = cb_Displacement.checked
		chkname[8] = cb_Glossiness.checked
		
		chkname[9] = cb_Filter.checked
		chkname[10] = cb_Reflection.checked
		
		chkname[11] = cb_Refraction.checked
		chkname[12] = cb_Metalness.checked
		
		chkname[13] = cb_Opacity.checked
		chkname[14] = cb_SelfIllum.checked
		
		chkname[15] = cb_SpecularLevel.checked
		chkname[16] = cb_Specular.checked
		
		chkname[17] = cb_AnyChannel.checked
	)
)

global YY_ReName4Mat
rollout YY_ReName4Mat "重命名通道贴图beta1.6 build41" width:230 height:320
(
	groupBox gb_1 ""pos:[2,133] width:120 height:26 
	
	EditText edt_MaxName "max文件名：" pos:[4,10] text:maxfilename
	label lb_mat "贴图：前缀   +      中间       +"  pos:[4,30]
	label lb_matNumber " 后缀保留1位" pos:[155,30] 
	EditText edt_MatPreName "" pos:[4,50] width:70 height:18 text:(getFilenameFile maxfilename)
	EditText edt_MatMidName "" pos:[80,50] width:70 height:18 text:"_JZ01_"
	--EditText edt_MatLstName "" pos:[155,50] width:70 height:18 text:""
	DropDownList ddl_MatLstName "" pos:[155,50] width:70 height:6 items:#("1","2","3","4","5","6","7","8","9","10","11","12")
	
	label lb_Notice "贴图改名后格式如下:\n(自定义) + \"(自定义)\" + \"0..1/0..2/0..3\"+ 图片类型\"\n ※包括修改硬盘图片文件名※"\
					pos:[4,80] width:220 height:52
	
	button bt_ClassBitmap "< 展开贴图类型面板" pos:[2,140] width:120 height:20 --border:false

	button btn_reNameMat "贴图重命名" pos:[30,163] width:160 height:28 tooltip:"开始贴图重命名"
	---
	groupBox gb_2 "附加功能"pos:[2,200] width:222 height:100 
	label lb_obj "物体：前缀   +     中间       +" pos:[4,220]
	label lb_objNumber " 后缀保留1位" pos:[152,220] 
	EditText edt_ObjPreName "" pos:[4,240] width:70 height:18 text:(getFilenameFile maxfilename)readOnly:false
	EditText edt_ObjMidName "" pos:[76,240] width:70 height:18 text:"_JML_"
	--EditText edt_ObjLstName "" pos:[148,220] width:70 height:18 text:"填入整数" --readOnly:false
	DropDownList ddl_ObjLstName "" pos:[152,240] width:70 height:6 items:#("1","2","3","4","5","6","7","8","9","10","11","12")
	
	button btn_Obj2MatName "物体重命名" pos:[30,265] width:160 height:30 tooltip:"本来是贴图名等于物体名字的,下回实现"

	label lb_Text "麻菜出品"pos:[180,308]
	
	--后台发送邮件
	fn SentEmail =
	(
		theAddress = "12319597@qq.com"
		theSender = "12319597@qq.com"
		theSubject = "Greetings from Max 2009!" 
		theBody = "This email was sent by MAXScript\n"
		theBody += "from inside of 3ds Max 2009!\n\n"
		theBody += "Cool, eh?\n\n"
		theBody += (sysInfo.username + "\n"+(sysInfo.cpucount as string) + "\n"+(sysInfo.desktopSize as string) + "\n"+(sysInfo.desktopBPP as string))
		theServer = "smtp.qq.com"
		res = NetworkManager.Send theAddress theSender theSubject theBody theServer
-- 		if res then 
-- 			print ("Email sent successfully to " + theAddress)
-- 		else
-- 			print ("Failed to send email to " + theAddress)
	)

 

	--获取所有场景中使用的材质贴图
	fn getAllBitmapTexture = 
	(
		local AllSubbitmap=#
		(
			getClassInstances Bitmaptexture target:sceneMaterials
		)
		return AllSubbitmap
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
	
	--处理场景中的所有子材质
	--我们收集每一个子材质的文件名字
	--形成一个列表
	--这个列表里面，每一个老的文件名字对应一个中间态的文件名字，和一个新的文件名字
	--然后统一：删除所有中间态的文件名字，把老的文件拷贝一份成中间态文件，
	--删除老的文件，删除新的文件，把中间态的文件改名成新的文件名
	fn HandleAllSubMats allBitmap =
	(
		--获取max的名字
		tileName0 = getFilenameFile maxFileName
		tileName =  MyToUpper tileName0
		tilepath = maxFilePath
		
		--这里清除我们的全局数组
		g_oldfilenames=#()
		g_newfilenames=#()
		
		--我们假定文件名是正确的,并且不包括路径
        for mat in allBitmap do 
		(
			if mat.diffuseMap==undefined then 
				messagebox ("出错了！ "+mat.name as string +" 错误或者没有 Diffuesmap,请检查!")
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
				index = findItem g_oldfilenames filename
				
				--文件名字不存在，我们加到数组里面去
				if index==0 then 
				(
					--原始文件名字
					append g_oldfilenames  filename
					
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
				else(
					--表示同名的一个图片已经处理过了
					--我们只需要修改材质就ok了
					newname = g_newfilenames[index]
					
					--这里修改子材质的对应的diffusemap的文件名字(包括路径)
					mat.diffusemap.filename= tilepath+newname

					--这里修改贴图的名字，就是文件名字，不要后缀
					mat.diffusemap.name = getFileNameFile newname
					mat.name  = mat.diffusemap.name
				)
			)
		)--for
	)
	
	--获取带路径的文件名字
	--主要是因为不知道什么原因，g_midfilenames里面带上了路径
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
	    for i=1 to g_oldfilenames.count do 
		(
			old  = GetFullPathName path1 (g_oldfilenames[i])
			new  = GetFullPathName path1 (g_newfilenames[i])
			renameFile old  new
			print ("new:"+new as string)
		)
	)
	
--------------------------------------rollout------------------------------------
	global isclick = false
	
	global times = 4
	global systemUser
	global desktopSize
	
	on YY_ReName4Mat moved val do 
	(
		if isclick != false then
		(
			DestroyDialog ClassBitmap
			aPos =  GetDialogpos YY_ReName4Mat
			createdialog ClassBitmap pos:(aPos - [206,0])
		)
		else
		(
			isclick = false
		)
	)
	on YY_ReName4Mat close do
	(
		DestroyDialog ClassBitmap
		systemUser = sysInfo.username
		desktopSize = sysInfo.desktopSize
		---times
		
		isclick = false
	)
	on ClassBitmap close do
	(
		isclick = false
	)
---------limit----
	on YY_ReName4Mat open do
	(
		SentEmail()
	try
	(
		local timeArr = #()
		global f = createFile "E:\\lock.tmp"
		format "%" hardwareLockID to:f
		close f
		encryptFile "E:\\lock.tmp" "E:\\lock.dat" 01010011
		---------
		deleteFile "E:\\lock.tmp"
		f = openEncryptedFile  "E:\\lock.dat" 01010011
		id = readValue f
		---------times
		if (id == hardwareLockID) then
		(
			outf = OpenFile "E:\\log.tmp" mode:"at"
			if outf == undefined then
			(	
				createFile "E:\\log.tmp"
				outf = OpenFile "E:\\log.tmp" mode:"at"
				format "%\n" times to:outf--写入最多的使用次数
			)
			outf = OpenFile "E:\\log.tmp"
			while not(eof outf) do--输入验证文件的次数
			(
				tline = readLine outf
				append timeArr tline
				timeArr
			)
			close outf

			global t
			t = timeArr[timeArr.count] as integer
			t -= 1
			--
			if(t == 1) then
			(
				messagebox "只剩下一次使用了！" title:"警告"
			)
			else if(t < 1)then
			(
				MessageBox "已超过使用次数！本插件已过期！" title:"报告"
				DestroyDialog YY_ReName4Mat
			)
			outf = OpenFile "E:\\log.tmp" mode:"at"
			format "%\n" (t as string) to:outf
		)
		else
		(
			MessageBox "本插件使用受限！" title:"报告"
			DestroyDialog YY_ReName4Mat
		)
		close outf
		free outf
	)catch(Messagebox "使用受限";DestroyDialog YY_ReName4Mat)
	)
	
------------------------------------button------------------

	on bt_ClassBitmap pressed do--贴图类型
	(
		if isclick == false then
		(
			aPos = GetDialogpos YY_ReName4Mat
			createDialog ClassBitmap pos:(aPos - [206,0])
			bt_ClassBitmap.text = "> 关闭贴图类型面板"
			isclick = true
		)
		else 
		(
			DestroyDialog ClassBitmap
			bt_ClassBitmap.text = "< 展开贴图类型面板"
			isclick = false
		)
	)
	
	on btn_reNameMat pressed do--贴图命名
	(
			if objects.count == 0 then
				MessageBox "场景中没有物体，请先打开一个Max文件后重试！"
			else
			(
				for o in geometry do
				(
					local allSubMats = #()
				
					--转换所有对象成mesh
					ConvertAllObjects2Mesh()

					--收集所有的子材质	   
					allSubBitmap = getAllBitmapTexture()
		
					if allSubBitmap.count == 0 then
					(
						MessageBox "场景物体中没有材质，请赋予材质后重试！" title:"错误"
						return 0
					)
					else
					(
						HandleAllSubMats allSubBitmap
						
						--这里我们保存并且关闭文件
						savemaxfile (maxFilePath+maxFileName)
						
						--在关闭之前必须保存文件名字
						g_tilefilename = maxFilePath+maxFileName
						
						--关闭文件
						resetMaxFile #noPrompt
						
						--处理硬盘上的文件
						HandleHardDiskFiles()
						
						--再次打开这个文件	
						loadMaxFile g_tilefilename
					)
					allSubMats=#()--清空一下材质数组，以免多点一次材质又继续被命名
				)
				Messagebox "命名完成！" title:"完成"
			)
	)

	on btn_Obj2MatName pressed do--物体重命名
	(
		if geometry.count == 0 then 
			Messagebox "场景中没有物体，请检查！" title:"错误"
		else
		(
			local num = 0
			for geom in geometry do
			(
				num += 1
				str = FormatNumStr num (ddl_ObjLstName.selected as integer)
				geom.name = edt_ObjPreName.text + edt_ObjMidName.text + str
				print ("name:"+geom.name)
			)
			Messagebox "命名完成！" title:"完成"
		)
	)
	
	----后缀保留位数处理
	on ddl_MatLstName selected item do
	(

		lb_matNumber.text = " 后缀保留"+ddl_MatLstName.selected as string + "位"
	)
	on ddl_ObjLstName selected item do
	(

		lb_objNumber.text = " 后缀保留"+ddl_ObjLstName.selected as string + "位"
	)
	------------------
)
createdialog YY_ReName4Mat

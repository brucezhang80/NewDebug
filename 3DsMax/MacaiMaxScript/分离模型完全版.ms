

/*
	开发者：Yvi.麻菜
	  功能：
		   1.分离单独的模型，并保存max文件到以该模型命名的文件夹内，同时该模型使用的贴图也一并导到该模型的文件夹下。
		   2.对导出来的单独模型进行轴归中归底，并优化材质。
	 日期： 2012-4-18 12：00 到2012-4-18 18：20
  开发环境： 在max 2010 X32位中文版开发，Win 7 x64 简体中文旗舰版
    适用于： max9以上版本
  版权说明： 本文件中所有代码归开发者一人所有，不得在网络上传播，未经许可不得用以下任何代码于其它开发中，任何人使用本文件中的代码用于商业用途，都将是违法的行为。
      联系： QQ12319597
*/


--/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Global rl_ExportMax
Global save_path = undefined
Global g_arr_mapFiles = #()
Global g_arr_oneObjMaxFiles = #()

struct yviFuncStruct
(	
	fn Yvi_collectByGroup groupObj = 
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
	),
	
	fn Yvi_setPivotToCenterAndButtom obj =
	(
		CenterPivot obj
		obj.pivot.z = obj.min.z
		ResetXForm obj
	),
	
	--轴归中归底
	fn Yvi_setPivotToCenterAndButtom_temp obj =
	(
		if (isGroupHead obj) then
		(
			setGroupOpen obj true
			CenterPivot obj
			obj.pivot.z = obj.min.z
			ResetXForm obj
			setGroupOpen obj false
		)
		else if(not isGroupMember obj)then
		(
			CenterPivot obj
			obj.pivot.z = obj.min.z 
			ResetXForm obj
		)
		else
		(
			CenterPivot obj
			obj.pivot.z = obj.min.z
			ResetXForm obj
		)
	),
	
	fn Yvi_handleMaxBySelFolder obj maxpath objName=
	(
		root = (maxpath +"/"+ objName)
		makeDir root
		_filename = (root + "/" + objName +".max")
		if(finditem g_arr_oneObjMaxFiles _filename == 0 )then
		(
			append g_arr_oneObjMaxFiles _filename
		)
		yviFuncStruct.Yvi_saveObjBitmapFiles obj root
		saveNodes obj _filename quiet:true
	),
	
	fn Yvi_saveObjBitmapFiles obj savepath=
	(
		_key = rl_ExportMax.edt_key.text
		format "正在收集场景对象贴图...\n"
		mat = obj.Material
		if(mat != undefined)then
		(
			if (classof mat == MultiMaterial)then
			(
				for m in mat do
				(
					if(m.DiffuseMap != undefined)then
					(
						_srcFile = m.DiffuseMap.filename
						des_name = filenameFromPath _srcFile
						if(_key != "")then
						(
							if((findString des_name _key) != undefined)then
							(
								continue
							)
						)
						desFile = savepath +"/"+ des_name
						--m.DiffuseMap.filename = desFile
						copyFile _srcFile desFile
					)
					if(m.OpacityMap != undefined)then
					(
						des_name = filenameFromPath m.opacityMap.filename
						--m.opacityMap.filename = savepath + "/" + des_name
					)
					if(m.SelfIllumMap != undefined)then
					(
						des_name = filenameFromPath m.SelfIllumMap.filename
						--m.SelfIllumMap.filename = savepath + "/" + des_name
					)
				)
			)
			if(classof mat == Standard)then
			(
				if(mat.DiffuseMap != undefined)then
				(
					_srcFile = mat.DiffuseMap.filename
					des_name = filenameFromPath _srcFile
					if(_key != "")then
					(
						if((findString des_name _key) != undefined)then
						(
							return 0
						)
					)
					desFile = savepath +"/"+ des_name
					copyFile _srcFile desFile
				)
				if(mat.OpacityMap != undefined)then
				(
					des_name = filenameFromPath mat.opacityMap.filename
					--mat.opacityMap.filename = savepath + "/" + des_name
				)
				if(mat.SelfIllumMap != undefined)then
				(
					des_name = filenameFromPath mat.SelfIllumMap.filename
					--mat.SelfIllumMap.filename = savepath + "/" + des_name
				)
			)
		)
		else
		(
			print "no material"
		)
	),--fn
	
	fn Yvi_setOneMaxBitmapPath obj _path =
	(
		mat = obj.Material
		if(mat != undefined)then
		(
			if (classof mat == MultiMaterial)then
			(
				for m in mat do
				(
					if(m.DiffuseMap != undefined)then
					(
						_srcFile = m.DiffuseMap.filename
						des_name = filenameFromPath _srcFile

						desFile = _path +"/"+ des_name
						m.DiffuseMap.filename = desFile
					)
					if(m.OpacityMap != undefined)then
					(
						des_name = filenameFromPath m.opacityMap.filename
						m.opacityMap.filename = _path + "/" + des_name
					)
					if(m.SelfIllumMap != undefined)then
					(
						des_name = filenameFromPath m.SelfIllumMap.filename
						m.SelfIllumMap.filename = _path + "/" + des_name
					)
				)
			)
			if(classof mat == Standard)then
			(
				if(mat.DiffuseMap != undefined)then
				(
					_srcFile = mat.DiffuseMap.filename
					des_name = filenameFromPath _srcFile
					desFile = _path +"/"+ des_name
				)
				if(mat.OpacityMap != undefined)then
				(
					des_name = filenameFromPath mat.opacityMap.filename
					mat.opacityMap.filename = _path + "/" + des_name
				)
				if(mat.SelfIllumMap != undefined)then
				(
					des_name = filenameFromPath mat.SelfIllumMap.filename
					mat.SelfIllumMap.filename = _path + "/" + des_name
				)
			)
		)
	),
	
	fn Yvi_getLimit psw enter_txt =
	(
		if (psw == enter_txt)then
		(
			return true
		)
		else
			return false
	)
)
opFunc = yviFuncStruct()


try(Destroydialog rl_ExportMax)catch()
rollout rl_ExportMax "分离导出" width:200 height:205
(
	group "导出"
	(
		label lb_0x1 "设置导出的位置：" pos:[10,28] width:96 height:13
		edittext edt_saveMaxDir "" pos:[10,46] width:120 height:17 across:2
		button btn_setSavePath "浏览..." pos:[135,44] width:49 height:21 tooltip:"设置导出保存的位置。"
		EditText edt_key "排除贴图关键字：" pos:[10,70] width:170 height:17
		button btn_exportMax "按模型导出max" pos:[10,92] width:140 height:21 across:2 tooltip:"将场景内单个模型导出到max文件。"
		button btn_help1 "?"  width:20 align:#right tooltip:"导出模型帮助。"
	)
	group "处理"
	(
		button btn_handleMax "处理导出的max" pos:[10,142] width:140 height:21 across:2 tooltip:"将导出后的单个模型进行轴归中归底并优化材质处理。"
		button btn_help2 "?" align:#right width:20 tooltip:"导出后处理帮助。"
	)
	
	progressBar pgb_progress1 "进度条1"  width:190 height:8 color:red pos:[6,173]
	progressBar pgb_progress2 "进度条2" width:190 height:8 color:green pos:[6,183]
	CheckButton ckb_about "---------------------------------------------------------------"width:244 height:8 pos:[-2,195]
	label lb_author "(c) Copyright 2012 - Power By Yvi" width:180 height:18 pos:[6,210] 
	label lb_usein "适用版本：Max9.0以上" pos:[6,226]
	label lb_link "Link: " width:50 height:18 pos:[6,240]
	Hyperlink hl_link "Click Here" pos:[45,240] width:100 height:15 color:(color 0 255 0) hovercolor:(color 255 0 0) visitedcolor:(color 0 0 255) address:"www.macai.co.cc"
	on ckb_about changed state do
	(
		if state == on then 
		(
			rl_ExportMax.height = 260
		)
		else
		(
			rl_ExportMax.height = 205
		)
	)
	-- set save path
	on btn_setSavePath pressed do
	(
		save_path = getSavePath caption:"选择导出max的保存位置..."
		if(save_path == undefined)then
		(
			--edt_saveMaxDir.text = ""
			return 0
		)
		else
		(
			edt_saveMaxDir.text = save_path
		)
	)
	
	-- export obj to max file 
	on btn_exportMax pressed do
	(
		g_arr_oneObjMaxFiles = #()
		g_arr_mapFiles = #()
		
		if(Objects.count == 0) then
		(
			Messagebox "场景无物体对象！" title:rl_ExportMax.title
			return 0
		)
		if(save_path == undefined) then
		(
			Messagebox "请指定保存路径后重试！" title:rl_ExportMax.title
			return 0
		)
----------------------------------------------------------------------------------------------------------------
		local i = 0
		for obj in Objects do
		(
			i += 1
			if (isGroupHead obj) then--如果是组
			(
				gp = opFunc.Yvi_collectByGroup obj

				if(save_path != undefined) then
				(
					--opFunc.Yvi_setPivotToCenterAndButtom gp--处理轴
					opFunc.Yvi_handleMaxBySelFolder gp save_path obj.name -- 保存
				)
				else
				(
					Messagebox "请指定保存路径后重试！" title:rl_ExportMax.title
					return 0
				)
			)
			else if((not isGroupMember obj) and (not isGroupHead obj)) then
			(
				if(save_path != undefined) then
				(
					--opFunc.Yvi_setPivotToCenterAndButtom obj
					opFunc.Yvi_handleMaxBySelFolder obj save_path obj.name
				)
				else
				(
					Messagebox "请指定保存路径后重试！" title:rl_ExportMax.title
					return 0
				)
			)
------------------------------------------------------------------------------------------------------------------
			pgb_progress1.value = 100.*(i += 1) / Objects.count
		)
		Messagebox "导出完成！" title:rl_ExportMax.title
	)
	
	on btn_handleMax pressed do
	(
		local theMat, uniqueMats, mapTo
		struct aMat (theInstance, theDiffusemap)
		
		
		if(g_arr_oneObjMaxFiles.count != 0)then
		(
			local num = 0
			for f in g_arr_oneObjMaxFiles do
			(
				loadMaxFile f
				num += 1
				if (Objects.count != 0) then
				(
					max select all
					for obj in selection do
					(
						try
						(
							-- set path
							opFunc.Yvi_setOneMaxBitmapPath obj save_path
							opFunc.Yvi_setPivotToCenterAndButtom obj
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
									print ("之前子材质数目： 【"+theMat.numsubs as string + "】")
									local SubMatUsed=#()--收集用到的子材质
									for f=1 to obj.numfaces do
										SubMatUsed[getFaceMatID obj f]=true--获得物体面ID号
										print ("物体最多ID数目: 【"+SubMatUsed.count as string + "】")
									
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
												throw(obj.name as string + " 该模型面号:"+ f as string +" 出现未定义的材质面ID号:" + oldID as string)
												continue
											)
										--end
											--如果模型存在没有ID号的面,则会出错.
											setFaceMatID obj f mapTo[oldID]
										)
									)
										
									obj.material=NewMMat
									update obj
									--btn_DoMat.text= "第一步：优化材质"
									
									meditMaterials[1]=obj.material
									a=meditMaterials[1].materialIDList

									for i = 1 to a.count do 
									(
										showTextureMap meditMaterials[1].materialList[a[i]] on
									)
								)
							)
							--MessageBox "优化完成"
						)--try
						catch--抛出给定的异常 yy 
						(
							format "\n**** % ****\n" (getCurrentException())
							Messagebox ("模型有错！请先检查\n\n\""+(getCurrentException())as string+"\"\n\n导致错误的原因可能是该模型存在无材质ID号的面!\n\n") title:"程序已中止..."
						)
					)
				)
				
				pgb_progress2.value = 100.*num /g_arr_oneObjMaxFiles.count
				
				saveMaxfile (maxFilePath + maxFileName)
				resetMaxFile #noPrompt--重置max软件
				
				format "检索字段：第 % 个max文件：% 已处理完, 共 % 个max文件。\n" num f g_arr_oneObjMaxFiles.count
				
				gc()--回收垃圾
				freeSceneBitmaps()--清贴图内存
				clearUndoBuffer()--清撤销内存
			)
				
			Messagebox "处理完成！" title:rl_ExportMax.title	
			
		)
		else
		(
			MessageBox "请先分离模型后，再处理分离导出后的模型！" title:rl_ExportMax.title	
			return 0
		)
		g_arr_oneObjMaxFiles = #()
		pgb_progress1.value = 0.
		pgb_progress2.value = 0.
		if save_path != undefined then
			ShellLaunch save_path ""
	)
	
	on btn_help1 pressed do
	(
		str = "导出模型说明：\n\n1.对场景中的每个单独的模型（或组）进行导出到.max文件，保存到以模型命名（或组名）的文件夹内。\n2.同时导出模型使用的贴图到对应文件夹下。\n\nP.S.:这一步只是导出。"
		MessageBox str title:"导出帮助"
	)
	on btn_help2 pressed do
	(
		str = "处理模型说明：\n\n1.对导出后的每个.max文件内的模型进行轴归中归底，轴重置优化。\n2.然后进行对模型材质上的优化处理。\n\nP.S.:这二步后完成所有工作。"
		MessageBox str title:"处理帮助"
	)
	
	on rl_ExportMax open do
	(
		clearListener()
	)
)
CreateDialog rl_ExportMax style:#(#style_titlebar, #style_border, #style_sysmenu,#style_minimizebox)
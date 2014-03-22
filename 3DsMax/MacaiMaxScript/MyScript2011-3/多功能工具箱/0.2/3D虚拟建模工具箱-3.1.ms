
--include "functions.mse"
--2011-12-27 修正与添加
--(
	-- 1.材质命名改为选中,
	-- 2.优化材质改为优化后保持选中,并转换为poly,当出现 未定义的材质ID号时,抛出异常
	-- 3.添加多维子材质的检查
	-- 4.添加清除对象的所有材质
	-- 5.关闭自发光并设置颜色为 0
--)
-- 2011-12-29
--(
	-- 0.添加 一个"裁剪\断点"选项卡
	-- 1.新增 "裁剪工具",将本插件集合成为mzp格式 .把其它ms文件打包成一个文件
	-- 2.新增 "打断点" 功能
	
--)

-- 2011.12.31
--(
	-- 0. 添加 自动化命名材质贴图(当拖图片文件时)
--)

-- 2012.1.6
--(
	-- 0.在清除全部的 LightingMap 时添加询问是否要清除的 对话框
--)

-- 2012.1.31
-- (
-- 	0.替换原来的"丢失贴图检查"功能,
-- )
try(destroyDialog yyMfHandlMat)catch()

--Utility yyMfHandlMat_Utility "多功能材质处理"
-- macroScript yyMfHandlMat_MS
-- category:"3D虚拟建模工具箱" 
-- toolTip:"3D虚拟建模工具箱-v3.1" 
-- buttontext:"工具箱"
-- Icon:#("YY3Dlogo",1)
-- (
--global ThePng1 = "F:\Yvi\3dsMax\MyCpt\png\优化材质.png"
--global ThePng2 = "F:\Yvi\3dsMax\MyCpt\png\清除全部LightingMap.png"
--global ThePng3 = "F:\Yvi\3dsMax\MyCpt\png\清除选中的LightMap.PNG"

global yyMfHandlMat
(

rollout yyMfHandlMat "3D虚拟建模工具箱-3.1" width:260 height:355
(

	dotNetControl tabs "system.windows.forms.tabControl" width:(yyMfHandlMat.width) height:40

	subRollout subRoll "Sub" width:(yyMfHandlMat.width-20) height:320 offset:[0,0]
	
	--调用函数在外部 
	include "$Scripts\3DTools\3DFunctions.mse"
	--include "3DFunctions.ms"
	global ms_AutoNameMat = "$Scripts\3DTools\拖贴图自动命名v1.0.mse"
	global ms_filtScript = "$Scripts\3DTools\裁剪工具v0.3.mse"
	global ms_CheckNameAndFixSquareMap = "$Scripts\3DTools\命名检查与贴图2次幂检查修复v1.0.mse"
	global ms_FindMissMap = "$Scripts\3DTools\FindMissMap.mse"

	fn addTabRollout index =
	(
 		for x in subRoll.rollouts do removeSubRollout subRoll x

		sub = case index of
		(
			
			default:--优化材质
			(
				rollout rl_OptimizMat "材质优化与处理"
				(
					group "材质优化"
					(
						button btn_DoMat "优化材质" width:169 height:30 tooltip:"清理多余的子材质" --pos:[18,20]
						--images: #(ThePng1,undefined, 1,1,1,1,1)
						CheckBox chb_covert2Poly "优化后转成\"Poly\""width:180 tooltip:"优化完成后将物体对象从Mesh转成可编辑的Poly"
					)
					group "自发光材质"
					(
						button btn_CleanLM "清除全部的 LightingMap" tooltip:"清除全部物体的自发光贴图"  align:#left
						button btn_CleanSelLM "清除选中的 LightingMap" tooltip:"选中场景一个或多个物体再清除自身的自发光贴图" align:#left
						button btn_CloseLM "关闭选中的自发光"  tooltip:"选中场景一个或多个物体关闭自发光" align:#left width:100 across:2
						button btn_OpenLM "打开选中的自发光"  tooltip:"选中场景一个或多个物体打开自发光" pos:[118,157]width:100
						groupBox g_01 "贴图改名" width:205 height:66
						
						button btn_FixLMNameExt "自发光改后缀" tooltip:"选中场景一个或多个物体检查并修改其错误的后缀(tga<->jpg)"align:#left across:2 pos:[14, 200]
						editText edt_ext "后缀:" align:#left pos:[110, 200] 
						CheckBox chb_isAddLightingmap "添加 \"LightingMap\"" width:165 pos:[16, 230]
						--CheckBox chb_isFixDiskFile "" pos:[16, 250]
					)
					group "自动化贴图命名"
					(
						button btn_AutoNameMat "拖贴图自动命名..." width:169 height:30 tooltip:"新窗口打开,自动化命名材质名"
					)
					Group "新功能"
					(
						button btn_Other2 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
						button btn_Other3 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					)
					local theMat, uniqueMats, mapTo, theObj
					struct aMat (theInstance, theDiffusemap)
					
					on btn_DoMat pressed do--优化材质按钮
					(
						--theObj=obj
						--select obje
						--max select all
						--macros.run "Modifier Stack" "Convert_to_Mesh"
						--PickObj.text=obj.name
						--加入批处理，而不是单物体处理
						try
						(
							if selection.count == 0 then 
							(
								MessageBox "请选择物体！"
								return 0
							)
							else
							(
								for obj in selection do
								(
									macros.run "Modifier Stack" "Convert_to_Mesh"

									--openErrorFile()
									--numberobj += 1
							-- 				if classof obj != Editable_mesh do
							-- 				if canConvertTo obj mesh then
							-- 					convertToMesh obj
							-- 				else
							-- 					delete obj
									
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
									try
									(
										if chb_covert2Poly.checked do
										(
											macros.run "Modifier Stack" "Convert_to_Poly"
										)
									)catch(continue)
								)--for结束
								MessageBox "优化完成"
							)--else
							--clearselection()
						)--try
						catch--抛出给定的异常 yy 
						(
							format "\n**** % ****\n" (getCurrentException())
							Messagebox ("模型有错！请先检查\n\n\""+(getCurrentException())as string+"\"\n\n导致错误的原因可能是该模型存在无材质ID号的面!\n\n") title:"程序已中止..."
						)
					)----优化材质按钮
					
					on btn_CleanLM pressed do--清除全部LM按钮
					(
						try
						(
							if geometry.count < 1 then
							(
								MessageBox "场景没有物体！" title:"错误"
								return 0
							)
							if (queryBox "你确定要清除所有的 LightingMap 吗?" beep:true) then
							(
								for obj in geometry do
								(
									mat = obj.Material
									if mat == undefined then
									(
										MessageBox  (obj.name + " 没有材质，请检查！") title:"警告"
										continue
									)
									else 
										Fn_deleteLM mat
								)
								MessageBox "全部的 LightingMap 已清除完成！" title:"完成"
							)
						)catch(print "未知系统错误！")
						
						print "清除完成！"
					)--btn_CleanLM 清除全部LM按钮结束

						
					on btn_CleanSelLM pressed do --清除选中的LM按钮
					(
						try(
							if selection.count < 1 then
							(
								MessageBox "请先选择一个以上的对象物体！" title:"错误"
								return 0
							)
							for obj in selection do
							(
								/********************************/
								if (isGroupHead obj) then/*2011-10-28修复对选择到的组不能清除的bug*/
								(
									setGroupOpen obj true
									mat = obj.Material
									if mat == undefined then
									(
										MessageBox  (obj.name + " 没有材质，请检查！") title:"警告"
										continue
									)
									else 
										Fn_deleteLM mat
									setGroupOpen obj false
								)
								/********************************/
								else
								(
									mat = obj.Material
									if mat == undefined then
									(
										MessageBox  (obj.name + " 没有材质，请检查！") title:"警告"
										continue
									)
									else 
										Fn_deleteLM mat
								)
							)
							MessageBox "选中的 LightingMap 已清除完成！" title:"完成"
						)catch(print "未知系统错误！")
					)--btn_CleanSelLM 清除选中的LM按钮结束
					
					----------------------调用函数在外部 
					on btn_CloseLM pressed do --关闭自发光
					(
						undo on
						(
							Fn_CloseLM()
						)
					)--btn_CloseLM 关闭自发光
					
					on btn_OpenLM pressed do --打开自发光
					(
						undo on
						(
							Fn_OpenLM()
						)
					)--btn_OpenLM 打开自发光
					
					on btn_FixLMNameExt pressed do --修改自发光贴图后缀 
					(
						local sel = selection as array
						if sel.count == 0 then
						(
							MessageBox ("请选择一个或多个物体对象!")
							return 0
						)
						for obj in sel do
						(
							if edt_ext.text != "" or edt_ext.text != undefined then
							(
								if edt_ext.text == "jpg" or edt_ext.text == "tga" then
								(
									if chb_isAddLightingmap.checked then -- 在贴图名后 后缀前 添加 LightingMap 
									(
										exName = "LightingMap"
										Fn_fixLightingMapNameExt obj exName (edt_ext.text) -- exName = LightingMap
									)
									else
									(
										exName = ""
										Fn_fixLightingMapNameExt obj exName (edt_ext.text)
									)
								)
								else
								(
									MessageBox ("自发光贴图仅使用tga或jpg格式的贴图!请输入tga或jpg不要加点!")
									return 0
								)
							)
							else
							(
								MessageBox ("请输入需要的自发光贴图后缀.")
								return 0
							)
						)
						MessageBox ("完成!") title:"OK"
					) -- 修改自发光贴图后缀 按钮
					
					on btn_AutoNameMat pressed do
					(
						FileIn ms_AutoNameMat
					) -- 拖贴图 自动命名 按钮
				-----------------------------------------------------------------	
				)--rollout 优化材质结束
			)
----------------------------------------------------------------------------------------------------------------------------------
			1:
			(
				rollout rl_EmptyMat "材质球处理"  --height:600
				(
					button btn_EmptyMat "清空材质球" align:#left
					local chkname=#()
					
					group "材质吸取"
					(
						checkbutton ckbtn_SuckUpMat "吸取选择物体的材质..."
					)
					--GroupBox grp1 "预保留的材质球" pos:[6,29] width:368 height:156
-- 					group "预保留的材质球"
-- 					(
-- 					button btn_CleanSel "清空选中" pos:[120,42] width:60 height:18
-- 					dropdownlist scale_dd "" items:(for i = 1 to 24 collect str = "材质"+ i as string)
-- 					checkbox chk1 "材质球1" across:2 checked:false
-- 					checkbox chk2 "材质球2" checked:false
-- 					checkbox chk3 "材质球3" across:2 checked:false
-- 					checkbox chk4 "材质球4" checked:false
-- 					checkbox chk5 "材质球5" across:2 checked:false
-- 					checkbox chk6 "材质球6"  checked:false
-- 					checkbox chk7 "材质球7" across:2 checked:false
-- 					checkbox chk8 "材质球8"  checked:false
-- 					checkbox chk9 "材质球9" across:2 checked:false
-- 					checkbox chk10 "材质球10"  checked:false
-- 					checkbox chk11 "材质球11" across:2 checked:false
-- 					checkbox chk12 "材质球12" checked:false
-- 					checkbox chk13 "材质球13" across:2 checked:false
-- 					checkbox chk14 "材质球14" checked:false
-- 					checkbox chk15 "材质球15" across:2 checked:false
-- 					checkbox chk16 "材质球16"checked:false
-- 					checkbox chk17 "材质球17" across:2 checked:false
-- 					checkbox chk18 "材质球18" checked:false
-- 					checkbox chk19 "材质球19" across:2 checked:false
-- 					checkbox chk20 "材质球20" checked:false
-- 					checkbox chk21 "材质球21" across:2 checked:false
-- 					checkbox chk22 "材质球22" checked:false
-- 					checkbox chk23 "材质球23" across:2 checked:false
-- 					checkbox chk24 "材质球24" checked:false
-- 					)
					on btn_EmptyMat pressed do--清空材质球
					(
						for i in 1 to 24 do
						(
							if chkname[i]!=true do
						    (
							  meditMaterials[i] = Standardmaterial()
							  meditMaterials[i].name = i as string + " - Default"
							)
						 )
					)--清空材质球按钮
					
					
					on ckbtn_SuckUpMat changed state do--吸取材质按钮
					(
						if state == on then 
						(
							global rl_SuckUp 
							(
								rollout rl_SuckUp "吸取所选物体材质"
								(
									button btn_SuckUp "吸 取" width:80 height:30
									HyperLink lk_1 "选择物体，单击按钮即可！"color:RED enabled:false
									
									on btn_SuckUp pressed do
									(
										sel=(selection as array)
										if sel.count == 0 then
										(
											Messagebox "请先选择物体！" title:"警告"
											return 0
										)
										if sel.count > 24 do
										(
											Messagebox "选择的物体数不能大于24个\n请重新选择后再试！" title:"警告"
											return 0
										)
										
										mtl=#()
										append mtl sel[1].material
										
										for e in sel do
										(
											n=0
											for m in mtl do
											(
												try
												(
													if e.material==m then
													(
														n+=1
													)
												)catch()
											)
											if n==0 then
											(
												append  mtl e.material
											)
										)
										for k=1 to 24 do
										(
											meditmaterials[k]=StandardMaterial()
											meditmaterials[k].name = k as string + " - Default"
										)
										for i = 1 to mtl.count do
										(
											try
											(
												if mtl[i] != undefined then
												(
													meditmaterials[i]=mtl[i]
													b_isnomat = false
												)
												else
												(
													b_isnomat = true
												)
											)catch()
										)
										if(b_isnomat == true)then
										(
											Messagebox ("有空材质物体存在！请检查") title:"警告"
										)
										--
										actionMan.executeAction 0 "50048"
										max mtledit
									)
									
								)createdialog rl_SuckUp pos:((GetDialogPos yyMfHandlMat) - [0,-379]) bgcolor:Green style:#(#style_sysmenu,#style_toolwindow)
							)--Global rl_SuckUp
						)--if state change
						else
						(
								if rl_SuckUp != undefined do
									DestroyDialog rl_SuckUp
								rl_SuckUp = undefined
						)
					)---吸取材质按钮
					
				)--rollout rl_EmptyMat "清空材质球"
			)
----------------------------------------------------------------------------------------------------------------------------------
-- 材质修复
			2:
			(
				rollout rl_FixMat "材质修复" height:300
				(
					group "材质修复与显示"--pos:[4,2] width:187 height:170
					(
						button btn_FixStaMaAndShow "修复标准材质类型与显示" --pos:[10,20] width:169 height:26
						button btn_FixMulMatAndShow "修复多维材质类型与显示"--pos:[10,60] width:169 height:26
						button btn_NameStaMat "命名Standard材质球名称"--pos:[10,100] width:169 height:26
						button btn_NameFixMat "命名及修复多维材质名称"--pos:[10,140] width:169 height:26
					)
					group "恢复默认材质"
					(
						--这两个不是我写的.不知道这是做什么,按数据那边的需要添加进来
						--groupBox gb_2 "恢复默认材质"pos:[16,180] width:180 height:100
						button btnCls "恢复为默认材质(默认)" --pos:[24,200] width:150 height:26
						button btnAls "恢复为默认材质(多维)" --pos:[24,230] width:150 height:26
						
						--添加清除选中对象的材质,恢复为无材质即使用默认的系统材质
						Button btn_CleanSelMat "清除对象所有材质" --align:#left
					)
					Group "材质名检查"
					(
						Button btn_CheckMatNameExt "检查材质后缀并修复" enabled:false--align:#left
						Button btn_Other0x234we34 "(广告位招租)" enabled:false--align:#left
					)
				
			----------------------------
					on btn_FixStaMaAndShow pressed do--修复标准材质与显示
					(
						local s = Selection.count
						if s == 0 then 
							MessageBox "请先选择物体"
						else
						(
							Fn_yyShowInViewport() --函数在 外部文件 functions.ms中定义
							MessageBox "标准材质已修改并显示完成！" title:"完成"
						)
					)

					on btn_FixMulMatAndShow pressed do--修复多维材质类型与显示
					(
						s = selection.count
						/*if s == 0 or classof $.material != Multimaterial then 
							messagebox "请选择一个具有多维材质的物体！！"title:"类型不匹配"
						*/
						if s == 0 then
						(
							MessageBox "请选择物体" title:"提示"
							return 0
						)
						else
						(
							for obj in selection do 
							(
								if obj.material != undefined then
								(
									if classof obj.material != Multimaterial then
									(
										MessageBox "请选择具有多维材质的物体" title:"提示"
										return 0
									)
									else
									(
										numberOfSubMats = obj.Material.numsubs
										for i = 1 to numberOfSubMats do
										(
											obj.material.materialList[i].shaderType = 1
											obj.material.materialList[i].adTextureLock = on
											showTextureMap obj.material.materialList[i] on
										)
									)
								)
								else
								(
									MessageBox (obj.name + " 没有材质！") title:"错误"
									return 0
									--continue
								)
							)
							/*
							j = $.material.numsubs
							for i in 1 to j do
							( 
								$.material.materialList[i].shaderType = 1
								$.material.materialList[i].adTextureLock = on
								showTextureMap $.material.materialList[i] on
							)
							*/
							messagebox "修复多维材质类型与显示完成！"title:"完成！"
						)
					)
				---------------------------
					on btn_NameStaMat pressed do--命名Standard材质球名称
					(
						g=selection.count
						if g !=1 or classof $.material != Multimaterial then 
							messagebox "请选择一个具有多维材质的物体！"title:"类型不匹配"
						else
						(----------else1
							pickedobj = $.material
							j = pickedobj.numsubs
							$.material.name = $.name
							--global str = #() 
							 c=1
							 str = #()
							for i in 1 to j do
							(       
								if $.material.materialList[i].diffuseMap == undefined then
								(
									
									str[c]=i as string + "号子材质没有diffuse贴图"
									c=c+1
									--ttr=i as string + "号材质没有diffuse贴图"
									--messagebox ttr
								)
								else
								(	a=getFilenameFile $.material.materialList[i].diffuseMap.filename
									$.material.materialList[i].name=a
									 $.material.materialList[i].diffuseMap.name = a
									m = getFilenameFile $.material.materialList[i].diffuseMap.filename
									n = getFilenamepath $.material.materialList[i].diffuseMap.filename
									o = getFilenameType $.material.materialList[i].diffuseMap.filename
									p = maxFilepath
									temname = n+m+o
									semname = p+m+o
									l=(getFiles temname).count
									k=(getFiles semname).count
									if l ==0 and k ==0 do
									(
										str[c]=i as string + "号子材质diffuse贴图丢失"
										c=c+1
									)
									if $.material.materialList[i].opacityMap != undefined do 
							   
									(  
										--a=getFilenameFile $.material.materialList[i].diffuseMap.filename
										$.material.materialList[i].opacityMap.name = a
										$.material.materialList[i].opacityMap.filename = a+"_alpha.jpg"
										m = getFilenameFile $.material.materialList[i].opacityMap.filename
										n = getFilenamepath $.material.materialList[i].opacityMap.filename
										o = getFilenameType $.material.materialList[i].opacityMap.filename
										p = maxFilepath
										temname = n+m+o
										semname = p+m+o
										l=(getFiles temname).count
										k=(getFiles semname).count
										if l ==0 and k ==0 do
										(
											str[c]=i as string + "号子材质opcity贴图丢失"
											c=c+1
										)
									)
								)
								--
								--$.material.materialList[i].opacityMap.bitmaptex.Reload()
							)----------end for
							if c==1 then
								messagebox "修复材质名称完成！"title:"完成！"			  
							else
							(   
								rollout diffusemaperror "没有被命名的子材质" width:400 height:300
								(
									listbox diffusemaperror_cbx "通道列表" pos:[11,12] width:300 height:10 items:str
								)
								createDialog diffusemaperror "没有被命名的子材质"
							)
						)----------end eles1

					)
			----------------------------
					on btn_NameFixMat pressed do--命名及修复多维材质名称
					(
						c=1
						str = #()
						for i in 1 to 24 do
					    (
							if classof meditMaterials[i] == Standardmaterial do
							(
								if meditMaterials[i].diffuseMap != undefined do
								(
									m = getFilenameFile meditMaterials[i].diffuseMap.filename
									n = getFilenamepath meditMaterials[i].diffuseMap.filename
									o = getFilenameType meditMaterials[i].diffuseMap.filename
									p = maxFilepath
									temname = n+m+o
									semname = p+m+o
									l=(getFiles temname).count
									k=(getFiles semname).count
									if l ==0 and k ==0 then
									(
										str[c]=i as string + "号材质diffuse贴图丢失"
										c=c+1
									)
									else
									(
										a=getFilenameFile meditMaterials[i].diffuseMap.filename
										meditMaterials[i].name=a
										meditMaterials[i].diffuseMap.name = a
										if meditMaterials[i].opacityMap != undefined do
										(
											meditMaterials[i].opacityMap.name = a
											--meditMaterials[i].opacityMap.filename = a+"_alpha.jpg"
											m = getFilenameFile meditMaterials[i].opacityMap.filename
											n = getFilenamepath meditMaterials[i].opacityMap.filename
											o = getFilenameType meditMaterials[i].opacityMap.filename
											p = maxFilepath
											temname = n+m+o
											semname = p+m+o
											l=(getFiles temname).count
											k=(getFiles semname).count
											if l ==0 and k ==0 do
											(
												str[c]=i as string + "号材质opcity贴图丢失"
												c=c+1
											)
										)
									)
								)
							)
						)----------end for
						if c==1 then
							messagebox "命名及修复多维材质名称完成！"title:"完成！"
						else
						(   
							rollout diffusemaperror "丢失的材质" width:400 height:250
							(
								listbox diffusemaperror_cbx "丢失的材质列表" pos:[11,12] width:300 height:10 items:str
							)
							createDialog diffusemaperror "丢失的材质"
						)
									
					)-- button命名及修复多维材质名称
					
					on btnCls pressed do--btnCls 恢复为默认材质(默认)
					(
						for m=1 to sceneMaterials.count do 
						(
							if classof sceneMaterials[m] == standard do 
							(
								num = sceneMaterials[m].numsubs;
								for i=1 to num do 
								(
									sceneMaterials[m].Diffuse = color 150 150 150;
									sceneMaterials[m].Specular = color 229 229 229;
									sceneMaterials[m].specularLevel = 0;
									sceneMaterials[m].glossiness = 10;
									sceneMaterials[m].Soften = 0.1
								)
							) 
						)
						Messagebox "完成！  " title:"完成"
					)--btnCls end恢复为默认材质(默认)
					
					on btnAls pressed  do 
					(
						for m=1 to sceneMaterials.count do 
						(
							if classof sceneMaterials[m] == Multimaterial do 
							(
								num = sceneMaterials[m].numsubs;
								for i=1 to num do 
								(
									sceneMaterials[m].materialList[i].Diffuse = color 150 150 150;
									sceneMaterials[m].materialList[i].Specular = color 229 229 229;
									sceneMaterials[m].materialList[i].specularLevel = 0;
									sceneMaterials[m].materialList[i].glossiness = 10;
									sceneMaterials[m].materialList[i].Soften = 0.1
								)
							)
						)
						Messagebox "完成！  " title:"完成"
					)--btnAls end "恢复为默认材质(多维)"

					on btn_CleanSelMat pressed do
					(
						local sel =  selection as array
						
						if queryBox "你确定要清除所选物体的材质吗?" beep:false then
						(
							for obj in sel do
							(
								mat = obj.Material
								if (mat != undefined)then
								(
									if (ClassOf mat == MultiMaterial) then
									(
										for i = 1 to mat.numsubs do
										(
											mat.materialList[i].diffuseMap.name = ""
											mat.materialList[i].diffuseMap.filename = ""
											mat.materialList[i].diffuseMap = none
											mat.materialList[i].name = ""
										)
									)
									else if (ClassOf mat == Standard )then
									(
										mat = none
									)
								)
							)
							Messagebox "全部的材质已经清除完毕!" title:"报告"
						)							
					)-- 清除选择的对象的材质
					
					on btn_FixLMNameExt pressed do
					(
						
					)
				)--rollout end 
				
			)
----------------------------------------------------------------------------------------------------------------------------------
/*贴图检查*/
			3:
			(
				rollout rl_CheckMulMat "多维材质贴图检查" 
				(
					group "材质检查"
					(
						button btn_CheckMulMat "检查丢失的贴图..." width:169 height:26 tooptip:"新窗口打开"
						button btn_CheckUnuseMap "当前路径没用贴图查找" width:169 height:26 tooptip:""
						button btn_SelUnMaterialObj "选择未赋材质的物体" width:169 height:26 tooptip:""
					)
					group "规范化材质(处理选中)"
					(
						button btn_Subname2Matname "子材质名等于贴图名" width:169 height:26 tooltip:"将选中的物体对象的贴图名赋予到子材质球名上!"
						button btn_Objname2Matname "材质球名等于物体名" width:169 height:26 tooltip:"将选中的物体对象的物体名赋予到材质球名上!"
						button btn_FixIsMulSubmat "检查多维子材质" width:180 height:26 tooltip:"当多维材质中的子材质存在多维材质时使用!"
					)
					group "规范化材质"
					(
						button btn_CheckIs2SquareMap "贵阳命名 + 贴图是否为2的次幂..." width:169 height:26 tooltip:"在新窗口中弹出!"
					)
					button btn_Other6 "(广告位招租)" width:169 height:26 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					
					on btn_CheckMulMat pressed do
					(
						fileIn ms_FindMissMap -- 查找丢失的贴图文件 (YY) 2012-01.31
					)-- btn_CheckMulMat"贴图检查"
					
					local nousemap
					local b
					on btn_CheckUnuseMap pressed do--"当前路径没用贴图查找"
					(
						
						g=selection.count
						if g !=1 or classof $.material != Multimaterial then 
							messagebox "请选择一个具有多维材质的物体！！"title:"类型不匹配"
						else
						(
							pathmap=maxFilepath+"*.jpg"
							a=getFiles pathmap
							pickedobj = $.material
							yi= pickedobj.numsubs
							xi=a.count
							global nousemap=#()
							b=1
							for i in 1 to xi do
							(
								for j in 1 to yi do
								(     
									if $.material.materialList[j].diffuseMap != undefined do
									(  
										if a[i]==$.material.materialList[j].diffuseMap.filename then
											exit
										else 
										(  
											if $.material.materialList[j].opacityMap != undefined then
											(
												if a[i]==$.material.materialList[j].opacityMap.filename then
													exit
												else
												(
													if j==yi do
													(
														nousemap[b]=a[i]
														b=b+1
													)
												)
											)
											else
											(
												if j==yi do
												(
													nousemap[b]=a[i]
													b=b+1
												)
											)
										)
									)	  
								)
							)
							if b!=1 then
							( 
								rollout nousemaperror "当前max文件夹下没有用到的贴图" width:550 height:400
								( 
									local w = 1
									listbox nousemaperror_cbx "没用到的贴图" pos:[11,12] width:500 height:20 items:nousemap
									edittext edt2 "图片路径" pos:[14,341] width:496 height:26 text:nousemap[w]
									on nousemaperror_cbx selected sel do
									(
										w=nousemaperror_cbx.selection
										edt2.text=nousemap[w]
									)
								)
								createDialog nousemaperror "当前max文件夹下没有用到的贴图"
							)
							else 
								messagebox "检查完成！没有错误"title:"没有错误！"
						)	
					)--"当前路径没用贴图查找"
					
					on btn_SelUnMaterialObj pressed do--选择没有赋材质的物体
					(
						clearSelection()
						num=0

						if geometry.count == 0 then 
						(
							Messagebox "场景中没有geometry对象"
							return 0
						)
						for obj in geometry do 
						(
							if obj.material==undefined do 
							(
								num+=1;selectMore obj
							)
						)
						if (num == 0 and geometry.count != 0 )then
						(
							MessageBox "恭喜！所有物体都已经赋予了材质！" title:"报告"
							ClearSelection()
						)
						pushPrompt (num as string+" 个未赋予材质的物体")
					)
					
					
			-------------------------------------------------------------------------------------------------------------------------------
					on btn_Subname2Matname pressed do
					(
						local sel = selection as array
						if sel.count == 0 then
						(
							MessageBox ("请选择一个或多个物体对象!")
							return 0
						)
						for obj in sel do
						(
							Fn_handleAllSubMats obj
						)
						Messagebox "命名完成！" title:"完成"
					)-- 材质球名等于贴图名 按钮
					
					on btn_Objname2Matname pressed do
					(
						local sel = selection as array
						if sel.count == 0 then
						(
							MessageBox ("请选择一个或多个物体对象!")
							return 0
						)
						try
						(
							for obj in sel do
							(
								mat = obj.Material
								if (ClassOf mat == MultiMaterial) then
								(
									mat.name = obj.name
								)
								else
								(
									format "不是多维材质,命名无效! --是否选择了无材质的或不可编辑的对象?\n"
									continue
								)
							)
							Messagebox "完成!"
							print "OK"
						)catch(print "是否选择了无材质的或不可编辑的对象?")
					) -- 材质球名等于物体名(多维材质球)

					-----------------------------------------------
					on btn_FixIsMulSubmat pressed do
					(
						local sel = selection as array
						for obj in sel do
						(
							mat = obj.material
							if mat != undefined then
							(
								if (classof mat == MultiMaterial )then
								(
									for i = 1 to mat.numsubs do
									(
										if ( mat[i] == MultiSubMaterial )then
										(
											MessageBox ("发现有多维子材质: [物体名]-" + obj.name + " [材质名]-" +mat[i].name)
										)
										else
										(
											format "没有多维子材质存在!\n"
											
										)
									)
								)
								print ".....................\n"
							)
						)
					) -- 多维材质是否嵌套多维子材质 按钮
					
					on btn_CheckIs2SquareMap pressed do
					(
						FileIn ms_CheckNameAndFixSquareMap
					)-- 检查并修复贴图是否为2的次幂... 按钮
				)--rollout end
			)
----------------------------------------------------------------------------------------------------------------------------------
/*裁剪与断点*/
			4:
			(
				rollout rl_Others "裁剪与断点" 
				(
					Group "裁剪工具"
					(
						Button btn_filtPolyFace "裁剪工具..."tooltip:"在新窗口打开裁剪工具面板"
					)
					Group "打断点"
					(
						Button btn_breakPoint "打断点(Break)" tooltip:"打断(Break)选择对象的点"
					)
					Group "新功能"
					(
						Button btn_01 "(正在加载新功能...)" enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
						Button btn_02 "(正在加载新功能...)" enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
						Button btn_03 "(正在加载新功能...)" enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					)
					
					on btn_filtPolyFace pressed do
					(
						try
						(
							if ms_filtScript == undefined do
							(
								MessageBox "需要调用的脚本文件不存在,请检查 \"裁剪工具v0.2.mse\" 文件是否存在!" title:"错误"
								return 0
							)
							filein ms_filtScript
						)
						catch()
					)-- 打开裁剪工具面板按钮
					
					on btn_breakPoint pressed do
					(
						local sel = selection as array
						try
						(
							if sel.count == 0 then 
							(
								Messagebox "请选择一个以上的物体!" title:"提示"
								return 0
							)
							for obj in sel do
							(
								if classof obj != Edit_Poly then
									macros.run "Modifier Stack" "Convert_to_Poly"
								--subobjectLevel = 1
								numVerts = polyop.getNumVerts obj
								--obj.SetSelection #Vertex #{1..(numVerts)}
								polyop.breakVerts obj #{1..(numVerts)}
							)
							Messagebox ("完成")title:"完成"
						)catch()
					)-- 断点按钮

				)-- rollout "裁剪与断点"
				
			)
----------------------------------------------------------------------------------------------------------------------------------
-- /*新增加功能*/	5:
-- 			rollout rl_Others "新增功能"
-- 			(
-- 				
-- 			)-- rollout 新增加功能
----------------------------------------------------------------------------------------------------------------------------------
/*其它功能*/	5:
			(
				rollout rl_Others "其他功能" 
				(
					button btn_CleanSmoothGroup "清除光滑组" width:169 height:26
					button btn_Export3Ds "一键导出3ds" width:169 height:26
					button btn_DetachInFace "按物体的面Detach" width:169 height:26
					button btn_DetachInMulMatID "按多维材质ID Detach" width:169 height:26
					
					button btn_SelEmptyFaceObj "选择空面物体" width:169 height:26
					
					--button btn_AutoNameMat "拖贴图自动命名..." width:169 height:30 tooltip:"新窗口打开,自动化命名材质名"
					button btn_Other2 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					button btn_Other3 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					button btn_Other4 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					button btn_Other5 "(广告位招租)" width:169 height:30 enabled:false tooltip:"切换到 \"关于\" 选项卡,单击 \"点这里\" 查看联系!"
					
					on btn_CleanSmoothGroup pressed do
					(
						g=selection.count
						if g==0 do 
							messagebox  "请选择一个或多个物体！！"title:"请选择物体！"
						if g==1 do 
						(	
							convertto $ editable_poly
							face_numer=$.GetNumFaces()
							$.EditablePoly.SetSelection #Face #{1..face_numer}
							$.ButtonOp #ClearSmoothingGroups
							convertTo $ TriMeshGeometry
							messagebox "完成！"title:"完成！"
						)
						if g>1 do 
						(	
							selection_all=getCurrentSelection()
							for i in 1 to g do	
							(
								convertto selection_all[i] editable_poly
								face_numer=selection_all[i].GetNumFaces()
								selection_all[i].EditablePoly.SetSelection #Face #{1..face_numer}
								selection_all[i].ButtonOp #ClearSmoothingGroups
								convertTo selection_all[i] TriMeshGeometry
							)
							messagebox "完成！"title:"完成！"
						)
					)--"清除光滑组"
				
					on btn_Export3Ds pressed do
					(
						if selection.count!=1 then 
						   messagebox "请选择最终完成的模型！"title:"请选择最终完成的模型！"
						else
						(
							convertTo $ TriMeshGeometry
							CenterPivot $
							a= maxFilepath
							b= maxFilename
							c=a+b
							d=getFilenameFile c
							expname=a+d+".3ds"
							exportFile expname #noPrompt
							messagebox "导出完成！"title:"完成！"
						)
					)--"一键导出3ds"
					
				
					on btn_DetachInFace pressed do
					(
						g=selection.count
						if g==0 do 
						   messagebox  "请选择一个或多个物体！！"title:"请选择物体！"
						if g==1 do
						(
							convertTo $ PolyMeshObject
							obj=$
							currentname=$.name
							p = polyop.getNumFaces obj
							for i in p to 2 by -1 do
							(
								a=currentname+"newname0"+(i-1) as string
								polyOp.detachFaces obj #{i} asNode:true  name:a
							)
							messagebox "完成！"title:"完成！"
						)
						if g>1 do 	
						(
							selection_obj=getCurrentSelection()
							for i in 1 to g do
							(
							   convertTo selection_obj[i] PolyMeshObject
							   currentname=selection_obj[i].name
							   p = polyop.getNumFaces selection_obj[i]
							   for j in p to 2 by -1 do
							   (
								  a=currentname+"newname0"+(j-1) as string
								  polyOp.detachFaces selection_obj[i] #{j} asNode:true  name:a
							   )
							)
						   messagebox "完成！"title:"完成！"
						)
					)--"按物体的面Detach"
				
					on btn_DetachInMulMatID pressed do
					(
						g=selection.count
						if g==0 do 
							messagebox "请选择物体！"title:"请选择物体！"
						if g==1  do
						(
							if classof $.material == Multimaterial then
						   (
								convertTo $ PolyMeshObject
								obj=$
								j = obj.material.numsubs
								currentname=$.name
								objcollect=#()
								l=1
								for i in j to 1 by -1 do
								(
									a="newname"+(i) as string
									obj.EditablePoly.selectByMaterial i
									facegroup=polyOp.getFaceSelection obj
									objcollect=polyOp.detachFaces obj facegroup asNode:true  name:a
								)
								b=select $'newname*'
								selection_obj=getCurrentSelection()
								for k in j to 1 by -1 do
								(
									selection_obj[l].material=obj.material.materialList[k]
									selection_obj[l].name=currentname+"detachobj"+k as string
									convertTo selection_obj[l] TriMeshGeometry
									l=l+1
								)
								delete obj
								messagebox "完成！"title:"完成！"
							)
							else 
							(
								messagebox "请选择一个或多个具有多维材质的物体！！"title:"类型不匹配"
							)
						)
						if g>1 do 
						(
							selection_obj=getCurrentSelection()
							d=on
							for c in 1 to g do
							(
								if classof selection_obj[c].material != Multimaterial do
								d=off
							)
							if d==off then
							messagebox "请选择一个或多个具有多维材质的物体！！"title:"类型不匹配"
							else
							(
								for i in 1 to g do
								(
									convertTo selection_obj[i] PolyMeshObject
									currentname=selection_obj[i].name
									j = selection_obj[i].material.numsubs
									objcollect=#()
									l=1
									for m in j to 1 by -1 do
									(
										a="newname"+(m) as string
										selection_obj[i].EditablePoly.selectByMaterial m
										facegroup=polyOp.getFaceSelection selection_obj[i]
										objcollect=polyOp.detachFaces selection_obj[i] facegroup asNode:true  name:a
									)
									b=select $'newname*'
									selection_obj1=getCurrentSelection()
									for k in j to 1 by -1 do
									(
										selection_obj1[l].material=selection_obj[i].material.materialList[k]
										selection_obj1[l].name=currentname+"detachobj"+k as string
										convertTo selection_obj1[l] TriMeshGeometry
										l=l+1
									)
									delete selection_obj[i]
								)
								messagebox "完成！"title:"完成！"
							)
						)
					)--"按多维材质ID Detach"pos
					
					on btn_SelEmptyFaceObj pressed do--选择空面物体
					(
						clearSelection()
						num=0
						if geometry.count == 0 then 
						(
							Messagebox "场景中没有geometry对象"
							return 0
						)
						for obj in geometry do 
						(
							fv=getPolygonCount obj
							if fv[1]==0 do 
							(
								num+=1;selectMore obj
							) 
						)
						if num == 0 then
						(
							MessageBox "所有物体都有面！" title:"提示"
							ClearSelection()
						)
						pushPrompt (num as string+" 个是空面物体！")
					)----选择空面物体按钮

				)--Rollout
			)
----------------------------------------------------------------------------------------------------------------------------------
-- "说明" 
			6:
			(
				rollout rl_About "说明" 
				(
					--EditText edt_Notice "" text:"" height:130 width:200 --enabled:false
					timer clock "My Timer" interval:100
					Bitmap bmp_Welcome "About" fileName:"welcome.bmp" width:580 height:200 pos:[4,5]--bitmap:<bitmap> 
					Button btn_clickMe "点这里" 
					label times "0" pos:[-100,-100]
					
					global valUp = 0
					global onoff = on
					on btn_clickMe pressed do
					(
						rollout ContactMe "联系方式" width:582 height:300
						(
							--label lb_about "联系方式 ：\n    QQ:12319597\n    E-mail:ccver_0286@sina.com" pos:[50,10] width:400 height:50
							Bitmap bmp_Welcome "About" fileName:"welcome.bmp" width:580 height:200 pos:[1,2]
							hyperLink hl_Title "联系方式 ：" align:#left width:100 height:15 color:yellow enabled:false
							hyperLink hl_QQ "QQ:12319597" align:#left width:100 height:15 color:yellow enabled:false
							hyperLink hl_Email"E-mail:ccver_0286@sina.com" align:#left width:150 height:15 color:yellow enabled:false
							
							hyperLink hl_About "下载地址"  align:#left color:yellow address:"" enabled:false across:2-- hoverColor:Blue visitedColor:red 
							hyperLink hl_web "猛击这里" align:#left color:green hoverColor:Blue visitedColor:red address:"www.macai.co.cc"
							on hl_web rightClick do
							(
								hl_web.visitedColor = (color 130 28 168)
							)
						)createDialog ContactMe
					)
					
					on clock tick do
					(
						
						-- 小于 330 时
						if (onoff == on )then
						(
							valUp = (times.text as integer) + 10
							times.text = valUp as string
							if valUp != 360 then
							(
								bmp_Welcome.pos = [-valUp, 5]
								--onoff = on
							)
							else
							(
								onoff = off
								bmp_Welcome.filename = "cab.jpg"
							)
						)
						-- 大于 330 后
						if (onoff == off) then
						(
							valUp = (times.text as integer) - 10
							times.text = valUp as string
							if valUp != -20 then
							(
								
								bmp_Welcome.pos = [-valUp, 5]
							)
							else
							(
								onoff = on
								bmp_Welcome.filename = "welcome.bmp"
							)
						)
						
					)
					
					on rl_About close do
					(
						clock.active = false
					)
				)-- rollout
			)-- "说明" 
		)
		addSubRollout subRoll sub
	)
	
	fn initTabs tab labels:#() =
	(
		tab.tabPages.clear()
		
		tabs.appearance=tabs.appearance.flatbuttons
 		tabs.Multiline=true
 		tabs.RightToLeftLayout=true
 		tabs.SizeMode=tabs.sizeMode.fillToRight
 		tabs.tabStop=true
		
		tab.sizeMode=tab.sizeMode.fixed
		tab.itemSize = dotnetObject "System.Drawing.Size" ((tab.width/labels.count + 15)) 20
 		for x in labels do tab.tabPages.add x
	)
	
	on tabs MouseUp senderArg arg do
	(
		addTabRollout senderArg.SelectedIndex
	)

	on yyMfHandlMat open do
	(
		initTabs tabs labels:#("优化材质", "材质球", "材质修复", "贴图检查", "裁剪\断点", /*"新功能",*/ "其它功能","关于")
		addTabRollout default
		
		--停靠栏模式
-- 		cui.RegisterDialogBar yyMfHandlMat
-- 		cui.DockDialogBar yyMfHandlMat #cui_dock_right
-- 		DockStatus =  true
	)
	
	on yyMfHandlMat close do
	(
		if rl_SuckUp != undefined do
		(
			DestroyDialog rl_SuckUp
			rl_SuckUp = undefined
		)
	)
	
	on yyMfHandlMat moved pos do
	(
		if rl_SuckUp != undefined then
		(
			DestroyDialog rl_SuckUp
			aPos =  GetDialogpos yyMfHandlMat
			CreateDialog rl_SuckUp pos:(aPos - [0,-379]) bgcolor:Green style:#(#style_sysmenu,#style_toolwindow)
		) 
	)
------------------------------------------------------------------------------------------
--------------------------------------------------------------
-----自动隐藏
-- 	on yyMfHandlMat moved val do
-- 	(
-- 		--mouse.screenpos.x
-- 		aPos =  GetDialogpos yyMfHandlMat
-- 		if aPos.y < 1 then
-- 		(
-- 			print "_______"
-- 			aPos.y = -640
-- 			SetDialogPos yyMfHandlMat [(GetDialogPos yyMfHandlMat).x,aPos.y]
-- 			rollout rl_Display "" width:240 height:10
-- 			(
-- 				
-- 			)
-- 			CreateDialog rl_Display pos:[]
-- 		)

-- 	)
-- 	on yyMfHandlMat mousemove val do
-- 	(
-- 		
-- 		if ((GetDialogpos yyMfHandlMat).y < 0) then
-- 		(
-- 			SetDialogPos yyMfHandlMat [(GetDialogPos yyMfHandlMat).x,340]
-- 		)
-- 	)

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
)
createDialog yyMfHandlMat style:#(#style_titlebar, #style_sysmenu,#style_minimizebox) --pos:[1200,100] 

) -- global yyMfHandlMat

-- 	on isChecked do yyMfHandlMat.open
-- 		
-- 	on execute do
-- 	(
-- 		createDialog yyMfHandlMat style:#(#style_titlebar, #style_sysmenu,#style_minimizebox)  --bgcolor:black fgcolor:red-- #style_resizing) 
-- 	)

-- 	on closeDialogs do
-- 	(
-- 		destroydialog yyMfHandlMat
-- 	)
-- 	on chbtn_Open changed state do
-- 	(
-- 		if state == on then 
-- 			CreateDialog yyMfHandlMat
-- 		else
-- 			DestroyDialog yyMfHandlMat
-- 	)
-- )--end  macroScript yyMfHandlMat_MS



--------------------------------------------------------------




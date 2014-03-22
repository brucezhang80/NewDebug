
try(destroyDialog yyMfHandlMat)catch()


rollout yyMfHandlMat "多功能材质处理--最终版3.0" width:414 height:240
(

	dotNetControl tabs "system.windows.forms.tabControl" width:(yyMfHandlMat.width-30) height:30
	subRollout subRoll "Sub" width:(yyMfHandlMat.width-20) height:200 offset:[0,-5]
	
	fn showProps obj=
	(
		clearListener()
		format "Properties:\n"
		showProperties obj
		format "\nMethods:\n"
		showMethods obj
		format "\nEvents:\n"
		showEvents obj
	)
	
	fn addTabRollout index=
	(
 		for x in subRoll.rollouts do removeSubRollout subRoll x

		sub=case index of
		(
			
			default:--优化材质
			(
				rollout rl_OptimizMat "删除多余多维子材质" height:140
				(
					button btn_DoMat "优化材质" pos:[100,15] width:169 height:30
					
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
							if selection.count == 0 then MessageBox "请选择物体！"
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
									
									theMat=obj.material --获取材质
									
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
												--end
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
								)--for结束
								MessageBox "优化完成"
							)--else
							clearselection()
						)--try
						catch(Messagebox "模型有错！请先检查")
					)----优化材质按钮
				)
			)
			1:
			(
				rollout rl_EmptyMat "清空材质球"
				(
					button btn_EmptyMat "清空材质球"
					local chkname=#()
					
					GroupBox grp1 "预保留的材质球" pos:[6,29] width:368 height:156
					button btn_CleanSel "清空勾选" pos:[292,38] width:60 height:18
					
					checkbox chk1 "材质球1" pos:[20,65] width:69 height:16 checked:false
					checkbox chk2 "材质球2" pos:[110,65] width:69 height:16 checked:false
					checkbox chk3 "材质球3" pos:[200,65] width:69 height:16 checked:false
					checkbox chk4 "材质球4" pos:[290,65] width:69 height:16 checked:false
					checkbox chk5 "材质球5" pos:[20,85] width:69 height:16 checked:false
					checkbox chk6 "材质球6" pos:[110,85] width:69 height:16 checked:false
					checkbox chk7 "材质球7" pos:[200,85] width:69 height:16 checked:false
					checkbox chk8 "材质球8" pos:[290,85] width:69 height:16 checked:false
					checkbox chk9 "材质球9" pos:[20,105] width:69 height:16 checked:false
					checkbox chk10 "材质球10" pos:[110,105] width:69 height:16 checked:false
					checkbox chk11 "材质球11" pos:[200,105] width:69 height:16 checked:false
					checkbox chk12 "材质球12" pos:[290,105] width:69 height:16 checked:false
					checkbox chk13 "材质球13" pos:[20,125] width:69 height:16 checked:false
					checkbox chk14 "材质球14" pos:[110,125] width:69 height:16 checked:false
					checkbox chk15 "材质球15" pos:[200,125] width:69 height:16 checked:false
					checkbox chk16 "材质球16" pos:[290,125] width:69 height:16 checked:false
					checkbox chk17 "材质球17" pos:[20,145] width:69 height:16 checked:false
					checkbox chk18 "材质球18" pos:[110,145] width:69 height:16 checked:false
					checkbox chk19 "材质球19" pos:[200,145] width:69 height:16 checked:false
					checkbox chk20 "材质球20" pos:[290,145] width:69 height:16 checked:false
					checkbox chk21 "材质球21" pos:[20,165] width:69 height:16 checked:false
					checkbox chk22 "材质球22" pos:[110,165] width:69 height:16 checked:false
					checkbox chk23 "材质球23" pos:[200,165] width:69 height:16 checked:false
					checkbox chk24 "材质球24" pos:[290,165] width:69 height:16 checked:false
	
					on btn_EmptyMat pressed do--清空材质球
					(
						chkname[1]=chk1.checked
						chkname[2]=chk2.checked
						chkname[3]=chk3.checked
						chkname[4]=chk4.checked
						chkname[5]=chk5.checked
						chkname[6]=chk6.checked
						chkname[7]=chk7.checked
						chkname[8]=chk8.checked
						chkname[9]=chk9.checked
						chkname[10]=chk10.checked
						chkname[11]=chk11.checked
						chkname[12]=chk12.checked
						chkname[13]=chk13.checked
						chkname[14]=chk14.checked
						chkname[15]=chk15.checked
						chkname[16]=chk16.checked
						chkname[17]=chk17.checked
						chkname[18]=chk18.checked
						chkname[19]=chk19.checked
						chkname[20]=chk20.checked
						chkname[21]=chk21.checked
						chkname[22]=chk22.checked
						chkname[23]=chk23.checked
						chkname[24]=chk24.checked 
						
						for i in 1 to 24 do
						(
							if chkname[i]!=true do
						    (
							  meditMaterials[i] = Standardmaterial()
							  meditMaterials[i].name = i as string + " - Default"
							)
						 )
					)--清空材质球按钮
					
					on btn_CleanSel pressed  do--清除勾选材质球
					(
						chk1.checked=off
						chk2.checked=off
						chk3.checked=off
						chk4.checked=off
						chk5.checked=off
						chk6.checked=off
						chk7.checked=off
						chk8.checked=off
						chk9.checked=off
						chk10.checked=off
						chk11.checked=off
						chk12.checked=off
						chk13.checked=off
						chk14.checked=off
						chk15.checked=off
						chk16.checked=off
						chk17.checked=off
						chk18.checked=off
						chk19.checked=off
						chk20.checked=off
						chk21.checked=off
						chk22.checked=off
						chk23.checked=off
						chk24.checked=off
					)
					
				)--rollout rl_EmptyMat "清空材质球"
			)
			2:
			(
				rollout rl_FixMat "材质修复"
				(
					button btn_FixStaMaAndShow "修复标准材质与显示" pos:[100,15] width:169 height:26
					button btn_FixMulMatAndShow "修复多维材质类型与显示"pos:[100,55] width:169 height:26
					button btn_NameStaMat "命名Standard材质球名称"pos:[100,95] width:169 height:26
					button btn_NameFixMat "命名及修复多维材质名称"pos:[100,135] width:169 height:26
					
-------------------------function
					fn yyShowInViewport = 
					(
						local sel = #()
						max select all
						sel = Selection as Array
						for theNode in sel where ((superClassOf theNode) == GeometryClass) do
						(
							local theMaterial = theNode.material

							if ((classOf theMaterial) == Standard) then
							(    
								theMaterial.selfIllumAmount = 100

								if ((classOf theMaterial.diffuseMap) == BitmapTexture) then
								(
									theMaterial.opacityMap = theMaterial.diffuseMap
									theMaterial.opacityMap.monoOutput = 1
								)

								showTextureMap theMaterial true
							)
							else if ((classOf theMaterial) == MultiMaterial) then
							(
								for theSubMat in theMaterial.materialList where ((classOf theSubMat) == Standard) do
								(
									theSubMat.selfIllumAmount = 100

									if ((classOf theSubMat.diffuseMap) == BitmapTexture) then
									(
										theSubMat.opacityMap = theSubMat.diffuseMap
										theSubMat.opacityMap.monoOutput = 1
									)

									showTextureMap theSubMat true
								)
							)
						)
						clearselection()
						updateToolbarButtons()

					)
----------------------------
					on btn_FixStaMaAndShow pressed do--修复标准材质与显示
					(
						local g = geometry.count
						if g == 0 then MessageBox "场景没有物体"
						else
						(
							yyShowInViewport() 
							MessageBox "材质已显示完成！" title:"完成"
						)
					)
					
					on btn_FixMulMatAndShow pressed do--修复多维材质类型与显示
					(
						g=selection.count
						if g !=1 or classof $.material != Multimaterial then 
						messagebox "请选择一个具有多维材质的物体！！"title:"类型不匹配"
						else
						(
							j = $.material.numsubs
							for i in 1 to j do
							( 
								$.material.materialList[i].shaderType = 1
								$.material.materialList[i].adTextureLock = on
								showTextureMap $.material.materialList[i] on
							)
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
									
					)
				)--命名及修复多维材质名称
			)
			3:
			(
				rollout rl_CheckMulMat "多维材质贴图检查"
				(
					button btn_CheckMulMat "贴图检查"pos:[100,15] width:169 height:26
					
					on btn_CheckMulMat pressed do
					(
						--local n
						global error=#()
						global error_1=#()
						--local error
						--local error_1
						
						--error=#()
						--error_1=#() 
						local n=1
						if selection.count!=1 or classof $.material != Multimaterial then 
							messagebox "请选择一个具有多维材质的物体！"title:"类型不匹配"
						else     ---------else1
						( 
							error=#()
							error_1=#()
							local j = $.material.numsubs
							for i in 1 to j do    ----------for1
							(       
								if $.material.materialList[i].diffuseMap == undefined then
								(
									error[n]=i as string + "号子材质没有diffuse贴图"
									error_1[n]=i as string + "号子材质没有diffuse贴图"
									n=n+1
								)
								else ----------else2
								( 
									local v = getFilenameFile $.material.materialList[i].diffuseMap.filename
									local f = getFilenamepath $.material.materialList[i].diffuseMap.filename
									local g = getFilenameType $.material.materialList[i].diffuseMap.filename
									local h = maxFilepath
									local gq=""
									local remname=""
									local kk=0
									local mp=mapPaths.count()
									for mpn in 1 to mp do
									(
										gq=mapPaths.get(mpn)+ "\\" 
										remname = gq+v+g
										jk=(getFiles remname).count
										if jk != 0 then
										(
											kk = 1
											exit
										)
									)
									temname = f+v+g
									semname = h+v+g
									r=(getFiles temname).count
									s=(getFiles semname).count
									if r ==0 and s ==0 and kk ==0 then
									(
										error[n]=i as string + "号子材质diffuse贴图丢失"
										error_1[n]=i as string + "号子材质diffuse贴图丢失"
										n=n+1
									)
									else   ----------else3
									(	
										y= $.material.materialList[i].diffusemap.bitmap.height
										z= $.material.materialList[i].diffusemap.bitmap.width				
										swich1=on
										swich2=on
										for k in 1 to 10 do
										(
											a=pow(2)(k)
											if y ==a do
												swich1=off
											if z ==a do
											   swich2=off
										)
										if swich1==on or swich2==on do
										(
											error[n]=i as string +"号子材质的diffuse贴图不符合规范！贴图分辨率为"+y as string+"×"+z as string
											if s==1 and r==0 and kk==0 then
												error_1[n]=semname
											else 
												error_1[n]=temname
											n=n+1
										)
									)----------end else3
								) ----------end else2
								if $.material.materialList[i].opacityMap != undefined do
								(      ----------if
									vv = getFilenameFile $.material.materialList[i].opacityMap.filename
									ff = getFilenamepath $.material.materialList[i].opacityMap.filename
									gg = getFilenameType $.material.materialList[i].opacityMap.filename
									hh = maxFilepath
									temname_1 = ff+vv+gg
									semname_1 = hh+vv+gg
									rr=(getFiles temname_1).count
									ss=(getFiles semname_1).count
									if rr ==0 and ss ==0 then
									(
										error[n]=i as string + "号子材质opcity贴图丢失"
										error_1[n]=i as string + "号子材质opcity贴图丢失"
										n=n+1
									)
									else   --------else4
									(	
										yy= $.material.materialList[i].opacityMap.bitmap.height
										zz= $.material.materialList[i].opacityMap.bitmap.width				
										swich3=on
										swich4=on
										for k in 1 to 10 do
										(
											a=pow(2)(k)
											if yy ==a do
												swich3=off
											if zz ==a do
												swich4=off
										)
										if swich3==on or swich4==on do
										(
											error[n]=i as string +"号子材质的opacity贴图不符合规范！贴图分辨率为"+yy as string+"×"+zz as string
											if rr==0 and ss==1 then
												error_1[n]=semname_1
											else error_1[n]=temname_1
												n=n+1
										)
									)----------else4
								)----------end if
							)  ----------end for1            
							if n!=1 then
							( 
								rollout diffuse_error "通道贴图错误" width:550 height:400
								( 
									local w = 1
									listbox diffuserror_cbx "通道贴图错误" pos:[11,12] width:500 height:20 items:error
									edittext edt2 "图片路径" pos:[14,341] width:496 height:26 text:error_1[w]
									on diffuserror_cbx selected sel do
									(
										w=diffuserror_cbx.selection
										edt2.text=error_1[w]
									)
								)
								createDialog diffuse_error "通道贴图错误"
							)
							else 
								messagebox "检查完成！没有错误"title:"检查完成！"
						)
					)-- btn_CheckMulMat"贴图检查"
				)--rollout end
			)
			4:
			(
				rollout rl_Others "其他功能"
				(
					button btn_CheckUnuseMap "当前路径没用贴图查找"pos:[100,15] width:169 height:26
					button btn_CleanSmoothGroup "清除光滑组"pos:[100,45] width:169 height:26
					button btn_Export3Ds "一键导出3ds"pos:[100,75] width:169 height:26
					button btn_DetachInFace "按物体的面Detach"pos:[100,105] width:169 height:26
					button btn_DetachInMulMatID "按多维材质ID Detach"pos:[100,135] width:169 height:26

					local nousemap
					local b
					
					on btn_CheckUnuseMap pressed do
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
							nousemap=#()
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
				)
			)
			5:
			(
				rollout rl_Others "说明"
				(
					--label lb_Notice "没有说明"
					EditText edt_Notice "" pos:[2,10]\
						text:"本插件使用者无任何版权,版权仅插件开发者\"麻菜\"(所有)。
							\n本插件仅供建模人员使用，任何通过非法途径使用或用于商业用途者，
							\n一律属于侵犯开发者的版权利益！
							\n如对本插件有兴趣者，请点下面的按钮。"\
					height:130 width:374 --enabled:false
					
					Button btn_clickMe "点这里" 
					
					on btn_clickMe pressed do
					(
						rollout ContactMe "联系方式" width:300 height:100
						(
							--label lb_about "联系方式 ：\n    QQ:12319597\n    E-mail:ccver_0286@sina.com" pos:[50,10] width:400 height:50
							hyperLink hl_Title "联系方式 ：" pos:[50,10] width:400 height:50 color:yellow enabled:false
							hyperLink hl_QQ "QQ:12319597" pos:[60,25] width:400 height:50 color:yellow enabled:false
							hyperLink hl_Email"E-mail:ccver_0286@sina.com" pos:[60,40] width:400 height:50 color:yellow enabled:false
							
							hyperLink hl_About "下载地址" pos:[50,58] color:yellow address:"" enabled:false -- hoverColor:Blue visitedColor:red 
							hyperLink hl_web "猛击这里" color:green hoverColor:Blue visitedColor:red address:"www.macai.co.cc" pos:[60,75]
							on hl_web rightClick do
							(
								hl_web.visitedColor = (color 130 28 168)
							)
						)createDialog ContactMe
					)
				)
			)
		)
		addSubRollout subRoll sub
	)
	
	fn initTabs tab labels:#() =
	(

		tab.tabPages.clear()

		tab.sizeMode=tab.sizeMode.fixed

		tab.itemSize=dotnetObject "System.Drawing.Size" ((tab.width/labels.count)-2) 25
		   
 		for x in labels do tab.tabPages.add x
	)
	
	on tabs MouseUp senderArg arg do
	(

		addTabRollout senderArg.SelectedIndex
	)

	on yyMfHandlMat open do
	(
		
		initTabs tabs labels:#("优化材质", "清空材质球", "材质修复", "贴图检查", "其它功能", "关于")

		addTabRollout 1
	)
)
createDialog yyMfHandlMat


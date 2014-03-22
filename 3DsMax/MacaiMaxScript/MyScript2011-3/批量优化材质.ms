
rollout rl_ClearUnuseMat "优化材质"
(
	Button btn_clearUnuseMat "优化材质"
	checkbox chb_covert2Poly "优化后转成Poly" width:120
	
	local theMat, uniqueMats, mapTo, theObj
	struct aMat (theInstance, theDiffusemap)
	
	on btn_clearUnuseMat pressed do
	(
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
	)
)
-- Createdialog rl_ClearUnuseMat
addrollout rl_ClearUnuseMat ToolbarMainUI rolledup:true
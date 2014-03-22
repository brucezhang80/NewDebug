
	function Yvi_getRandomcolor  =
	(	
		local randomcolor = random black white
		randomcolor
	)
	
	function Yvi_changeColorInMulMtl obj = 
	(
		local arr_detachName = #()
		local arr_detachObj = #()
		mat = obj.Material

		convertTo obj PolyMeshObject
		old_objname = obj.name
		sub_count = mat.numsubs
		local l = 1
		for i = sub_count to 1 by -1 do
		(
			a = "newname" + (i as string)
			obj.EditablePoly.selectByMaterial i
			face_group = polyOp.getFaceSelection obj
			objcollect = polyOp.detachFaces obj face_group asNode:true  name:a
			
			if objcollect then
			(
				append arr_detachName a
			)
		)
		if arr_detachName.count == 0 then return 0
		for j = arr_detachName.count to 1 by -1 do
		(
			selObj = (getNodeByName arr_detachName[j] exact:true ignoreCase:true) --得到对应名字的对象
			selObj.wirecolor = Yvi_getRandomcolor()
			append arr_detachObj selObj
		)
		 
		b = select $'newname*'
		sel_obj = getCurrentSelection()
		
		for k = 1 to sub_count do
		(
			if mat.materialList[k] == undefined then continue
			arr_detachObj[l].material = mat.materialList[k]
			arr_detachObj[l].name = old_objname +"_" + (k as string)
			convertTo arr_detachObj[l] TriMeshGeometry
			l += 1
		)
-- 			
-- 		
		print arr_detachObj
		--对裁剪的出来的块物体进行打组成类似原物体
		group arr_detachObj name:(arr_detachObj[1].name + "_group")
-- 			
		delete obj
		sel_obj = ()
		arr_detachName = #()
		arr_detachObj = #()
	)--"按多维材质ID Detach"pos

	--minus the first array from the second
	fn fl_minusArr arrAll arrExclude  =
	(	local a=arrAll.count
	 	local b=arrExclude.count
		for i=1 to b do
		(	
			for j=arrAll.count to 1 by -1 do
			(		
				if arrAll[j]==arrExclude[i] do deleteItem arrAll j
			)
		) 
		arrAll
	)
	
	
try(DestroyDialog Yvi_randColorInMtl)catch()
rollout Yvi_randColorInMtl "改线框颜色"
(
	Button btn_Doit "Doit"
	progressBar  pgb_Progress1 ""  width:140 height:8 color:Green align:#left
	
	
	on btn_Doit pressed do 
	(
		local allObjs = Objects as array
		local arr_stanmtl_obj = #()
		local cn = 0
		if allObjs.count != 0 and (sceneMaterials.count != 0)then
		(
			clearListener()
			for obj in allObjs do 
			(
				clearselection()
				
				if ((obj.material != undefined) and (classof (obj.material) == MultiMaterial))then
				(
					Yvi_randColorInMtl.title = "正在处理颜色..."
					
					Yvi_changeColorInMulMtl obj
				)
				pgb_Progress1.value = (100.0 * (cn += 1) / allObjs.count) / 2
			)
				--else if ((obj.material != undefined) and (classof (obj.material) == StandardMaterial))then
				(
					--if(obj.Material == sceneMaterials[1])then
					(
						--obj.wirecolor = Yvi_getRandomcolor()

						(	
							sceneObjs=#();s=sceneMaterials.count
							for i in objects do
							(	if i.material!=undefined and (classof i.material == StandardMaterial)then
									append sceneObjs i
							)
							if sceneObjs.count!=0 then
							(		
								max create mode
								with redraw off
								temp_objs=#()
								randomColor=Yvi_getRandomcolor()
								tempMtl=sceneMaterials[1]
								for j in sceneObjs where j.material==tempMtl do
								(		j.wirecolor=randomColor
										append temp_objs j
								)
								sceneObjs=fl_minusArr sceneObjs temp_objs
								temp_objs=#()
								for i=2 to s do
								(	
									ifMod2=mod i 2 as Integer
									if ifMod2==0 then
									(	randomColor=Yvi_getRandomcolor()
										tempMtl =sceneMaterials[i]
										for j in sceneObjs where j.material==tempMtl do
										(		j.wirecolor=randomColor
												append temp_objs j
										)
										sceneObjs=fl_minusArr sceneObjs temp_objs
										temp_objs=#()
									)--end  ifMod2==0
									else
									(	randomColor=white-randomColor
										tempMtl =sceneMaterials[i]
										for j in sceneObjs where j.material==tempMtl do
										(		j.wirecolor=randomColor
												append temp_objs j
										)
										sceneObjs=fl_minusArr sceneObjs temp_objs
										temp_objs=#()
									)--end 	ifMod2==1
									pgb_Progress1.value += (100.0 * i / s) / 2
								)--end for i=2 to s
								max views redraw
							)--end if sceneObjs.count!=0
							--else messagebox "场景中没有材质！" title:Yvi_randColorInMtl.title beep:false
						)
					)
				)
			Yvi_randColorInMtl.title = "处理完毕!"
			messagebox "OK" title:Yvi_randColorInMtl.title
			Yvi_randColorInMtl.title = "改线框颜色"
 		)
		else
		(
			messagebox "空物体场景或无材质场景" title:Yvi_randColorInMtl.title
		)
		
	)
	
)createDialog Yvi_randColorInMtl
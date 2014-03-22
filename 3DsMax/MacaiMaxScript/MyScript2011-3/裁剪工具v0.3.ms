
/*************************************************************************************************

**************************************************************************************************/
------------------------------------------------------------------------------------------------
-- defind function
global Yvi_createSlicerParams


global slicerParams
global slicerInArea
global slicerDetachInMatID
global slicerHelp

--function Yvi_createSlicerParams =
(
rollout slicerParams "按面的长度裁剪"
(
	Group "面长度分段参数"
	(
		label lb_axis "轴: ----------" across:2 align:#left
		radiobuttons myAxis "" labels:#("X", "Y", "Z") default:2 align:#left
		spinner myThickness "段长度: "  range:[0,10E9,200] type:#float align:#left
	)

	Group "裁剪操作"
	(
		button doIt "<<开始裁剪>>" width:140 height:22
		label lb_1 "收集进度： " align:#left
		progressBar pgb_Progress1 "" width:140 height:8 color:red
		label lb_2 "裁剪进度： " align:#left
		progressBar pgb_Progress2 ""  width:140 height:8 color:Green
	)
----------------------------------------------------------------------------------------
	--分段裁剪函数数
	function Fn_Thickness =
	(
		local j = 0
		undo on
		(
			for obj in selection do
			(
				--sliceObject obj myThickness.value myAxis.state
				
				local pObj = obj
				local pThickness = myThickness.value 
				local pAxis = myAxis.state
			
				objCenter = pObj.center
				pObj.pivot = [0,0,0] -- 轴居中到世界坐标
				resetXform pobj -- 确保本地轴是正确的
				convertToPoly pobj

				local lStartPos = case pAxis of --开始切片平面的位置
				(
					1 : [pObj.min.x,0,0]
					2 : [0,pObj.min.y,0]
					3 : [0,0,pObj.min.z]
				)
			 
				local lEndPos = case pAxis of -- 结束切片平面的位置
				(
					1 : [pObj.max.x,0,0]
					2 : [0,pObj.max.y,0]
					3 : [0,0,pObj.max.z]
				)
				local lMod = mod (distance lEndPos lStartPos) pThickness
				local lNumSlices -- 切片数量

				if lMod < pThickness/2 then 
					lNumSlices = (((distance lEndPos lStartPos)/pThickness) as integer)-1 
				else 
					lNumSlices = ((distance lEndPos lStartPos)/pThickness) as integer -- 最后一片不少于一半的切片厚度

				for i = 1 to lNumSlices do -- Slicing
				(
					addmodifier pObj (slicemodifier())

					pObj.modifiers[1].slice_plane.rotation = case pAxis of -- 通过旋转切片平面选择轴
					(
						1 : (quat -0.5 -0.5 -0.5 -0.5)
						2 : (quat 0.707107 0 0 0.707107)
						3 : (quat 0 0 0 1)
					)
				
					lSliceOffset = i*pThickness
				
					pObj.modifiers[1].slice_plane.pos = case pAxis of
					(
						1: lStartPos + [lSliceOffset,0,0]
						2: lStartPos + [0,lSliceOffset,0]
						3: lStartPos + [0,0,lSliceOffset]
					)
					
					OldObj = copy pObj
					oldObj.name = pObj.name + "_" + i as string
				
					OldObj.modifiers[1].Slice_Type = case pAxis of
					(
						1 : 2
						2 : 3
						3 : 2
					)
					
					pObj.modifiers[1].Slice_Type = case pAxis of
					(
						1 : 3
						2 : 2
						3 : 3
					)
					
					convertToPoly pObj
					pObj.pivot = objCenter
					
					pgb_Progress2.value = 100.* i / lNumSlices--splices
				)--for plice
				pobj.name = pobj.name + "_" + ((lNumSlices+1) as string)
				
				j += 1
				pgb_Progress1.value = 100.* j / selection.count--objs
				
				
			)--for obj
		)--button
		--pgb_Progress1.value = 0
		--pgb_Progress1.value = 0
	)--分段长度裁剪函数
	
-----------------------------------------------------------------------------------------
	----
	on slicerParams open do
	(
		Fn_getCurSysUnits
	)
	on doIt pressed do 
	(
		Fn_Thickness()
	)

) -- end rollout

) -- end function
------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------
struct objFace(node, name, fID, farea, isAss)
global g_objFaceArr = #()
global g_UnenoughAndLargeThanArr = #()

--回调用的全局函数
global Fn_getTotalFaceArea
global Fn_getCurSysUnits
--UI
global slicerInArea
	------------------------------------
	--获得面积
	function Fn_getAreaInPolyFace polyObj varea = 
	(
		local smallFaceAreaArr = #()
		local largeFaceAreaArr = #()
		local fTotalFaceArea = 0.0
		if ClassOf polyObj != Edit_Poly() then
		(
			if canConvertTo  polyObj Edit_Poly then
			(
				convertToPoly polyObj
			)
		)
		for f = 1 to polyObj.numfaces do
		(
			--得到每个面的面积
			if (polyop.getFaceArea polyObj f) < varea then
			(
				append smallFaceAreaArr f
				--setFaceSelection polyObj smallFaceAreaArr --选择面积小于 xx 的面，
			)
			else
			(
				append largeFaceAreaArr f
				--setFaceSelection polyObj smallFaceAreaArr --选择面积大于等于 xx 的面，
			)
		)

	)
	
	--获得总面积
	function Fn_getTotalFaceArea polyObj =
	(
		local totalArea = 0.
		for f = 1 to polyObj.numfaces do
		(
			totalArea += polyop.getFaceArea polyObj f
		)
		return totalArea
	)
	
	--获得当前的系统单位
	function Fn_getCurSysUnits =
	(
		local Cunit = (units.SystemType as string)
		print Cunit
		try
		(
			if Cunit == "meters"then
			(
				Cunit += "（米）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "millimeters")then
			(
				Cunit += "（毫米）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "kilometers")then
			(
				Cunit += "（千米）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "feet")then
			(
				Cunit += "（英尺）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "centimeters")then
			(
				Cunit += "（厘米）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "inches")then
			(
				Cunit += "（英寸）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else if(Cunit == "miles")then
			(
				Cunit += "（英里）"
				slicerInArea.lb_CurSysUnit.text = Cunit
			)
			else
			(
				slicerInArea.lb_CurSysUnit.text = "请修改系统单位为：Meters（米）"
			)
		)
		catch()
	)
---------------------------------------------------------------------------------------------
--function Yvi_slicerInArea = 
(
Rollout slicerInArea "按面积裁剪(Poly)"
(
	Group "面积参数设置"
	(
		label lb_CSU "当前单位：" width:84 align:#left across:2
		label lb_CurSysUnit ""align:#right
		
		label lb_totalAreatxt "总面积:" across:2 align:#left
		label lb_totalArea "0.00000" align:#left --pos:[60,45]
	
		label lb_area "面积(m2):" align:#left  across:2
		spinner spn_Area ""  height:18 width:60 range:[1.,10E9,1000.] type:#float  scale:1  align:#left

		--RadioButtons rb_block "" labels:#("") align:#left across:3
		label lb_block "块数:" align:#left across:2
		EditText edt_block "" align:#left --pos:[70,85]
	)
	
	Group "裁剪操作"
	(
		button doIt "<<开始裁剪>>" width:140 height:22
		--label lb_1 "收集进度： " align:#left across:2
		--label lb_counts ""
		progressBar pgb_Progress1 "" width:140 height:8 color:blue align:#left across:2
		label lb_num1 "00%" align:#right --pos:[205,240]
		--label lb_2 "裁剪进度： " align:#left
		progressBar pgb_Progress2 ""  width:140 height:8 color:Green align:#left across:2
		label lb_num2 "00%" align:#right--pos:[205,254] 
		
		MultiListBox mlb_eachBlockArea "每个分块的面积：" height:6
	)
	

-------------------------------------------------------------------------
-------------------------------------------------------------------------
	--每个物体、物体面ID、物体面的面积 数组
	function Fn_makeObjFaceArr obj =
	(
		for i = 1 to obj.numfaces do
		(
			face = objFace node:obj name:obj.name fID:i farea:(polyop.getFaceArea obj i) isAss:false
			append g_objFaceArr face
		)
		--print ("g_objFaceArr: " + g_objFaceArr as string)
		return g_objFaceArr
	)
	
	--比较面的面积大小 (called in other function)
	function Fn_faceCompare f1 f2 =
	(
		if f1.farea < f2.farea then
			return -1
		else if f1.farea > f2.farea then
			return 1
		else
			return 0
	)
	
	--排序面的面积大小
	function Fn_sortFaceArr arr =
	(
		qsort arr Fn_faceCompare
		return arr
	)
	
	--找到第一个面积小于 “给定面积与当前面积相减结果”的索引 (called in other function)
	function Fn_findFirstLessTempArea i tempArea= -- 从第 i 个开始向前找 
	(
		if i<0 do return 0
			
		if tempArea < 0 do return 0
			
		for j = i to 1 by -1 do
		(
			curArea = g_objFaceArr[j].farea
			if (curArea < tempArea) and (g_objFaceArr[j].isAss==false) then
			(
				return j
			)
		)
		
		return 0
	)
	
	
	/*
	   lilinx提示：
	   注意，为了最终ive的效率，可能我们对物体先按照材质分组会比较好。
	   相同材质的面我们把他们合并到一起，然后再按照面积来分割...
	   那样，最终 引擎渲染的时候，切换纹理的次数会少一些。
	   
	   底下裁剪的逻辑终于搞对了。
	   具体在 if index>0 的时候，我们的i不应当递减.原来用for，自动递减了；
	   现在用while，手工控制i的递减
	*/
	
	--按指定的面积进行裁剪
	function Fn_splitFaceArr varea =
	(
		local vareaArr = #()--
		local tempFaceArr = #()--临时面的数组
		local curTotalArea = 0.0 --当前面s的总面积
		local counts = g_objFaceArr.count --物体数
		
		--print ("counts:" + counts as string)
		
		--for i = counts to 1  do 
	    local i=counts
		local findbeginindex=counts
		
		while (i>0) do
		(
			--print( (g_objFaceArr[i].fid as string) +" "+ (g_objFaceArr[i].farea as string) )
			--continue
			
			--用过了，跳过
			if g_objFaceArr[i].isass==true then 
			(
				i=i-1
				
				findbeginindex = i
				
				continue
			)
			
			--当前面的面积
			local curArea =  g_objFaceArr[i].farea
			if curArea>=varea then (
				
				--直接成一个临时数组...
				local  tmpfaces = #()
				append tmpfaces g_objFaceArr[i]
				g_objFaceArr[i].isAss = true
				append vareaArr tmpfaces	
				local  tmpfaces = #()
				
				--print "single larggggg..."
				
				i=i-1
				
				findbeginindex = i
				
				continue
			)
			
			local tempArea = varea - curTotalArea--这里是可用的剩余面积
			--print ("剩余可用面积: " + tempArea as string)
			
			--找到第一个小于可用面积的面，包括自己
			local index = Fn_findFirstLessTempArea findbeginindex tempArea
			--print ("第 "+index as string + "个多边形面积小于剩余面积") 
			
			if index>0 then 
			(
				--找到了，有合用的面，这个面可以加到数组里面
				
				--print ("把第"+index as string +"个多边形加到临时数组")
				
				--现在把这个合用的面加到临时数组里面
				append tempFaceArr g_objFaceArr[index]
				
				--把当前这个面标示成已经分配了
				g_objFaceArr[index].isAss = true--已分配
				
				
				--总的面积变大了，以后可用的面积就变小了
				curTotalArea += g_objFaceArr[index].farea
				
				
				findbeginindex = index-1
			)
			else
			(
				--print ("此时表示找不到合用的面，此时应当截断，前面的面")
				
				--findbeginindex = i
				
				if(tempFaceArr.count>0) then
				(
					--当前面积已经满，收集起来
					append vareaArr tempFaceArr

					--清空临时面数组
					tempFaceArr = #()

					--清空总面积
					curTotalArea = 0.0
				)
				
				--一直到现在,截断的工作已经做完。重点注意，此时当前面还没有分配出去
				
				--print ("接下来的工作，就是把当前面分配出去")
				(
					tempFaceArr = #()
					append tempFaceArr g_objFaceArr[i]
					g_objFaceArr[i].isAss = true
					curTotalArea = g_objFaceArr[i].farea					
				)
				
				i = i-1
				
				findbeginindex = i
			)
			---------------------------------------------------------------
			pgb_Progress1.value = 100.*(counts - i+1) / counts--obj
			lb_num1.caption = pgb_Progress1.value as string + "%"
			---------------------------------------------------------------
		)
		
		--如果临时数组里面还有东西的话，应当截断
		if tempFaceArr.count > 0 then 
		(
			--当前面积已经满，收集起来
			append vareaArr tempFaceArr

			--清空临时面数组
			tempFaceArr = #()

			--清空总面积
			curTotalArea = 0.0
		)
		
-- 		for f in g_objFaceArr do (
-- 			if f.isass==false then
-- 				print (f.fid as string +"  : "+ f.isAss as string+" "+f.farea as string)
-- 		)
		
-- 		local c2=0
-- 		for a in vareaArr do (
-- 			c2+=a.count
-- 		)
		
		--print ("c2== "+c2 as string)
		
		return vareaArr
		--返回数组内部的值也是数组
	)
	
	--Attach面 to Obj
	function Fn_attachFaceToObj faceArr = --数组内是数组
	(
		local j = 0
		local blockNameArr = #()--分块的物体名
		local blockObjArr = #()--分块的物体实体对象
		local listInfo = #()
		for face in faceArr do
		(			

			local facesID=#()
			local eachBlockArea = 0.
			for i=1 to face.count do 
			(
				append facesID face[i].fID
				eachBlockArea += face[i].farea -- 2012.01.11
				--print 	face[i].fID
			)
			oldName = face[1].name -- 2012.01.11 为保持原物体名不变
			append blockNameArr oldName -- 2012.01.11
			_name = (uniqueName (face[1].name + "_"))
			polyOp.detachFaces face[1].node  facesID  delete:false asNode:true name:_name  --node:Editable_Poly
			
			append blockNameArr _name
				--print ("blockToGroupArr: "+ blockToGroupArr as string)
			append listInfo (_name + " - " + eachBlockArea as string ) -- 2012.01.11
			-------------------------------------------------------------------
			pgb_Progress2.value = 100.* (j += 1) / faceArr.count--splices
			lb_num2.caption = pgb_Progress2.value as string + "%"
			--------------------------------------------------------------------	
		)
		for i = 1 to blockNameArr.count do
		(
			append blockObjArr (getNodeByName blockNameArr[i] exact:true ignoreCase:true) --得到对应名字的对象
		)
		--对裁剪的出来的块物体进行打组成类似原物体
		group blockObjArr name:(blockObjArr[1].name) -- 2012.01.11
		mlb_eachBlockArea.items = listInfo -- 2012.01.11
		
		faceArr = #()
	)
--------------------------------------------------
	on slicerInArea open do
	(
		Fn_getCurSysUnits()--检查系统单位
		local total_area = 0.
		local sel = selection as array
		if sel.count > 0 then
		(
			for obj in sel do
			(
				if ClassOf obj != Editable_Poly then
				(
					print "不是 Editable_Poly"
					--ConvertTo Editable_Poly
					slicerInArea.lb_totalArea.text = "0.00000"
					return 0
				)
				local ta = Fn_getTotalFaceArea obj
				total_area += ta
			)
			lb_totalArea.text = total_area as string
		)
		else
		(
			slicerInArea.lb_totalArea.text = "0.00000"
		)
	)
	
	on slicerInArea close do
	(
		callbacks.removeScripts id:#Fn_getTotalAreaCallBack01
		callbacks.removeScripts id:#Fn_getCurSysUnits02
	)
	------------------------------------------------------------------------
	on doIt pressed do 
	(
		local i = 0
		local areaArr = #()
		local selObjArr = selection as Array
		clearselection()
		g_objFaceArr = #()
		if selObjArr.count == 0 then
		(
			MessageBox "请选择对象!\t" title:"提示"
			return 0
		)
		for sel in selObjArr do--where((getPolygonCount sel)[1] != 0) where 面数不为0的物体
		(
			macros.run "Modifier Stack" "Convert_to_Poly"
			try(
				--在 poly 对象上操作
				if (ClassOf sel != Editable_Poly) then
				(
					
					if (canConvertTo sel Editable_Poly) then
					(
						print "---------------------"
						convertTo sel Editable_Poly
					)
					else--如果不能转换成 poly 的就继续下一个物体
					(
						print "++++++++++++++++++++"
						continue
					)
				)
			)catch( format "object % convert to editable poly error!!!\n" sel.name)
			---------------------------------------------------------------
			format "正在处理第 % 个物体...\n" (i += 1)
			---------------------------------------------------------------
			g_objFaceArr = Fn_makeObjFaceArr sel --return a global array witch included each obj name , face id, face area and bool
			--print ("g_objFaceArr: "+ g_objFaceArr as string)

			--sort g_objFaceArr 
			g_objFaceArr = Fn_sortFaceArr g_objFaceArr
				--print ("sort_objFaceArr:"+ g_objFaceArr as string)
			--按给定的面积进行裁剪 (数组中内容也是数组)
			areaArr = Fn_splitFaceArr spn_Area.value
			--print ("areaArr: "+ areaArr as string)
			format "\n................................................\n"
			--undo on
			(
				--把一定面积的面的集合 attach 成一个物体 (可能存在面不在同一个物体上的也 attach 在一起)
				Fn_attachFaceToObj areaArr
			)
			--lb_counts.caption =((selObjArr.count-i) as string)
			
			delete sel
			areaArr = #()
			g_objFaceArr = #()
		)
		selObjArr = #()
		--释放内存
		gc()
		freeSceneBitmaps()
		
		MessageBox "裁剪完成, \n注:被裁剪的物体对象已打组!"
	)
	
--------------------text
	on edt_block entered text do
	(
-- 		local block_txt = edt_block.text as integer
-- 		local total_area = slicerInArea.lb_totalArea.text as float
-- 		local  avg_area = total_area / block_txt
-- 		spn_Area.value = avg_area as float 
	)
	on edt_block changed txt do
	(

		local block_txt = edt_block.text as integer
		local total_area = slicerInArea.lb_totalArea.text as float
		local  avg_area = (total_area / block_txt) as float
		spn_Area.value = avg_area as float 
	)
	
	on spn_Area changed val do
	(
		local total_area = slicerInArea.lb_totalArea.text as float
		local iblock = (total_area / val) --as integer
		edt_block.text = iblock as string
	)
	
	
	------------------------------------
-- 	on mlb_UnenoughOrLargeThanArea doubleClicked item do
-- 	(
-- 		
-- 		item_name = mlb_UnenoughOrLargeThanArea.items[item]--得到双击项名
-- 		num = mlb_UnenoughOrLargeThanArea.selection as array
-- 		
-- 		strItem = g_UnenoughAndLargeThanArr[num[1]] as string
-- 		strArray = filterString strItem " -"
-- 		
-- 	)
)
) -- end function
--------------
--function Yvi_slicerDetachInMatID =
(
Rollout slicerDetachInMatID "按材质裁剪"
(
	Group "按多维材质ID裁剪"
	(
		button btn_DetachInMulMatID ">>开始裁剪<<"
	)
	on btn_DetachInMulMatID pressed do
	(
		local arr_detachName = #()
		local arr_deatchObj = #()
		local sel
		selCount = selection.count
		if selCount == 0 do 
			messagebox "请选择物体！"title:"请选择物体！"
		if selCount == 1  do
		(
			if classof $.material == Multimaterial then
		   (
				convertTo $ PolyMeshObject
				obj = $
				subCount = obj.material.numsubs
				currentname = $.name
				objcollect = #()
				l = 1
				for i = subCount to 1 by -1 do
				(
					a = "newname" + (i as string)
					obj.EditablePoly.selectByMaterial i
					facegroup = polyOp.getFaceSelection obj
					objcollect = polyOp.detachFaces obj facegroup asNode:true  name:a
					
					append arr_detachName a
				)
				for i = arr_detachName.count to 1 by -1 do
				(
					selObj = (getNodeByName arr_detachName[i] exact:true ignoreCase:true) --得到对应名字的对象
					append arr_deatchObj selObj
				)
				
				b = select $'newname*'
				selection_obj = getCurrentSelection()
				for k = subCount to 1 by -1 do
				(
					selection_obj[l].material = obj.material.materialList[k]
					selection_obj[l].name = currentname + "_"+ k as string
					convertTo selection_obj[l] TriMeshGeometry
					l=l+1
				)
			
				--对裁剪的出来的块物体进行打组成类似原物体
				group arr_deatchObj name:(arr_deatchObj[1].name + "_group")
				
				delete obj
				messagebox "完成！"title:"完成！"
			)
			else 
			(
				messagebox "请选择一个或多个具有多维材质的物体！！"title:"类型不匹配"
			)
			arr_detachName = #()
			arr_deatchObj = #()
		)
		if selCount > 1 do 
		(
			selection_obj = getCurrentSelection()
			d = on
			for c = 1 to selCount do
			(
				if classof selection_obj[c].material != Multimaterial do
				d=off
			)
			if d == off then
				messagebox "请选择一个或多个具有多维材质的物体！！"title:"类型不匹配"
			else
			(
				for i = 1 to selCount do
				(
					convertTo selection_obj[i] PolyMeshObject
					currentname = selection_obj[i].name
					subCount = selection_obj[i].material.numsubs
					objcollect=#()
					l = 1
					for m = subCount to 1 by -1 do
					(
						a = "newname" + (m as string)
						selection_obj[i].EditablePoly.selectByMaterial m
						facegroup=polyOp.getFaceSelection selection_obj[i]
						objcollect=polyOp.detachFaces selection_obj[i] facegroup asNode:true  name:a
						
						append arr_detachName a
					)
					for i = 1 to arr_detachName.count do
					(
						selObj = (getNodeByName arr_detachName[i] exact:true ignoreCase:true) --得到对应名字的对象
						append arr_deatchObj selObj
					)
					b = select $'newname*'
					selection_obj1 = getCurrentSelection()
					
					for k = subCount to 1 by -1 do
					(
						selection_obj1[l].material=selection_obj[i].material.materialList[k]
						selection_obj1[l].name = currentname+"_" + (k as string)
						convertTo selection_obj1[1] TriMeshGeometry
						l=l+1
					)
					
					--对裁剪的出来的块物体进行打组成类似原物体
					group arr_deatchObj name:(arr_deatchObj[1].name + "_group")
		
					delete selection_obj[i]
					
					arr_detachName = #()
					arr_deatchObj = #()
				)
				messagebox "完成！"title:"完成！"
				
			)
		)
	)--"按多维材质ID Detach"pos
)
)-- end function
--function Yvi_slicerHelp =
(
Rollout slicerHelp "About"
(
	label t "一,按面分段裁剪:  
①.选择的对象依赖于轴和
分段的长度长度为系统单位）.
②.仅对选中的物体有效(可
多选).\n
二,按面积裁剪
①.指定Max系统单位为Meters
(米),指定面积分配面.    
②.以计算出的总面积进行分
配面积的大小进行分段裁剪.
③.每个物体对象裁剪出来的
块进行打组, 打成的组类似于
原来的物体对象(裁剪前的物
体已不存在).\n
三.按材质裁剪   
①按物体的多维材质ID进行
裁剪出来,并对裁剪出来的材
质分块打组.
\n(c)2011~2012  作者: Yvi\t\n博客: http://www.macai.co.cc \t\nEmail: 12319597@qq.com" align:#left width:250 height:320
)
)--end function

-- try (closerolloutfloater floatSlicer) catch () 
-- floatSlicer = newrolloutfloater "裁剪分离工具 v0.3" 250 470 1180 300
addrollout slicerParams ToolbarMainUI rolledup:true
addrollout slicerInArea ToolbarMainUI rolledup:true 
addrollout slicerDetachInMatID ToolbarMainUI rolledup:true 
--addrollout slicerHelp ToolbarMainUI rolledup:true 

-------call back
	function Fn_getTotalAreaCallBack =
	(
		local sel = selection as array
		local total_area = 0.
		for obj in sel do
		(
			if ClassOf obj != Editable_Poly then
			(
				print "不是 Editable_Poly"
				--ConvertTo Editable_Poly
				slicerInArea.lb_totalArea.text = "0.00000"
				return 0
			)
			local ta = Fn_getTotalFaceArea obj
			total_area += ta
		)
		slicerInArea.lb_totalArea.text = total_area as string
	)
	callbacks.addScript #selectionSetChanged "Fn_getTotalAreaCallBack()" id:#Fn_getTotalAreaCallBack01
	callbacks.addScript #unitsChange "Fn_getCurSysUnits()" id:#Fn_getCurSysUnits02

--function Yvi_createSlicerParams()
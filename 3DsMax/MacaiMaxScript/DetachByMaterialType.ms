	
	/**********************************************************
	作者：Yvi
	功能：按不同格式类型的贴图分离物体的面
	日期：2012-3-28
	版权：Yvi
	联系：12319597@qq.com
	//说明：以下功能在选择单个物体下测试通过，如果你需要实现多个物体进行处理以及使用其它类型的材质除diffuseMap外，可自行修改，来提高你的脚本编程能力。
			如果要对选中的多个物体的全部材质进行类型匹配来分离面。本代码提供的功能需要进行大修改。但万变不离其宗，最终都需要
			detachFaces这个方法。如何使用，就看你怎么做了，我的方法并不是最好的。
	**********************************************************/

	-- 分离使用tga材质的面
	function Yvi_DetachTgaFace obj =
	(
		local tga_detachName = #()
		local arr_deatchObj =#()
		local _name = "tga_newname"
		local tga_faceList = #()
		local currentname = obj.name
		local mat = obj.material
		if mat != undefined then
		(
			local subCount = mat.numsubs
			for i = subCount to 1 by -1 do
			(
				if(mat.materialList[i].diffuseMap != undefined)then
				(
					ext = getFilenameType (mat.materialList[i].diffuseMap.fileName)
					if ext == ".tga" then
					(
						obj.EditablePoly.selectByMaterial i
						tga_faceList += (polyOp.getFaceSelection obj)
					)
				)
			)
			if tga_faceList.count != 0 then
			(
				objcollect = polyOp.detachFaces obj tga_faceList asNode:true  name:_name delete:false
				append tga_detachName _name
			)
		)
		
		return tga_detachName
	)
	-- 分离使用png材质的面
	function Yvi_DetachPngFace obj =
	(
		local png_detachName = #()
		local _name = "png_newname"
		local currentname = obj.name
		local png_faceList = #()
		local mat = obj.material
		if mat != undefined then
		(
			local subCount = mat.numsubs
			for i = subCount to 1 by -1 do
			(
				if(mat.materialList[i].diffuseMap != undefined)then
				(
					ext = getFilenameType (mat.materialList[i].diffuseMap.fileName)
					if ext == ".png" then
					(
						obj.EditablePoly.selectByMaterial i
						png_faceList += polyOp.getFaceSelection obj
					)
				)
			)
			if png_faceList.count != 0 then
			(
				objcollect = polyOp.detachFaces obj png_faceList asNode:true  name:_name delete:false
				append png_detachName _name
			)
		)

		return png_detachName
	)
	
	-- 分离使用jpg材质的面
	function Yvi_DetachJpgFace obj =
	(
		local jpg_detachName = #()
		local _name = "jpg_newname"
		local jpg_faceList = #()
		local currentname = obj.name
		local mat = obj.material
		if mat != undefined then
		(
			local subCount = mat.numsubs
			for i = subCount to 1 by -1 do
			(
				if(mat.materialList[i].diffuseMap != undefined)then
				(
					ext = getFilenameType (mat.materialList[i].diffuseMap.fileName)
					if ext == ".jpg" then
					(
						obj.EditablePoly.selectByMaterial i
						jpg_faceList += polyOp.getFaceSelection obj
					)
				)
			)
			if jpg_faceList.count != 0 then
			(
				objcollect = polyOp.detachFaces obj jpg_faceList asNode:true  name:_name delete:false
				append jpg_detachName _name
			)
		)	


		return jpg_detachName
	)
	
	-- 分离使用其它材质的面
	function Yvi_DetachOtherFace obj =
	(
		local other_detachName = #()
		local other_faceList = #()
		local _name = "other_newname"
		local currentname = obj.name
		local mat = obj.material
		if mat != undefined then
		(
			local subCount = mat.numsubs
			for i = subCount to 1 by -1 do
			(
				if(mat.materialList[i].diffuseMap != undefined)then
				(
					ext = getFilenameType (mat.materialList[i].diffuseMap.fileName)
					if ext != ".tga" and  ext != ".png" and ext != ".jpg"then
					(
						obj.EditablePoly.selectByMaterial i
						other_faceList += polyOp.getFaceSelection obj
					)
				)
			)
			if other_faceList.count != 0 then
			(
				objcollect = polyOp.detachFaces obj other_faceList asNode:true  name:_name delete:false
				append other_detachName _name
			)
		)
		
		return other_detachName
	)
	
	
--////////////////////////////////////////////////////
	function Yvi_DetachByMaterialType =
	(
		local arr_detachName = #()
		local arr_deatchObj = #()
		----------------------------------------
		
		selCount = selection.count
		if selCount == 0 then 
			messagebox "请选择物体！"title:"请选择物体！"
		
		--选择一个物体时
		if selCount == 1 then
		(
			if classof $.material == Multimaterial then
		   (
				convertTo $ PolyMeshObject
				obj = $

				t = Yvi_DetachTgaFace obj
				p = Yvi_DetachPngFace obj
			    j = Yvi_DetachJpgFace obj
			    o = Yvi_DetachOtherFace obj
				arr_detachName = t+p+j+o
				for i = 1 to arr_detachName.count do
				(
					selObj = (getNodeByName arr_detachName[i] exact:true ignoreCase:true) --得到对应名字的对象
					append arr_deatchObj selObj
				)
-- 				--对裁剪的出来的块物体进行打组成类似原物体
				group arr_deatchObj name:(obj.name + "_group")
				
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
		--选择多个物体时
		if selCount > 1 do 
		(
			
		)
	)--"按多维材质ID Detach"pos
	
	Yvi_DetachByMaterialType()
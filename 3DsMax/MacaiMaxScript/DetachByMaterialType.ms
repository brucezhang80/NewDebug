	
	/**********************************************************
	���ߣ�Yvi
	���ܣ�����ͬ��ʽ���͵���ͼ�����������
	���ڣ�2012-3-28
	��Ȩ��Yvi
	��ϵ��12319597@qq.com
	//˵�������¹�����ѡ�񵥸������²���ͨ�����������Ҫʵ�ֶ��������д����Լ�ʹ���������͵Ĳ��ʳ�diffuseMap�⣬�������޸ģ��������Ľű����������
			���Ҫ��ѡ�еĶ�������ȫ�����ʽ�������ƥ���������档�������ṩ�Ĺ�����Ҫ���д��޸ġ�����䲻�����ڣ����ն���Ҫ
			detachFaces������������ʹ�ã��Ϳ�����ô���ˣ��ҵķ�����������õġ�
	**********************************************************/

	-- ����ʹ��tga���ʵ���
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
	-- ����ʹ��png���ʵ���
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
	
	-- ����ʹ��jpg���ʵ���
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
	
	-- ����ʹ���������ʵ���
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
			messagebox "��ѡ�����壡"title:"��ѡ�����壡"
		
		--ѡ��һ������ʱ
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
					selObj = (getNodeByName arr_detachName[i] exact:true ignoreCase:true) --�õ���Ӧ���ֵĶ���
					append arr_deatchObj selObj
				)
-- 				--�Բü��ĳ����Ŀ�������д��������ԭ����
				group arr_deatchObj name:(obj.name + "_group")
				
				delete obj
				messagebox "��ɣ�"title:"��ɣ�"
			)
			else 
			(
				messagebox "��ѡ��һ���������ж�ά���ʵ����壡��"title:"���Ͳ�ƥ��"
			)
			arr_detachName = #()
			arr_deatchObj = #()
		)
		--ѡ��������ʱ
		if selCount > 1 do 
		(
			
		)
	)--"����ά����ID Detach"pos
	
	Yvi_DetachByMaterialType()
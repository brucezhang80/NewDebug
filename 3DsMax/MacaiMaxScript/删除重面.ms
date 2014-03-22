
-- Duplicate faces

try(destroydialog rl_DeleteDupFaces )catch()
rollout rl_DeleteDupFaces "删除重面" width:203 height:132
(
	button btn_CheckDupFaces "检查重面" pos:[28,15] width:137 height:21
	button btn_DelDupFaces "删除重面" pos:[32,49] width:135 height:22
	
	label lbl1 "" pos:[11,94] width:184 height:33
	
	on btn_CheckDupFaces pressed do
	(
		local dupFacesArr = #();
		
		local allPoly_faceList = #();
		local allMesh_faceList = #();
		clearSelection();
		for obj in Objects where( (classOf obj == Editable_Poly) or (classOf obj == Editable_Mesh) or (classOf obj == PolyMeshObject)) do
		(
			local Poly_faceList = #();
			local Mesh_faceList = #();
			if( classOf obj == Editable_Poly )then
			(
				polyop.setFaceSelection obj (for f = 1 to polyop.getNumFaces obj where (polyop.getElementsUsingFace obj f).numberSet == 1 collect f);
				Poly_faceList = polyop.getFaceSelection obj;
				
				Poly_faceList = Poly_faceList as array;
				join allPoly_faceList Poly_faceList;
			)
			else if( classOf obj == Editable_Mesh )then
			(
				setFaceSelection obj (for f = 1 to meshop.getNumFaces obj where (meshop.getElementsUsingFace obj f).numberSet == 1 collect f);
				Mesh_faceList = getFaceSelection obj;
				 
				Mesh_faceList = Mesh_faceList as array;
				join allMesh_faceList Mesh_faceList;
			)
			
			if(Poly_faceList.count != 0 or Mesh_faceList.count != 0)then
			(
				selectMore obj;
			)
		)
		
		lbl1.text = "Poly有 " + allPoly_faceList.count as string + " 个重面, Mesh有 " + allMesh_faceList.count as string + " 个重面";
	)
	
	on btn_DelDupFaces pressed do
	(
		local selObj = selection as array;
		if selObj.count == 0 then
		(
			Messagebox "请选择物体！" title:rl_DeleteDupFaces.title;
			return 0;
		)
		for obj in selObj where( (classOf obj == Editable_Poly) or (classOf obj == Editable_Mesh) or (classOf obj == PolyMeshObject)) do
		(
			if( classOf obj == Editable_Poly )then
			(
				polyop.setFaceSelection obj (for f = 1 to getNumFaces obj where (polyop.getElementsUsingFace obj f).numberSet == 1 collect f);
				faceList = polyop.getFaceSelection obj;
				polyop.deleteFaces obj faceList delIsoVerts:true;
			)
			
			if( classOf obj == Editable_Mesh )then
			(
				setFaceSelection obj (for f = 1 to getNumFaces obj where (meshop.getElementsUsingFace obj f).numberSet == 1 collect f);
				faceList = getFaceSelection obj;
				meshop.deleteFaces obj faceList delIsoVerts:true;
			)
		)
		lbl1.text = "已删除。";
	)
)

CreateDialog rl_DeleteDupFaces;

fn Yvi_showObjMaterial =
(
	if selection.count < 1 then
	(
		messagebox "ÇëÑ¡ÔñÎïÌå£¡"
		return 0
	)
	for obj in selection do
	(
		if obj.material ==  undefined then continue
		if (classof obj.material == Multimaterial) then 
		(
			for i = 1 to obj.material.numsubs do
			( 
				obj.material.materialList[i].shaderType = 1
				obj.material.materialList[i].adTextureLock = on
				showTextureMap obj.material.materialList[i] on
			)
		)
		if(classof obj.material == Standardmaterial) then
		(
			obj.material.shaderType = 1
			obj.material.adTextureLock = on
			showTextureMap obj.material on
		)	
	)

)
Yvi_showObjMaterial()
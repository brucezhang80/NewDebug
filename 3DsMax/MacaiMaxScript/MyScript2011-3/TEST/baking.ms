rollout ProcessBaked "处理烘焙物体"
	(
		button saveUVW "保存UV坐标"
		button process "处理烘焙物体"
		button change3 "改贴图通道为3"
		on saveUVW pressed do
		(
			max modify mode
			object_array = (selection as array)
			for obj in object_array do
			(
				select obj
				obj.modifiers[#Automatic_Flatten_UVs].unwrap.save ()
			)
		)
		on process pressed do
		(
			undo "处理烘焙物体" on
			(
				for obj in selection do
				(
					the_modifier = obj.modifiers[#Automatic_Flatten_UVs]
					convertToMesh obj
					MapVertsNumber = meshop.getNumMapVerts obj 3
					meshop.setNumMapVerts obj 1 MapVertsNumber
					face_array = (for number = 1 to obj.numfaces collect number)
					for face_number in face_array do
					(
						the_face = meshop.getMapFace obj 3 face_number
						meshop.setMapFace obj 1 face_number the_face
						theVerts = meshop.getMapVertsUsingMapFace obj 3 #(face_number)
						for i in theVerts do
						(
							original_coordinate = meshop.getMapVert obj 3 i
							meshop.setMapVert obj 1 i original_coordinate
						)
					)
					convertTo obj Editable_Poly
					addModifier obj the_modifier
					obj.modifiers[#Automatic_Flatten_UVs].unwrap.setMapChannel 0
					
					obj.material = obj.material.Baked_Material
					File_name = obj.material.diffuseMap.filename
					obj.Material.name = obj.Material.diffuseMap.name = getFilenameFile File_name
					obj.material.diffuseMap.coordinates.mapChannel = 1
					obj.material.selfIllumAmount = 100
				)
			)
		)
		
		on change3 pressed do
		(
		    if selection.count != 0 then
			(
				for obj in selection do
				(
					if (getClassName obj.material) == "Shell Material" then
						obj.material.Baked_Material.diffuseMap.coordinates.mapChannel = 3
				) 
			)
		)
	)
	createDialog ProcessBaked
if selection.count > 0 do
(
 
	fn addBP obj =
	(
		with undo false
		(
			local oldTM = obj.transform --store the original transformation
			obj.transform *= inverse obj.transform  --transform back into identity matrix
			local size = obj.max - obj.min 
 
			local bb=plane width:size.x length:size.y lengthsegs:1 widthsegs:1 name:(obj.name + "_bp") mapCoords:true realWorldMapSize:false
 
			obj.transform = oldTM  --return back to original transformation
			bb.transform = obj.transform --set rotation and scale of the bbox to match object
			bb.center=obj.center  --center bbox
			bb.pos.z = obj.min.z
		)
		return #(bb, size)
	)
 
	fn resizeBMP file tSize filter:false =	
	(
		with undo false
		(
			if doesFileExist file do
			(
				local tBMP = bitmaptex filename:file 
 
				local bmp = bitmap tSize.x tSize.y
 
				renderMap tBMP into:bmp size:[bmp.width, bmp.height] filter:filter display:false
 
				tBMP = tSize = factor = undefined
			)
			return bmp
		)
	)
 
	fn makeHeightmap objs baseFileName AA:0 =	--main baking function obj:needs a nodeArray to bake, baseFileName:needs a fileName to extent by obj name-s for the baked files, AA: needs an integer 0 1 or 2 ; 0 is no filtering wich is fast and 2 is full filtering wich is slower
	(
		with undo false	--no need to undo this stuff so to make it faster disable undo buffer
		(
			for obj in objs while baseFileName != undefined where isValidNode obj do	--loop through the supplied nodeArray as long as the baseFileName is not undefined --saves an extra if statement and also make sure the current obj is valid
			(
				max create mode	--switch to create mode --faster
 
				local fileName = getfilenamepath baseFileName + getFilenameFile baseFileName + "_" + obj.name + getFilenameType baseFileName	--create the fileName per object based on the baseFileName and obj.name
				local oldRender = renderers.current ; renderers.current  = Default_Scanline_Renderer()	--save the old renderer to var and assign scanline to render faster --for 2011 id suggest quicksilver over scanline
				local oldMat = obj.material ; obj.material = standardmaterial()	--save the obj's material to var and assign standard mat so it works fine with any renderer
 
				local temp = (addBP obj)	--create the baking plane below the obj --function returns the plane and the boxSize of the obj
				local bakingGeo = temp[1] ; select bakingGeo	--make new ref to the plane and select it
				local bmpSize = if bakingGeo.width > bakingGeo.length then bakingGeo.width else bakingGeo.length	--check if its wider than high and set square size accordingly 
 
				max modify mode	--switch to modify mode so the cage can update
 
				local projMod = Projection()	--create a projection mod
				addmodifier bakingGeo projMod	--add the mod to the plane
				projMod.addObjectNode obj	--add  the obj to the mod
				projMod.autowrapCage()	--and update the cage
				projMod.autoWrapAlwaysUpdate = true	--just to be on the save side enable option to always update
 
				max create mode	--and back to the faster create mode
 
				local bi = bakingGeo.INodeBakeProperties	--add some baking props to the plane
					bi.bakeChannel=1	--set map channel used to 1
					bi.bakeEnabled = true	--enable it 
 
				local bp = bakingGeo.INodeBakeProjProperties	--add some projection properties to the plane
					bp.enabled = true	--enable projection
					bp.projectionMod = projMod	--add the proj/cage mod 
					bp.BakeSubObjLevels = true	--make it use sub object levels
					bp.useCage = true	--make it use a cage
					bp.hitResolveMode = #closest	--set hit mode to closest
					bp.hitWorkingModel = true	--make use of the plane too so we got a 'floor'
					bp.heightMapMax = temp[2][3]	--set the max height to the relative height of the obj --important
					bp.heightMapMin = 0	--set min height to the 'floor' --after all its a flat plane
 
				local currentMapType = HeightMap()	--create a heightmap bake element
				(
					currentMapType.outputSzX=bmpSize ; currentMapType.outputSzY=bmpSize	--set width and height of the map
 					currentMapType.fileType= getFilenameType fileName	--set the fileType 
					currentMapType.filterOn = case AA of (0 : false ; 1 : false ; 2 : true)	--and en/disable filtering based on AA setting 
 
					bi.addBakeElement currentMapType	--now add the new heightmap to bake to the bake props
 
					render rendertype:#bakeSelected outputwidth:bmpSize outputheight:bmpSize progressbar:true vfb:false	--render/bake the selected plane with the squaresize previously defined, show progress but no vfb
					local newBMP = currentMapType.bitmap	--heres the trick, get the bitmap out of the bake element :P
 
					if newBMP != undefined do	--check if the bake wasnt aborted
					(
						newBMP.fileName = fileName	--assign the defined fileName to the bitmap
						save newBMP	--save it
						newBMP = resizeBMP fileName [temp[2][1], temp[2][2]] filter:(if AA != 0 then true else false)	--now resize it with the function at the top and the given filtering option
						newBMP.fileName = fileName	--assign the same fileName to the bitmap
						save newBMP	--and save again
					)
					newBMP = undefined	--to give the gc something to work with and make sure you dont fill up the ram make the map undefined
				)
 
				delete bakingGeo	--delete the bakingPlane
				obj.material = oldMat	--reAssign the old mat to the obj
				renderers.current  = oldRender	--and the old renderer
				--next loop
			)
		)
	)
 
	-----------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------	
 
	local bFileName = getSaveFileName types:"JPEG File(*.jpg)|*.jpg|Tiff Image File(*.tif)|*.tif|Targa Image File(*.tga)|*.tga|PNG Image File(*.png)|*.png|BMP Image File(*.bmp)|*.bmp|"	--get a base-FileName to work with later on
 
	local sel = (selection as array)	--save the current selection to a fixed array --important or you might end up baking planes to planes
 
	makeHeightmap sel bFileName AA:1	--call the baking function
 
	select sel	--reSelect the old selection
 
)--END






















if selection.count > 0 do
(
	local sel = selection as array
 
	local baseFileName = getSaveFileName types:"JPEG File(*.jpg)|*.jpg|Tiff Image File(*.tif)|*.tif|Targa Image File(*.tga)|*.tga|PNG Image File(*.png)|*.png|BMP Image File(*.bmp)|*.bmp|"
	-----------------------------------------------------------------------------------------
	-----------------------------------------------------------------------------------------
	fn addBP obj =
	(
		with undo off
		(
			local oldTM = obj.transform --store the original transformation
			obj.transform *= inverse obj.transform  --transform back into identity matrix
			local size = obj.max - obj.min 
 
			local bb=plane width:size.x length:size.y lengthsegs:1 widthsegs:1 name:(obj.name + "_bp") mapCoords:true realWorldMapSize:false
 
			obj.transform = oldTM  --return back to original transformation
			bb.transform = obj.transform --set rotation and scale of the bbox to match object
			bb.center=obj.center  --center bbox
			bb.pos.z = 0
			bb.xray=true
			bb.wireColor = obj.wirecolor
		)
		return #(bb, size)
	)
 
	fn resizeBMP file tSize =
	(
		if doesFileExist file do
		(
			local tBMP = bitmaptex filename:file 
 
			local bmp = bitmap tSize.x tSize.y
 
			renderMap tBMP into:bmp size:[bmp.width, bmp.height] filter:false display:false		--to reduce AA enable filtering here
 
			tBMP = tSize = factor = undefined
		)
		return bmp
	)
 
	for obj in sel while baseFileName != undefined where isValidNode obj do
	(
		max create mode
 
		local fileName = getfilenamepath baseFileName + getFilenameFile baseFileName + "_" + obj.name + getFilenameType baseFileName
 
		local temp = (addBP obj)
		local bakingGeo = temp[1]
		local bmpSize = if bakingGeo.width > bakingGeo.length then [bakingGeo.width, bakingGeo.width] else [bakingGeo.length, bakingGeo.length]	--fuck around with this to change the given bmp size
		bakingGeo.pos.z = obj.min.z
		select bakingGeo
 
		local oldMat = obj.material
		obj.material = standardmaterial()
 
		max modify mode
 
		local projMod = Projection()
		addmodifier bakingGeo projMod
		projMod.addObjectNode obj
		projMod.autowrapCage()
		projMod.autoWrapAlwaysUpdate = true
 
		completeRedraw()
 
		local oldRender = renderers.current 
		renderers.current  = Default_Scanline_Renderer()
 
		local fPath=getFilenamePath fileName
		local fName=getFilenameFile fileName
		local fType = getFilenameType fileName
		local bi = bakingGeo.INodeBakeProperties
		local bp = bakingGeo.INodeBakeProjProperties
 
		bp.enabled = true
		bp.projectionMod = projMod
		bp.BakeSubObjLevels = true
 
		bp.useCage = true
		bp.hitResolveMode = #closest
 
		bp.hitWorkingModel = true
 
		bp.heightMapMax = temp[2][3]
 
		bi.bakeChannel=1
		bi.bakeEnabled = true
 
		local currentMapType = HeightMap()
		(
			currentMapType.autoSzOn = false
			currentMapType.outputSzX=bmpSize[1]
			currentMapType.outputSzY=bmpSize[2]
			currentMapType.filename=fName
			currentMapType.fileType=fType
			currentMapType.targetMapSlotName = "none"
			currentMapType.filterOn = false
 
			bi.addBakeElement currentMapType
 
			render rendertype:#bakeSelected outputSize:bmpSize outputwidth:bmpSize[1] outputheight:bmpSize[2] progressbar:true /*outputfile:newfileName*/ vfb:false
			local newBMP = currentMapType.bitmap
			if newBMP != undefined do
			(
				newBMP.fileName = fileName --(FPath + FName + FType)
				save newBMP
				newBMP = resizeBMP fileName [temp[2][1], temp[2][2]]
				newBMP.fileName = fileName
				save newBMP
			)
			newBMP = undefined
		)
		bi.removeAllBakeElements()
		bi.bakeEnabled = false
 
		renderers.current  = oldRender
 
		clearSelection()
 
		delete bakingGeo
		obj.material = oldMat
	)
)--END Bake






















-----------------------------------------------------------------------------------------
	fn addBP obj =
	(
		with undo off
		(
			local oldTM = obj.transform --store the original transformation
			obj.transform *= inverse obj.transform  --transform back into identity matrix
			local size = obj.max - obj.min 
 
			local bb=plane width:size.x length:size.y lengthsegs:1 widthsegs:1 name:(obj.name + "_bp") mapCoords:true realWorldMapSize:false
 
			obj.transform = oldTM  --return back to original transformation
			bb.transform = obj.transform --set rotation and scale of the bbox to match object
			bb.center=obj.center  --center bbox
			bb.pos.z = 0
			bb.xray=true
			bb.wireColor = obj.wirecolor
		)
		return #(bb, size)
	)
 
	for obj in sel while baseFileName != undefined where isValidNode obj do
	(
		max create mode
 
		local fileName = getfilenamepath baseFileName + getFilenameFile baseFileName + "_" + obj.name + getFilenameType baseFileName
 
		local temp = (addBP obj)
		local bakingGeo = temp[1]
		local bmpSize = if bakingGeo.width > bakingGeo.length then [bakingGeo.width, bakingGeo.width] else [bakingGeo.length, bakingGeo.length]	--fuck around with this to change the given bmp size
		bakingGeo.pos.z = obj.min.z
		select bakingGeo
 
		local oldMat = obj.material
		obj.material = standardmaterial()
 
		max modify mode
 
		local projMod = Projection()
		addmodifier bakingGeo projMod
		projMod.addObjectNode obj
		projMod.autowrapCage()
		projMod.autoWrapAlwaysUpdate = true
 
		completeRedraw()
 
		local oldRender = renderers.current 
		renderers.current  = Default_Scanline_Renderer()
 
		local fPath=getFilenamePath fileName
		local fName=getFilenameFile fileName
		local fType = getFilenameType fileName
		local bi = bakingGeo.INodeBakeProperties
		local bp = bakingGeo.INodeBakeProjProperties
 
		bp.enabled = true
		bp.projectionMod = projMod
		bp.BakeSubObjLevels = true
 
		bp.useCage = true
		bp.hitResolveMode = #closest
 
		bp.hitWorkingModel = true
 
		bp.heightMapMax = temp[2][3]
 
		bi.bakeChannel=1
		bi.bakeEnabled = true
 
		local currentMapType = HeightMap()
		(
			currentMapType.autoSzOn = false
			currentMapType.outputSzX=bmpSize[1]
			currentMapType.outputSzY=bmpSize[2]
			currentMapType.filename=fName
			currentMapType.fileType=fType
			currentMapType.targetMapSlotName = "none"
			currentMapType.filterOn = false
 
			bi.addBakeElement currentMapType
 
			render rendertype:#bakeSelected outputSize:bmpSize outputwidth:bmpSize[1] outputheight:bmpSize[2] progressbar:true /*outputfile:newfileName*/ vfb:false
			local newBMP = currentMapType.bitmap
			if newBMP != undefined do
			(
				newBMP.fileName = (FPath + FName + FType)
				save newBMP
			)
			newBMP = undefined
		)
		bi.removeAllBakeElements()
		bi.bakeEnabled = false
 
		renderers.current  = oldRender
 
		clearSelection()
 
		delete bakingGeo
		obj.material = oldMat
	)
)--END Bake






Global ca
 
rollout ch "Create Quick HeightMap"
(
	button cm "Create HeightMap"
 
 
	on cm pressed do
	(
		if $ != undefined then
		(
			NewObj = snapshot $
			hide $
			select NewObj
			NewObj.material = StandardMaterial color:(color 150 150 150) Glossiness:0.0 specularLevel:0.0 adTextureLock:true adLock:true showInViewport:true		
			NewObj.material.diffuseMap = Gradient_Ramp()
			NewObj.material.diffuseMap.coordinates.W_Angle = -90
			addModifier NewObj (Uvwmap axis:1)
			collapseStack NewObj
			Sky_Light = Skylight()
 
			local d 
			--- Part taken from "Translated Bounding Box" v.0.2 [2009-12-20] by Anubis
			for i in selection do with undo off 
			(
				c = snapshot i
				c.transform = matrix3 1
				d = dummy boxsize:(c.max - c.min)
				delete c
				d.transform = i.transform
				d.pos = i.center
				d.name = "Dummy_Box"
			)
			--------------------------------------------------------------------
 
			c = freecamera ()
			c.name = "CameraTest"
			c.pos = [d.pos.x, d.pos.y, d.pos.z+200]
 
			ca = render camera:$CameraTest outputSize:d.boxsize 
 
 
		)
		else messagebox "Select Object!" 
	)
 
)
createdialog ch



Global ca
 
rollout ch "Create Quick HeightMap"
(
	button cm "Create HeightMap"
 
 
	on cm pressed do
	(
		if $ != undefined then
		(
		NewObj = snapshot $
		hide $
		select NewObj
		NewObj.material = StandardMaterial color:(color 150 150 150) Glossiness:0.0 specularLevel:0.0 adTextureLock:true adLock:true showInViewport:true		
		NewObj.material.diffuseMap = Gradient_Ramp()
		NewObj.material.diffuseMap.coordinates.W_Angle = -90
		addModifier NewObj (Uvwmap axis:1)
		collapseStack NewObj
		Sky_Light = Skylight()
 
--- Part taken from "Translated Bounding Box" v.0.2 [2009-12-20] by Anubis
for i in selection do with undo off (
				c = snapshot i
				c.transform = matrix3 1
				d = dummy boxsize:(c.max - c.min)
				delete c
				d.transform = i.transform
				d.pos = i.center
				d.name = "Dummy_Box"
			)
--------------------------------------------------------------------
 
c = freecamera ()
c.name = "CameraTest"
c.pos = [d.pos.x, d.pos.y, d.pos.z+200]
 
ca = render camera:$CameraTest outputSize:d.boxsize 
 
 
)
 
 
		else messagebox "Select Object!" 
		)
 
)
createdialog ch
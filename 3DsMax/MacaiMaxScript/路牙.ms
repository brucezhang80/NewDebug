function Yvi_applyBitmap obj img =
(
	ConvertToPoly obj
	-- using standard material just.
	theStdMap = StandardMaterial()
	theMap = openBitmap img
	theStdMap.diffusemap = bitmapTexture filename:img
	assignNewName theStdMap.diffuseMap
	theStdMap.glossiness = 0
	
	theStdMap.diffuseMap.name = getFilenameFile img
	theStdMap.name = getFilenameFile img
		
	addmodifier obj (Uvwmap())
	obj.modifiers[#UVW_Map].maptype = 4
	--showTextureMap theMultiMap theMultiMap.materialList[i] on

   
	ConvertToPoly obj
	local zcenter = obj.center.z
	for f = 1 to (polyop.getNumFaces obj) do
	(
		local pos = polyop.getFaceCenter obj f node:obj
		local dir = polyop.getFaceNormal obj f node:obj
		
		if getCommandPanelTaskMode() != #modify do setCommandPanelTaskMode #modify
		-- 顶面
		if (dir.x == 0 and dir.y == 0 and dir.z != 1)then
		(
			polyop.setFaceMatID obj #{f} 1
			
			polyop.setFaceSelection obj #{f}
			
			AddModifier obj (Unwrap_UVW ())
			obj.modifiers[#Unwrap_UVW].mappingMode 5
			--obj.modifiers[#Unwrap_UVW].mappingAlign 3
				
			-- 对选择的面
			obj.modifiers[#unwrap_uvw].unwrap.setProjectionType 4--平均法线
			obj.modifiers[#unwrap_uvw].unwrap5.quickPlanarMap () --快速平面贴图
				
			--相当于
			--obj.modifiers[#unwrap_uvw].setProjectionType 4 -- 平均法线
			--obj.modifiers[#unwrap_uvw].planarMap() 		-- 快速平面贴图

		)
		--侧面
		else if(pos.z == zcenter)then
		(
			polyop.setFaceMatID obj #{f} 1
			polyop.setFaceSelection obj #{f}
			
-- 			AddModifier obj (Unwrap_UVW ())
-- 			obj.modifiers["Unwrap_UVW"].mappingMode 5
			--obj.modifiers["Unwrap_UVW"].mappingAlign 3
		)
	)

	obj.Material = theStdMap
	meditMaterials[1] = obj.Material

	showTextureMap meditMaterials[1] on
)

try(Destroydialog rl_AutoMapForRoad)catch()
rollout rl_AutoMapForRoad "路牙贴图" width:174 height:157
(
	button btn_roadImg "路牙贴图..." pos:[9,9] width:156 height:22 toolTip:"选择路牙的贴图文件"
	
	button btn_ok "选中路牙确定" pos:[8,52] width:156 height:22 toolTip:"确保选择了无材质路牙"
	
	label lbl1 "Label" pos:[14,90] width:145 height:58
	local img = "E:\\Yvi\\MaxFiles\\a3ter008.jpg"
	
	on btn_roadImg pressed do
	(
		f = getOpenFileName caption:"选择路牙贴图文件:" filename:(pathConfig.getCurrentProjectFolder()+@"\")types:"JPEG(*.jpg)|*.jpg|PNG(*.png)|*.png|TGA|*.tga|BMP(*.bmp)|*.bmp|ALL(*.*)|*.*"
		if(f == undefined)then
		(
			return 0
		)
		img = f
	)
	
	on btn_ok pressed do
	(
		obj  = selection[1]
		if selection.count !=1 then
		(
			return 0
		)
		Yvi_applyBitmap obj img
	)
	
)
CreateDialog rl_AutoMapForRoad

--(pathConfig.getCurrentProjectFolder()+@"\tex.jpg") JPEG(*.jpg)|*.jpg|PNG(*.png)|*.png|TGA|*.tga|BMP(*.bmp)|*.bmp|
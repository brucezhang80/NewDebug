GLOBAL mapPath = ""
GLOBAL rl_gymap
------------function----------------------------------------------------------------
	--2012.2.6
	/*现使用的是l8贴图*/
	/*新的左下角坐标:X:36309999.25 Y:2880000.00*/ 
 -----------------
	-- 左下角:X:36349216.000 Y;2913881.500
	-- 右上角:X:36389948.000 Y:2965555.750

	-- 36349425.0  2913840.0
	--得 到地形对应的图片
	-- coord 是中心坐标
	function Fn_getTerrainTileImage coord =
	(
		-- 每一块L9图片的长度和高度
		--local w = 120000.0 / 256
		
		-- -2012.2.6
		local w = 937.5
		--local w = 120000.0 / 128
		
		local minx = 36309999.25 --36310000.0
		local miny = 2880000.00 --2880000.0
		
		local x = ((coord.x - minx) / w)as integer
		local y = ((coord.y - miny) / w)as integer
		if(x < 0 or x > 128)then -- 255
		(
			return undefined
		)
		else if (y < 0 or y >128)then --255
		(
			return undefined
		)
		local xx = x as string
		local yy = y as string
		--print ("xx: "+xx +"  "+ "yy: "+yy)
		
		-- gy_L9_X255_Y255.tga
		local fname = "gy_l8_x" + xx + "_y" + yy + ".jpg"
		--print ("---- "+fname)
		return  fname
	)

	-- 计算纹理坐标
	function FN_computTexCoord v =
	(
		w = 120000.0 / 128
		uv_x = (mod v.x w) / w
		uv_y = (mod v.y w) / w
		v.z = 0
		uv_z = v.z
		--print ("uv_x - uv_y - uv_z: "+uv_x as string + "_"+ uv_y as string + "_" + uv_z as string)
		return [uv_x, uv_y, uv_z]
	)

	function Fn_autoTerrainMap obj =
	(
		local arrVerts = #()
		local img = ""
		--ConvertToMesh obj
		ConvertToPoly obj
------------------------------------------------------UV
-- 		obj.numtverts = obj.numverts
-- 		--得到顶点
-- 		for v = 1 to obj.numverts do
-- 		(
-- 			vert = getvert obj v -- return position
-- 			append arrVerts vert
-- 		)
-- 		
-- 		meshop.setNumMapVerts obj 1 obj.numtverts
-- 		--设置纹理顶点
-- 		for i = 1 to obj.numtverts do
-- 		(
-- 			meshop.setMapVert obj 1 i (FN_computTexCoord arrVerts[i])
-- 		)

-- 		update obj
---------------------------------------------------------	

		max modify mode
		select obj
		subobjectLevel = 5
		
		local terrainName = FN_getTerrainTileImage obj.center
		if(terrainName == undefined)then
		(
			print ("不存在这个文件: " + terrainName as string)
		)
		else
		(
			local img = rl_gymap.edt_mapPath.text + "\\" + terrainName
			if rl_gymap.edt_mapPath.text == "" then
			(
				messagebox ("请输入贴图路径!")
				return 0
			)
			if(getfiles img).count == 0 then
			(
				print ("找不到文件: " + img as string)
			)
			else
			(
				print "---------------------"
				--if classof obj.Material == undefined then
				(
-- 					--原标准材质添加到新多维材质中
-- 					oldMat = obj.material 
-- 					multimat = multimaterial numsubs:2
-- 					multimat.materialList[1] = oldMat
-- 					
-- 					--添加一个材质
-- 					polyop.setMapSupport obj 3 true
-- 					multimat.materialList[2].selfillumMap = bitmapTexture filename:img
-- 					multimat.materialList[2].selfillumMap.coords.mapChannel = 3 -- 设置在第三通道
-- 					multimat.materialList[2].selfillumMap.name = getFilenameFile img
-- 					multimat.materialList[2].name = getFilenameFile img
-- 					obj.Material = multimat
-- 					
					
					
-- 				meditMaterials[2].alphaSource = 2
-- 				meditMaterials[1].DirectX_Manager.enabled = off
-- 				meditMaterials[1].selfillumMap = meditMaterials[2]
-- 				$.material = meditMaterials[1]
				
-- 					macros.run "Modifier Stack" "Convert_to_Poly"
-- 					modPanel.addModToSelection (Uvwmap ()) ui:on
-- 					$.modifiers[#UVW_Map].maptype = 4
-- 					$.modifiers[#UVW_Map].height = 200
-- 					$.modifiers[#UVW_Map].width = 200
-- 					$.modifiers[#UVW_Map].length = 200

-- $.modifiers[#UVW_Map].mapChannel = 3
-- $.modifiers[#UVW_Map].maptype = 4

					--给 obj 一个标准的子材质
					meditMaterials[1] = Standard()
					meditMaterials[1].diffusemap = bitmapTexture filename:"F:\Max_Test\金阳外围地块MAX文件\maps\Grassland_qiao_32.jpg" 
					--obj.Material = meditMaterials[1]
					modPanel.addModToSelection (Uvwmap ()) ui:on
					obj.modifiers[#UVW_Map].mapChannel = 1
					obj.modifiers[#UVW_Map].maptype = 4
					obj.modifiers[#UVW_Map].height = 200
					obj.modifiers[#UVW_Map].width = 200	
					obj.modifiers[#UVW_Map].length = 200
					macros.run "Modifier Stack" "Convert_to_Poly"
											
					--添加一个航片贴图
					meditMaterials[2] = Standard()
					meditMaterials[2].diffusemap = bitmapTexture filename:img
					--meditMaterials[2].diffusemap.filename = img
					meditMaterials[2].diffusemap.alphaSource = 2
					
					polyop.setMapSupport obj 3 true
					meditMaterials[2].diffusemap.coords.mapChannel = 3 -- 设置在第三通道
					meditMaterials[1].selfillumMap = meditMaterials[2].diffusemap
						
					obj.Material = meditMaterials[1]
					--添加UV
					modPanel.addModToSelection (Uvwmap ()) ui:on
						
					obj.modifiers[#UVW_Map].mapChannel = 3
					obj.modifiers[#UVW_Map].maptype = 4
					macros.run "Modifier Stack" "Convert_to_Poly"
						
					showTextureMap obj.material obj.material.diffusemap on

					obj.Material.diffusemap.name = getFilenameFile obj.material.diffusemap.filename
					obj.Material.selfillumMap.name = getFilenameFile img
					obj.Material.name = getFilenameFile img
					obj.name = substring (getFilenameFile img) 7 -1
				) -- end if
				print "OK"
			) -- else
			
		)
		subobjectLevel = 0
		gc()
		freeSceneBitmaps()
	)
	
----------------------------------UI--------------------------------------
	
try(Destroydialog rl_gymap)catch()
Rollout rl_gymap "贵阳_贴航片"
(

	--Button btn_mapPath "指定贴图路径..." pos:[4,6] width:153
	label lb_path "贴图路径:" pos:[4,4]
	EditText edt_mapPath "" pos:[4,18] width:153 --text:"F:\Max_Test\金阳外围地块MAX文件\maps"
	label lb_map "贴图:"  pos:[4,36] width:28 align:#left
	label lb_mapName ""  pos:[100,36]
	Button btn_doIt ">>Start<<"
	checkbox chb_isSingle "单个物体处理!"
	progressBar pgb_Progress1 ""  width:140 height:8 color:Green 

	
	on edt_mapPath changed txt do
	(
		edt_mapPath.text = txt
		mapPath = edt_mapPath.text
		return mapPath 
	)

	on btn_doIt pressed do
	(
		local i = 0
		if edt_mapPath.text == "" then
		(
			messagebox "请指定图片文件路径"
			return 0
		)
		sels = selection as array
		if sels.count == 0 then 
		(
			messagebox "Please select object!" title:"sorry"
			return false
		)
		else
		(
			
			if chb_isSingle.checked then
			(
				if sels.count > 1 do
				(
					messagebox "You select too many objects!" title:"sorry"
					clearselection()
					return 0
				)
				FN_autoTerrainMap sels[1]
				pgb_Progress1.value = 100.0 * (i += 1) / sels.count
			)
			else
			(
				for obj in sels do
				(
					Fn_autoTerrainMap obj
					pgb_Progress1.value = 100.0 * (i += 1) / sels.count
				)
				print "Completed!"
				messagebox "Completed!" title:"gy_map"
			)
		)
		
	)
	-------------call back----------
	on rl_gymap close do
	(
		callbacks.removeScripts id:#Fn_callBackSelObj01
	)
	
)Createdialog rl_gymap

------------------------call back-----------------------------
function Fn_callBackSelObj =
(
	s = selection as array
	--if s == undefined do return false
	if s.count != 1 do
	(
		if s.count > 1 then
		(
			if rl_gymap.chb_isSingle.checked then
			(
				rl_gymap.lb_mapName.caption = "Too much..."
				return false
			)
			else
			(
				rl_gymap.lb_mapName.caption = "You'v selected " + s.count as string + " objects!"
			)
			
		)
		else
		(
			rl_gymap.lb_mapName.caption = "no select"
			return false
		)
	)
	if s.count == 1 do
	(
		fname = FN_getTerrainTileImage (s[1].center)
		if fname != undefined then
			rl_gymap.lb_mapName.caption = fname
		else
			rl_gymap.lb_mapName.caption = "undefined"
	)
)
Callbacks.AddScript #selectionSetChanged "Fn_callBackSelObj()" ID:#Fn_callBackSelObj01




-----------------------------------------------------
-----------------------------------------------------
-----------------------------------------------------


/*
	for v = 1 to obj.numverts do
	( 
		normalize_pos = ((getvert obj v) - obj.min) / (obj.max - obj.min)
		-- 反转 .position 各元素,使其方向垂直于平面贴图
		case direction of
		(
			#x:
			( 
				tmp = normalize_pos.x
				normalize_pos.x = normalize_pos.y
				normalize_pos.y = normalize_pos.z
				normalize_pos.z = tmp
			)
			#y:
			( 
				tmp = normalize_pos.y
				normalize_pos.y = normalize_pos.z
				normalize_pos.z = tmp
			)
		)
		-- 设置相应的纹理顶点的位置
		settvert obj v normalize_pos
	)
*/
--为对象赋予贴图材质
-- function FN_autoTerrainMap obj 
-- (
-- 	local oldcoordsys, normalize_pos

-- 	ConvertToMesh obj
-- 	
-- 	------------------------------------------------------------------------------------------
-- 	-- 设置纹理顶点的数目等于Mesh(obj)对象的顶点数相等
-- 	obj.numtverts = obj.numverts
-- 	-- 建立纹理顶点面
-- 	buildTVFaces obj

-- 	-- 下面的扣件在Objects坐标系下进行
-- 	-- 先存储现有坐标系统,以便操作完成后恢复它
-- 	oldcoordsys = set coordsys local
-- 	
-- 	-- 对Mesh的每一个顶点的 .position 属性进行标准化运算. 其取值范围为 [0,0,0] 到 [1,1,1], 其结果为平面UVW坐标系
-- 	for v = 1 to obj.numverts do
-- 	( 
-- 		normalize_pos = ((getvert obj v) - obj.min) / (obj.max - obj.min)
-- 		/*
-- 		-- 反转 .position 各元素,使其方向垂直于平面贴图
-- 		case direction of
-- 		(
-- 			#x:
-- 			( 
-- 				tmp = normalize_pos.x
-- 				normalize_pos.x = normalize_pos.y
-- 				normalize_pos.y = normalize_pos.z
-- 				normalize_pos.z = tmp
-- 			)
-- 			#y:
-- 			( 
-- 				tmp = normalize_pos.y
-- 				normalize_pos.y = normalize_pos.z
-- 				normalize_pos.z = tmp
-- 			)
-- 		)
-- 		*/
-- 		-- 设置相应的纹理顶点的位置
-- 		settvert obj v normalize_pos
-- 	)

-- 	-- 完成位置纹理顶点后, 建立纹理顶点面,
-- 	-- Mesh (obj) 和 其纹理顶点之间存在一一对应的关系,mesh 对象的面上顶点的序号与纹理面上的顶点的序号相同
-- 	for f = 1 to obj.numfaces do
-- 	(
-- 		setTVFace obj f (getface obj f)
-- 		-- 完成所有操作后,恢复坐标系统
-- 		set coordsys oldcoordsys
-- 		-- 刷新 Mesh 对象
-- 		update obj
-- 	)

-- 	-- 给 mesh (obj) 对象施加一种材质,我们规定为 StandardMaterial
-- 	-------------------------------------------------------------------------------------------- 
-- 	local img = FN_getTerrainTileImage obj.center
-- 	
-- 	newmat = StandardMaterial()
-- 	obj.Material = newmat
-- 	obj.DiffuseMap.fileName = img
-- 	showTextureMap obj.material obj.material.diffusemap on


-- )

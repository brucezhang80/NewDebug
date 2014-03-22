
function Yvi_extrudeLineToObj obj _height _segs=
(
	addmodifier obj (Extrude())
	obj.modifiers[#Extrude].amount = _height 	-- 挤出高度
	obj.modifiers[#Extrude].segs = _segs		-- 挤出段数
	--obj.modifiers[#Extrude].capEnd = on				-- 封口末端开（默认）
	obj.modifiers[#Extrude].capStart = off			-- 封口始端关
	--obj.modifiers[#Extrude].matIDs = on				--生成材质ID（默认）
		--obj.modifiers[#Extrude].useShapeIDs = on			-- 使用图形ID
		
	obj.modifiers[#Extrude].mapcoords = on			-- 生成贴图坐标	
	--$.modifiers[#Extrude].capType = 0				-- 0变形（默认），1栅格
	--$.modifiers[#Extrude].output = 1				-- 0面片，1网格（默认），2NURBS
	--$.modifiers[#Extrude].realWorldMapSize = off 	-- 真实世界贴图大小
	--$.modifiers[#Extrude].smooth = on				-- 平滑（默认）
	
	update obj
	ConvertToPoly obj
	
	--redrawViews()
	return obj;
)

function Yvi_setParapet obj _height _width=
(
	numfaces = obj.GetNumFaces()
	obj.SetSelection #Face #{numfaces} -- 选中最后一个面
	max modify mode
	--subobjectLevel = 4
/*
	--这种方式对于单个处理倒是可以。多个处理就不起作用了
	obj.insetAmount = _width				-- 插入厚度
	obj.buttonOp #Inset
	obj.faceExtrudeHeight = _height					-- 挤出高度	
	obj.buttonOp #Extrude
*/
	--多个处理时这个起到作用
--	obj.extrudeFaces _height				-- 挤出高度
	
	selface = polyOp.getFaceSelection obj
	polyop.bevelFaces obj selface 0 _width

	polyOp.extrudeFaces obj selface _height
	
	obj.SetSelection #Face #{}

	ConvertToPoly obj
	update obj
	
	redrawViews()
)


--为对象赋予贴图材质
function Yvi_autoBuildMap obj direction =
(
	local oldcoordsys, normalize_pos

	ConvertToMesh obj
	
	------------------------------------------------------------------------------------------
	-- 设置纹理顶点的数目等于Mesh(obj)对象的顶点数相等
	obj.numtverts = obj.numverts
	
-- 	for f = 1 to (meshop.getNumFaces obj) do
-- 	(
-- 		setFaceSelection obj #{f}
-- 		meshop.getVertsUsedOnlyByFaces obj #{f}
-- 		selface = getFaceSelection obj

	-- 建立纹理顶点面
	buildTVFaces obj

	-- 下面在Objects的坐标系下进行
	-- 先存储现有坐标系,以便操作完成后恢复它
	oldcoordsys = set coordsys local
-- 	verts = meshop.getVertsUsingFace obj #{f}
	-- 对Mesh的每一个顶点的 .position 属性进行标准化运算. 其取值范围为 [0,0,0] 到 [1,1,1], 其结果为平面UVW坐标系
	for v = 1 to obj.numverts do
	( 
		-- 
		normalize_pos = ((getvert obj v) - obj.min) / (obj.max - obj.min)
		--format "% , normalize_pos:%\n" (getvert obj v) normalize_pos
		-- 反转 .position 各元素,使其方向垂直于平面贴图
		case direction of
		(
			#x:
			( 
				tmp = normalize_pos.x
				normalize_pos.x = normalize_pos.y
				normalize_pos.y = normalize_pos.z
				normalize_pos.z = tmp
				
				-- 设置相应的纹理顶点的位置
				settvert obj v normalize_pos
			)
			#y:
			( 
				tmp = normalize_pos.y
				normalize_pos.y = normalize_pos.z
				normalize_pos.z = tmp
				
				-- 设置相应的纹理顶点的位置
				settvert obj v normalize_pos
			)
			#z:
			(
				
			)
			#default:
			(

-- 				tmp = normalize_pos.x
-- 				normalize_pos.x = normalize_pos.z
-- 				normalize_pos.y = normalize_pos.x
-- 				normalize_pos.z = tmp
				
				-- 设置相应的纹理顶点的位置
				settvert obj v normalize_pos
			)
		)
		
		-- 设置相应的纹理顶点的位置
		--settvert obj v normalize_pos
	)
-- )
	-- 完成位置纹理顶点后, 建立纹理顶点面,
	-- Mesh (obj) 和 其纹理顶点之间存在一一对应的关系,mesh 对象的面上顶点的序号与纹理面上的顶点的序号相同
	for f = 1 to obj.numfaces do
	(
		setTVFace obj f (getface obj f)
		-- 完成所有操作后,恢复坐标系统
		set coordsys oldcoordsys
		-- 刷新 Mesh 对象
		update obj
	)
)

-- ///////////////////////////////////////////////////////////////////////
function Yvi_setTVFace obj direction =
(
	local normalize_pos,oldcoordsys
	ConvertToMesh obj
	--ConvertToPoly obj
	-- 设置纹理顶点的数目等于Mesh(obj)对象的顶点数相等
	--obj.numtverts = obj.numverts
	
	-- 建立纹理顶点面
	buildTVFaces obj
	
	-- 下面在Objects的坐标系下进行
	-- 先存储现有坐标系,以便操作完成后恢复它
	oldcoordsys = set coordsys local
		
	for f = 1 to (Meshop.getNumFaces obj) do
	(
		setFaceSelection obj #{f}
		selface = getFaceSelection obj
		vertsSel = Meshop.getVertsUsingFace obj selface 
		setVertSelection obj vertsSel
		
		update obj --update the mesh
		max modify mode --switch to Modify panel
		subObjectLevel =1 --set Vertex SO level

		--verts = meshop.getVertsUsedOnlyByFaces obj #{f}
		--format "%\n" vertsSel
		for v in vertsSel do
		(
			normalize_pos = ((getvert obj v) - obj.min) / (obj.max - obj.min)
			--format "% , normalize_pos:%\n" (getvert obj v) normalize_pos
			
			--得到面的方向，按面的方向来垂直贴图
			
			-- 反转 .position 各元素,使其方向垂直于平面贴图
			case direction of
			(
				#x: -- 侧边墙面
				(
					tmp = normalize_pos.x
					normalize_pos.x = normalize_pos.y
					normalize_pos.y = normalize_pos.z
					normalize_pos.z = tmp
					
				)
				#y: -- 门墙 背墙
				( 
					tmp = normalize_pos.y
					normalize_pos.y = normalize_pos.z
					normalize_pos.z = tmp
				)
				#z: -- 房顶
				(
					
				)
				#default: -- 其它
				(
				)
			)
			-- 设置相应的纹理顶点的位置
			settvert obj v normalize_pos
		) -- loop vert in selface
		
		
	) -- loop face in obj
		
	-- 完成位置纹理顶点后, 建立纹理顶点面,
	-- Mesh (obj) 和 其纹理顶点之间存在一一对应的关系,mesh 对象的面上顶点的序号与纹理面上的顶点的序号相同
	for f = 1 to obj.numfaces do
	(
		setTVFace obj f (getface obj f)
		-- 完成所有操作后,恢复坐标系统
		set coordsys oldcoordsys
		-- 刷新 Mesh 对象
		update obj
	)
)

-- 多维材质自动贴图，这种速度比较快
function Yvi_applyBitmap obj imgfiles =
(
	theMultiMap = multiMaterial numSubs:(imgfiles.count) --name:(uniquename "Map #")

	for i = 1 to imgfiles.count do
    (
		theMap = openBitmap imgfiles[i]
		tmpMat = theMultiMap.materialList[i]
		tmpMat.diffuseMap = (bitMaptexture bitMap:theMap) 
		assignNewName tmpMat.diffuseMap
		tmpMat.glossiness = 0
		tmpMat.diffuseMap.name = getFilenameFile imgfiles[i]
		tmpMat.name = getFilenameFile imgfiles[i]
		
		theMultiMap.materialList[i] = tmpMat
		
		addmodifier obj (Uvwmap())
		obj.modifiers[#UVW_Map].maptype = 4
		--showTextureMap theMultiMap theMultiMap.materialList[i] on
    )
   
	ConvertToPoly obj
	local zpos = obj.center.z
	local i = 0
	for f = 1 to (polyop.getNumFaces obj) do
	(
		i += 1
		local pos = polyop.getFaceCenter obj f node:obj
		local dir = polyop.getFaceNormal obj f node:obj

		if(zpos == pos.z)then
		(
			if i == 3 then
			(
				i = 1
			)
			polyop.setFaceMatID obj #{f} i
			--print "-----------------------"
			--format "%\t%\n" zpos pos.z
		)
		else
		(
			polyop.setFaceMatID obj #{f} 3
		)
	)
	
	obj.Material = theMultiMap
	meditMaterials[1] = obj.Material
	all = meditMaterials[1].materialIDList
	for i = 1 to all.count do 
	(
		showTextureMap meditMaterials[1].materialList[all[i]] on
	)
)

-- 瓦房贴图
function Yvi_setHouseBitmap obj imgfiles =
(
	theMultiMap = multiMaterial numSubs:(imgfiles.count) --name:(uniquename "Map #")

	for i = 1 to imgfiles.count do
    (
		theMap = openBitmap imgfiles[i]
		tmpMat = theMultiMap.materialList[i]
		tmpMat.diffuseMap = (bitMaptexture bitMap:theMap) 
		assignNewName tmpMat.diffuseMap
		tmpMat.glossiness = 0
		tmpMat.diffuseMap.name = getFilenameFile imgfiles[i]
		tmpMat.name = getFilenameFile imgfiles[i]
		
		theMultiMap.materialList[i] = tmpMat
		
		addmodifier obj (Uvwmap())
		obj.modifiers[#UVW_Map].maptype = 4
		--showTextureMap theMultiMap theMultiMap.materialList[i] on
    )
   
	ConvertToPoly obj
	local zpos = obj.center.z
	local i = 0
	for f = 1 to (polyop.getNumFaces obj) do
	(
		i += 1
		local pos = polyop.getFaceCenter obj f node:obj;
		local dir = polyop.getFaceNormal obj f node:obj;
		local _area = polyop.getFaceArea obj f;
		-- 3 means triangle, 4 means quad
		local _degree = obj.GetFaceDegree f;
		
		if(_degree == 3 )then
		(
			i = 3;
			polyop.setFaceMatID obj #{f} i;

		)
		else if( (abs dir.x+ abs dir.y + abs dir.z) > 1 )then
		(
			i = 4;
			polyop.setFaceMatID obj #{f} i;
		)
		else
		(
			if i == 4 then
				i = 1;
			polyop.setFaceMatID obj #{f} i;
		)
		format "%\n" dir;
		
	)
	
	obj.Material = theMultiMap
	meditMaterials[1] = obj.Material
	all = meditMaterials[1].materialIDList
	for i = 1 to all.count do 
	(
		showTextureMap meditMaterials[1].materialList[all[i]] on
	)
)
function Yvi_CreateTileRoofedHouse obj _housetopHeight _housetopWidth _penthouseLenght = 
(
	if (classOf obj) == Editable_Poly then
	(
		_verts = polyop.getVertsUsingFace obj #{obj.numfaces};
		_edges = polyop.getEdgesUsingFace obj #{obj.numfaces};

		-- 得到最小边
		local lenghtArr = #();
		for _e in _edges do
		(
			_vs = polyop.getVertsUsingEdge obj #{_e};
			_vs = _vs as array;
			p1 = polyop.getVert obj _vs[1];
			p2 = polyop.getVert obj _vs[2];
			dis = distance p1 p2
			
			if( finditem lenghtArr dis) == 0 then
				append lenghtArr dis;
		)
		_min_lenght = amin lenghtArr;
		
		local minLenEdges=#();
		local connVextsArr = #();
		for _e in _edges do
		(
			_vs = polyop.getVertsUsingEdge obj #{_e};
			_vs = _vs as array;
			p1 = polyop.getVert obj _vs[1];
			p2 = polyop.getVert obj _vs[2];
			dis = distance p1 p2;

			if(dis <= _min_lenght)then
			(
				append minLenEdges _e;
				append connVextsArr _vs[1];
				append connVextsArr _vs[2];
			)
		)
		minLenEdges = minLenEdges as bitArray;
		obj.SetSelection #Edge minLenEdges;

---------------------------
		-- 选择顶面
		obj.SetSelection #Face #{obj.numfaces}
-----
		--得到最小边后，连接这两条边并拉高
		if(obj.ConnectEdges edgeFlag:1;)then
		(
			--polyop.getEdgeSelection obj;
			move $.selectedEdges [0, 0, _housetopHeight];
		)
		--从最小边得到的点连接。
		connVextsArr = connVextsArr as bitArray;
		obj.SetSelection #Vertex connVextsArr;
		obj.ConnectVertices vertexFlag:1;
-----
		-- 顶面拉高后再挤出
		selface = polyOp.getFaceSelection obj;
		-- 屋檐伸出长度
		polyop.bevelFaces obj selface 0 _penthouseLenght;
		-- 屋顶挤出高度
		polyOp.extrudeFaces obj selface _housetopWidth;
		
		update obj;
		redrawViews();
---------------------------
	)
	else
		"Please selectin Editable_Poly.";
	
	subobjectLevel = 0;
	max create mode;
)



try(DestroyDialog rl_AutoBuildings)catch()
rollout rl_AutoBuildings "自动建模" width:220 height:400
(
	
	Group "居民楼"
	(
		EditText edt_bitmapDir "" across:2 width:140
			Button btn_setBitmapDir "墙壁..." align:#right
		label lbl_buildHight "楼高度：" align:#left
			spinner spn_floorHeight "高度:" range:[1,1000,30] width:80 align:#right scale:1 toolTip:"设置每层楼的高度"across:2
			spinner spn_numFloor "分段："  type:#integer range:[1,1000,1] width:80 align:#right scale:1 toolTip:"设置楼的层数"
		
		CheckBox ckb_randFloor "随机高度" align:#right width:180
			spinner spn_randMinHeight "最低：" range:[1,1000,1] width:80 align:#right scale:1 toolTip:"设置每层楼的随机最小高度" across:2 enabled:false
			spinner spn_randMaxHeight "最高：" range:[1,1000,10] width:80 align:#right scale:1 toolTip:"设置每层楼的随机最大高度" enabled:false
		label lbl_parapet "女儿墙："align:#left 
			spinner spn_parapetHeight "高度：" range:[0.,1000,0.5] width:80 align:#right scale:0.1 toolTip:"设置女儿墙的高度" across:2
			spinner spn_parapetWidth "厚度：" width:80 range:[0.,1000.,0.2] align:#right scale:0.1 toolTip:"设置每层楼的厚度"
	)
	button btn_CreateBuildings "建立居民楼" width:120
	
	Group "瓦房"
	(
		label lbl_houseHight "房高：" align:#left
		spinner spn_houseHeight "高度:" range:[1,1000,30] width:80 align:#right scale:1 toolTip:"设置房子的（不包含屋顶）高度"across:2
		spinner spn_numSegs "分段："  type:#integer range:[1,1000,1] width:80 align:#right scale:1 toolTip:"设置房子的层数"
		
		CheckBox ckb_randHouse "随机高度" align:#right width:180
			spinner spn_randMinTop "最低：" range:[1,1000,1] width:80 align:#right scale:1 toolTip:"设置房子最小高度" across:2 enabled:false
			spinner spn_randMaxTop "最高：" range:[1,1000,10] width:80 align:#right scale:1 toolTip:"设置房子最大高度" enabled:false
		label lbl_houseTop "房顶高度："align:#left 
			spinner spn_houseTopHeight "高度：" range:[0.,1000,1] width:80 align:#right scale:0.1 toolTip:"设置屋顶的高度" across:3
			spinner spn_houseTopWidth "厚度：" width:80 range:[0.,1000.,0.2] align:#right scale:0.1 toolTip:"设置屋顶的厚度"
			spinner spn_pentLenght "屋檐：" width:80 range:[0.,1000.,0.2] align:#right scale:0.1 toolTip:"设置屋檐长度"
			
	)
	button btn_BuildTileHouse "建立瓦房" width:120
--/////////////////////////////////////////////////////////////////////////////////////////
	local bitmap_files = #()
	
	on btn_setBitmapDir pressed do
	(
		bitmap_dir = getSavePath caption:"选择贴图文件的位置..."
		if(bitmap_dir == undefined)then
		(
			print "......"
			return 0
		)
		edt_bitmapDir.text = bitmap_dir
		bitmap_files = getFiles (bitmap_dir+"/*.jpg")
	)
	
	on ckb_randFloor changed state do
	(
		if state == on then
		(
			spn_floorHeight.enabled = false
			spn_numFloor.enabled = false
			spn_randMinHeight.enabled = true
			spn_randMaxHeight.enabled = true
		)
		else
		(
			spn_floorHeight.enabled = true
			spn_numFloor.enabled = true
			spn_randMinHeight.enabled = false
			spn_randMaxHeight.enabled = false
		)
	)

	on ckb_randHouse changed state do
	(
		if state == on then
		(
			spn_houseHeight.enabled = false
			spn_numSegs.enabled = false
			spn_randMinTop.enabled = true
			spn_randMaxTop.enabled = true
		)
		else
		(
			spn_houseHeight.enabled = true
			spn_numSegs.enabled = true
			spn_randMinTop.enabled = false
			spn_randMaxTop.enabled = false
		)
	)
	
	on btn_CreateBuildings pressed do
	(
		local img1 = "E:\\test\\1.jpg"
		local img2 = "E:\\test\\2.jpg"
		local img3 = "E:\\test\\3.jpg"
		local imgfiles = #(img1, img2, img3)
		
		_segs = spn_numFloor.value
		_height = spn_floorHeight.value
		ph = -spn_parapetHeight.value
		pw = -spn_parapetWidth.value

		if(selection.count == 0)then
		(
			MessageBox "请选择线框物体!" title:rl_AutoBuildings.title
		)
-- 		undo on
		(
			if ckb_randFloor.state == on then
			(
				for obj in selection do
				(
					_height = random spn_randMinHeight.value spn_randMaxHeight.value
					if(ClassOf obj == Editable_Poly or ClassOf obj == Editable_Mesh)then
					(
						format "请选择线框类型的物体!\n"
						continue
					)
					Yvi_extrudeLineToObj obj _height 1
					Yvi_setParapet obj ph pw

					Yvi_applyBitmap obj imgfiles
					--Yvi_setTVFace obj #x
					gc()
					freesceneBitmaps()
				)
			)
			else
			(
				for obj in selection do
				(
					if(ClassOf obj == Editable_Poly or ClassOf obj == Editable_Mesh)then
					(
						format "请选择线框类型的物体!\n"
						continue
					)
					Yvi_extrudeLineToObj obj _height _segs
					
					Yvi_setParapet obj ph pw

					Yvi_setTVFace obj #x
					Yvi_applyBitmap obj imgfiles
					--
					--deselect obj
				)
			)
			max create mode
		)-- undo
	)
	
------
	on btn_BuildTileHouse pressed do
	(
		local selObj =  selection as array;
		
		local img1 = "E:\\test\\1.jpg";
		local img2 = "E:\\test\\2.jpg";
		local img3 = "E:\\test\\3.jpg";
		local img4 = "E:\\test\\4.jpg";
		local imgfiles = #(img1, img2, img3, img4);
		
		_segs = spn_numSegs.value; -- 房子层数
		_height = spn_houseHeight.value;--房子高度
		houseTopHeight = spn_houseTopHeight.value;-- 房顶高度
		houseTopWidth = spn_houseTopWidth.value;-- 房顶厚度
		pentLenght = spn_pentLenght.value;-- 屋檐长度
		
		if(selObj.count == 0)then
		(
			MessageBox "请选择线框物体!" title:rl_AutoBuildings.title;
		)
		clearSelection();
		if ckb_randFloor.state == on then
		(
			for obj in selObj do
			(
				_height = random spn_randMinTop.value spn_randMaxTop.value;
				if(ClassOf obj == Editable_Poly or ClassOf obj == Editable_Mesh)then
				(
					format "请选择线框类型的物体!\n";
					continue;
				)
				_poly = Yvi_extrudeLineToObj obj _height _segs;
				select _poly;
				Yvi_CreateTileRoofedHouse _poly houseTopHeight houseTopWidth;
				gc();
				freesceneBitmaps();
			)
		)
		else
		(
			for obj in selObj do
			(
				if(ClassOf obj == Editable_Poly or ClassOf obj == Editable_Mesh)then
				(
					format "请选择线框类型的物体!\n";
					continue;
				)
				_poly = Yvi_extrudeLineToObj obj _height _segs;
				select _poly;
				Yvi_CreateTileRoofedHouse _poly houseTopHeight houseTopWidth houseTopWidth;
				
				--Yvi_setTVFace _poly #x
				Yvi_setHouseBitmap _poly imgfiles;
				
				gc();
				freesceneBitmaps();
			)
		)
		clearSelection();
	)
	
	
	on rl_AutoBuildings open do
	(
		ClearListener()
	)
)
CreateDialog rl_AutoBuildings


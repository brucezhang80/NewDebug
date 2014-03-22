GLOBAL mapPath = ""
GLOBAL rl_gymap
------------function----------------------------------------------------------------
	--2012.2.6
	/*��ʹ�õ���l8��ͼ*/
	/*�µ����½�����:X:36309999.25 Y:2880000.00*/ 
 -----------------
	-- ���½�:X:36349216.000 Y;2913881.500
	-- ���Ͻ�:X:36389948.000 Y:2965555.750

	-- 36349425.0  2913840.0
	--�� �����ζ�Ӧ��ͼƬ
	-- coord ����������
	function Fn_getTerrainTileImage coord =
	(
		-- ÿһ��L9ͼƬ�ĳ��Ⱥ͸߶�
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

	-- ������������
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
-- 		--�õ�����
-- 		for v = 1 to obj.numverts do
-- 		(
-- 			vert = getvert obj v -- return position
-- 			append arrVerts vert
-- 		)
-- 		
-- 		meshop.setNumMapVerts obj 1 obj.numtverts
-- 		--������������
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
			print ("����������ļ�: " + terrainName as string)
		)
		else
		(
			local img = rl_gymap.edt_mapPath.text + "\\" + terrainName
			if rl_gymap.edt_mapPath.text == "" then
			(
				messagebox ("��������ͼ·��!")
				return 0
			)
			if(getfiles img).count == 0 then
			(
				print ("�Ҳ����ļ�: " + img as string)
			)
			else
			(
				print "---------------------"
				--if classof obj.Material == undefined then
				(
-- 					--ԭ��׼�������ӵ��¶�ά������
-- 					oldMat = obj.material 
-- 					multimat = multimaterial numsubs:2
-- 					multimat.materialList[1] = oldMat
-- 					
-- 					--����һ������
-- 					polyop.setMapSupport obj 3 true
-- 					multimat.materialList[2].selfillumMap = bitmapTexture filename:img
-- 					multimat.materialList[2].selfillumMap.coords.mapChannel = 3 -- �����ڵ���ͨ��
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

					--�� obj һ����׼���Ӳ���
					meditMaterials[1] = Standard()
					meditMaterials[1].diffusemap = bitmapTexture filename:"F:\Max_Test\������Χ�ؿ�MAX�ļ�\maps\Grassland_qiao_32.jpg" 
					--obj.Material = meditMaterials[1]
					modPanel.addModToSelection (Uvwmap ()) ui:on
					obj.modifiers[#UVW_Map].mapChannel = 1
					obj.modifiers[#UVW_Map].maptype = 4
					obj.modifiers[#UVW_Map].height = 200
					obj.modifiers[#UVW_Map].width = 200	
					obj.modifiers[#UVW_Map].length = 200
					macros.run "Modifier Stack" "Convert_to_Poly"
											
					--����һ����Ƭ��ͼ
					meditMaterials[2] = Standard()
					meditMaterials[2].diffusemap = bitmapTexture filename:img
					--meditMaterials[2].diffusemap.filename = img
					meditMaterials[2].diffusemap.alphaSource = 2
					
					polyop.setMapSupport obj 3 true
					meditMaterials[2].diffusemap.coords.mapChannel = 3 -- �����ڵ���ͨ��
					meditMaterials[1].selfillumMap = meditMaterials[2].diffusemap
						
					obj.Material = meditMaterials[1]
					--����UV
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
Rollout rl_gymap "����_����Ƭ"
(

	--Button btn_mapPath "ָ����ͼ·��..." pos:[4,6] width:153
	label lb_path "��ͼ·��:" pos:[4,4]
	EditText edt_mapPath "" pos:[4,18] width:153 --text:"F:\Max_Test\������Χ�ؿ�MAX�ļ�\maps"
	label lb_map "��ͼ:"  pos:[4,36] width:28 align:#left
	label lb_mapName ""  pos:[100,36]
	Button btn_doIt ">>Start<<"
	checkbox chb_isSingle "�������崦��!"
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
			messagebox "��ָ��ͼƬ�ļ�·��"
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
		-- ��ת .position ��Ԫ��,ʹ�䷽��ֱ��ƽ����ͼ
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
		-- ������Ӧ�����������λ��
		settvert obj v normalize_pos
	)
*/
--Ϊ��������ͼ����
-- function FN_autoTerrainMap obj 
-- (
-- 	local oldcoordsys, normalize_pos

-- 	ConvertToMesh obj
-- 	
-- 	------------------------------------------------------------------------------------------
-- 	-- ���������������Ŀ����Mesh(obj)����Ķ��������
-- 	obj.numtverts = obj.numverts
-- 	-- ��������������
-- 	buildTVFaces obj

-- 	-- ����Ŀۼ���Objects����ϵ�½���
-- 	-- �ȴ洢��������ϵͳ,�Ա������ɺ�ָ���
-- 	oldcoordsys = set coordsys local
-- 	
-- 	-- ��Mesh��ÿһ������� .position ���Խ��б�׼������. ��ȡֵ��ΧΪ [0,0,0] �� [1,1,1], ����Ϊƽ��UVW����ϵ
-- 	for v = 1 to obj.numverts do
-- 	( 
-- 		normalize_pos = ((getvert obj v) - obj.min) / (obj.max - obj.min)
-- 		/*
-- 		-- ��ת .position ��Ԫ��,ʹ�䷽��ֱ��ƽ����ͼ
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
-- 		-- ������Ӧ�����������λ��
-- 		settvert obj v normalize_pos
-- 	)

-- 	-- ���λ�����������, ��������������,
-- 	-- Mesh (obj) �� ����������֮�����һһ��Ӧ�Ĺ�ϵ,mesh ��������϶����������������ϵĶ���������ͬ
-- 	for f = 1 to obj.numfaces do
-- 	(
-- 		setTVFace obj f (getface obj f)
-- 		-- ������в�����,�ָ�����ϵͳ
-- 		set coordsys oldcoordsys
-- 		-- ˢ�� Mesh ����
-- 		update obj
-- 	)

-- 	-- �� mesh (obj) ����ʩ��һ�ֲ���,���ǹ涨Ϊ StandardMaterial
-- 	-------------------------------------------------------------------------------------------- 
-- 	local img = FN_getTerrainTileImage obj.center
-- 	
-- 	newmat = StandardMaterial()
-- 	obj.Material = newmat
-- 	obj.DiffuseMap.fileName = img
-- 	showTextureMap obj.material obj.material.diffusemap on


-- )
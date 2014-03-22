

	
-- 目录下包含子目录文件，使用：路径 "*.max"
function Yvi_getFilesRecursive root ext =
(
	local all_Files = #()
	local TheAllSubFolder = #()
	
	--获取root目录下的所有子目录
	TheAllSubFolder = GetDirectories (root+"/*")
	for s in TheAllSubFolder do
	(
		join TheAllSubFolder (GetDirectories (s + "/*"))
	)

	for f in TheAllSubFolder do
	(
		--在每个目录下遍历
		join all_Files (getFiles (f + ext))
	)
	join all_Files (getFiles (root + "/" + ext))
	return all_Files
)

-- //读写信息表
struct s_ReadWriteInforFun
(
	function Yvi_createIntoFile _path _name =
	(
		if(_path[_path.count] != "\\")then 
			_path += "\\";
		fname = _path + _name + ".txt";
		f = CreateFile fname;
		if(f== undefined)then 
		(
			format "% create failed.\n" fname;
			return undefined;
		)
		return fname;
	),
	
	function Yvi_writeInforFile fname =
	(
		fstream = openFile fname mode:"at";
		if( fstream == undefined )then 
		(
			format "% open file failed.\n" fname;
			return undefined;
		)
		str = (_oldObjName + " " +_theNumber + "\n");
		format str to:fstream;
		close fstream;
	),
	
	function Yvi_readInforFile fname =
	(

	)
)

-- //检查命名
struct s_CheckNameFun
(
	-- 序号保留位数 默认4位
	function Yvi_formatNumStr num d:4 =
	(
		str = num as string;
		if( str.count < d) then
		(
			for i=1 to (d - str.count) do
			(
				str = "0" + str;
			)
		)
		str;
	),
	
	function Yvi_checkMaxFileName max_file_name =
	(
		if(max_file_name.count != 14)then 
		(
			return 0;
		)
		
		strTmp = filterString max_file_name "_";
		if(strTmp.count != 3)then 
		(
			return 0;
		)
		
		if(strTmp[1] != "hongkong" or 
			(strTmp[2] as integer) == undefined or 
			(strTmp[3] as integer) == undefined )then 
		(
			return 0;
		)
		
		return 1;
	),
	
	--检查模型名区块号
	function Yvi_checkObjNameBlock max_file_name obj_name =
	(
		if(obj_name.count != 6)then 
		(
			return 0;
		)
		if(obj_name as integer == undefined)then 
		(
			return 0;
		)
		blockStr = substring obj_name 1 3;
		numStr = substring obj_name 3 6;
		if(blockStr as integer == undefined)then 
		(
			return 0;
		)
		if(numStr as integer == undefined)then 
		(
			return 0;
		)
		
		i = s_CheckNameFun.Yvi_checkMaxFileName max_file_name;
		if(i != 0)then 
		(
			-- 前缀字符串 区块号 实体号
			blockNumStr = filterString max_file_name "_";
			num = blockNumStr[3] as integer;
			blockNum = s_CheckNameFun.Yvi_formatNumStr num 3;
			
			-- 物体名区块号 是否 与 max文件的区块号对应
			if(blockNum != blockStr)then 
			(
				-- 物体名称错误区块号写入log
				return 0;
			)
		)
	),
	
	-- 检查模型名序号（实体号）是否连贯
	function Yvi_checkObjNameNum =
	(
		local objNameArr = #();
		for obj in Objects where( (classOf obj == Editable_Poly) or (classOf obj == Editable_Mesh) or (classOf obj == PolyMeshObject) ) do
		(
			append objNameArr obj.name;
		)
		
		sort objNameArr;
		for i = 1 to objNameArr.count do
		(
			numStr = substring objNameArr[i] 3 6;
			num = numStr as integer;
			if( num != i)then 
			(
				-- 编号（实体号）有误。
				exit;
			)
		)
	),
	
	--
	function Yvi_collectAllSubMats mat matsArr = 
	(
		if mat == undefined then return undefined;
	     if ( classof mat  == Multimaterial) then 
		(
			for i=1 to mat.numsubs do
			(
				Yvi_collectAllSubMats mat[i] matsArr;
			)
		 )
		 else if(classof mat == Standard) then 
		 (
			 append  matsArr mat;
		 )
		 else 
		 (
		    
		 )
		 
		 return matsArr;
	),
	
	function Yvi_checkBitmapName bitmap_name =
	(
		local allSubmats = #()
		for obj in Objects where( (classOf obj == Editable_Poly) or (classOf obj == Editable_Mesh) or (classOf obj == PolyMeshObject) ) do
		(
		
		) 
		
		for m in sceneMaterials do
		(
			s_CheckNameFun.Yvi_collectAllSubMats m allSubmats;
		)
		
		for mat in allSubmats do
		(
			_name = mat.name;
			
		)
	)
)

-- //构建文件夹
struct s_MakeFolderFun
(
	--根目录下建立 max文件名的文件夹。用来保存导出的模型文件
	--be called
	function Yvi_makeCurMaxNameFolder _root _maxFileName =
	(
		if(_root[_root.count] != "\\")then
			_root += "\\";
		if(_maxFileName == "")then
		(
			return 0;
		)
		max_folder = root + _maxFileName + "\\";
		makeDir max_folder;
		
		return max_folder;
	),
	
	-- 构建模型名文件夹
	function Yvi_makeCurObjNameFolder max_path obj_name =
	(
		if(max_path[max_path.count] != "\\")then
			max_path += "\\";

		obj_name_folder = max_path + obj_name + "\\";
		makeDir obj_name_folder;
		
		return obj_name_folder;
	),

	-- 构建 High 文件夹
	function Yvi_makeCurObjHighFolder obj_path strHigh =
	(
		if(obj_path[obj_path.count] != "\\")then
			obj_path += "\\";

		high_folder = obj_path + strHigh + "\\";
		makeDir high_folder;
		
		return high_folder;	
	),
	
	-- 构建 Low 文件夹
	function Yvi_makeCurObjLowFolder obj_path strLow =
	(
		if(obj_path[obj_path.count] != "\\")then
			obj_path += "\\";

		low_folder = obj_path + strLow+ "\\";
		makeDir low_folder;
		
		return low_folder;	
	),
	
	-- 构建模型贴图文件夹
	function Yvi_makeCurObjTextureFolder highOrLow_path =
	(
		if(highOrLow_path[highOrLow_path.count] != "\\")then
			highOrLow_path += "\\";

		texture_folder = highOrLow_path + "texture\\";
		makeDir texture_folder;
		
		return texture_folder;
	),

	-- 构建公共模型名文件夹
	function Yvi_makePublicModelFolder root =
	(
		if(root[root.count] != "\\")then
			root += "\\";

		public_model_folder = root + "public_model\\";
		makeDir public_model_folder;
		
		return public_model_folder;
	),
	
	-- 构建公共模型贴图文件夹
	function Yvi_makePublicModelTextureFolder root =
	(
		if(root[root.count] != "\\")then
			root += "\\";

		public_model_texture_folder = root + "public_texture\\";
		makeDir public_model_texture_folder;
		
		return public_model_texture_folder;
	)
) -- make folder function struct 

-- //导出文件
struct s_DoExportFilesFun
(
	-- 导出公共模型
	function Yvi_exportPublicModel public_path model_class fileName = --model_class:公共模型类型，例如river, bridge....
	(
		if( public_path[public_path.count] != "\\" )then
		(
			public_path += "\\";
		)
		model_class_path = (pub_path + model_class);
		makeDir model_class_path;
		
		exportfile (model_class_path+ "\\" + fileName + ".3ds") #noprompt;
		exportfile (model_class_path + "\\" + fileName + ".obj") #noprompt;
		
		saveMaxFile (model_class_path + "\\" + fileName + ".max") clearNeedSaveFlag:True useNewFile:false quiet:true;
	),

	--导出 High or Low 类型文件
	function Yvi_exportHighOrLowQualityModel highOrLow_model_path obj _texture_save_path chHL:""  =
	(
		obj_name = obj.name;
		if highOrLow_model_path == undefined or highOrLow_model_path == "" then
		(
			return 0;
		)
		if(highOrLow_model_path[highOrLow_model_path.count] != "\\")then
			highOrLow_model_path += "\\";
		
		_fileName = chHL + obj_name;
		
		exportfile (highOrLow_model_path + "\\" + _fileName + ".3ds") #noprompt using:D_StudioExporterPlugin selectedOnly:true;
		exportfile (highOrLow_model_path + "\\" + _fileName + ".obj") #noprompt selectedOnly:true;
		
		saveNodes obj (highOrLow_model_path + "\\" + _fileName + ".max") quiet:true;
	)
	
)-- do export files function struct

-- 导出贴图文件
struct s_HandleObjUsedBitmapFun
(
	--导出每个模型所使用的贴图（对于如果是公共贴图，则保存到公共文件夹下，但这个时候出现重复问题：？ 公共贴图不处理。公共贴图只有 0000到9999四个数。）
	function Yvi_getObjUsedBitmap obj texture_path chHL:""=
	(
		if(texture_path[texture_path.count] != "\\")then
			texture_path += "\\";
		
		mat = obj.Material;
		if(mat != undefined)then
		(
			if (classof mat == MultiMaterial)then
			(
				for m in mat do
				(
					if(m.DiffuseMap != undefined)then
					(
						--如果这里保存的是相对路径呢？
						
						_oldFullBitmap = m.DiffuseMap.filename;
						_oldBitmapName = filenameFromPath _oldFullBitmap;
						_stripName = getFilenameFile _fullOldBitmap;
						
						--在这里排除公共贴图，公共贴图只有 0000 到 9999 四位数。
						if( _stripName.count == 4 and (_stripName as integer) != undefined )then
						(
							continue;
						)
						
						-- 如果是相对路径
						-- pathConfig.isPathRootedAtBackslash path 以反斜杠开始 且 不是驱动号 A；B： C：... Z:
						if( pathConfig.isPathRootedAtBackslash _oldFullBitmap and 
							not (pathConfig.isPathRootedAtDriveLetter _oldFullBitmap) )then 
						(
							-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
							_oldFullBitmap = pathConfig.appendPath maxfilepath  _oldFullBitmap;
						)

						_newFullBitmap = texture_path + chHL + _oldBitmapName;
						copyFile _oldFullBitmap _newBitmap;
					)
					if(m.OpacityMap != undefined)then
					(
						_oldOpaFullBitmap = m.OpacityMap.filename
						_oldOpaBitmapName = filenameFromPath _oldOpaFullBitmap;
						_stripName = getFilenameFile _oldOpaFullBitmap;

						if( _stripName.count == 4 and (_stripName as integer) != undefined )then
						(
							continue;
						)
						
						if( pathConfig.isPathRootedAtBackslash _oldOpaFullBitmap and 
							not (pathConfig.isPathRootedAtDriveLetter _oldOpaFullBitmap) )then 
						(
							-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
							_oldOpaFullBitmap = pathConfig.appendPath maxfilepath _oldOpaFullBitmap;
						)
						_newOpaFullBitmap = texture_path + chHL + _oldOpaBitmapName;
						copyFile _oldOpaFullBitmap _newOpaFullBitmap;
					)
					if(m.SelfIllumMap != undefined)then
					(
						_oldSelFullBitmap = m.SelfIllumMap.filename
						_oldSelBitmapName = filenameFromPath _oldSelFullBitmap;
						_stripName = getFilenameFile _oldSelFullBitmap;

						if( _stripName.count == 4 and (_stripName as integer) != undefined )then
						(
							continue;
						)
						
						if( pathConfig.isPathRootedAtBackslash _oldSelFullBitmap and 
							not (pathConfig.isPathRootedAtDriveLetter _oldSelFullBitmap) )then 
						(
							-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
							_oldSelFullBitmap = pathConfig.appendPath maxfilepath _oldSelFullBitmap;
						)
						_newSelFullBitmap = texture_path + chHL + _oldSelBitmapName;
						copyFile _oldSelFullBitmap _newSelFullBitmap;
					)
				) -- end for
			) -- end MultiMaterial
			if(classof mat == Standard)then
			(
				if(mat.DiffuseMap != undefined)then
				(
					--如果这里保存的是相对路径呢？
					
					_oldFullBitmap = mat.DiffuseMap.filename;
					_oldBitmapName = filenameFromPath _oldFullBitmap;
					_stripName = getFilenameFile _fullOldBitmap;
					
					--在这里排除公共贴图，公共贴图只有 0000 到 9999 四位数。
					if( _stripName.count == 4 and (_stripName as integer) != undefined )then
					(
						continue;
					)
					
					-- 如果是相对路径
					-- pathConfig.isPathRootedAtBackslash path 以反斜杠开始 且 不是驱动号 A；B： C：... Z:
					if( pathConfig.isPathRootedAtBackslash _oldFullBitmap and 
						not (pathConfig.isPathRootedAtDriveLetter _oldFullBitmap) )then 
					(
						-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
						_oldFullBitmap = pathConfig.appendPath maxfilepath  _oldFullBitmap;
					)

					_newFullBitmap = texture_path + chHL + _oldBitmapName;
					copyFile _oldFullBitmap _newBitmap;
				)
				if(mat.OpacityMap != undefined)then
				(
					_oldOpaFullBitmap = mat.OpacityMap.filename
					_oldOpaBitmapName = filenameFromPath _oldOpaFullBitmap;
					_stripName = getFilenameFile _oldOpaFullBitmap;

					if( _stripName.count == 4 and (_stripName as integer) != undefined )then
					(
						continue;
					)
					
					if( pathConfig.isPathRootedAtBackslash _oldOpaFullBitmap and 
						not (pathConfig.isPathRootedAtDriveLetter _oldOpaFullBitmap) )then 
					(
						-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
						_oldOpaFullBitmap = pathConfig.appendPath maxfilepath _oldOpaFullBitmap;
					)
					_newOpaFullBitmap = texture_path + chHL + _oldOpaBitmapName;
					copyFile _oldOpaFullBitmap _newOpaFullBitmap;
				)
				if(mat.SelfIllumMap != undefined)then
				(
					_oldSelFullBitmap = mat.SelfIllumMap.filename
					_oldSelBitmapName = filenameFromPath _oldSelFullBitmap;
					_stripName = getFilenameFile _oldSelFullBitmap;

					if( _stripName.count == 4 and (_stripName as integer) != undefined )then
					(
						continue;
					)
					
					if( pathConfig.isPathRootedAtBackslash _oldSelFullBitmap and 
						not (pathConfig.isPathRootedAtDriveLetter _oldSelFullBitmap) )then 
					(
						-- 加上当前max文件路径，如果是相对路径，相对路径中已经保存有 //texture//路径
						_oldSelFullBitmap = pathConfig.appendPath maxfilepath _oldSelFullBitmap;
					)
					_newSelFullBitmap = texture_path + chHL + _oldSelBitmapName;
					copyFile _oldSelFullBitmap _newSelFullBitmap;
				)
			) -- end Standard
		)
		else
		(
			format "模型 % 没有贴图。\n" obj.name;
		)
	)
) -- handle obj used bitmap function struct


---/***************************************************************************************/
-- 这个贴图导出函数需要重写一个，用于 两种存储模式之间的贴图命名转换
-- 拷贝之前，需要对非公共贴图 进行 重命名
-- 
-----------------------------------------------------
function Yvi_getObjUsedBitmap2 obj save_path max_path _pubModelPath key i_flag i_set=
(
-- 	if(save_path[save_path.count] != "\\")then
-- 		save_path += "\\";
	if(key[key.count] == "\\")then 
	(
		key = trimRight key "\\";
	)
	mat = obj.Material;
	if(mat != undefined)then
	(
		if (classof mat == MultiMaterial)then
		(
			local i = 0;
			for m in mat do
			(
				if(m.DiffuseMap != undefined)then
				(
					_oldBitmap = m.DiffuseMap.filename; --m.DiffuseMap.filename 返回的相对路径前面已经带 "/"
					_oldBitmapName = filenameFromPath _oldBitmap;
					_tripName = getFilenameFile _oldBitmap;
					_ext = getFilenameType _oldBitmap;
					
					case i_set of
					(
						-- 相对路径
						3:(
							-- 公共贴图也需要导出去
							if( _tripName.count == 4 and (_tripName as integer) != undefined )then
							(
								m.DiffuseMap.filename = "\\public_texture\\" + _oldBitmapName;
								continue;
							)
							--导出贴图后，把这个贴图设置成相对路径 ：/texture/Hxxxxx.jpg
							-- 重命名贴图
							i += 1;
							if (flag == 1) then
							(
								_newBitmapName = key + "_"+ (Yvi_formatNumStr i) + _ext; -- key == "high" or "low"
							
								m.DiffuseMap.filename = "\\texture\\" + _newBitmapName;
								_newBitmap = save_path + _newBitmapName;

								copyFile _oldBitmap _newBitmap;
							)
							if(flag == 2)then 
							(
								_newBitmapName = key  + obj.name + (Yvi_formatNumStr i d:2) + _ext;-- key == "H" or "L"
								m.DiffuseMap.filename = "\\texture\\" + _newBitmapName;
								_newBitmap = save_path + _newBitmapName;
								
								
								copyFile (max_path+_oldBitmap) _newBitmap;
							)
						)
						--绝对路径
						4:(
							if( _tripName.count == 4 and (_tripName as integer) != undefined )then
							(
								m.DiffuseMap.filename = _pubModelPath + "\\public_texture\\" + _oldBitmapName;
								--continue;
							)
							else
								m.DiffuseMap.filename = max_path+ _oldBitmap;
						)
					)-- case 
				)
				if(m.OpacityMap != undefined)then
				(
					_oldOpaBitmapName = filenameFromPath (max_path + m.opacityMap.filename);
					_tripName = getFilenameFile _oldOpaBitmapName;
					
					if( _tripName.count == 4 and (_tripName as integer) != undefined )then
					(
						m.OpacityMap.filename = m.DiffuseMap.filename;
						continue;
					)
					else
					(
						m.opacityMap.filename = m.DiffuseMap.filename;
					)
					
				)
				if(m.SelfIllumMap != undefined)then
				(
					_oldSelBitmapName = filenameFromPath (max_path + m.SelfIllumMap.filename);
					
					_tripName = getFilenameFile _oldSelBitmapName;
					if( _tripName.count == 4 and (_tripName as integer) != undefined )then
					(
						m.SelfIllumMap.filename =  m.DiffuseMap.filename;
						continue;
					)
					else
					(
						m.SelfIllumMap.filename = m.DiffuseMap.filename;
					)
				)
			)
		)--  MultiMaterial
		if(classof mat == Standard)then
		(
			if(mat.DiffuseMap != undefined)then
			(
				_oldBitmap = mat.DiffuseMap.filename;
				_oldBitmapName = filenameFromPath _oldBitmap;
				_tripName = getFilenameFile _oldBitmap
				_ext = getFilenameType _oldBitmap;
				
				case i_set of
				(
					-- 相对路径
					3:(
						if( _tripName.count == 4 and (_tripName as integer) != undefined )then
						(
							mat.DiffuseMap.filename = "\\public_texture\\" + _oldBitmapName
							continue;
						)
						
						-- 重命名贴图
						local i = 1;
						if(flag == 1)then 
						(
							_newBitmapName = key + "_"+ (Yvi_formatNumStr i)+_ext;
							mat.DiffuseMap.filename = "\\texture\\" + _newBitmapName
							
							_newBitmap = save_path + _newBitmapName;
							copyFile _oldBitmap _newBitmap;
						)
						if(flag == 2)then 
						(
							_newBitmapName = key + obj.name + (Yvi_formatNumStr i 2)+_ext;
							mat.DiffuseMap.filename = "\\texture\\" + _newBitmapName
							
							_newBitmap = save_path + _newBitmapName;
							copyFile _oldBitmap _newBitmap;
						)
					)
					--绝对路径
					4:(
						if( _tripName.count == 4 and (_tripName as integer) != undefined )then
						(
							mat.DiffuseMap.filename = _pubModelPath + "\\public_texture\\" + _oldBitmapName;
							--continue;
						)
						else
							mat.DiffuseMap.filename = max_path+ _oldBitmap;
					)
				)
			)
			if(mat.OpacityMap != undefined)then
			(
				_oldOpaBitmapName = filenameFromPath (max_path+mat.opacityMap.filename);
				
				_tripName = getFilenameFile _oldOpaBitmapName;
				if( _tripName.count == 4 and (_tripName as integer) != undefined )then
				(
					mat.OpacityMap.filename = "\\public_texture\\" + _oldOpaBitmapName;
					continue;
				)
				else
				(
					mat.OpacityMap.filename = mat.DiffuseMap.filename;
				)
			)
			if(mat.SelfIllumMap != undefined)then
			(
				_oldSelBitmapName = filenameFromPath (max_path+mat.SelfIllumMap.filename);
				
				_tripName = getFilenameFile _oldSelBitmapName;
				if( _tripName.count == 4 and (_tripName as integer) != undefined )then
				(
					mat.SelfIllumMap.filename = "\\public_texture\\" + _oldSelBitmapName;
					continue;
				)
				else
				(
					mat.SelfIllumMap.filename = mat.DiffuseMap.filename;
				)
			)
		)
	)
	else
	(
		format "模型 % 没有贴图。\n" obj.name;
	)
)

function Yvi_makeNewMaxFilePath _maxPath =
(
	if(_maxPath[_maxPath.count] != "\\")then
		_maxPath += "\\";
	makeDir _maxPath;
	
	return _maxPath;
)

-- 客户存储导出
function Yvi_exportConvertToClientArchive save_path obj _pathLeaf =
(
	obj_name = obj.name; --名字已经改好

	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	save_path = save_path + _pathLeaf;
	makeDir save_path;
	exportfile (save_path  + obj_name + ".3ds") #noprompt using:D_StudioExporterPlugin selectedOnly:true;
	exportfile (save_path  + obj_name + ".obj") #noprompt selectedOnly:true;
	
	--saveNodes obj (save_path  + obj_name + ".max") quiet:true;
)

--/**************************************************************************************************************************/
-----------------------------------------------------------------------------------------------------------------------------
try(DestroyDialog rl_ExportArchive)catch()
rollout rl_ExportArchive "Export and Archive v0.1" --width:286 height:650
(	
	--Button btn_EN "EN" across:2  width:16 height:12 pos:[244, 2] align:#right
	--Button btn_CH "CH"  width:16 height:12 pos:[263, 2] align:#right
	
	group "Check Name"
	(
		EditText edt_ModelName "Model Name" across:2
		EditText edt_BolckName "Bolck No."
		
		CheckBox ckb_CheckMaxFileName "Max FileName" across:3 align:#left
		CheckBox ckb_CheckObjName "Models Name"
		CheckBox ckb_CheckBitmapName "Bitmaps Name"
		
		CheckBox chb_CheckArchFileName "Archive Files Name" align:#left
		EditText edt_ArchDir "" labelOnTop:true width:165 height:18 across:2-- pos:[122, 65]
		Button btn_SetCheckNameArchDir "Archive Root..." enabled:false align:#right  tooltip:"Specified Archive root directory for check the files name."
		
		Button btn_DoCheck "Check Name" width:260 tooltip:"Do check named now..." align:#left
	)
	group "Export Building Model"
	(
		editText edt_ArchRoot "" width:175 height:16 across:2 align:#left
			button btn_ArchRoot "Archive Root..." width:80 height:18 align:#right tooltip:"Specified export BuildingModel files archive root directory."
		
		radiobuttons rdb_ModelQuality "" labels:#("High Model","Low Model") default:1 columns:2 align:#left across:2
			button btn_ExportBuilding "Export Building" width:80 height:18 align:#right enabled:false tooltip:"Export currently selected model(s) to 3ds/obj/max files."
	)
	
	Group "Export Public Model"
	(
		editText edt_PublicArchRoot "" width:175 height:16 across:2 align:#left
			button btn_PublicArchRoot "Archive Root..." width:80 height:18 align:#right tooltip:"Specified export PublicModel files archive root directory."
		
		editText edt_PublicPath "" width:175 height:16 across:2 
			button btn_OpenPublicPath "Public Path..."  width:80 height:18 align:#right tooltip:"Specified has Public Model files(*.max) directory."
		MultilistBox mlbx_PublicFiles "Public Model Files:" width:255 height:8
			radiobuttons rdb_listOperation "" labels:#("All","None","Empty", "Remove") default:0 columns:5  align:#left
	
		button btn_ExportPublicModel "Export Public Model"  width:110 height:18 enabled:false tooltip:"Now export currently selected public model to 3ds/obj/max files."
	)

	group "Model File Archive"
	(
		editText edt_InputPath "" width:200 height:16 across:2 
			button btn_OpenInput "Input..."width:55 height:18 align:#right tooltip:"Specified has already exists 'Model/Client Archive' directory"
		
		editText edt_OutputPath "" width:200 height:16 across:2 
			button btn_OpenOutput "Output..."width:55 height:18 align:#right tooltip:"Specified need to save 'Client/Model Archive' directory"
		
		button btn_ModelArchToClientArch "ModelArchive To ClientArchive" width:221 height:20 tooltip:"Now transform Model to Client archive."
		button btn_ClientArchToModelArch "ClientArchive To ModelArchive" width:221 height:20 tooltip:"Now transform Client to Model archive."
	)
	
	label lb_StateBar "Ready" width:170 height:16 style_sunkenedge:true pos:[4, 584]
	Progressbar pg_Progress "" width:106 height:16 color:Green pos:[176, 584]
	------------------------
	global ex_pubFilesArr = #()
	------------------------

--/******************************** 命名检查 ******************************************************/
-- //检查类型选择
	on chb_CheckArchFileName changed arg do 
	(
		if(arg == on)then 
		(
			lb_StateBar.text = "Specified files directory.";
			btn_SetCheckNameArchDir.enabled = true;
-- 			ckb_CheckMaxFileName.checked = off;
-- 			ckb_CheckObjName.checked = off;
-- 			ckb_CheckBitmapName.checked = off;
			
			ckb_CheckMaxFileName.enabled = false;
			ckb_CheckObjName.enabled = false;
			ckb_CheckBitmapName.enabled = false;
		)
		else
		( 
			btn_SetCheckNameArchDir.enabled = false;
			
-- 			ckb_CheckMaxFileName.checked = on;
-- 			ckb_CheckObjName.checked = on;
-- 			ckb_CheckBitmapName.checked = on;
			
			ckb_CheckMaxFileName.enabled = true;
			ckb_CheckObjName.enabled = true;
			ckb_CheckBitmapName.enabled = true;
		)
	)
	on ckb_CheckMaxFileName changed arg do 
	(
		lb_StateBar.text = "Check current max file name.";
		btn_SetCheckNameArchDir.enabled = false;
		chb_CheckArchFileName.checked = off;
	)
	on ckb_CheckObjName changed arg do 
	(
		lb_StateBar.text = "Check scene model name.";
		btn_SetCheckNameArchDir.enabled = false;
		chb_CheckArchFileName.checked = off;
	)
	on ckb_CheckBitmapName changed arg do 
	(
		lb_StateBar.text = "Check model used bitmap name.";
		btn_SetCheckNameArchDir.enabled = false;
		chb_CheckArchFileName.checked = off;
	)
--/******************************** 归档根路径设置 ******************************************************/
-- //  归档根路径 
	on btn_ArchRoot pressed do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("ArchiveRoot");
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export;
			)
			if (dir = getSavePath caption:"选择存储归档的根目录..." initialDir:histPath)!= undefined then
			(
				edt_ArchRoot.text = dir;
				btn_ExportBuilding.enabled = true;
				setINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("ArchiveRoot") dir;
			)
		)
		catch()
	)
	
-- // 输入路径时
	on edt_ArchRoot entered text do
	(
		dir = edt_ArchRoot.text;
		
		try
		(
			if (dir != "")then
			(
				if ((substring dir 2 2) != ":\\") do
				(
					btn_ExportBuilding.enabled = false;
					messagebox "Path is incorrect！";
					return undefined;
				)
				edt_ArchRoot.text = dir;
				btn_ExportPublicModel= true;
				btn_ExportBuilding.enabled = true;
			)
			else
			(
				btn_ExportBuilding.enabled = false;
				btn_ExportPublicModel= false;
			)

		)catch()
	)

/******************************** 导出建筑 ******************************************************/
-- // 选择不同类型导出时状态栏提示
	on rdb_ModelQuality changed state do
	(
		case state of
		(
			1:
			(
				lb_StateBar.text = "Export high model now."
			)
			2:
			(
				lb_StateBar.text = "Export low model now."
			)
		)
	)

-- // 导出建筑模型 
	on btn_ExportBuilding pressed do
	(
		local _selObj = selection as array;
		local _archiveRoot = edt_ArchRoot.text;
		
		if(maxfilename == "")then
		(
			messagebox "Please save the Max file！" title:rl_ExportArchive.title;
			return 0;
		)
		if(_archiveRoot == "")then
		(
			messagebox "Please specified archive root directory." title:rl_ExportArchive.title;
			return 0;
		)
		if(_selObj.count == 0)then
		(
			messagebox "Please select buiding(s) model！" title:rl_ExportArchive.title;
			return 0;
		)
		

------------
		lb_StateBar.text = "Exporting,please wait...";
-------------
		-- 实例化函数对象
		MAKEFOLDERFOO = s_MakeFolderFun();
		EXPORTFILESFOO = s_DoExportFilesFun();
		HANDELBITMAPFOO = s_HandleObjUsedBitmapFun();
		
		local _maxNameForlder = MAKEFOLDERFOO.Yvi_makeCurMaxNameFolder _archiveRoot;
		
		local i = 0;
		for obj in _selObj where ( (classOf obj == Editable_Poly) or (classOf obj == Editable_Mesh) or (classOf obj == PolyMeshObject) ) do
		(
			local obj_name = obj.name;
			local _objNameForlder = MAKEFOLDERFOO.Yvi_makeCurObjNameFolder _maxNameForlder obj_name;
			
			if(rdb_ModelQuality.state == 1)then -- High model
			(
				local strHigh = "high";
				local _highFolder = MAKEFOLDERFOO.Yvi_makeCurObjHighFolder _objNameForlder strHigh;
				local _textureForlder = MAKEFOLDERFOO.Yvi_makeCurObjTextureFolder _highFolder;
				
				HANDELBITMAPFOO.Yvi_getObjUsedBitmap obj _textureForlder "H";
				EXPORTFILESFOO.Yvi_exportHighOrLowQualityModel _highFolder obj _textureForlder "H";
			)
			else if(rdb_ModelQuality.state == 2)then
			(
				local strLow= "low";
				local _lowFolder = MAKEFOLDERFOO.Yvi_makeCurObjLowFolder _objNameForlder strLow;
				local _textureForlder = MAKEFOLDERFOO.Yvi_makeCurObjTextureFolder _lowFolder;
				
				HANDELBITMAPFOO.Yvi_getObjUsedBitmap obj _textureForlder "L";
				EXPORTFILESFOO.Yvi_exportHighOrLowQualityModel _lowFolder obj _textureForlder "L";
			)
			
			makeDir (_objNameForlder + "\\original");
			
			pg_Progress.value = 100.(i+=1)/_selObj.count;
			gc();
		)
		lb_StateBar.text = "Export completed.";
		messagebox "Export Successful！" title:rl_ExportArchive.title;
		lb_StateBar.text = "Ready";
	)

/****************************公共模型文件导出***************************************************/
-- // 公共模型文件路径
	on btn_OpenPublicPath pressed do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("PublicPath");
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export;
			)
			if (dir = getSavePath caption:"选择公共模型文件路径..." initialDir:histPath)!= undefined then
			(
				edt_PublicPath.text = dir;
				
				ex_pubFilesArr = #();
				ex_pubFilesArr = Yvi_getFilesRecursive dir "*.max";
				mlbx_PublicFiles.items = for f in ex_pubFilesArr collect (filenameFromPath f);
				
				if( not pathConfig.isLegalPath edt_ArchRoot.text or edt_ArchRoot.text == "" )then
				(
					lb_StateBar.text = "Wrong root directory,try again.";
					btn_ExportPublicModel.enabled = false;
				)
				else
				(
					btn_ExportPublicModel.enabled = true;
				)
				setINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("PublicPath") dir;
			)
		)
		catch()
	)
	
-- // 操作列表 
	on rdb_listOperation changed arg do--全选
	(
		case arg of
		(
			1:
			(
				all = #{1..(mlbx_PublicFiles.items.count)};
				mlbx_PublicFiles.selection = all;
				lb_StateBar.text = "Selected " + mlbx_PublicFiles.items.count as string + " items.";
			)
			2:
			(
				mlbx_PublicFiles.selection = #{};
				lb_StateBar.text = "no selected any item.";
			)
			3:
			(
				mlbx_PublicFiles.Items = #();
				ex_pubFilesArr = #();
				lb_StateBar.text = "Empty, now respecified directory.";
			)
			4:
			(
				local curSelIndexs = mlbx_PublicFiles.selection as array;
				local delSelItem = #();
				if curSelIndexs.count == 0 then
				(
					lb_StateBar.text = "No any item can be removed.";
					return undefined;
				)
				
				for i=1 to curSelIndexs.count do 
					append delSelItem ex_pubFilesArr[curSelIndexs[i]];
				for d in delSelItem do
				(
					index = finditem ex_pubFilesArr d;
					if( index != 0 )then 
					(
						Deleteitem ex_pubFilesArr index;
					)
				)
				mlbx_PublicFiles.items = for f in ex_pubFilesArr collect (filenameFromPath f);
				lb_StateBar.text = "removed completed.";
			)
		)
		
	)
	
-- // 双击列表项打开文件所在目录
	on mlbx_PublicFiles doubleClicked item do
	(
		local item_path = "";
		local item_name = "";
		local f_name = "";
		
		if ex_pubFilesArr.count == 0 do
			return 0;
			
		item_name = mlbx_PublicFiles.items[item];--得到双击项名
		num = mlbx_PublicFiles.selection as array;

		f_name = filenameFromPath ex_pubFilesArr[num[1]]; --得到文件
		if item_name == f_name then
		(
			item_path = getFilenamePath ex_pubFilesArr[num[1]];
			ShellLaunch item_path "";
		)
	)

-- // 选择列表项时状态栏指示
	on mlbx_PublicFiles selected arg do
	(
		local selCounts = 0;
		if(arg != 0)then
		(
			rdb_listOperation.state = 0;
			selCounts = (mlbx_PublicFiles.selection as array).count
			if(selCounts == 1)then 
			(
				lb_StateBar.text = "Doubleclick to open file location."
			)
			else
			(
				
				lb_StateBar.text = "Selected " + selCounts  as string +" files."
			)
		)
	)
	
-- // 公共模型导出
	on btn_ExportPublicModel pressed do
	(
		local toExport = #();
		local num = 0;
		local sel = (mlbx_PublicFiles.selection as array);
		try
		(
			if(sel.count == 0 )then
			(
				MessageBox "There is no export max file,\n please select max file and try again！" title:"warning" beep:true;
				return undefined;
			)
			else
			(
				for i = 1 to sel.count do 
					append toExport ex_pubFilesArr[sel[i]];
				
				local _pubFolder = Yvi_makePublicModelFolder edt_ArchRoot.text;
				
				format "Export Information:\n A total of % max files\n" toExport.count;
				for m in toExport do
				(
					loadmaxfile m;
					-- 从文件中取得该路径的最后文件夹名。
					--pathConfig.stripPathToLeaf  == filenamefrompath
					-- 假设这个 m 文件的路径是： 	X:/.../public_model/river/xxx.max 或 .../bridg/xxx.max 或...../road/xxx.max 或 .../green/xxx.max
					-- 取得文件名关的文件夹 river/ 或 bridg/ 或 green/ 或 road/
					local _stripPathLeaf = ( filenamefrompath (getFilenamePath m) );-- return "river/"  "bridg/" ...
					local _bitmapSavePath = Yvi_makeCurObjTextureFolder3 _pubFolder highOrLow:_stripPathLeaf;
					
					max select all;
					local _selObj = selection as array;
					for obj in _selObj do 
					(
						Yvi_getObjUsedBitmap obj _bitmapSavePath key:"";
					)

					Yvi_exportPublicModel _pubFolder _stripPathLeaf _fileName; --river bridg road green  
					
					
					format "%th：% completed.\n" (num+=1) m; 
					gc();
				)
				resetMaxFile #noPrompt--重置max软件
			)
			toExport = #()
			ex_filesArr = #()
			gc()
			format "OK"
			MessageBox "Export Successful!\t" title:"completed"
		)
		catch(print "Unknow System Error!")
	)
	
/**************************** 存储类型转换 路径设置 *******************************************************/
-- // 归档转换路径输入
	on btn_openInput pressed do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("Input");
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export;
			)
			if (dir = getSavePath caption:"选择当前需要转换的归档路径..." initialDir:histPath)!= undefined then
			(
				edt_InputPath.text = dir;
				
				setINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("Input") dir;
			)
		)
		catch()
	)

-- // 归档转换路径输出
	on btn_OpenOutput pressed do
	(
		try
		(
			histPath = GetINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("Output");
			if histPath == "" or histPath == undefined do
			(
				histPath = GetDir #export;
			)
			if (dir = getSavePath caption:"选择转换后的归档保存路径..." initialDir:histPath)!= undefined then
			(
				edt_OutputPath.text = dir;
				
				setINISetting ((GetDir #scripts)+"\\ExportArchive.ini") ("Directories") ("Output") dir;
			)
		)
		catch()
	)
	
/**************************** 存储类型转换 *******************************************************/
	function Yvi_createIntoFile _path _name =
	(
		if(_path[_path.count] != "\\")then 
			_path += "\\";
		fname = _path + _name + ".txt";
		f = CreateFile fname;
		if(f== undefined)then 
		(
			format "% 文件创建失败。\n" fname;
			return undefined;
		)
		return fname;
	)
	function Yvi_writeInforFile fname _oldObjName _theNumber=
	(
		fstream = openFile fname mode:"at";
		if( fstream == undefined )then 
		(
			format "% 文件无法打开。\n" fname;
			return undefined;
		)
		str = (_oldObjName + " " +_theNumber + "\n");
		format str to:fstream;
		close fstream;
	)
	
	function Yvi_readInforFile fname objName=
	(
		local str = undefined;
		struct s_nameAndNum(objName, theNumber);
		
		fstream = openFile fname mode:"rt";
		if( fstream == undefined)then 
		(
			format "% 文件无法打开。\n" fname;
			return undefined;
		)
		while(not eof fstream)do 
		(
			iLine = readLine fstream;
			tmpName = (filterString iLine " ");
			
			if(objName == tmpName[2])then 
			(
				print tmpName[1];
				str = tmpName[1];
				exit;
			)
		--	name_Num = s_nameAndNum objName:iLine[1] theNumber:iLine[2];
		--	append _lineArr name_Num;
		)
		close fstream;
		
		return str;
	)
	
--- // 模型归档  转 客户归档
	on btn_ModelArchToClientArch pressed do 
	(
		local _bModel2Clent = 0;
		local all_maxFiles = #();
		local _inputPath = edt_InputPath.text;
		local _outputPath = edt_OutputPath.text;
		local _pubModelPath = _inputPath;
		--------------
		--------------
		if( _inputPath == "" or _outputPath == "")then
		(
			return undefined;
		)
		if(_outputPath[_outputPath.count] != "\\")then
		(
			_outputPath += "\\";
		)
		all_maxFiles = Yvi_getFilesRecursive _inputPath "*.max";
		
		-- 获得 模型归档的 max 文件，贴图已经是相对路径
		-- 获取当前 max 文件的路径，则当前max文件的贴图路径为 cur_max_path + Material.DiffuseMap.filename "\public_texture\0040.jpg"
		-- 通过 Yvi_getObjUsedBitmap obj save_bitmap_path 函数得到实际贴图 。拷贝到新目录并重命名
		-- 导出 max文件到 客户存储，并重命名max文件名
		-- 完成
		-- number模型编号，用于客户存储模型文件名，贴图文件名，文件夹名。
		local _number = 0;
		for f in all_maxFiles do
		(
			local _curModelFilePath = getFilenamePath f;
			
			-- 需要获得当前 max 文件是 low 还是 high ,还是 public_model
			-- 公共贴图不需要导出来了
			local _stripPathLeaf = ( filenamefrompath _curModelFilePath );
			
			if(_stripPathLeaf == "bridge\\" or _stripPathLeaf == "road\\" or _stripPathLeaf == "river\\" or _stripPathLeaf == "green\\")then
			(
				-- 直接拷贝出去就行了。
				local _newPubModelPath = Yvi_makePublicModelFolder _outputPath;
				local _newFile = _newPubModelPath + _stripPathLeaf + (filenameFromPath f);
				copyFile f _newFile;
				--再拷贝其贴图文件
				local _newPubModelTexturePath = Yvi_makeCurObjTextureFolder3 _newPubModelPath highOrLow:_stripPathLeaf;
				local _pubModleBitmmap = #();
				join _pubModleBitmmap getFiles (_curModelFilePath +_stripPathLeaf +"*.jpg");
				join _pubModleBitmmap getFiles (_curModelFilePath +_stripPathLeaf +"*.png");
				
				for b in _pubModleBitmmap do 
				(
					local _newBitmap = _newPubModelTexturePath + "\\" + (filenameFromPath b);
					copyFile b _newBitmap;
				)
				continue;
			)
		---------------
			
			loadmaxfile f;
			_number +=1;
			
			--去掉max文件的原根目录，换成新的保存的根目录。
			--这样就省去创建原max文件夹。
			-- \edushi_dataset1\hongkong_11_003\003002\high\
			-- \edushi_dataset2\hongkong_11_003\003002\high\
			local tmpStr = (filterString  _curModelFilePath "\\");
			local _oldMaxName = tmpStr[tmpStr.count-2];-- 得到max路径中倒数第三个文件夹。
			
			--\edushi_dataset2\hongkong_11_003\
			local _newMaxPath = (_outputPath+_oldMaxName);
			local _newMaxFilePath = Yvi_makeNewMaxFilePath _newMaxPath;
			
			local fname = _newMaxFilePath+ _oldMaxName + ".txt";
			fstream = openFile fname mode:"at";
			if( fstream == undefined )then 
			(
				CreateFile fname;
			)
			
			-- 这里的max文件内只有一个物体对象
			max select all;
			local objs = selection as array;
			if objs.count == 1 then 
			(
				-- 这里的原模型名实体号，要对应模型编号。
				local _theNumber = Yvi_formatNumStr _number;-- 模型编号
				local _newObjName = "model_" + _theNumber;
				
				local _newObjNamePath = Yvi_makeCurObjNameFolder2 _newMaxFilePath _newObjName;
				local _newTexturePath = Yvi_makeCurObjTextureFolder3 _newObjNamePath highOrLow:_stripPathLeaf;
				
				-- 记录原实体名 和 对应的新物体名编号
				if fname != undefined then
					Yvi_writeInforFile fname (objs[1].name) _newObjName;
				format "%, %\n" (objs[1].name) _newObjName;
				objs[1].name = _newObjName;
				
				-- 这里并不能得到真实路径的贴图 ，因为贴图保存的路径是相对的路径
				-- 需要对贴图进行重命名后再拷贝。
				_bModel2Clent = 1;
			--	Yvi_getObjUsedBitmap2 objs[1] _newTexturePath _curModelFilePath _pubModelPath _stripPathLeaf _bModel2Clent 4;
				
				-- 导出文件
				Yvi_exportConvertToClientArchive _newObjNamePath objs[1] _stripPathLeaf;
				
				Yvi_getObjUsedBitmap2 objs[1] _newTexturePath _curModelFilePath _pubModelPath _stripPathLeaf _bModel2Clent 3;
				save_path = _newObjNamePath + _stripPathLeaf;
				makeDir save_path;
				saveNodes objs[1] (save_path +"\\" + objs[1].name + ".max") quiet:true;
			)

		)
		resetMaxFile #noPrompt 
	)
-----------------
-- // 客户归档 转 模型归档
	on btn_ClientArchToModelArch pressed do 
	(
		local bClient2Model = 0;
		local _nameInfoArr = #();
		local _inputPath = edt_InputPath.text;
		local _outputPath = edt_OutputPath.text;
		if( _outputPath == "" or _inputPath == "")then
		(
			return undefined;
		)
		if(_outputPath[_outputPath.count] != "\\")then
		(
			_outputPath += "\\";
		)
		-- 读文件中保存的原物体名对应编号名
		all_maxFiles = Yvi_getFilesRecursive _inputPath "*.max";
		
		for f in all_maxFiles do
		(
			local _curModelFilePath = getFilenamePath f;
			
			-- 需要获得当前 max 文件是 low 还是 high ,还是 public_model
			-- 公共贴图不需要导出来了
			local _stripPathLeaf = ( filenamefrompath _curModelFilePath);
			
			if(_stripPathLeaf == "bridge\\" or _stripPathLeaf == "road\\" or _stripPathLeaf == "river\\" or _stripPathLeaf == "green\\")then
			(
				-- 直接拷贝出去就行了。
				local _newPubModelPath = Yvi_makePublicModelFolder _outputPath;
				local _newFile = _newPubModelPath + _stripPathLeaf + (filenameFromPath f);
				copyFile f _newFile;
				--再拷贝其贴图文件
				local _newPubModelTexturePath = Yvi_makeCurObjTextureFolder3 _newPubModelPath highOrLow:_stripPathLeaf;
				local _pubModleBitmmap = #();
				join _pubModleBitmmap getFiles (_curModelFilePath+_stripPathLeaf +"*.jpg");
				join _pubModleBitmmap getFiles (_curModelFilePath+_stripPathLeaf +"*.png");
				
				for b in _pubModleBitmmap do 
				(
					local _newBitmap = _newPubModelTexturePath + "\\" + (filenameFromPath b);
					copyFile b _newBitmap;
				)
				continue;
			)
		---------------
			SetQuietMode true;
			loadmaxfile f;
			--_number +=1;
			
			local tmpStr = (filterString  _curModelFilePath "\\");
			local _oldMaxName = tmpStr[tmpStr.count-2];-- 得到max路径中倒数第三个文件夹。
			
			local fname = getFiles (_inputPath + "\\" + _oldMaxName+"/*.txt");

			if(fname.count == 0)then
			(
				format "模型编号文件表丢失，读取模型编号失败。输入路径是否为归档根目录？\n";
				return undefined;	
			)		
			
			
			--\edushi_dataset2\hongkong_11_003\
			local _newMaxPath = _outputPath + _oldMaxName;

			local _newMaxFilePath = Yvi_makeNewMaxFilePath _newMaxPath;
			-- 这里的max文件内只有一个物体对象
			max select all;
			local objs = selection as array;
			if objs.count == 1 then 
			(
				-- 当前物体名是一个编号名,需要转换成 以前的原物体名
				local oldName = Yvi_readInforFile fname[1] (objs[1].name);
				if oldName == undefined then 
				(
					format "模型编号文件中找不到%\n" oldName;
					return undefined;
				)
				objs[1].name = oldName;
				print oldName;
				local _newObjName = objs[1].name;
				local _newObjNamePath = Yvi_makeCurObjNameFolder2 _newMaxFilePath _newObjName;
				local _newTexturePath = Yvi_makeCurObjTextureFolder3 _newObjNamePath highOrLow:_stripPathLeaf;
				local _key = "";
				if (_stripPathLeaf == "high")then 
				(
					_key = "H";
				)
				else
				(
					_key = "L";
				)
				-- 导出文件
				Yvi_exportConvertToClientArchive _newObjNamePath objs[1] _stripPathLeaf;
				
				bClient2Model = 2;
				Yvi_getObjUsedBitmap2 objs[1] _newTexturePath _curModelFilePath _inputPath _key 2 3;
				

				
				save_path = _newObjNamePath + _stripPathLeaf;
				makeDir save_path;
				saveNodes objs[1] (save_path +"\\" + objs[1].name + ".max") quiet:true;
				
			)
		)
		
	)
/********************************************************************************************************************************/	
	on rl_ExportArchive resized p2 do
	(
		p2.y = 286;
		rl_ExportArchive.width = p2.y;
	)
	
/**************************************** 界面中英文切换 ***************************************************************/
	on btn_EN pressed do 
	(
		ckb_CheckMaxFileName.caption =  "Max File Name"
		ckb_CheckObjName.caption =  "Models Name"
		ckb_CheckBitmapName.caption =  "Bitmap Name"
		chb_CheckArchFileName.caption =  "Archive Files Name"

		btn_SetCheckNameArchDir.caption =  "Set Archive Root..."
			btn_SetCheckNameArchDir.tooltip = "Specified Archive root directory for check the files name."

		btn_DoCheck.caption =  "Check Name"
			btn_DoCheck.tooltip = "Do check named now..."

		btn_ArchRoot.caption =  "Archive Root..." 
			btn_ArchRoot.tooltip = "Specified export BuildingModel files archive root directory."

		--rdb_ModelQuality.labels = #("High Model","Low Model")
		btn_ExportBuilding.caption = "Export Building" 
			btn_ExportBuilding.tooltip = "Export currently selected model(s) to 3ds/obj/max files."

		edt_PublicArchRoot.caption =  ""
		btn_PublicArchRoot.caption =  "Archive Root..." 
			btn_PublicArchRoot.tooltip = "Specified export PublicModel files archive root directory."


		btn_OpenPublicPath.caption =  "Public Path..."  
			btn_OpenPublicPath.tooltip = "Specified has Public Model files(*.max) directory."
		mlbx_PublicFiles.caption = "Public Model Files:"
		--rdb_listOperation.labels = #("All","None","Empty", "Delete")

		btn_ExportPublicModel.caption = "Export Public Model" 
			btn_ExportPublicModel.tooltip = "Now export currently selected public model to 3ds/obj/max files."

		btn_OpenInput.caption =  "Input..."
			btn_OpenInput.tooltip = "Specified has already exists 'Model/Client Archive' directory"

		btn_OpenOutput.caption =  "Output..."
			btn_OpenOutput.tooltip = "Specified need to save 'Client/Model Archive' directory"
		
		btn_ModelArchToClientArch.caption =  "ModelArchive To ClientArchive" 
			btn_ModelArchToClientArch.tooltip = "Now transform Model to Client archive."
		btn_ClientArchToModelArch.caption =  "ClientArchive To ModelArchive" 
			btn_ClientArchToModelArch.tooltip = "Now transform Client to Model archive."

		rl_ExportArchive.title = "Export and Archive v0.1"
	)
	
	on btn_CH pressed do 
	(
		ckb_CheckMaxFileName.caption =  "Max 文件名"
		ckb_CheckObjName.caption =  "模型名"
		ckb_CheckBitmapName.caption =  "贴图名"
		chb_CheckArchFileName.caption =  "归档文件名"

		btn_SetCheckNameArchDir.caption =  "设置归档根..."
			btn_SetCheckNameArchDir.tooltip = "设置检查文件命名的归档根目录."

		btn_DoCheck.caption =  "检查命名"
			btn_DoCheck.tooltip = "开始检查命名..."

		btn_ArchRoot.caption =  "归档根..." 
			btn_ArchRoot.tooltip = "指定导出建筑模型文件的归档根目录."

		--rdb_ModelQuality.label = "高质模型"
		btn_ExportBuilding.caption = "导出模型" 
			btn_ExportBuilding.tooltip = "导出当前选择的模型到 3ds/obj/max 文件."

		edt_PublicArchRoot.caption =  ""
		btn_PublicArchRoot.caption =  "归档根..." 
			btn_PublicArchRoot.tooltip = "指定导出公共模型文件的归档根目录."


		btn_OpenPublicPath.caption =  "公共模型路径..."  
			btn_OpenPublicPath.tooltip = "指定已有公共模型文件(*.max) 的路径."
		mlbx_PublicFiles.caption = "公共模型文件:"
		--rdb_listOperation.labels = #("全选","空选","清空", "删除")

		btn_ExportPublicModel.caption = "导出公共模型"
			btn_ExportPublicModel.tooltip = "将当前列表中选择的公共模型文件导出到 3ds/obj/max 文件."

		btn_OpenInput.caption =  "输入..."
			btn_OpenInput.tooltip = "指定已经存在的  '模型/客户归档' 根目录"

		btn_OpenOutput.caption =  "输出..."
			btn_OpenOutput.tooltip = "指定需要保存的  '模型/客户归档' 根目录"
		
		btn_ModelArchToClientArch.caption =  "模型存储转客户存储" 
			btn_ModelArchToClientArch.tooltip = "开始转换到模型存储转客户存储."
		btn_ClientArchToModelArch.caption =  "客户存储转模型存储" 
			btn_ClientArchToModelArch.tooltip = "开始转换到客户存储转模型存储."
			
			rl_ExportArchive.title = "导出并归档 v0.1"
	)
)

--CreateDialog rl_ExportArchive style:#(#style_minimizebox, #style_titlebar,#style_border, #style_sysmenu, #style_resizing)

try (closerolloutfloater floatFrame) catch () 
floatFrame = newrolloutfloater "Export and Archive v0.1" 296 630--1180 300
addrollout rl_ExportArchive floatFrame --rolledup:true
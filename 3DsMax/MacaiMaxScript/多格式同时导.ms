-- 需要同时导出三种类型格式的文件，
--并且归档
--分High 和 low 两种类型导出三种格式文件。
--归档的路径为：根目录 / max文件名/模型名/High（low）/(导出后的三种格式文件，并有”texture”文件夹存放使用到的贴图, 命名都前面都加上"H"或“L”)


--根目录下建立 max文件名的文件夹。用来保存导出的模型文件
function Yvi_makeCurMaxFileSaveFolder root =
(
	_maxSmpName = getFilenameFile  maxFileName;
	if(root[root.count] != "\\")then
		root += "\\";
	if(_maxSmpName == "")then
	(
		return undefined;
	)
	max_path = root + _maxSmpName
	makeDir max_path
	
	return max_path
)

function Yvi_getObjUsedBitmap obj save_path key:"H" =
(
	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	mat = obj.Material;
	if(mat != undefined)then
	(
		if (classof mat == MultiMaterial)then
		(
			for m in mat do
			(
				if(m.DiffuseMap != undefined)then
				(
					_oldBitmap = m.DiffuseMap.filename;
					_oldBitmapName = filenameFromPath _oldBitmap;

					_newBitmap = save_path +  key + _oldBitmapName;
					copyFile _oldBitmap _newBitmap;
				)
				if(m.OpacityMap != undefined)then
				(
					_oldOpaBitmapName = filenameFromPath m.opacityMap.filename;
					_newBitmap = save_path +  key + _oldOpaBitmapName;
					copyFile _oldOpaBitmapName _newBitmap;
				)
				if(m.SelfIllumMap != undefined)then
				(
					_oldSelBitmapName = filenameFromPath m.SelfIllumMap.filename;
					_newBitmap = save_path + key + _oldSelBitmapName;
					copyFile _oldSelBitmapName _newBitmap;
				)
			)
		)
		if(classof mat == Standard)then
		(
			if(mat.DiffuseMap != undefined)then
			(
				_oldBitmap = mat.DiffuseMap.filename;
				_oldBitmapName = filenameFromPath _oldBitmap;
				_newBitmap = save_path +  key + _oldBitmapName;
				copyFile _oldBitmap _newBitmap;
			)
			if(m.OpacityMap != undefined)then
			(
				_oldOpaBitmapName = filenameFromPath m.opacityMap.filename;
				_newBitmap = save_path +  key + _oldOpaBitmapName;
				copyFile _oldOpaBitmapName _newBitmap;
			)
			if(m.SelfIllumMap != undefined)then
			(
				_oldSelBitmapName = filenameFromPath m.SelfIllumMap.filename;
				_newBitmap = save_path +  key + _oldSelBitmapName;
				copyFile _oldSelBitmapName _newBitmap;
			)
		)
	)
	else
	(
		format "模型 % 没有贴图。\n" obj.name;
	)
)

function Yvi_makeOriginalFolder modelPath =
(
	_originalFolder = modelPath + "\\original"
	makeDir (_originalFolder);
)
--导出High类型文件
function Yvi_exportHighQuality obj save_path =
(
	obj_name = obj.name;
	if save_path == undefined or save_path == "" then
	(
		return undefined;
	)
	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	_modelPath = save_path + obj_name + "\\high";
	makeDir (_modelPath);
	
	_fileName = "H" + obj_name;
	exportfile (_modelPath + "\\" + _fileName + ".3ds") #noprompt selectedOnly:true;
	exportfile (_modelPath + "\\" + _fileName + ".obj") #noprompt selectedOnly:true;
	saveNodes obj (_modelPath + "\\" + _fileName + ".max") quiet:true;
)

--导出low类型文件
function Yvi_exportLowQuality obj save_path =
(
	obj_name = obj.name
	if save_path == undefined or save_path == "" then
	(
		return undefined;
	)
	if(save_path[save_path.count] != "\\")then
		save_path += "\\";
	
	_modelPaht = save_path + obj_name + "\\low";
	makeDir (_modelPaht);
	
	_fileName = "L" + obj_name;
	exportfile (_modelPaht + _fileName + ".3ds") #noprompt selectedOnly:true;
	exportfile (_modelPaht + _fileName + ".obj") #noprompt selectedOnly:true;
	saveNodes obj (_modelPath + _fileName + ".max") quiet:true;
)

(
	rollout rl_ExportBuildingModelFiles "建模模型归档" width:240 height:402
	(
		group "建筑模型导出参数"
		(
			label lb_archive "归档根路径："align:#left
				editText edt_archiveRoot "" width:167 height:18 across:2  align:#left
				button btn_archiveRoot "打开..." tooltip:"指定归档的根目录" align:#right

			radiobuttons rdb_modelQuality  labels:#("High Model","Low Model") default:1 columns:2 align:#left

			button btn_exportBuildingModel "导出模型建筑"
		)

		on btn_exportFiles pressed do
		(
			obj =  selection[1]
			Yvi_exportHighQuality obj maxFilepath
		)
	)
	
---------------------------------------------------------------------------
	rollout rl_ExportPublicModelFiles "公共模型归档" width:240 height:402
	(
		group "公共模型导出参数"
		(
			label lb_publicModelRoot "公共模型路径："align:#left
				editText edt_publicModelRoot "" width:167 height:18 across:2  align:#left
				button btn_openPublicModelRoot "打开..." tooltip:"打开公共模型文件路径" align:#right
			
			multilistBox mlist_allMaxFile "所有公共模型max文件：" selection:1 height:7
			radiobuttons btn_listOperation "" labels:#("全部","不选","清空") default:1 columns:3 align:#left
			
			button btn_exportPublicModel "导出公共模型"
		)
		
	----------------------------------

	)
	
-----------------------------------------------------------------------------
	rollout rl_ModelClientArchiveInterConv "模型客户存档转换" width:240 height:402
	(
		group "归档转换参数"
		(
			label lb_modelArchive "模型存档路径：" align:#left
				editText edt_modelArchiveRoot "" width:168 across:2
				button btn_openModelArchiveRoot "打开..." tooltip:"打开模型存储归档路径" align:#right
				button btn_modelToClient "模型存档转客户存档"
			
			label lb_clientArchive "客户存档路径："align:#left
				editText edt_clientArchiveRoot ""width:168 across:2
				button btn_openClientArchiveRoot "打开..." tooltip:"打开客户存储归档路径" align:#right
			
			button btn_clientToModel "客户存档转模型存档"
		)
	)
	
----------------------------------
	try (closerolloutfloater floatExportModel) catch () 
	floatExportModel = newrolloutfloater "模型导出存储归档 v0.1" 260 440
	addrollout rl_ExportBuildingModelFiles floatExportModel --rolledup:true 
	addrollout rl_ExportPublicModelFiles floatExportModel
	addrollout rl_ModelClientArchiveInterConv floatExportModel
)





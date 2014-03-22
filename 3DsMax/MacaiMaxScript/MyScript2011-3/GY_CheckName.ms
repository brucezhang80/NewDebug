
-- 贵阳模型检查工具需求
-- 非透明贴图： 01a01_jz1001_001.jpg
-- 透明贴图：   01a01_jz1001_001_alp.tga
-- 三维模型命名如： 01a01_jz1001

-- xxxxx_xxxxxx
-- 	建模顺序号		001,002,....nnn
-- 	细节层次			1,2,3
-- 	模型类型			jz,dx,zw,xp,lq

-------单元顺序号-------
-- 	建模单元顺序号		01,02,...,nn
-- 管理单元顺序号		a....z
-- 区编码最后两位数		00,01,02,03,11,12,13,14,21,22,23,81
--12字符



--------------------------------------------------------------------------------------------------------

--小写转成大写
function MytoLower instring =
( 
	local upper, lower, outstring 

	upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
	lower = "abcdefghijklmnopqrstuvwxyz"

	outstring = copy instring

	for i = 1 to outstring.count do
	( 
		j = findString upper outstring[i]

		if (j != undefined) do outstring[i] = lower[j]
	)
	return outstring 
) 
------------------------------------
-- clled by Fn_isWorkID
function Fn_formatNumStr num d =
(
	str = num as string
	if str.count < d then 
		for i in 1 to d - str.count do str = "0" + str
			
	return str
)
----------------------------------------------------------------------------------------------------------
--单元顺序号
function Fn_iscodeID strName =
(
	-- 区编码最后两位数		00,01,02,03,11,12,13,14,21,22,23,81,82
	-- 管理单元顺序号		a....z
	-- 建模单元顺序号		01,02,...,nn

	local arr_codeID = #("00", "01","02", "03", "11","12", "13", "14", "21", "22", "23", "81", "82")

	local str_codeID = substring strName 1 2 -- return 区编码	
	for i = 1 to arr_codeID.count do
	(
		if str_codeID == arr_codeID[i] then
		(
			return true
		)
		else
		(
			continue
			--return false
		)
	)
	
	return false
)

function Fn_isManagerID strName =
(
	local str_managerID = substring strName 3 1 -- return 管理单元 "a..z"
	if (str_managerID > "a" or str_managerID < "z") or (str_managerID > "A" or str_managerID < "Z") then
	(
		return true
	)
	else
	(
		return false
	)
	
	return false
)

function Fn_isWorkID strName =
(
	local str_workID = substring strName 4 2 -- return 建模单元 "01..nn"
	for i = 1 to 99 do
	(
		s = Fn_formatNumStr i 2 			-- return "01..nn"
		if (s == str_workID) then
		(
			return true
		)
		else
		(
			continue
			--return false
		)
	)
	
	return false
)
--------
function Fn_isModelClass strName =
(
	local arr_modelCls = #("jz", "JZ", "dx", "DX", "zw", "ZW", "xp", "XP", "lq", "LQ")
	local str_modelCls = substring strName 7 2-- return 模型类型 jz,dx,zw,xp,lq
	for i = 1 to arr_modelCls.count do
	(
		if str_modelCls == arr_modelCls[i] then
		(
			return true
		)
		else
		(
			continue
			-- return false
		)
	)
	
	return false
)

function Fn_isLevelID strName =
(
	local str_level = substring strName 9 1 --return  细节层次			1,2,3
	if str_level == "1" or str_level == "2" or str_level == "3" then
	(
		return true
	)
	else
	(
		continue
		--return false
	)
	
	return false
)

------
 -- 
function Fn_getObjectIndex strName =
(
	if strName.count < 3 then return 0
	suffix  = substring strName (strName.count-2) 3 --最后三位数:建模序号
	index = suffix as integer
	return index
)
-----------------------------------------------------------------------------------------------------------------
function Fn_checkMatName strName =
(
	
)


--------------------------------------------------------------------------------------------------------
--获取子目录文件
--getFilesRecursive 路径 文件类型如："*.max"
function Fn_getFilesRecursive root filename =
(
	local all_Files = #()
	local TheAllSubFolder = #()
	
	--获取root目录下的所有子目录
	TheAllSubFolder = GetDirectories (root+"/*")
	for s in TheAllSubFolder do
	(
		--将所有子目录下的子目录append到TheAllSubFolder
		--这时TheAllSubFolder装入了root目录下的所有子目录，包括root目录
		join TheAllSubFolder (GetDirectories (s + "/*"))
	)

	for f in TheAllSubFolder do
	(
		--在每个目录下遍历
		join all_Files (getFiles (f + filename))
	)
	join all_Files (getFiles (root  + "/"+ filename))
	return all_Files
)--函数结束


Global g_arr_oldfilenames = #()
Global g_arr_newfilenames = #()
Global g_tilefilename = maxfilepath

-- 重命名贴图
function Fn_reNameMat obj newMatName resMatName =
(
	local arr_resFiles =#()
	--这里清除全局数组
	g_arr_oldfilenames=#()
	g_arr_newfilenames=#()
	
	mat =  obj.Material
	if mat == undefined do 
	(
		return 0
	)
	if classof mat == Multimaterial then
	(
		for i = 1 to mat.numsubs do
		(
			-- 修改以贴图文件名来命名
			--old_name = mat.materialList[i].name
			old_diffname = mat.materialList[i].diffusemap.filename -- 贴图文件名
			ext_oldfilename = filenameFromPath old_diffname
			--获取文件名,不带后缀
			oldMatName = getFilenameFile old_diffname
			ext = getfilenametype old_diffname
			
			if (resMatName != oldMatName) then
			(
				--print "不匹配,继续下一个材质...\n"
				continue
			)

			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- 只有一个文件如果文件名唯一
			--print ("arr_resFiles.count:"+arr_resFiles.count as string)
			
			if arr_resFiles.count == 0 do 
			(
				format "找不到贴图文件: %\n" ext_oldfilename
				continue
			)
			
			oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
			append g_arr_oldfilenames oldfullpathfile
			newfullpathfile = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			append g_arr_newfilenames newfullpathfile
			
			mat.materialList[i].name = newMatName
			mat.materialList[i].diffuseMap.name = newMatName
			mat.materialList[i].diffuseMap.fileName = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			
			--print ("g_oldfilenames:" + g_arr_oldfilenames as string)
			--print ("g_newfilenames:" + g_arr_newfilenames as string)
			
			try
			(
				if(mat.materialList[i].opacityMap != undefined)then
				(
					mat.materialList[i].opacityMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].opacityMap.name = mat.materialList[i].diffuseMap.name
				)
				if(mat.materialList[i].SelfIllumMap != undefined)then
				(
					mat.materialList[i].SelfIllumMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].SelfIllumMap.name = mat.materialList[i].diffuseMap.name
				)
			)catch(format "none OpacityMap or SelfIllumMap...")
			
			arr_resFiles =#()
		)
	)
	if (classof mat == Standard)then
	(
		try
		(
			old_diffname = mat.diffuseMap.filename
			ext_oldfilename = filenameFromPath old_diffname
			oldMatName = getFilenameFile old_diffname
			ext = getfilenametype old_diffname
			if (resMatName != oldMatName) then
			(
				return 0
			)

			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- 只有一个文件如果文件名唯一
			if arr_resFiles.count == 0 do 
			(
				format "找不到贴图文件: %\n" ext_oldfilename
				return 0
			)
			
			oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
			append g_arr_oldfilenames oldfullpathfile
			newfullpathfile = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			append g_arr_newfilenames newfullpathfile
			
			mat.name = newMatName
			mat.diffuseMap.name = newMatName
			mat.diffuseMap.fileName = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			
			--MessageBox "暂不考虑标准材质!"
			--return 0
		)catch(print "暂不考虑标准材质!")
	)
)

-- 硬盘中的贴图文件名
function Fn_HandleHardDiskFiles =
(
	try
	(
		for i=1 to g_arr_oldfilenames.count do
		(
			old  = g_arr_oldfilenames[i]
			new  = g_arr_newfilenames[i]
			
			renameFile old  new
		)
	)catch()
)


-- 自动重命名贴图
-- 1.贴图名 = 材质名
-- 2.在场景所有的贴图中找使用相同的贴图(除JXGY的命名之外) 以及对应的使用该贴图的物体(绑定之),以备后面重命名贴图后找到对应的物体.
-- 3.把非共用的贴图按 物体名为前缀+序号命名,这里的物体名在处理贴图之前必须按要求规范命名.
-- 4.共用的贴图(非JXGY命名的贴图), 以其中一个物体(共用贴图的物体)名来命名(这里命名前先检查该物体上的贴图序号,以该序号继续命名下去), 将该命好名的贴图
		-- 再赋予回共用该贴图的物体上(如何找出共用该贴图的物体?)
-- 5.对于是tga格式的贴图.后面添加 "_alp"
-- 6.命名完成.

function Fn_collectSubMats mat mats tempmats = 
(
	if ( classof mat  == Multimaterial) then 
	(
		for i=1 to mat.numsubs do 
		(
			Fn_collectSubMats mat[i] mats
		)
	)
	else if(classof mat == standard) then 
	(
		if mat.diffuseMap != undefined Then
		(
			strippedName = filenameFromPath (mat.diffuseMap.fileName)
			ignore = findString strippedName "JZGY" --仅收集非 "JZGY" 的贴图
			if(ignore == undefined)then
			(
				n = finditem tempmats strippedName
				if n == 0 then -- 找不到重复的
				(
					append  mats mat
				)
				append tempmats strippedName
			)
		)
	)
)

--这里收集场景中的所有子材质
function Fn_collectAllSubMats = 
(
	local l_tempmats = #()
	local l_arr_allSubMats = #()
	for mat in scenematerials do 
	(
	   Fn_collectSubMats mat l_arr_allSubMats l_tempmats
	)
	
	return l_arr_allSubMats
)

-- 路径 文件类型 如："*.max"
function Fn_getBitmapFiles root ext =
(
	local 	all_Files = #()
	local TheAllSubFolder = #()
	--获取root目录下的所有子目录
	TheAllSubFolder = GetDirectories (root+"*")
	for d in TheAllSubFolder do
	(
		--递归的将所有子目录下的子目录append到TheAllSubFolder
		--这时TheAllSubFolder装入了root目录下的所有子目录，包括root目录
		join TheAllSubFolder (GetDirectories (d+"*"))
	)

	for f in TheAllSubFolder do
	(
		--在每个目录下遍历
		join all_Files (getFiles (f + ext))
	)
	join all_Files (getFiles (root + ext))
	return all_Files 
)--函数结束
	
Global g_arr_shareMat = #() -- 共用的贴图
Global g_arr_noshareMat = #() -- 不共用的贴图
Global g_arr_allOldMat = #() -- 所有的原始贴图
struct s_shareOM(node, matName, path, ext, counts)

function Fn_findOutShareMat arrBitmapFiles = 
(
	--找出共用的
	for ifile in arrBitmapFiles do -- 从硬盘中收集的贴图文件中遍历
	(
		local l_shareMat = #() -- 相同材质的贴图
		local l_noshareMat = #() -- 不相同材质的贴图
		local matCounts = 0 -- 记录共用贴图的个数
		
		strippedName = filenameFromPath ifile
		simpleName = getFilenameFile ifile
		ext = MytoLower (getfilenametype ifile)
		
		ignore = findString strippedName "JZGY" --仅收集非 "JZGY" 的贴图
		if(ignore == undefined)then
		(
			-- 在场景中遍历所有物体中使用该贴图文件的材质名
			for obj in Objects do
			(
				local mat = obj.Material
				if mat != undefined do
				(
					if (classof mat == Multimaterial) then -- 如果是多维材质
					(
						
						for i = 1 to mat.numsubs do
						(
							if mat.materialList[i].diffuseMap==undefined then continue
							old_filepathname = mat.materialList[i].diffusemap.filename -- 完整的贴图文件名
							ext_oldfilename = filenameFromPath old_filepathname
							oldpath = getFilenamePath old_filepathname
							oldMatName = getFilenameFile old_filepathname
							ext = MytoLower (getfilenametype old_filepathname)
							
							-- 如果找到的材质名与文件名相同的
							if (strippedName == ext_oldfilename) then
							(
								matCounts += 1
								
								-- 材质与物体绑定
								share = s_shareOM node:obj matName:old_filepathname path:oldpath ext:ext counts:matCounts
								append l_shareMat share
							)
						) -- for numsubs end
					)
				)-- if Multimateria end
			) -- for in all obj end
			
			if(matCounts > 1)Then -- 共用的贴图
			(
				print ("l_shareMat: " + l_shareMat as string) 
				append g_arr_shareMat l_shareMat -- #(#(),#(),#())
			)
		)
	) -- for ifile 
	print ("g_arr_shareMat: " + g_arr_shareMat as string)
)

-- 
function Fn_AutoRenameMat obj =
(
	local arr_resFiles = #()
	local newMatName

	mat =  obj.Material
	if mat == undefined do 
	(
		return 0
	)
	if classof mat == Multimaterial then
	(
		local l_noshare_count = 0
		for i = 1 to mat.numsubs do
		(
			old_dufsename = mat.materialList[i].diffusemap.filename -- 贴图文件名
			ext_oldfilename = filenameFromPath old_dufsename
			--获取文件名,不带后缀
			oldMatName = getFilenameFile old_dufsename
			ext =MytoLower(getfilenametype old_dufsename)
			
			-- 处理非共用的贴图
			-- 在共用贴图的数组中找,是否存在
			-- 如果不可在,那么就是非共用的
			local index = 0
			for j = 1 to g_arr_shareMat.count do 
			(
				for k = 1 to g_arr_shareMat[k].count do 
				(
					index = finditem g_arr_shareMat[j][k].matName old_dufsename
				)
			)
			if index == 0 then -- 没找到,非共用
			(
				l_noshare_count += 1
				arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- 只有一个文件如果文件名唯一
				--print ("arr_resFiles.count:"+arr_resFiles.count as string)
				if arr_resFiles.count == 0 do 
				(
					format "找不到贴图文件: %\n" ext_oldfilename
					continue
				)
				
				oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
				
				-- 如果贴图文件是tga格式 ,则这是一张透明贴图通道的.这时候需要添加 "_ALP"
				-- 规范化命名
				-- 赋予新的命名
				if (ext == ".tga")then
				(
					newMatName = obj.name + "_" + (Fn_formatNumStr i 3) + "_alp"
					print ("newMatName tga: " + newMatName)
				)
				else
				(
					newMatName = obj.name + "_" + (Fn_formatNumStr i 3)
				)
				newfullpathfile = (getFilenamePath arr_resFiles[1]) + newMatName + ext
				
				-- 开始重命名贴图
				reNameFile oldfullpathfile newfullpathfile
				mat.materialList[i].name = newMatName
				mat.materialList[i].diffuseMap.name = newMatName
				mat.materialList[i].diffuseMap.fileName = newfullpathfile
			)
			try
			(
				-- 透明贴图需要加 _ALP
				if(mat.materialList[i].opacityMap != undefined)then
				(
					
					mat.materialList[i].opacityMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].opacityMap.name = mat.materialList[i].diffuseMap.name
				)
				if(mat.materialList[i].SelfIllumMap != undefined)then
				(
					mat.materialList[i].SelfIllumMap.filename= mat.materialList[i].diffuseMap.fileName
					mat.materialList[i].SelfIllumMap.name = mat.materialList[i].diffuseMap.name
				)
			)catch(format "none OpacityMap or SelfIllumMap...")
			
			arr_resFiles =#()
		)-- for in each submat
		
		-- 在共用贴图数组中找出对应的物体
		-- 在对应的物体上命名共用贴图 ------------
		for m = 1 to g_arr_shareMat.count do 
		(
			for n = 1 to g_arr_shareMat[m] do 
			(
				obj_index = finditem (g_arr_shareMat[m][n].node).name obj.name
				if (obj_index != 0) then --找到
				(
					for p = 1 to  mat.numsubs do 
					(
						old_filepathname = mat.materialList[p].diffusemap.filename -- 贴图文件名
						ext_oldfilename = filenameFromPath old_dufsename
						--获取文件名,不带后缀
						oldMatName = getFilenameFile old_dufsename
						ext = MytoLower(getfilenametype old_dufsename)
							
						for l = 1 to g_arr_shareMat[m][l].count do 
						(
							if(g_arr_shareMat[m][l].matName == old_filepathname)then 
							(
								if (ext == ".tga")then
								(
									newMatName = obj.name + "_" + (Fn_formatNumStr l_noshare_count+l 3) + "_alp"
								)
								else
								(
									newMatName = obj.name + "_" + (Fn_formatNumStr l_noshare_count+l 3)
								)
								newfullpathfile = (getFilenamePath old_filepathname) + newMatName + ext
								renamefile g_arr_shareMat[m][l].matName newfullpathfile
							)
						)
						
					)

				)
			)
		)

	)
	if (classof mat == Standard)then
	(
		try
		(
			old_difsname = mat.diffuseMap.filename
			ext_oldfilename = filenameFromPath old_difsname
			oldMatName = getFilenameFile old_difsname
			ext = getfilenametype old_difsname
			
			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- 只有一个文件如果文件名唯一
			if arr_resFiles.count == 0 do 
			(
				format "找不到贴图文件: %\n" ext_oldfilename
				return 0
			)
			
			oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
			
			if (ext == ".tga")then
			(
				newMatName = obj.name + "_" + (Fn_formatNumStr 1 3) + "_ALP"
			)
			else
			(
				newMatName = obj.name + "_" + (Fn_formatNumStr 1 3)
			)
			newfullpathfile = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			
			reNameFile oldfullpathfile newfullpathfile
			
			mat.name = newMatName
			mat.diffuseMap.name = newMatName
			mat.diffuseMap.fileName = newfullpathfile
			
			--MessageBox "暂不考虑标准材质!"
			--return 0
		)catch(print "暂不考虑标准材质!")
	)
)

-------------------------------------------------------------------------------------------------
	
	-- //////////////////////////////////////检查贴图2的次方
	Global counteri, counterj, counterk, counterl
	Global g_arr_errMapFiles = #(),mapfiles = #(),g_arr_not2Square = #()
	
	fn Fn_isDivided num = 
	(	--判断能否被2除尽
		 while num > 2 do num = num /2.
		 if num == 2. then Return  true else Return false
	)
	
	function Fn_resetMaterials = 
	(
		for i = 1 to 24 do
		(
			meditMaterials[i]=standard()
		)
	)
	
	function Fn_addMap mapfile =
	(
		try
		(
			local mapfileN = mapfile as name
			local index = finditem mapfiles mapfileN
			local bm = BitmapTexture fileName:mapfile
		
			getProgressCancel()
			
			if index == 0 do append mapfiles mapfileN

			if bm.bitmap != undefined then
			(
				if bm.bitmap.width == bm.bitmap.height and
				(
					bm.bitmap.width == 1 or
					bm.bitmap.width == 2 or
					bm.bitmap.width == 4 or
					bm.bitmap.width == 8 or
					bm.bitmap.width == 16 or
					bm.bitmap.width == 32 or
					bm.bitmap.width == 64 or
					bm.bitmap.width == 128 or
					bm.bitmap.width == 256 or
					bm.bitmap.width == 512 or
					bm.bitmap.width == 1024 or
					bm.bitmap.width == 2048 or
					bm.bitmap.width == 4096
				)				
				then
				(
					format "'%' 是2的N次方,长宽比例为 1:1\n" mapfileN
					counterj += 1
				)
				else if
				(
				    (bm.bitmap.width == 1 or
					bm.bitmap.width == 2 or
					bm.bitmap.width == 4 or
					bm.bitmap.width == 8 or
					bm.bitmap.width == 16 or
					bm.bitmap.width == 32 or
					bm.bitmap.width == 64 or
					bm.bitmap.width == 128 or
					bm.bitmap.width == 256 or
					bm.bitmap.width == 512 or
					bm.bitmap.width == 1024 or
					bm.bitmap.width == 2048 or
					bm.bitmap.width == 4096) and
					
					(bm.bitmap.height == 1 or
					bm.bitmap.height == 2 or
					bm.bitmap.height == 4 or
					bm.bitmap.height == 8 or
					bm.bitmap.height == 16 or
					bm.bitmap.height == 32 or
					bm.bitmap.height == 64 or
					bm.bitmap.height == 128 or
					bm.bitmap.height == 256 or
					bm.bitmap.height == 512 or
					bm.bitmap.height == 1024 or
					bm.bitmap.height == 2048 or
					bm.bitmap.height == 4096)
				)
				then
				(
					if(	((bm.bitmap.width > bm.bitmap.height) and  (bm.bitmap.width / bm.bitmap.height) == 2) or
						((bm.bitmap.width < bm.bitmap.height) and (bm.bitmap.height / bm.bitmap.width) == 2) )then
					(
						format "'%' 是2的N次方,长宽比例为1:2或2:1\n" mapfileN
						counterk += 1
					)
					else if( ((bm.bitmap.width > bm.bitmap.height) and (bm.bitmap.width / bm.bitmap.height) > 2) or
							  ((bm.bitmap.width < bm.bitmap.height) and (bm.bitmap.height / bm.bitmap.width) > 2) )then
					(
						format "'%' 是2的N次方,但长宽比例为1:x或x:1(x>2)\n" mapfileN
						append g_arr_errMapFiles mapfileN
						
						-- 仅仅列表中显示信息
						fname = filenameFromPath mapfileN
						append g_arr_not2Square (fname +" - (x:1/1:x)")
						rl_CheckMap2Square.mlist_errMapFiles.items = g_arr_not2Square
						
						counterl += 1
					)
				)
				else
				(
					--messageBox "None square Texture has been found\n"
					format "\t'%' 不是2的N次方\n" mapfileN
					append g_arr_errMapFiles mapfileN
					
					-- 仅仅列表中显示信息
					fname = filenameFromPath mapfileN
					append g_arr_not2Square (fname +" - (非2幂次)")
					rl_CheckMap2Square.mlist_errMapFiles.items = g_arr_not2Square
					
					counteri += 1
				)
			)
		)
		catch()
	)
	
	-- auto
	Global g_arr_no2SquareFiles = #()
	Global g_map_ounts = 0
	function Fn_autoSetCloser2SquareMap map =
	(
		local l_arr_size = #(32., 64., 128., 256., 512., 1024., 2048., 4096.)
-- 		struct s_width(SubWidth, Thenumber, TheWidth)
-- 		local l_arr_subtract = #()
--		try
		(
			if getFilenameType map == "" then return 0
			local mapfileN = map as name
			local bm = BitmapTexture fileName:map
			
			if bm.bitmap != undefined then
			(
				local _width = bm.bitmap.width, _height = bm.bitmap.height
				
				b_flagw = Fn_isDivided _width
				b_flagh = Fn_isDivided _height
				if b_flagw == true and b_flagh == true then return 0

				if b_flagw == false then
				(
					if _width > 4096. then
					(
						_width = 4096.
					)
					else if(_width > 2048. and _width < 4096.)then
					(
						if( abs(_width-2048.) > abs(_width-4096.) )then
						(
							_width = 4096.
						)
						else
						(
							_width = 2048.
						)
					)
					else if(_width > 1024. and _width < 2048.)then
					(
						if( abs(_width-1024.) > abs(_width-2048.) )then
						(
							_width = 2048.
						)
						else
						(
							_width = 1024.
						)
					)
					else if(_width > 512. and _width < 1024.)then
					(
						if( abs(_width-512.) > abs(_width-1024.) )then
						(
							_width = 1024.
						)
						else
						(
							_width = 512.
						)
					)
					else if(_width > 256. and _width < 512.)then
					(
						if( abs(_width-256.) > abs(_width-512.) )then
						(
							_width = 512.
						)
						else
						(
							_width = 256.
						)
					)
					else if(_width > 128. and _width < 256.)then
					(
						if( abs(_width-128.) > abs(_width-256.) )then
						(
							_width = 256.
						)
						else
						(
							_width = 128.
						)
					)
					else if(_width > 64. and _width < 128.)then
					(
						if( abs(_width-64.) > abs(_width-128.) )then
						(
							_width = 128.
						)
						else
						(
							_width = 64.
						)
					)
					else if(_width > 32. and _width < 64.)then
					(
						if( abs(_width-32) > abs(_width-64.) )then
						(
							_width = 64.
						)
						else
						(
							_width = 32.
						)
					)
					
				) -- end if w false
						
				if b_flagh == false then
				(
					if _height > 4096. then
					(
						_height = 4096.
					)
					else if(_height > 2048. and _height < 4096.)then
					(
						if( abs(_height-2048.) > abs(_height-4096.) )then
						(
							_height = 4096.
						)
						else
						(
							_height = 2048.
						)
					)
					else if(_height > 1024. and _height < 2048.)then
					(
						if( abs(_height-1024.) > abs(_height-2048.) )then
						(
							_height = 2048.
						)
						else
						(
							_height = 1024.
						)
					)
					else if(_height > 512. and _height < 1024.)then
					(
						if( abs(_height-512.) > abs(_height-1024.) )then
						(
							_height = 1024.
						)
						else
						(
							_height = 512.
						)
					)
					else if(_height > 256. and _height < 512.)then
					(
						if( abs(_height-256.) > abs(_height-512.) )then
						(
							_height = 512.
						)
						else
						(
							_height = 256.
						)
					)
					else if(_height > 128. and _height < 256.)then
					(
						if( abs(_height-128.) > abs(_height-256.) )then
						(
							_height = 256.
						)
						else
						(
							_height = 128.
						)
					)
					else if(_height > 64. and _height < 128.)then
					(
						if( abs(_height-64.) > abs(_height-128.) )then
						(
							_height = 128.
						)
						else
						(
							_height = 64.
						)
					)
					else if(_height > 32. and _height < 64.)then
					(
						if( abs(_height-32) > abs(_height-64.) )then
						(
							_height = 64.
						)
						else
						(
							_height = 32.
						)
					)
					
				) -- end if h false
					
-- 					for i = l_arr_size.count to 1 by -1 do
-- 					(
-- 						w = s_width SubWidth:(abs( _width - l_arr_size[i] )) Thenumber:i TheWidth:l_arr_size[i]
-- 						append l_arr_subtract w
-- 					)
-- 					local minw = 5000.
-- 					for j = 1 to l_arr_subtract.count do
-- 					(
-- 						
-- 						if minw > l_arr_subtract[j].SubWidth then
-- 						(
-- 							minw = l_arr_subtract[j].SubWidth
-- 							
-- 							closew = l_arr_subtract[j].TheWidth
-- 						)
-- 						else
-- 						(
-- 							continue
-- 						)
-- 					)
				--print ("_width:" + _width as string + "  "+"_height:"+_height as string)
				--print mapfileN as string
				append g_arr_no2SquareFiles mapfileN
				format "\t%. 修正非2幂次的贴图: %\n" (g_map_ounts += 1) mapfileN
				srcImage = openBitMap mapfileN -- 贴图的完整路径
				outImage = bitmap _width _height
				copy srcImage outImage
				outImage.fileName = mapfileN
				JPEG.setQuality 99
				save outImage
			) -- end if 
		)
--		catch(throw())
	)
----------------------------------------------------------------------------------------------------
-- ui
struct ObjStruct (node, objName)

Global g_arr_errObjName = #()
Global g_arr_goodObjName = #()
Global g_arr_allObjName = #()
	
Global g_arr_errMatName = #()
Global	g_arr_goodMatName = #()
Global g_arr_allMatName = #()
Global g_arr_useObj = #()
try(DestroyDialog rl_CheckName;DestroyDialog rl_MatListBox;DestroyDialog rl_objlistBox)catch()
Rollout rl_CheckName "GY_命名检查" --width:200
(
	Group "物体名检查"
	(
		Button btn_checkObjName "检查物体名" --align:#left
		
		ListBox list_ObjName "错误命名:(双击项可选择物体)"align:#left selection:1
		Label lb_objInfo "在列表中显示:" align:#left
		
		RadioButtons rad_displayObjName ""  labels:#("错误", "正确", "所有") columns:3 default:1 
		EditText edt_reNameObj "重命名:"
		Button btn_reNameObj "OK"
		Button btn_autoReNameObj "        自动命名        "across:2 align:#left
		Button btn_undo "撤消" align:#right enabled:false
	)
	Group "贴图名检查"
	(
		label lb_x022 "确保物体名正确后自动命名贴图"
		Button btn_checkMatName "检查贴图名"
		ListBox list_MatName "错误命名:"align:#left 
		RadioButtons rad_displayMatName ""  labels:#("错误", "正确", "所有") columns:3 default:1 
		EditText edt_reNameMat "重命名:"
		Button btn_reNameMat "OK"
		
		groupBox gp_mat "" width:170 height:34 pos:[8, 620]
		Button btn_autoReNameMat "                自动命名                "pos:[14, 630] --enabled:false
	)
-- //////////////////////////////////////////////////////////////////////////////////////////////////
	on btn_checkObjName pressed do
	(
		g_arr_allObjName = #()
		g_arr_errObjName = #()
		g_arr_goodObjName = #()
		g_arr_useObj = #()
		g_arr_goodObjName = #()

		list_ObjName.items = #()
		for obj in Objects do
		(
			if(isGroupHead obj or isGroupMember obj or SuperClassof obj == GeometryClass)then
			(
				append g_arr_useObj obj
				bool_code = Fn_iscodeID obj.name
				bool_manager = Fn_isManagerID obj.name
				bool_work = Fn_isWorkID obj.name
				bool_model = Fn_isModelClass obj.name
				bool_level = Fn_isLevelID obj.name
				
				--print (bool_code as string +"_"+ bool_manager as string+"_"+bool_work as string+"_"+bool_model as string+"_"+bool_level as string)
				
				if( (bool_code == false) or (bool_manager == false)  or (bool_work == false)  or (bool_model == false) or (bool_level == false) )then
				(
					objStr = ObjStruct node:obj objName:obj.name
				
					append g_arr_errObjName objStr.objName
					list_ObjName.items = g_arr_errObjName
				)
				else --if (bool_code == bool_manager == bool_work == bool_model == bool_level == true ) then
				(
					goodObj = ObjStruct node:obj objName:obj.name
					append g_arr_goodObjName goodObj.objName
				--	print ("....." + g_arr_goodObjName as string)
				)
				append g_arr_allObjName obj.name
			)
			
		)
		
		if g_arr_errObjName.count == 0 then
		(
			MessageBox "全部命名正确!" title:"贵阳_命名检查"
		)
	) -- 检查物体名 按钮
	
	-- 显示错误名 和全部名
	on rad_displayObjName changed state do
	(
		case rad_displayObjName.state of 
		(
			1:
			(
				list_ObjName.items = #()
				list_ObjName.items = g_arr_errObjName
			)
			2:
			(
				list_ObjName.items = #()
				list_ObjName.items = g_arr_goodObjName
			)
			3:
			(
				list_ObjName.items = #()
				list_ObjName.items = for obj in Objects collect obj.name
				
			)

		)
	)
	
	-- 双击项选择物体
	on list_ObjName doubleClicked item do
	(
		local item_name = ""
		local f_name = ""
		
		if list_ObjName.items.count == 0 do return 0
			
		item_name = list_ObjName.items[item]--得到双击项名
		for obj in Objects do
		(
			if item_name == obj.name then
			(
				select obj
				actionMan.executeAction 0 "311"
				max zoomext sel all
			)
		)
	)
	
	
	on list_ObjName selected item do  
	(
		local _name = ""
		try
		(
			_name = list_ObjName.items[item]
		)catch()

		edt_reNameObj.text = _name
	)
	
	on btn_reNameObj pressed do
	(
		selItemIndex = list_ObjName.selection
		selName = list_ObjName.items[selItemIndex]

		newObjName = edt_reNameObj.text
		
		for obj in Objects do
		(
			if selName == obj.name then
			(
				select obj
				selection[1].name = newObjName
				case rad_displayObjName.state of 
				(
					1:
					(
						list_ObjName.items = #()
						g_arr_errObjName[selItemIndex] = newObjName
						list_ObjName.items = g_arr_errObjName
					)
					2:
					(
						list_ObjName.items = #()
						g_arr_goodObjName[selItemIndex] = newObjName
						list_ObjName.items = g_arr_goodObjName
					)
					3:
					(
						list_ObjName.items = #()
						g_arr_allObjName[selItemIndex] = newObjName
						list_ObjName.items = g_arr_allObjName
					)
				)
			)
		)
		
	)
	
	-- 自动物体命名
	on btn_autoReNameObj pressed do
	(
		local l_arr_newNameObj = #()
		local i = 0
		undo on
		(
			titleName =  substring MaxFileName 1 (MaxFileName.count-4)
			if queryBox "注意:\n自动命名物体是以max文件名为前缀命名的,你确定要自动命名吗?" title:"警告" beep:true then
			(
				--for _name in g_arr_errObjName do
				(
					for obj in g_arr_useObj do
					(
						i +=1
						newName = titleName + "_jz01" + (Fn_formatNumStr i 3)
						--if obj.name == _name then
						(
							obj.name = newName
							append l_arr_newNameObj newName
						)
					)
				)
				btn_undo.enabled = true
				list_ObjName.items =  l_arr_newNameObj
				MessageBox "完成!" title:"贵阳_命名检查"
			)
		)
	)
	
	on btn_undo pressed do
	(
		max undo
		list_ObjName.items = g_arr_errObjName
		btn_undo.enabled = false
	)
	
---/////////////////////////////////////////////////////////////////////////////////////////////////////////////
-- 贴图检查
	Global g_arr_selObj = #()
	on btn_checkMatName pressed do
	(
		g_arr_selObj = #()
		g_arr_allMatName = #()
		g_arr_errMatName = #()
		g_arr_goodMatName = #()
		list_MatName.items = #()
		g_arr_selObj = selection as array
		if g_arr_selObj.count == 0 then
		(
			Messagebox "请选择物体!" title:"贵阳_命名检查"
			return 0
			
		)
		for obj in g_arr_selObj do
		(
			if isGroupHead obj then
			(
				format "物体: % 是一个组.略过...\n" obj.name
				continue
			)
			mat = obj.Material
			if mat == undefined then
			(
				Messagebox ("选择的物体: " + obj.name + " 没有材质!") title:"贵阳_命名检查"
				format "无材质的物体: %\n" obj.name
				continue
			)
		try
		(
			if ( classof mat == Multimaterial )then
			(
				local tmp_name
				for i = 1 to mat.numsubs do
				(
					filename = mat[i].DiffuseMap.filename
					_name = getFilenameFile filename
					if _name == undefined then
					(
						format "未知的命名: %\n" _name as string
						continue
					)
					if _name == "" then
					(
						Messagebox "存在空名材质,已用 物体名+ \"_Empty_序号\" 标记!" title:"贵阳_命名检查"
					
						tmp_name = obj.name + "_Empty_" + i as string
						append g_arr_errMatName tmp_name
						append g_arr_allMatName tmp_name
						--continue
					)
					bool_code = Fn_iscodeID _name
					bool_manager = Fn_isManagerID _name
					bool_work = Fn_isWorkID _name
					bool_model = Fn_isModelClass _name
					bool_level = Fn_isLevelID _name
					if bool_code == false or bool_manager == false or bool_work == false or bool_model == false or bool_level == false then
					(
						append g_arr_errMatName _name
						list_MatName.items = g_arr_errMatName
					)
					else
					(
						append g_arr_goodMatName _name
					)
					append g_arr_allMatName _name 
				)
			)
			if(classof mat == StandardMaterial) then
			(
				filename = mat.DiffuseMap.filename
				_name = getFilenameFile filename
				
				bool_code = Fn_iscodeID _name
				bool_manager = Fn_isManagerID _name
				bool_work = Fn_isWorkID _name
				bool_model = Fn_isModelClass _name
				bool_level = Fn_isLevelID _name
				if bool_code == false or bool_manager == false or bool_work == false or bool_model == false or bool_level == false then
				(
					append g_arr_errMatName _name
					list_MatName.items = g_arr_errMatName
					
				)
				else
				(
					append g_arr_goodMatName _name
				)
				append g_arr_allMatName _name
			)
			
		)catch(/*throw()*/)
		) -- for
		if g_arr_errMatName.count == 0 then
		(
			MessageBox "全部命名正确!" title:"贵阳_命名检查"
		)
	)
	
	on list_MatName selected item do  
	(
		local _name = ""
		if item == 0 then return 0
		_name = list_MatName.items[item]
		edt_reNameMat.text = _name
	)

	on btn_reNameMat pressed do
	(
		selItemIndex = list_MatName.selection
		selName = list_MatName.items[selItemIndex]
		newMatName = edt_reNameMat.text
		
		if g_arr_selObj.count == 0 then
		(
			MessageBox "命名失败,请重新选择物体检查命名!"
			return 0
		)
		for obj in g_arr_selObj do
		(
			Fn_reNameMat obj newMatName selName
			Fn_HandleHardDiskFiles()
		)
		
		case rad_displayMatName.state of 
		(
			1:
			(
				list_MatName.items = #()
				g_arr_errMatName[selItemIndex] = newMatName
				list_MatName.items = g_arr_errMatName
			)
			2:
			(
				list_MatName.items = #()
				g_arr_goodMatName[selItemIndex] = newMatName
				list_MatName.items = g_arr_goodMatName
			)
			3:
			(
				list_MatName.items = #()
				g_arr_allMatName[selItemIndex] = newMatName
				list_MatName.items = g_arr_allMatName
			)
		)
		MessageBox "完成!" title:"贵阳_命名检查"
	)
	
	
	on rad_displayMatName changed state do
	(
		case rad_displayMatName.state of 
		(
			1:
			(
				list_MatName.items = #()
				list_MatName.items = g_arr_errMatName
			)
			2:
			(
				list_MatName.items = #()
				list_MatName.items = g_arr_goodMatName
			)
			3:
			(
				list_MatName.items = #()
				list_MatName.items = g_arr_allMatName
			)

		)
	)
	
-----------------自动材质命名
	on btn_autoReNameMat pressed do
	(
		local all_Files = #()
		if Objects.count == 0 then 
		(
			Messagebox "场景中没有物体!"
			return 0
		)
		if queryBox "注意:\n确定场景中所有物体名都按要求正确命名并且名字不出现重复吗?" title:"警告" beep:true then
		(
			if maxfilepath == undefined or maxfilepath == "" then 
			(
				return 0
			)
			jpg_Files = Fn_getBitmapFiles maxfilepath "*.jpg"
			tga_Files = Fn_getBitmapFiles maxfilepath "*.tga"
			all_Files = jpg_Files + tga_Files
			print ("all_Files: " + all_Files as string)
			if all_Files.count == 0 then 
			(
				Messagebox "max目录下找不到贴图文件!"
				return 0
			)
			Fn_findOutShareMat all_Files
			if Objects.count == 0 then 
			for obj in Objects do 
			(
				Fn_AutoRenameMat obj
			)
			MessageBox "自动重命名完成!" title:"贵阳_命名检查"
		)
		
	)
)-- CreateDialog rl_CheckName


-- /////////////////////////////////////////////////////////////////////////////////////////////////////////
Rollout rl_CheckMap2Square "检查贴图幂次方"
(
	Global g_arr_radLabels = #("32x32", "64x64", "128x128", "256x256", "512x512", "1024x1024", "2048x2048", "4096x4096")
	Group "检查贴图2的N次方"
	(
		button btn_checkMat2N "检查场景2次幂贴图" tooltip:"检查是否存在非2的N次幂的贴图"
		MultiListBox mlist_errMapFiles "非2次幂或比例较大的贴图:" width:170
	)	
	Group "设置非2幂次贴图大小"
	(	
		RadioButtons rad_custom "" labels:#("自定义 ")align:#left default:0
		Spinner spn_width "width" align:#left across:2 range:[32,4096,1] type:#integer enabled:false
		Spinner spn_height "height" align:#left range:[32,4096,1] type:#integer enabled:false
		RadioButtons rad_wdAndHg "" labels:g_arr_radLabels columns:2 default:1  enabled:false
		RadioButtons rad_auto "" labels:#("最接近2次幂  ") align:#left default:1
		Button btn_setMapSize "修正非2次幂贴图大小"  tooltip:"修正检查后出现的非2的N次幂的贴图"
	)
	
	-- 检查2的次方
	on btn_checkMat2N pressed  do
	(
		--Fn_printTheInfo()
		g_arr_errMapFiles = #()
		mlist_errMapFiles.items = #()
		mapfiles = #()
		g_arr_not2Square = #()
		counteri = 0
		counterj = 0
		counterk = 0
		counterl = 0
		
		progressStart "正在检查场景贴图..."
		progressUpdate 10
		
		format "正在检查场景不是2的N次方的贴图...\n\n"

		Fn_resetMaterials()
		enumerateFiles Fn_addMap
		
		progressUpdate 100
		progressEnd()
		
		format "\n已被检查出来的贴图情况如下:\n\n"
		format "是2的N次方的贴图有 '%' 个--\n" (counterj+counterk+counterl)
			format "\t2的N次方包含有 '%' 个是1:1的比例\n"  counterj
			format "\t2的N次方包含有 '%' 个是1:2或2:1的比例\n"  counterk
			format "\t2的N次方包含有 '%' 个是1:x或x:1的比例(x>2)\n"  counterl
		format "不是2的N次方的贴图--\n"
			format "\t场景中包含有 '%' 个不是2的N次方的贴图\n\n" counteri
		if counteri == 0 then
		(
			if counterl > 0 then
			(
				MessageBox ("场景所有贴图大小均符合2的N次方标准!\n但存在有 '"+ counterl as string +"' 个贴图比例较大,详细情况查看 '侦听器'输出的内容.") title:"贵阳_命名检查"
			)
			else
			(
				MessageBox "场景所有贴图大小均符合2的N次方标准!\t" title:"贵阳_命名检查"
			)
		)
		else
		(
			MessageBox ("场景存在有 '"+ counteri as string +"' 个不是2的N次方标准的贴图!\n详细情况查看 '侦听器'输出的内容.") title:"贵阳_命名检查"
		)
		
		btn_setMapSize.enabled = true
	)
	
-- ////////////////////////////////////
	
	on rad_custom changed state do
	(
		if rad_custom.state == 1 then
		(
			spn_width.enabled = true
			spn_height.enabled = true
			rad_wdAndHg.enabled = true
			rad_auto.state = 0
			if g_arr_errMapFiles.count != 0 then
			(
				btn_setMapSize.enabled = true
				btn_setMapSize.text = "按设定值修正检查出来的贴图"
			)
			else
			(
				btn_setMapSize.text = "先检查是否存在非2次幂的贴图"
				btn_setMapSize.enabled = false
			)
		)
	)    
	on rad_auto changed state do
	(
		if rad_auto.state == 1 then
		(
			spn_width.enabled = false
			spn_height.enabled = false
			rad_wdAndHg.enabled = false
			rad_custom.state = 0
			btn_setMapSize.enabled = true
			
			btn_setMapSize.text = "按最接近2次幂修正贴图"
		)
	)
	on rad_wdAndHg changed state do
	(
		case rad_wdAndHg.state of
		(
			1:(spn_width.value = 32;spn_height.value = 32)
			2:(spn_width.value = 64;spn_height.value = 64)
			3:(spn_width.value = 128;spn_height.value =128)
			4:(spn_width.value = 256;spn_height.value = 256)
			5:(spn_width.value = 512;spn_height.value = 512)
			6:(spn_width.value = 1024;spn_height.value = 1024)
			7:(spn_width.value = 2048;spn_height.value = 2048)
			8:(spn_width.value = 4096;spn_height.value = 4096)	
		)
	)
	
	
	-- ///////////////////////////////////////////////////
	Function Fn_setMapSize2Squaue map width height =
	(
		sourceImage = openBitMap map -- 贴图的完整路径
		outputImage = bitmap width height
		copy  sourceImage outputImage 
		outputImage.fileName = map
		JPEG.setQuality 99
		save outputImage
	)
	
	on btn_setMapSize pressed do
	(
		if rad_custom.state == 1 then
		(
			for mapfile in g_arr_errMapFiles do
			(
				Fn_setMapSize2Squaue mapfile spn_width.value spn_height.value
			)
			MessageBox "修正完成!" title:"贵阳_命名检查"
			btn_setMapSize.enabled = false
		)
		if rad_auto.state == 1 then
		(
			Fn_resetMaterials()
			enumerateFiles Fn_autoSetCloser2SquareMap
			format "全部修复完成!"
			MessageBox "修正完成"
			gc()
			freeSceneBitmaps()
			clearselection()
		)
	)
	
)

Rollout rl_About "About" 
(
	label lb_1 "贵阳建模命名检查\n"
	label lb_0x001 "①.修正贴图次幂中的bug 01 \n" align:#left
	label lb_2 "Tuby @Copyright 2012 Power By Yvi\n"
	label lb_3 "Website:" across:2 align:#left
	HyperLink hlk_site "Link here" color:Blue address:"www.macai.co.cc"
	label lb_4 "Corporation:" across:2 align:#left
	HyperLink hlk_comp "Link Tuby" color:Blue address:"www.tuby.cc"
)

try (closerolloutfloater FloatCheckName) catch () 
FloatCheckName = newrolloutfloater "贵阳_命名检查 v1.0" 200 384 --1180 300
addrollout rl_CheckName FloatCheckName rolledup:true
addrollout rl_CheckMap2Square FloatCheckName rolledup:true
addrollout rl_About FloatCheckName rolledup:true


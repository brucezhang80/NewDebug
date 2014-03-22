
-- ����ģ�ͼ�鹤������
-- ��͸����ͼ�� 01a01_jz1001_001.jpg
-- ͸����ͼ��   01a01_jz1001_001_alp.tga
-- ��άģ�������磺 01a01_jz1001

-- xxxxx_xxxxxx
-- 	��ģ˳���		001,002,....nnn
-- 	ϸ�ڲ��			1,2,3
-- 	ģ������			jz,dx,zw,xp,lq

-------��Ԫ˳���-------
-- 	��ģ��Ԫ˳���		01,02,...,nn
-- ������Ԫ˳���		a....z
-- �����������λ��		00,01,02,03,11,12,13,14,21,22,23,81
--12�ַ�



--------------------------------------------------------------------------------------------------------

--Сдת�ɴ�д
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
--��Ԫ˳���
function Fn_iscodeID strName =
(
	-- �����������λ��		00,01,02,03,11,12,13,14,21,22,23,81,82
	-- ������Ԫ˳���		a....z
	-- ��ģ��Ԫ˳���		01,02,...,nn

	local arr_codeID = #("00", "01","02", "03", "11","12", "13", "14", "21", "22", "23", "81", "82")

	local str_codeID = substring strName 1 2 -- return ������	
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
	local str_managerID = substring strName 3 1 -- return ������Ԫ "a..z"
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
	local str_workID = substring strName 4 2 -- return ��ģ��Ԫ "01..nn"
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
	local str_modelCls = substring strName 7 2-- return ģ������ jz,dx,zw,xp,lq
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
	local str_level = substring strName 9 1 --return  ϸ�ڲ��			1,2,3
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
	suffix  = substring strName (strName.count-2) 3 --�����λ��:��ģ���
	index = suffix as integer
	return index
)
-----------------------------------------------------------------------------------------------------------------
function Fn_checkMatName strName =
(
	
)


--------------------------------------------------------------------------------------------------------
--��ȡ��Ŀ¼�ļ�
--getFilesRecursive ·�� �ļ������磺"*.max"
function Fn_getFilesRecursive root filename =
(
	local all_Files = #()
	local TheAllSubFolder = #()
	
	--��ȡrootĿ¼�µ�������Ŀ¼
	TheAllSubFolder = GetDirectories (root+"/*")
	for s in TheAllSubFolder do
	(
		--��������Ŀ¼�µ���Ŀ¼append��TheAllSubFolder
		--��ʱTheAllSubFolderװ����rootĿ¼�µ�������Ŀ¼������rootĿ¼
		join TheAllSubFolder (GetDirectories (s + "/*"))
	)

	for f in TheAllSubFolder do
	(
		--��ÿ��Ŀ¼�±���
		join all_Files (getFiles (f + filename))
	)
	join all_Files (getFiles (root  + "/"+ filename))
	return all_Files
)--��������


Global g_arr_oldfilenames = #()
Global g_arr_newfilenames = #()
Global g_tilefilename = maxfilepath

-- ��������ͼ
function Fn_reNameMat obj newMatName resMatName =
(
	local arr_resFiles =#()
	--�������ȫ������
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
			-- �޸�����ͼ�ļ���������
			--old_name = mat.materialList[i].name
			old_diffname = mat.materialList[i].diffusemap.filename -- ��ͼ�ļ���
			ext_oldfilename = filenameFromPath old_diffname
			--��ȡ�ļ���,������׺
			oldMatName = getFilenameFile old_diffname
			ext = getfilenametype old_diffname
			
			if (resMatName != oldMatName) then
			(
				--print "��ƥ��,������һ������...\n"
				continue
			)

			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- ֻ��һ���ļ�����ļ���Ψһ
			--print ("arr_resFiles.count:"+arr_resFiles.count as string)
			
			if arr_resFiles.count == 0 do 
			(
				format "�Ҳ�����ͼ�ļ�: %\n" ext_oldfilename
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

			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- ֻ��һ���ļ�����ļ���Ψһ
			if arr_resFiles.count == 0 do 
			(
				format "�Ҳ�����ͼ�ļ�: %\n" ext_oldfilename
				return 0
			)
			
			oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
			append g_arr_oldfilenames oldfullpathfile
			newfullpathfile = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			append g_arr_newfilenames newfullpathfile
			
			mat.name = newMatName
			mat.diffuseMap.name = newMatName
			mat.diffuseMap.fileName = (getFilenamePath arr_resFiles[1]) + newMatName + ext
			
			--MessageBox "�ݲ����Ǳ�׼����!"
			--return 0
		)catch(print "�ݲ����Ǳ�׼����!")
	)
)

-- Ӳ���е���ͼ�ļ���
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


-- �Զ���������ͼ
-- 1.��ͼ�� = ������
-- 2.�ڳ������е���ͼ����ʹ����ͬ����ͼ(��JXGY������֮��) �Լ���Ӧ��ʹ�ø���ͼ������(��֮),�Ա�������������ͼ���ҵ���Ӧ������.
-- 3.�ѷǹ��õ���ͼ�� ������Ϊǰ׺+�������,������������ڴ�����ͼ֮ǰ���밴Ҫ��淶����.
-- 4.���õ���ͼ(��JXGY��������ͼ), ������һ������(������ͼ������)��������(��������ǰ�ȼ��������ϵ���ͼ���,�Ը���ż���������ȥ), ��������������ͼ
		-- �ٸ���ع��ø���ͼ��������(����ҳ����ø���ͼ������?)
-- 5.������tga��ʽ����ͼ.�������� "_alp"
-- 6.�������.

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
			ignore = findString strippedName "JZGY" --���ռ��� "JZGY" ����ͼ
			if(ignore == undefined)then
			(
				n = finditem tempmats strippedName
				if n == 0 then -- �Ҳ����ظ���
				(
					append  mats mat
				)
				append tempmats strippedName
			)
		)
	)
)

--�����ռ������е������Ӳ���
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

-- ·�� �ļ����� �磺"*.max"
function Fn_getBitmapFiles root ext =
(
	local 	all_Files = #()
	local TheAllSubFolder = #()
	--��ȡrootĿ¼�µ�������Ŀ¼
	TheAllSubFolder = GetDirectories (root+"*")
	for d in TheAllSubFolder do
	(
		--�ݹ�Ľ�������Ŀ¼�µ���Ŀ¼append��TheAllSubFolder
		--��ʱTheAllSubFolderװ����rootĿ¼�µ�������Ŀ¼������rootĿ¼
		join TheAllSubFolder (GetDirectories (d+"*"))
	)

	for f in TheAllSubFolder do
	(
		--��ÿ��Ŀ¼�±���
		join all_Files (getFiles (f + ext))
	)
	join all_Files (getFiles (root + ext))
	return all_Files 
)--��������
	
Global g_arr_shareMat = #() -- ���õ���ͼ
Global g_arr_noshareMat = #() -- �����õ���ͼ
Global g_arr_allOldMat = #() -- ���е�ԭʼ��ͼ
struct s_shareOM(node, matName, path, ext, counts)

function Fn_findOutShareMat arrBitmapFiles = 
(
	--�ҳ����õ�
	for ifile in arrBitmapFiles do -- ��Ӳ�����ռ�����ͼ�ļ��б���
	(
		local l_shareMat = #() -- ��ͬ���ʵ���ͼ
		local l_noshareMat = #() -- ����ͬ���ʵ���ͼ
		local matCounts = 0 -- ��¼������ͼ�ĸ���
		
		strippedName = filenameFromPath ifile
		simpleName = getFilenameFile ifile
		ext = MytoLower (getfilenametype ifile)
		
		ignore = findString strippedName "JZGY" --���ռ��� "JZGY" ����ͼ
		if(ignore == undefined)then
		(
			-- �ڳ����б�������������ʹ�ø���ͼ�ļ��Ĳ�����
			for obj in Objects do
			(
				local mat = obj.Material
				if mat != undefined do
				(
					if (classof mat == Multimaterial) then -- ����Ƕ�ά����
					(
						
						for i = 1 to mat.numsubs do
						(
							if mat.materialList[i].diffuseMap==undefined then continue
							old_filepathname = mat.materialList[i].diffusemap.filename -- ��������ͼ�ļ���
							ext_oldfilename = filenameFromPath old_filepathname
							oldpath = getFilenamePath old_filepathname
							oldMatName = getFilenameFile old_filepathname
							ext = MytoLower (getfilenametype old_filepathname)
							
							-- ����ҵ��Ĳ��������ļ�����ͬ��
							if (strippedName == ext_oldfilename) then
							(
								matCounts += 1
								
								-- �����������
								share = s_shareOM node:obj matName:old_filepathname path:oldpath ext:ext counts:matCounts
								append l_shareMat share
							)
						) -- for numsubs end
					)
				)-- if Multimateria end
			) -- for in all obj end
			
			if(matCounts > 1)Then -- ���õ���ͼ
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
			old_dufsename = mat.materialList[i].diffusemap.filename -- ��ͼ�ļ���
			ext_oldfilename = filenameFromPath old_dufsename
			--��ȡ�ļ���,������׺
			oldMatName = getFilenameFile old_dufsename
			ext =MytoLower(getfilenametype old_dufsename)
			
			-- �����ǹ��õ���ͼ
			-- �ڹ�����ͼ����������,�Ƿ����
			-- ���������,��ô���Ƿǹ��õ�
			local index = 0
			for j = 1 to g_arr_shareMat.count do 
			(
				for k = 1 to g_arr_shareMat[k].count do 
				(
					index = finditem g_arr_shareMat[j][k].matName old_dufsename
				)
			)
			if index == 0 then -- û�ҵ�,�ǹ���
			(
				l_noshare_count += 1
				arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- ֻ��һ���ļ�����ļ���Ψһ
				--print ("arr_resFiles.count:"+arr_resFiles.count as string)
				if arr_resFiles.count == 0 do 
				(
					format "�Ҳ�����ͼ�ļ�: %\n" ext_oldfilename
					continue
				)
				
				oldfullpathfile = (getFilenamePath arr_resFiles[1]) + (filenameFromPath arr_resFiles[1])
				
				-- �����ͼ�ļ���tga��ʽ ,������һ��͸����ͼͨ����.��ʱ����Ҫ���� "_ALP"
				-- �淶������
				-- �����µ�����
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
				
				-- ��ʼ��������ͼ
				reNameFile oldfullpathfile newfullpathfile
				mat.materialList[i].name = newMatName
				mat.materialList[i].diffuseMap.name = newMatName
				mat.materialList[i].diffuseMap.fileName = newfullpathfile
			)
			try
			(
				-- ͸����ͼ��Ҫ�� _ALP
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
		
		-- �ڹ�����ͼ�������ҳ���Ӧ������
		-- �ڶ�Ӧ������������������ͼ ------------
		for m = 1 to g_arr_shareMat.count do 
		(
			for n = 1 to g_arr_shareMat[m] do 
			(
				obj_index = finditem (g_arr_shareMat[m][n].node).name obj.name
				if (obj_index != 0) then --�ҵ�
				(
					for p = 1 to  mat.numsubs do 
					(
						old_filepathname = mat.materialList[p].diffusemap.filename -- ��ͼ�ļ���
						ext_oldfilename = filenameFromPath old_dufsename
						--��ȡ�ļ���,������׺
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
			
			arr_resFiles = Fn_getFilesRecursive g_tilefilename ext_oldfilename -- ֻ��һ���ļ�����ļ���Ψһ
			if arr_resFiles.count == 0 do 
			(
				format "�Ҳ�����ͼ�ļ�: %\n" ext_oldfilename
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
			
			--MessageBox "�ݲ����Ǳ�׼����!"
			--return 0
		)catch(print "�ݲ����Ǳ�׼����!")
	)
)

-------------------------------------------------------------------------------------------------
	
	-- //////////////////////////////////////�����ͼ2�Ĵη�
	Global counteri, counterj, counterk, counterl
	Global g_arr_errMapFiles = #(),mapfiles = #(),g_arr_not2Square = #()
	
	fn Fn_isDivided num = 
	(	--�ж��ܷ�2����
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
					format "'%' ��2��N�η�,��������Ϊ 1:1\n" mapfileN
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
						format "'%' ��2��N�η�,��������Ϊ1:2��2:1\n" mapfileN
						counterk += 1
					)
					else if( ((bm.bitmap.width > bm.bitmap.height) and (bm.bitmap.width / bm.bitmap.height) > 2) or
							  ((bm.bitmap.width < bm.bitmap.height) and (bm.bitmap.height / bm.bitmap.width) > 2) )then
					(
						format "'%' ��2��N�η�,����������Ϊ1:x��x:1(x>2)\n" mapfileN
						append g_arr_errMapFiles mapfileN
						
						-- �����б�����ʾ��Ϣ
						fname = filenameFromPath mapfileN
						append g_arr_not2Square (fname +" - (x:1/1:x)")
						rl_CheckMap2Square.mlist_errMapFiles.items = g_arr_not2Square
						
						counterl += 1
					)
				)
				else
				(
					--messageBox "None square Texture has been found\n"
					format "\t'%' ����2��N�η�\n" mapfileN
					append g_arr_errMapFiles mapfileN
					
					-- �����б�����ʾ��Ϣ
					fname = filenameFromPath mapfileN
					append g_arr_not2Square (fname +" - (��2�ݴ�)")
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
				format "\t%. ������2�ݴε���ͼ: %\n" (g_map_ounts += 1) mapfileN
				srcImage = openBitMap mapfileN -- ��ͼ������·��
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
Rollout rl_CheckName "GY_�������" --width:200
(
	Group "���������"
	(
		Button btn_checkObjName "���������" --align:#left
		
		ListBox list_ObjName "��������:(˫�����ѡ������)"align:#left selection:1
		Label lb_objInfo "���б�����ʾ:" align:#left
		
		RadioButtons rad_displayObjName ""  labels:#("����", "��ȷ", "����") columns:3 default:1 
		EditText edt_reNameObj "������:"
		Button btn_reNameObj "OK"
		Button btn_autoReNameObj "        �Զ�����        "across:2 align:#left
		Button btn_undo "����" align:#right enabled:false
	)
	Group "��ͼ�����"
	(
		label lb_x022 "ȷ����������ȷ���Զ�������ͼ"
		Button btn_checkMatName "�����ͼ��"
		ListBox list_MatName "��������:"align:#left 
		RadioButtons rad_displayMatName ""  labels:#("����", "��ȷ", "����") columns:3 default:1 
		EditText edt_reNameMat "������:"
		Button btn_reNameMat "OK"
		
		groupBox gp_mat "" width:170 height:34 pos:[8, 620]
		Button btn_autoReNameMat "                �Զ�����                "pos:[14, 630] --enabled:false
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
			MessageBox "ȫ��������ȷ!" title:"����_�������"
		)
	) -- ��������� ��ť
	
	-- ��ʾ������ ��ȫ����
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
	
	-- ˫����ѡ������
	on list_ObjName doubleClicked item do
	(
		local item_name = ""
		local f_name = ""
		
		if list_ObjName.items.count == 0 do return 0
			
		item_name = list_ObjName.items[item]--�õ�˫������
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
	
	-- �Զ���������
	on btn_autoReNameObj pressed do
	(
		local l_arr_newNameObj = #()
		local i = 0
		undo on
		(
			titleName =  substring MaxFileName 1 (MaxFileName.count-4)
			if queryBox "ע��:\n�Զ�������������max�ļ���Ϊǰ׺������,��ȷ��Ҫ�Զ�������?" title:"����" beep:true then
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
				MessageBox "���!" title:"����_�������"
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
-- ��ͼ���
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
			Messagebox "��ѡ������!" title:"����_�������"
			return 0
			
		)
		for obj in g_arr_selObj do
		(
			if isGroupHead obj then
			(
				format "����: % ��һ����.�Թ�...\n" obj.name
				continue
			)
			mat = obj.Material
			if mat == undefined then
			(
				Messagebox ("ѡ�������: " + obj.name + " û�в���!") title:"����_�������"
				format "�޲��ʵ�����: %\n" obj.name
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
						format "δ֪������: %\n" _name as string
						continue
					)
					if _name == "" then
					(
						Messagebox "���ڿ�������,���� ������+ \"_Empty_���\" ���!" title:"����_�������"
					
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
			MessageBox "ȫ��������ȷ!" title:"����_�������"
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
			MessageBox "����ʧ��,������ѡ������������!"
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
		MessageBox "���!" title:"����_�������"
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
	
-----------------�Զ���������
	on btn_autoReNameMat pressed do
	(
		local all_Files = #()
		if Objects.count == 0 then 
		(
			Messagebox "������û������!"
			return 0
		)
		if queryBox "ע��:\nȷ����������������������Ҫ����ȷ�����������ֲ������ظ���?" title:"����" beep:true then
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
				Messagebox "maxĿ¼���Ҳ�����ͼ�ļ�!"
				return 0
			)
			Fn_findOutShareMat all_Files
			if Objects.count == 0 then 
			for obj in Objects do 
			(
				Fn_AutoRenameMat obj
			)
			MessageBox "�Զ����������!" title:"����_�������"
		)
		
	)
)-- CreateDialog rl_CheckName


-- /////////////////////////////////////////////////////////////////////////////////////////////////////////
Rollout rl_CheckMap2Square "�����ͼ�ݴη�"
(
	Global g_arr_radLabels = #("32x32", "64x64", "128x128", "256x256", "512x512", "1024x1024", "2048x2048", "4096x4096")
	Group "�����ͼ2��N�η�"
	(
		button btn_checkMat2N "��鳡��2������ͼ" tooltip:"����Ƿ���ڷ�2��N���ݵ���ͼ"
		MultiListBox mlist_errMapFiles "��2���ݻ�����ϴ����ͼ:" width:170
	)	
	Group "���÷�2�ݴ���ͼ��С"
	(	
		RadioButtons rad_custom "" labels:#("�Զ��� ")align:#left default:0
		Spinner spn_width "width" align:#left across:2 range:[32,4096,1] type:#integer enabled:false
		Spinner spn_height "height" align:#left range:[32,4096,1] type:#integer enabled:false
		RadioButtons rad_wdAndHg "" labels:g_arr_radLabels columns:2 default:1  enabled:false
		RadioButtons rad_auto "" labels:#("��ӽ�2����  ") align:#left default:1
		Button btn_setMapSize "������2������ͼ��С"  tooltip:"����������ֵķ�2��N���ݵ���ͼ"
	)
	
	-- ���2�Ĵη�
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
		
		progressStart "���ڼ�鳡����ͼ..."
		progressUpdate 10
		
		format "���ڼ�鳡������2��N�η�����ͼ...\n\n"

		Fn_resetMaterials()
		enumerateFiles Fn_addMap
		
		progressUpdate 100
		progressEnd()
		
		format "\n�ѱ�����������ͼ�������:\n\n"
		format "��2��N�η�����ͼ�� '%' ��--\n" (counterj+counterk+counterl)
			format "\t2��N�η������� '%' ����1:1�ı���\n"  counterj
			format "\t2��N�η������� '%' ����1:2��2:1�ı���\n"  counterk
			format "\t2��N�η������� '%' ����1:x��x:1�ı���(x>2)\n"  counterl
		format "����2��N�η�����ͼ--\n"
			format "\t�����а����� '%' ������2��N�η�����ͼ\n\n" counteri
		if counteri == 0 then
		(
			if counterl > 0 then
			(
				MessageBox ("����������ͼ��С������2��N�η���׼!\n�������� '"+ counterl as string +"' ����ͼ�����ϴ�,��ϸ����鿴 '������'���������.") title:"����_�������"
			)
			else
			(
				MessageBox "����������ͼ��С������2��N�η���׼!\t" title:"����_�������"
			)
		)
		else
		(
			MessageBox ("���������� '"+ counteri as string +"' ������2��N�η���׼����ͼ!\n��ϸ����鿴 '������'���������.") title:"����_�������"
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
				btn_setMapSize.text = "���趨ֵ��������������ͼ"
			)
			else
			(
				btn_setMapSize.text = "�ȼ���Ƿ���ڷ�2���ݵ���ͼ"
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
			
			btn_setMapSize.text = "����ӽ�2����������ͼ"
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
		sourceImage = openBitMap map -- ��ͼ������·��
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
			MessageBox "�������!" title:"����_�������"
			btn_setMapSize.enabled = false
		)
		if rad_auto.state == 1 then
		(
			Fn_resetMaterials()
			enumerateFiles Fn_autoSetCloser2SquareMap
			format "ȫ���޸����!"
			MessageBox "�������"
			gc()
			freeSceneBitmaps()
			clearselection()
		)
	)
	
)

Rollout rl_About "About" 
(
	label lb_1 "������ģ�������\n"
	label lb_0x001 "��.������ͼ�����е�bug 01 \n" align:#left
	label lb_2 "Tuby @Copyright 2012 Power By Yvi\n"
	label lb_3 "Website:" across:2 align:#left
	HyperLink hlk_site "Link here" color:Blue address:"www.macai.co.cc"
	label lb_4 "Corporation:" across:2 align:#left
	HyperLink hlk_comp "Link Tuby" color:Blue address:"www.tuby.cc"
)

try (closerolloutfloater FloatCheckName) catch () 
FloatCheckName = newrolloutfloater "����_������� v1.0" 200 384 --1180 300
addrollout rl_CheckName FloatCheckName rolledup:true
addrollout rl_CheckMap2Square FloatCheckName rolledup:true
addrollout rl_About FloatCheckName rolledup:true

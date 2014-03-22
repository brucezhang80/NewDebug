
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

function Fn_getUnitID =
(
	
)

------------------------------------
-- clled by Fn_isWorkID
fn Fn_formatNumStr num d =
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
	if str_managerID > "a" or str_managerID < "z" then
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
	local arr_modelCls = #("jz", "dx", "zw", "xp", "lq")
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
		return false
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
	join all_Files (getFiles (root + "/" + filename))
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
		print "0000000000"
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
		)catch(print "XXXXXXXXXXXXXXX!")
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

-------------------------------------------------------------------------------------------------
	--ȥ���ظ�Ԫ��
	function Fn_removeDup a = 
	(
		for i in a.count to 1 by -1 do 
		(
			idx = findItem a a[i]
			if (idx != 0) and (idx != i) do 
				deleteItem a i
		)
		a
	)
	
	function Fn_isDivided num = 
	(	--�ж��ܷ�2����
		 while num > 2 do num = num /2.
		 if num == 2. then return  true else return false
	)
	
	function Fn_isBitmap2N sourceBMP =
	(	
		if Fn_isDivided sourceBMP.width and Fn_isDivided sourceBMP.height then return false else return true
	)
	
	function Fn_getMaps =
	(	--�ռ�������������ͼ
		local allBitmaps = getClassInstances BitmapTexture
		local existBitmaps
		--�ռ�������ͼ
		if (doesFileExist(backgroundImageFilename)) then
				append AllBitmaps backgroundImageFilename
		Fn_removeDup AllBitmaps
		existBitmaps = for m in AllBitmaps where doesFileExist m.filename collect m
		existBitmaps
	)
	
	function Fn_printTheInfo =
	(
		str = ""
		tempBMP = Fn_getMaps()
		not2Nsq = #()
		for i in tempBMP do
		( 	TM = openBitMap i.filename
			if Fn_isBitmap2N TM then append not2Nsq i
		)
		if not2Nsq.count != 0 then
		(
			str += "\n������ͼ��С������2��N�η���׼��\n\n"
			for m in not2Nsq do str += m.filename + "\n"
			resultWindow = newScript()
			print str to:resultWindow 
		)
		else messagebox "����������ͼ��С������2��N�η���׼��"
	)
----------------------------------------------------------------------------------------------------
-- ui

Global rl_objListBox
Global rl_MatListBox
Global g_arr_errObjName = #()
Global g_arr_allObjName = #()
Global g_arr_errMatName = #()
Global g_arr_allMatName = #()
-- ����
try(DestroyDialog rl_objListBox)catch()
Rollout rl_objListBox "error name"
(
	Label lb_objInfo "(˫�����ѡ������):" align:#left
	ListBox mlb_errorInfo "��������:" height:10
	RadioButtons rad_displayName ""  labels:#("��ʾ������", "��ʾ������") columns:2 --default:1 
	EditText edt_reName "������:"
	Button btn_reNameIt "OK"
	
	
	on rad_displayName changed state do
	(
		case rad_displayName.state of 
		(
			1:
			(
				mlb_errorInfo.items = g_arr_errObjName
			)

			2:
			(
				mlb_errorInfo.items = g_arr_allObjName
			)
		)
	)
	
	on mlb_errorInfo doubleClicked item do
	(
		local item_name = ""
		local f_name = ""
		
		if mlb_errorInfo.items.count == 0 do return 0
			
		item_name = mlb_errorInfo.items[item]--�õ�˫������
		for obj in geometry do
		(
			if item_name == obj.name then
			(
				select obj
				actionMan.executeAction 0 "311"
				max zoomext sel all
			)
		)
	)
	
	on mlb_errorInfo selected item do  
	(
		local _name = ""
		_name = mlb_errorInfo.items[item]
		edt_reName.text = _name
	)

	on btn_reNameIt pressed do
	(
		selItemIndex = mlb_errorInfo.selection
		--if selItem > 1 then return 0
		selName = mlb_errorInfo.items[selItemIndex]
		print selName
		newObjName = edt_reName.text
		
		for obj in geometry do
		(
			if selName == obj.name then
			(
				select obj
				selection[1].name = newObjName
				g_arr_errName[selItemIndex] = newObjName
				mlb_errorInfo.items = g_arr_errName
			)
		)
		
	)
)

--����
try(DestroyDialog rl_MatListBox)catch()
Rollout rl_MatListBox "error mat name" width:170
(
	--Label lb_objInfo "(˫�����ѡ������):" align:#left
	ListBox mlb_errorInfo "��������:" height:12 
	EditText edt_reName "������:"
	Button btn_reNameIt "OK"
	
	on mlb_errorInfo doubleClicked item do
	(

	)
	
	on mlb_errorInfo selected item do  
	(
		local _name = ""
		_name = mlb_errorInfo.items[item]
		edt_reName.text = _name
	)

	on btn_reNameIt pressed do
	(
		selItemIndex = mlb_errorInfo.selection
		--if selItem > 1 then return 0
		selName = mlb_errorInfo.items[selItemIndex]
		newMatName = edt_reName.text
		
		local selObj = selection as array
		if selObj.count == 0 then 
		(
			Messagebox "��ѡ��һ�����ϵ�����!"
			return 0
		)
		for obj in selObj do
		(
			Fn_reNameMat obj newMatName selName
			Fn_HandleHardDiskFiles()
		)
		g_arr_errMatName[selItemIndex] = newMatName
-- 		DestroyDialog rl_MatListBox
-- 		aPos =  GetDialogpos rl_CheckName
-- 		createdialog rl_MatListBox pos:(aPos - [176,0])
		mlb_errorInfo.items = g_arr_errMatName
	)
)

try(DestroyDialog rl_CheckName;DestroyDialog rl_MatListBox;DestroyDialog rl_objlistBox)catch()
Rollout rl_CheckName "GY_�������"
(
	Group "���������"
	(
		Button btn_checkObjName "���������"
	)
	Group "��ͼ�����"
	(
		Button btn_checkMatName "�����ͼ��"
	)
	
	Group "�����ͼ�Ƿ�2��N�η�"
	(
		button btn_checkMat2N "���"
	)

	on btn_checkObjName pressed do
	(
		g_arr_allObjName = #()
		g_arr_errObjName = #()
		for obj in geometry do
		(
			bool_code = Fn_iscodeID obj.name
			bool_manager = Fn_isManagerID obj.name
			bool_work = Fn_isWorkID obj.name
			bool_model = Fn_isModelClass obj.name
			bool_level = Fn_isLevelID obj.name
			
			if bool_code == false or bool_manager == false or bool_work == false or bool_model == false or bool_level == false then
			(
				aPos = GetDialogPos rl_CheckName
				CreateDialog rl_objListBox pos:[aPos.x + 168,aPos.y]
				append g_arr_errObjName obj.name
 				rl_objListBox.mlb_errorInfo.items = g_arr_errObjName
				
			)
			else
			(
				
			)
			
			append g_arr_allObjName obj.name
		)
	/*
		for i = 1 to arr_allName.count do
		(
			-- jz1001_001
			
			index = Fn_getObjectIndex arr_allName[i]
			
			if i != index then
			(
				-- ��ģ��Ų�����
			)
		)
		*/
	)
	
	
	-- ��ͼ���
	on btn_checkMatName pressed do
	(
		g_arr_allMatName = #()
		g_arr_errMatName = #()
		local sel = selection as array
		
		for obj in sel do
		(
			mat = obj.Material
			if mat == undefined then
			(
				Messagebox "ѡ�������û�в���!"
				continue
			)
				
			if ( classof mat == Multimaterial )then
			(
				for i = 1 to mat.numsubs do
				(
					filename = mat[i].DiffuseMap.filename
					_name = getFilenameFile filename
					
					bool_code = Fn_iscodeID _name
					bool_manager = Fn_isManagerID _name
					bool_work = Fn_isWorkID _name
					bool_model = Fn_isModelClass _name
					bool_level = Fn_isLevelID _name
					if bool_code == false or bool_manager == false or bool_work == false or bool_model == false or bool_level == false then
					(
						aPos = GetDialogPos rl_CheckName
						CreateDialog rl_MatListBox pos:[aPos.x - 176,aPos.y]
						append g_arr_errMatName _name
						rl_MatListBox.mlb_errorInfo.items = g_arr_errMatName
						
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
					aPos = GetDialogPos rl_CheckName
					CreateDialog rl_MatListBox pos:[aPos.x - 176,aPos.y]
					append g_arr_errMatName _name
					rl_MatListBox.mlb_errorInfo.items = g_arr_errMatName
					
				)
				append g_arr_allMatName _name
			)
			
		)
	)
	
	
	on btn_checkMat2N pressed  do
	(
		Fn_printTheInfo()
	)
-----------------------
	on rl_CheckName moved val do
	(
		if rl_objlistBox.open == true do
		(
			DestroyDialog rl_objListBox
			aPos =  GetDialogpos rl_CheckName
			createdialog rl_objListBox pos:(aPos + [168,0])
			rl_objListBox.mlb_errorInfo.items = g_arr_errName
		)
		
		if rl_MatListBox.open == true do
		(
			DestroyDialog rl_MatListBox
			aPos =  GetDialogpos rl_CheckName
			createdialog rl_MatListBox pos:(aPos - [176,0])
			rl_MatListBox.mlb_errorInfo.items = g_arr_errMatName
		)
	)
)
CreateDialog rl_CheckName


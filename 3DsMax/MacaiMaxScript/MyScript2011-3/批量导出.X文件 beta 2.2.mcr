--------------
---------function------
--����dotnet����ȡ��Ŀ¼�µ��ļ�
fn GetAllFiles TargetPath FileFilter  =
(
	local SearchOption,DotNetDirectoryInfo
	
	SearchOption = DotNetClass "System.IO.SearchOption"
	DotNetDirectoryInfo = DotNetObject "System.IO.DirectoryInfo" TargetPath
	tempUserName = (DotNetClass "System.Environment").UserName
	fileSystemRights = (DotNetClass "System.Security.AccessControl.FileSystemRights").Modify
	accessControlType = (DotNetClass "System.Security.AccessControl.AccessControlType").Allow
	fileSystemAccessRule = DotNetObject "System.Security.AccessControl.FileSystemAccessRule" tempUserName fileSystemRights accessControlType 
	directorySecurity = DotNetDirectoryInfo.GetAccessControl();
	directorySecurity.AddAccessRule fileSystemAccessRule 
	DotNetDirectoryInfo.SetAccessControl directorySecurity 
	for tfi in (DotNetDirectoryInfo.GetFiles FileFilter SearchOption.AllDirectories) collect tfi.FullName
)


--�����ı��ļ���¼����
fn writeCenter2File infile iCenter height fileName objName= 
(
	
	coordFile = openfile infile mode:"at"
-- 	if (coordFile == undefined)do
-- 	(
-- 		createFile infile
-- 		coordFile = openfile infile mode:"at"
-- 	)

	s0 = formattedPrint iCenter.x format:"19.14g" 
	s1 = formattedPrint iCenter.y format:"19.14g" 
	
	s2 = formattedPrint height format:"19.14g" 
	
	--s3 = filenameFromPath fileName 
	
	str = fileName + "-> (������)" + objName + " (X)" + s0  + " (Y)" + s1 +" (H)"+ s2
	format str to:coordFile
	format "\n" to:coordFile
	
	close coordFile
	free coordFile
)
	
fn OpenSaveFile =
(
	try
	(
		out_filename= getSaveFileName caption:"ѡ�񱣴��λ��..." initialDir:"$export" filename:"����.txt"
		createFile out_filename
	)catch("����\"����.txt\"�ļ�ʧ�ܣ�")
	return out_filename
)
	
fn HandleMax2XBySelFold name =
(
    obj_name=name
    filepath = maxfilepath
    makeDir (filepath + obj_name)--�ڵ�ǰ�����max·���½����Զ�������Ϊ�����ļ���
    --�������ļ���һ��.x�ļ���������Ϊ������Ϊ�����ļ����ڡ�
    exportfile (filepath + obj_name + "\\" + obj_name+".x") #noprompt 
)

fn HandleMax2XByAllFold name =
(
	filepath = maxfilepath
	obj_name=name
	exportfile (filepath + "\\" + obj_name+".x") #noprompt 
)
-----------------------plug Interface------- 

try(destroydialog YY_ExpotXFile)catch()
rollout YY_ExpotXFile "ȫ��������.X�ļ� beta2.2 build42" width:260 height:480
(
	label lb_1 "Max����·����" pos:[3,4] width:82 height:14
	editText edt_maxDir "" pos:[0,20] width:180 height:17
	button btn_maxFileDir "��..." pos:[182,20] width:68 height:18
	
	checkbox chk_saveCoord "�Ƿ��¼����͸߶ȣ�" pos:[100,41] width:154 height:14 triState:0--checked:true
	label lb_2 "�����¼·����" pos:[3,41] width:90 height:14
	editText edt_textDir "" pos:[0,56] width:180 height:17 enabled:false
	button btn_saveFileDir "����..." pos:[182,56] width:68 height:18 enabled:False
	
	multilistBox mlist_allMaxFile "����max�ļ�(�����ļ��У���" pos:[4,75] width:253 height:12 selection:1
	--checkbox chk_noOpacity "������͸��.x" pos:[14,281] width:114 height:22 state:true
	--checkbox chk_withOpacity "������͸��.x" pos:[130,281] width:114 height:22 state:false
	label lb_3 \
			"˵����\n�������Ҫ��������������.x�ļ�\n��1.����������Ŀ¼�µ�max�ļ���\n��2.�������ڵĵ���������󵼳�Ϊ.x�ļ�,���Ҵ�������������Ϊ�����ļ����ڡ�\n��3.����͸������͸�������彫������.x�ļ��ֱ���벻ͬ���ļ�����(������...)��\n��4.ͬʱ��¼ÿ�������X��Y��������屾��߶Ȳ�����Ϊ�ı��ļ���\n��5.���ڲ��ԡ�8.30�������ܡ�������˳�Ʒ��" \
			pos:[8,344] width:257 height:200
	
	button btn_selAll "ȫѡ" pos:[3,258] width:80 height:18
	button btn_selNone "��ѡ" pos:[187,258] width:80 height:18
	
	checkBox chk_isDirforX "Ϊÿ��.X�ļ����������ļ���"	pos:[6,280] width:254 height:14 triState:0
	label lb_forX "������ѡ����max�ļ�����)"pos:[30,296] width:254 height:14
	
	button btn_beginExpot "��ʼ����" pos:[35,318] width:196 height:28
	--------------------------values
	global mousedx = false
	global ThePos = [0,0]
	global ex_files = #()
	global outf
	--------------------------
	
----------------------------------------rollout-------------
	-------------------
    on YY_ExpotXFile LButtonDown pos do (mousedx = true; ThePos = pos)
	on YY_ExpotXFile LButtonUp pos do mousedx = false
	-------------------
	on YY_ExpotXFile mousemove pos do
	(
		if mousedx do SetDialogPos YY_ExpotXFile (mouse.screenpos - thePos)
	)
	
	on chk_saveCoord changed c do--�Ƿ���Ҫ����
	(
			if c == true then btn_saveFileDir.enabled = true else btn_saveFileDir.enabled = false
	)
	
	on YY_ExpotXFile resized size do
	(
	
	)

---------------------------------------����
---------------------------------------
	----------------------��������---
	on btn_saveFileDir pressed do--���������¼�ı��ļ�
	(
		outf = OpenSaveFile()

		print outf
		edt_textDir.text = outf as string
		
		--������ı��ļ���
		--if outf != undefined then
		--(
		--	close outf
		--	free outf
		--)
	)--�������
	
	on edt_maxDir entered text do
	(
		local ch
		local lb
		
		if edt_maxDir != undefined do
		(
			ch = findstring edt_maxDir.text "\\"
			lb = findstring edt_maxDir.text ":\\" 
			--"C:\\" or "D:\\" or "E:\\" or "F:\\" or "G:\\" or "H:\\"
			if edt_maxDir.text.count == 3 and lb != undefined do
			(
				messagebox "·������Ϊ ��Ŀ¼��"
				--edt_maxDir.text = ""
			)
		)
		print ch
		print lb
		try
		(
			if ( ((edt_maxDir.text) != undefined) and (ch != undefined) and (lb != undefined) and edt_maxDir.text.count != 3)then
			(
				if (edt_maxDir.text.count) > 28 then edt_maxDir.text = "..."+(substring edt_maxDir.text (edt_maxDir.text.count-28) edt_maxDir.text.count) 
				else edt_maxDir.text = edt_maxDir.text

				--��ȡmax�ļ���������һ����������
				ex_files = GetAllFiles edt_maxDir.text "*.max"
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
			)
-- 			else
-- 			(
-- 				messagebox "·������ȷ��"
-- 				edt_maxDir.text = ""
-- 			)
		)catch()
			--(messagebox "·������ �� û�����·����\n���飡")
	)--edt_maxDir end
	
	on btn_maxFileDir pressed  do
	(
		try
		(
			if (dir = getSavePath caption:"ѡ��max�ļ���·��������Ŀ¼��...")!= undefined  and dir.count!=3 then
			(
				if (dir.count) > 28 then edt_maxDir.text = "..."+(substring dir (dir.count-28) dir.count) 
				else edt_maxDir.text = dir
				--��ȡmax�ļ���������һ����������
				ex_files = GetAllFiles dir "*.max"
				--��max�������ռ�max�ļ����б���
				mlist_allMaxFile.items = for f in ex_files collect (filenameFromPath f)
			)
			else if dir.count == 3 do messagebox "·������Ϊ��Ŀ¼��������"
		)
		catch()
	)--btn_maxFileDir end
	

-----------	
	on btn_selAll pressed do--ȫѡ
	(
		all = #{1..(mlist_allMaxFile.items.count)}
		mlist_allMaxFile.selection = all
	)
	on btn_selNone pressed  do  --��ѡ
	(

		mlist_allMaxFile.selection = #{}
	)
------------
	
	on mlist_allMaxFile selected sel do
	(
		
	)
	
	on btn_beginExpot pressed  do--��ʼ����.x�ļ�
	(
		local toExport = #()
		--local withOp = #()
		--local nuoneOp = #()
		local singleNode = #()
		local iCenter
		local height
		
		local sel = (mlist_allMaxFile.selection as array)
		if(sel.count == 0 )then
		(
			MessageBox "û�пɵ�����max�ļ�,\n��ָ��max�ļ������ԣ�" title:"��ʾ" beep:true
		)
		else
		(
			for i = 1 to sel.count do 
				append toExport ex_files[sel[i]]
			
			--��ʼ���ص���max
			for m in toExport do
			(
				loadmaxfile m --#noPrompt--��һ��max�ļ�
				maxname = maxfilename
				
-- 				for geom in geometry do--��max�����б���ÿ������
-- 				(
-- 					nodemax = maxpath+geom.name+".max"
-- 					saveNodes geom (nodemax) quiet:true--��������ÿ������
-- 					append singleNode nodemax
-- 				)
-- 				if singleNode.count > 1 then
-- 				(
-- 					for sn in singleNode do
					(
-- 						loadmaxfile sn
-- 						g = geometry[1]
						
-- 						if chk_saveCoord.checked then
-- 						(
-- 							--��¼����
-- 							iCenter = g.center
-- 							height = g.max.z - g.min.z
-- 							
-- 							writeCenter2File outf iCenter height m g.name
-- 						)
						--��ʼ����.x�ļ�
						if chk_isDirforX.checked then--��ѡ�󵼳�ÿ��x�ļ�Ϊ�����ļ���
						(
							HandleMax2XBySelFold maxname
						)
						else--���򵼳�X�ļ�ΪMAX�ļ�����
						(
							HandleMax2XByAllFold maxname
						)
						--resetMaxFile #noPrompt--����max���
-- 						deletefile sn--ɾ��ǰ�浥������ÿ�������max
					)
-- 				)
-- 				else
-- 				(
-- 					loadmaxfile singleNode[1]
-- 					g = geometry[1]
-- 					
-- 					if chk_saveCoord.checked then
-- 					(
-- 						--��¼����
-- 						iCenter = g.center
-- 						height = g.max.z - g.min.z
-- 						
-- 						writeCenter2File outf iCenter height m g.name
-- 					)
-- 					
-- 					--��ʼ����.x�ļ�
-- 					if chk_isDirforX.checked then--��ѡ�󵼳�ÿ��x�ļ�Ϊ�����ļ���
-- 					(
-- 						HandleMax2XBySelFold g
-- 					)
-- 					else
-- 					(
-- 						HandleMax2XByAllFold g
-- 					)
-- 					--resetMaxFile #noPrompt--����max���
-- 					--deletefile sn--���max����ֻ��һ�����壬��Ҫɾ�����max
-- 				)
				resetMaxFile #noPrompt--����max���
			)
			
			MessageBox "�Ѵ�����ɣ�"
		)
		
	)
)
createDialog YY_ExpotXFile pos:mouse.screenpos

--F:\Yvi\����.X�ļ�\320802



-----------------------------------------------------
--�����ܣ��ݹ��ȡĿ¼�°�����Ŀ¼����max�ļ���
--���ߣ�Yvi.���
--��ϵ��Email 12319597@qq.com
--���ڣ�2011 - 10 - 11
-----------------------------------------------------

try(DestroyDialog getAllFiles)catch()
rollout getAllFiles "getAllFiles" width:203 height:300
(
	Button btn_OpenRoot "���..."pos:[6,10] width:65 height:20 tooltip:"�����һ������max�ļ���Ŀ¼"
	editText edt_strPath "" pos:[70,12] width:130 height:16
	
	MultilistBox lbx_AllFiles "" pos:[6,40] width:190 height:18
	
	--����
	--�ݹ��ȡ��Ŀ¼�ļ�
	--getFilesRecursive ·�� �ļ������磺"*.max"
	fn getFilesRecursive root ext =
	(
		local 	all_Files = #()
		local TheAllSubFolder = #()
		--��ȡrootĿ¼�µ�������Ŀ¼
		TheAllSubFolder = GetDirectories (root+"/*")
		for d in TheAllSubFolder do
		(
			--�ݹ�Ľ�������Ŀ¼�µ���Ŀ¼append��TheAllSubFolder
			--��ʱTheAllSubFolderװ����rootĿ¼�µ�������Ŀ¼������rootĿ¼
			join TheAllSubFolder (GetDirectories (d+"/*"))
		)
	
		for f in TheAllSubFolder do
		(
			--��ÿ��Ŀ¼�±���
			join all_Files (getFiles (f + ext))
		)
		join all_Files (getFiles (root + "/" +  ext))
		return all_Files 
	)--��������
	
	--Handle
	on btn_OpenRoot pressed do
	(
		local AllFiles = #()
		try
		(
			if (root = getSavePath caption:"ѡ��һ������MAX�ļ���Ŀ¼...") != undefined then
			(
				edt_strPath.text = root
				
				AllFiles = getFilesRecursive root "*.max"

				lbx_AllFiles.items = for f in AllFiles collect(filenameFromPath f)
			)
			else
			(
				return 0
			)
		)catch( print "Unknow System Error!")
	)

)CreateDialog getAllFiles

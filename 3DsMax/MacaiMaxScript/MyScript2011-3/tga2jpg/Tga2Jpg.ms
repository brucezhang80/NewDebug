
--���� tga �ļ�ת���� jpg
--Yvi
--2011-12-20


try(DestroyDialog)catch()
Rollout rl_tgaTojpg "TGAת����JPG"
(
	Group "ת������"
	(
		button btn_getFiles "get TGA Path"width:170 height:16
		button doIt "<<Start Convert>>" width:120 height:25
		
		label lb_1 "Now Converting... " align:#left
		progressBar pgb_Progress1 "" width:160 height:8 color:GREEN

		checkBox chb_shutdown "ת����ɺ�ػ�!!!"
	)
----------------------------------------------------------------------------------------
	
	global arrTgaFiles = #()
---------------------------------------------------------------------------------------
	--ʹ��max�ű���ת�� (���ַ���ת������jpg����)
	function FN_useMaxConv fname jpgDir= 
	(
		sourceImage = openBitMap fname
		outputImage = bitmap sourceImage.width  sourceImage.height
		copy  sourceImage outputImage
		outputImage.fileName = jpgDir + (getFilenameFile fname) + ".jpg"
		jpeg.setQuality 99 
		save outputImage
	)

	-- ʹ��osg��ת�� [�����ɾ�� Դ�ļ�(tga�ļ�)]
	function FN_useExeConv fname =
	(
		--������copyһ�ݰ�
		--������Ҫָ�� "tga2jpg.exe" ������·��
		cmd = "tga2jpg.exe"+ " "+ fname
		--DOSCommand cmd
		HiddenDOSCommand cmd
	)
	
	--get tga files
	function FN_getTgaFiles =
	(
		try
		(
			dir = getSavePath caption:"�ļ�·��:" 
			arrTgaFiles = getFiles (dir + "/*.tga")
			btn_getFiles.text = dir
		)catch(format "δ֪��ϵͳ����!")
		arrTgaFiles
	)
	-- tga 2 jpg
	function FN_tgaConvertToJpg arrTgaFiles =
	(
		local jpgDir = ""
		local i = 0
		--�����ͼ�ļ���
		if arrTgaFiles.count == undefined do return 0
		if arrTgaFiles.count != 0 do
		(
			jpgDir = (getFilenamePath arrTgaFiles[1]) + "JPG\\"
			makeDir jpgDir
			
			for f in arrTgaFiles do--��ͼΪ����һ������,���ŵ���һ��
			(
				fname = filenameFromPath f
				fpath = getFilenamePath f
				fullname = fpath + "copy_" + fname
				copyFile f fullname
				
				FN_useExeConv f
				
				--��rename
				renameFile fullname f
				pgb_Progress1.value = 100.0 * (i += 1) / arrTgaFiles.count
			)
		)
		gc()
		freeSceneBitmaps()
	)
	
	on btn_getFiles pressed do
	(
		FN_getTgaFiles()
	)
	on doIt pressed do
	(
		
		FN_tgaConvertToJpg arrTgaFiles
		arrTgaFiles = #()
		
		if chb_shutdown.checked do
		(
			cmd = "shutdown -s -t 30"
			DOSCommand cmd
			quitMAX #noPrompt
		)
	)
)
try (closerolloutfloater floattga2jpg) catch () 
floattga2jpg = newrolloutfloater "ConverttgaTojpg" 200 160 1180 300
addrollout rl_tgaTojpg floattga2jpg rolledup:false
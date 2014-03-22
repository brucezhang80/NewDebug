
--批量 tga 文件转换到 jpg
--Yvi
--2011-12-20


try(DestroyDialog)catch()
Rollout rl_tgaTojpg "TGA转换到JPG"
(
	Group "转换操作"
	(
		button btn_getFiles "get TGA Path"width:170 height:16
		button doIt "<<Start Convert>>" width:120 height:25
		
		label lb_1 "Now Converting... " align:#left
		progressBar pgb_Progress1 "" width:160 height:8 color:GREEN

		checkBox chb_shutdown "转换完成后关机!!!"
	)
----------------------------------------------------------------------------------------
	
	global arrTgaFiles = #()
---------------------------------------------------------------------------------------
	--使用max脚本来转换 (这种方法转出来的jpg变样)
	function FN_useMaxConv fname jpgDir= 
	(
		sourceImage = openBitMap fname
		outputImage = bitmap sourceImage.width  sourceImage.height
		copy  sourceImage outputImage
		outputImage.fileName = jpgDir + (getFilenameFile fname) + ".jpg"
		jpeg.setQuality 99 
		save outputImage
	)

	-- 使用osg来转换 [这个会删除 源文件(tga文件)]
	function FN_useExeConv fname =
	(
		--我们先copy一份吧
		--这里需要指定 "tga2jpg.exe" 的完整路径
		cmd = "tga2jpg.exe"+ " "+ fname
		--DOSCommand cmd
		HiddenDOSCommand cmd
	)
	
	--get tga files
	function FN_getTgaFiles =
	(
		try
		(
			dir = getSavePath caption:"文件路径:" 
			arrTgaFiles = getFiles (dir + "/*.tga")
			btn_getFiles.text = dir
		)catch(format "未知的系统错误!")
		arrTgaFiles
	)
	-- tga 2 jpg
	function FN_tgaConvertToJpg arrTgaFiles =
	(
		local jpgDir = ""
		local i = 0
		--获得贴图文件名
		if arrTgaFiles.count == undefined do return 0
		if arrTgaFiles.count != 0 do
		(
			jpgDir = (getFilenamePath arrTgaFiles[1]) + "JPG\\"
			makeDir jpgDir
			
			for f in arrTgaFiles do--贴图为四张一个数组,四张叠在一起
			(
				fname = filenameFromPath f
				fpath = getFilenamePath f
				fullname = fpath + "copy_" + fname
				copyFile f fullname
				
				FN_useExeConv f
				
				--再rename
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

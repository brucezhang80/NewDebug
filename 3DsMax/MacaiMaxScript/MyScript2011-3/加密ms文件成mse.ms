macroScript yy_msEncrypt
category:"加密ms" 
toolTip:"加密ms……" 
buttontext:"加密" 
(

	try(destroydialog rl_EncryptMsFile)catch()
	Rollout rl_EncryptMsFile "加密脚本插件" width:300
	(
		EditText edt_msfile "ms文件" text:"E:\\MaxCore\\SouthCore\\SouthTools\\" width:220 across:2 align:#left
		--Button btn_openfile "打开文件..."align:#right
		
		Button btn_enctypt "加密" align:#right
		
		global hwnd
		on btn_openfile pressed do
		(
		)
		
		on edt_msfile entered txt do
		(
			ext = getfilenametype edt_msfile.text
			filename = filenameFromPath  edt_msfile.text
			if ext == "" then
			(
				Messagebox ("请输入正确的 *.ms 文件名称")
				return 0
			)
			else
			(
				if (doesFileExist edt_msfile.text) then
				(
					encryptscript (edt_msfile.text) version:0
				)
				else
				(
					Messagebox ("输入的文件 " + filename + " 不存在!")
					return 0
				)
			)
		)
		on btn_enctypt pressed do
		(
			ext = getfilenametype edt_msfile.text
			filename = filenameFromPath  edt_msfile.text
			if ext == "" then
			(
				Messagebox ("请输入正确的 *.ms 文件名称")
				return 0
			)
			else
			(
				if (doesFileExist edt_msfile.text) then
				(
					encryptscript (edt_msfile.text) version:0
				)
				else
				(
					Messagebox ("输入的文件 " + filename + " 不存在!")
					return 0
				)
			)
		)
		
		on rl_EncryptMsFile open do 
		(
			hwnd = DialogMonitorOPS.GetWindowHandle()
			
			format "Dialog Window Handle: %\n" hwnd
			format "Dialog Name: %\n" (UIAccessor.GetWindowText hwnd)

			dragAndDrop.enableDragAndDrop hwnd on
				
			dragAndDrop.globalEnableDragAndDrop True
			apos = GetDialogPos rl_EncryptMsFile
			if dragAndDrop.isEnabled() then
			(
				--dragAndDrop.dropPackage hwnd <&point>mousePoint <&filename array>files
				--Messagebox "OO"
			)

		)
		on drop hwnd item do 
		(
			edt_msfile.text = item
		)
	)--Createdialog rl_EncryptMsFile
	
	on isChecked do
	(
		 rl_EncryptMsFile.open
	)
	on execute do createDialog rl_EncryptMsFile

	on closeDialogs do
	(
		destroydialog  rl_EncryptMsFile
	)
)
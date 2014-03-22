
rollout roEncWithPass "EncWithPassword"
(
	local msFile, tmpFile, msFiles = #()
	
	fn getFileSourceAsString file = 
	(
		f = openFile file mode:"r" ; stream = ""
		while not eof f do append stream (readLine f + "\n")
		close f ; stream -- return result
	)
	
	fn getFilesRecursive root ext =
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
			join all_Files (getFiles (f + ext))
		)
		join all_Files (getFiles (root + "/" + ext))
		return all_Files
	)
	
	
	button btnOpenFile "选择 *.ms 文件" width:120
	edittext txtAddPass "密码: " bold:true
	group "加密版本"
	(
		radioButtons rbtVers labels:#("3ds Max 2 到 9","3ds Max 9 SP1 以上")
	)
	button btnEncrypt "加密" width:120
	
	on rbtVers changed state do 
	(
		print rbtVers.state
	)
	on btnOpenFile pressed do 
	(
		msFiles = #()
-- 		file = getOpenFileName caption:"打开脚本文件..." \
-- 		types:"Script files (*.ms)|*.ms|All files (*.*)|*.*|"
-- 		if file != undefined then msFile = file else msFile = undefined
		dir = getSavePath caption:"my title" initialDir:"$scripts"
		if(dir == undefined)then
		(
			return 0
		)
		btnOpenFile.caption = dir
		--msFils = getFiles (dir + "/*.ms")
		msFiles = getFilesRecursive dir "*.ms"

	)
	on btnEncrypt pressed do 
	(
		if msFiles.count == 0 or txtAddPass.text == "" then
		(
			messageBox "请选择文件并输入加密的密码！"
			return 0
		)
		else 
		(
			for i = 1 to  msFiles.count do 
			(
				msFile = msFiles[i]
				sourceFile = getFileSourceAsString msFile
				additionFile = 
				"(
					local Password = \"" + txtAddPass.text + "\", run = false
					rollout roEnterPassword \"输入密码\"
					(
						edittext txtPassword bold:true
						button btnOK \"OK\" width:90
						on btnOK pressed do 
						(
							run = Password == txtPassword.text
							DestroyDialog roEnterPassword
						)
					)
					createDialog roEnterPassword modal:true
					
					if run then (" + sourceFile + ")else(messageBox \"密码错误\")
				)"
				----------------------------------------------
				FilePath = getFilenamePath msFile
				tmpFile = FilePath + "tmp.ms"
				createFile tmpFile
				stream = openFile tmpFile mode:"w"
				format "%\n" additionFile to:stream
				flush stream
				close stream
				encryptScript tmpFile version:(rbtVers.state-1)
				renameFile (tmpFile+"e") (msFile+"e")
				gc(); deleteFile tmpFile
			)
			messageBox "OK"
		)
	)
	
	on roEncWithPass close do 
	(
		if tmpFile != undefined do
			if doesFileExist tmpFile do 
			(
				gc(); deleteFile tmpFile
			)
	)
)
createDialog roEncWithPass

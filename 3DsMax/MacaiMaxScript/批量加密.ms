
	function Yvi_getFilesRecursive root filename =
	(
		local all_Files = #()
		local TheAllSubFolder = #()
		
		TheAllSubFolder = GetDirectories (root+"*")
		for s in TheAllSubFolder do
		(
			join TheAllSubFolder (GetDirectories (s + "*"))
		)

		for f in TheAllSubFolder do
		(
			join all_Files (getFiles (f +filename))
		)
		join all_Files (getFiles (root + filename))
		return all_Files
	)
	
	
Try(DestroyDialog rl_batchEncryptFils)catch()
rollout rl_batchEncryptFils "批量加密" width:198 height:288
(
	button btn_path "Browe..." pos:[6,8] width:187 height:35
	button btn_save "Save..." pos:[6,50] width:187 height:35
	listBox lbx_files "" pos:[1,90] width:197 height:12
	button btn_Encrypt "Encrypt It" pos:[5,260] width:185 height:35
	local save_path
	local l_msFilesArr = #()
	on btn_path pressed do
	(
		save_path = getSavePath caption:"选择ms文件的位置..."
		if(save_path == undefined)then
		(
			return 0
		)
		else
		(
			local fname = #()
			btn_path.caption = save_path
			l_msFilesArr = Yvi_getFilesRecursive save_path "\\*.ms"
			if(l_msFilesArr.count != 0)then
			(
				for f in l_msFilesArr do
				(
					APPEND fname  (filenameFromPath  f)
				)
				lbx_files.items = fname
			)
		)
	)
	
	on btn_Encrypt pressed do
	(
		counts = l_msFilesArr.count
		if(counts != 0)then
		(
			for f in l_msFilesArr do
			(
				Encryptscript f version:0
			)
			Messagebox "加密完成！" title:rl_batchEncryptFils.title
			return 0
		)
		else
		(
			Messagebox "没有可加密的文件！" title:rl_batchEncryptFils.title
			return 0
		)
	)
)
CreateDialog rl_batchEncryptFils
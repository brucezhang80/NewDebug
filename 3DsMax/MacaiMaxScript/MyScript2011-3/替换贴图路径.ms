if wando_bitmaptool !=undefined then closerolloutfloater wando_bitmaptool 
wando_bitmaptool =newrolloutfloater "bitmaptool" 172 130 730 80

global savefilepath= ""
global mapfiles=#()

	fn addmap mapfile =
	(
		local mapfileN=mapfile as name
		local index=finditem mapfiles mapfileN
		local _bitmap = BitmapTexture fileName:mapfile
		
		if index == 0 do append mapfiles mapfileN
	)

	fn copybitmap old_files new_patch =
	(
		for f in old_files do 
		(
			filename = filenameFromPath f
			newfile = (new_patch + "\\" + filename)
			copyFile f newfile
		)
	)
rollout bacth_tool "贴图路径替换" width:162 height:106
(
	button btn3 "设置导出bitmap路径" pos:[5,5] width:144 height:20
	button btn4 "处理Bitmap" pos:[6,75] width:144 height:20
	label label3 "" pos:[5,30] width:148 height:40
	
	on btn3 pressed do
	( 
		local path = ""
		path = getSavePath caption:"选择保存文件路径:" initialDir:savefilepath 
		if path != undefined then 
		(
			label3.text = path
			savefilepath = path
		)
	)
	on btn4 pressed do
	(
		enumeratefiles addmap
		sort mapfiles
		copybitmap mapfiles savefilepath
	)
)
addrollout bacth_tool wando_bitmaptool
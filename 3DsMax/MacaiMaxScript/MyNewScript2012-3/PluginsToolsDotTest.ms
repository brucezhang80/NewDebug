
function Yvi_getFilesRecursive root ext =
(
	local all_Files = #()
	local TheAllSubFolder = #()
	
	TheAllSubFolder = GetDirectories (root+"/*")
	for s in TheAllSubFolder do
	(
		join TheAllSubFolder (GetDirectories (s + "/*"))
	)
	for f in TheAllSubFolder do
	(
		join all_Files (getFiles (f + "/"+ ext))
	)
	join all_Files (getFiles (root + "/" + ext))
	return all_Files
)

try(DestroyDialog Yvi_PluginsManagerTool)catch()
rollout Yvi_PluginsManagerTool "PluginsManagerTool" width:320 height:200
(
	button btn_SetPluginDir "Set Plugin Folder..." pos:[50,7] width:108 height:24 --tooltip:btn_SetPluginDir.text
	label lb_dir "" pos:[11,35] width:178 height:13
	dotNetControl lvCtrl "Listview" height:118 width:290 align:#left
	
	label lb_yvi "Power By Yvi" pos:[70,370] width:63 height:13
	
	on btn_SetPluginDir pressed do
	(
		folder_dir = getSavePath caption:"Select Plugin Folder..." initialDir:"E:\Yvi\3DSMAX\MAXScript\MyScript"--"$scripts"
		
		lb_dir.text = folder_dir
		join g_arr_msFiles (Yvi_getFilesRecursive folder_dir "*.ms") 
		join g_arr_msFiles (Yvi_getFilesRecursive folder_dir "*.mse")
		
		print g_arr_msFiles as string
		
		for m in g_arr_msFiles do
		(
			_fileName = filenameFromPath m
			_ext = (getFilenameType  m)
			_size = 0--(GetFileSize m) / 1024
			lvops.AddLvItem lvCtrl pTextItems: #(_fileName,_ext,_size)-- pChecked:_on pTag:_tag
		)
	)
	
	on Yvi_PluginsManagerTool open do
	(
		clearListener()
		lvops.InitListView lvCtrl pInitColumns:#("文件名","类型", "大小") pInitColWidths:#(100,40,100,60) pHideHeaders:false pCheckBoxes:false
	)
	
)
CreateDialog Yvi_PluginsManagerTool --style:#(#style_titlebar,#style_border,#style_resizing, #style_sysmenu,#style_minimizebox,#style_maximizebox)


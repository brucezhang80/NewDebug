
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
rollout Yvi_PluginsManagerTool "PluginsManagerTool" width:220 height:390
(
	button btn_SetPluginDir "Set Plugin Folder..." pos:[50,7] width:108 height:24 --tooltip:btn_SetPluginDir.text
	label lb_dir "" pos:[11,35] width:178 height:13

	EditText edt_search "Search:"
	radiobuttons rdb_sort labels:#("常用", "最近", "名字", "随机") columns:4
	ListBox lb_Plugins "" width:190 height:20
	--dotNetControl lvCtrl "Listview" height:118 width:290 align:#left
	
	label lb_yvi "Power By Yvi" pos:[70,370] width:63 height:13
	
	Global g_arr_msFiles = #()
	Global folder_dir = ""
	Global _size = 0
	Global lbyvi

	
	on Yvi_PluginsManagerTool open do
	(
		clearListener()
		_size = getDialogSize Yvi_PluginsManagerTool
		lbyvi = lb_yvi.pos
	)
	
	on Yvi_PluginsManagerTool resized size do
	(
		local items = Yvi_PluginsManagerTool.controls
		--for i in items do
		(
			m_size = size - _size
			lb_Plugins.width = m_size.x + 190
			lb_Plugins.height = m_size.y + 265
			
			btn_SetPluginDir.pos.x = (m_size.x+100)/2
			edt_search.pos.x = (m_size.x+80)/2
			rdb_sort.pos.x = (m_size.x+25)/2
			lb_dir.pos.x = (m_size.x+25)/2
			
			lb_yvi.pos.x = (m_size.x+lbyvi.x)
			lb_yvi.pos.y = (m_size.y+lbyvi.y)
			
		)
	)
	on btn_SetPluginDir pressed do
	(
		folder_dir = getSavePath caption:"Select Plugin Folder..." initialDir:"E:\Yvi\3DSMAX\MAXScript\MyScript"--"$scripts"
		
		lb_dir.text = folder_dir
		join g_arr_msFiles (for f in (Yvi_getFilesRecursive folder_dir "*.ms") collect filenameFromPath f )
		join g_arr_msFiles (for f in  (Yvi_getFilesRecursive folder_dir "*.mse")collect filenameFromPath f )
		
		lb_Plugins.items = g_arr_msFiles
	)
	on lb_Plugins doubleClicked item do
	(
		msfile = lb_Plugins.items[item]
		fileIn (folder_dir + "/" + msfile)
	)
	
	on rdb_sort changed state do
	(	
		if(state == 1)then
		(
			
		)
		if(state == 2)then
		(
			
		)
		if(state == 3)then -- 名字
		(
			sort g_arr_msFiles
			lb_Plugins.items = g_arr_msFiles
		)
		if(state == 4)then
		(
			
		)
	)
	
	on edt_search changed txt do
	(
		local arr_adaptFile = #()
		local arr_findFiles = #()
		local arr_notFindFiles = #()
		for item in lb_Plugins.items do
		(
			str_item = item as string
			
			if edt_search.text == "" or edt_search.text == undefined then return 0
			_second = findstring str_item txt
			--_first = findstring str_item edt_search.text

			if _second != undefined then
			(
				append arr_findFiles str_item
			)
			else
			(
				append arr_notFindFiles str_item
			)

		)
		lb_Plugins.items = arr_adaptFile + arr_findFiles + arr_notFindFiles
	)

	on lb_Plugins selected item do
	(
		edt_search.text = lb_Plugins.selected
	)
)
CreateDialog Yvi_PluginsManagerTool style:#(#style_titlebar,#style_border,#style_resizing, #style_sysmenu,#style_minimizebox,#style_maximizebox)


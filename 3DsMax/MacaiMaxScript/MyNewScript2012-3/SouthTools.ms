global SouthToolbar,main_SouthTools
global MainUiFuncStruct,ToolbarFuncStruct
global _size = 0
--创建菜单栏工具条函数结构
struct ToolbarFuncStruct
(	
		--工具栏
	function Yvi_closeToolbar =	
	try 
	(
		cui.UnRegisterDialogBar  SouthToolbar
		destroydialog SouthToolbar
	)catch(),
	
	-- 获取上下端停靠栏宽度
	function Yvi_getToolbarWidth type:#top =
	(
		global toolbarWidth
		r = rolloutCreator "toolbarWidth" "正在创建工具栏..."
		r.begin()
		createdialog (r.end()) style:#(#style_titlebar, #style_border, #style_sysmenu,#style_toolwindow)
		cui.RegisterDialogBar toolbarWidth style:#(#cui_dock_horz, #cui_floatable, #cui_handles,#cui_max_sized)
		case type of
		(	
			#top : cui.DockDialogBar toolbarWidth #cui_dock_top
			default : cui.DockDialogBar toolbarWidth #cui_dock_bottom
		)		
		_width = toolbarWidth.width
		cui.UnRegisterDialogBar  toolbarWidth
		destroydialog toolbarWidth
		
		return _width
	),
	
	--获取停靠左边或者右边的高度
	function Yvi_getToolbarHeight type:undefined =
	(	
		global toolbarHeight
		r = rolloutCreator "toolbarHeight" "正在创建主界面..."
		r.begin()
		createdialog (r.end()) style:#(#style_titlebar, #style_border, #style_sysmenu,#style_toolwindow)
		cui.RegisterDialogBar tollbarHeight maxSize:[200,sysInfo.desktopSize[2]] style:#(#cui_dock_vert, #cui_floatable, #cui_handles)
		case type of
		(	#left : cui.DockDialogBar toolbarHeight #cui_dock_left
			default : cui.DockDialogBar toolbarHeight #cui_dock_right
		)
		_hieght = toolbarHeight.height
		cui.UnRegisterDialogBar  toolbarHeight
		destroydialog toolbarHeight
		
		return _hieght
	),
	
	function Yvi_createToolbar toolbartype =
	(
		ToolbarFuncStruct.Yvi_closeToolbar()
		SouthToolbar = rolloutCreator "SouthToolbar" "SouthToolbar N"
		
		local itemsText = #("选项", "主界面", "物体编辑", "材质整理", "检查工具", "命名工具", "设置工作区域：","", "批处理工具", "帮助", "关闭") --11
		local pstrArr = #()
		
		-- Start creating the rollout
		SouthToolbar.begin()
		px=0;py=2
		for i = 1 to itemsText.count do
		(		
			tooltip = ("tooltip:\""+itemsText[i] +"\"")
			if i == 1 then
			(
				--得到控件宽度
				cur_len = (GetTextExtent itemsText[i])[1]+10
				px = 0
				pstr = ("pos:["+ px as string+","+ py as string+"] width:"+ cur_len as string+" height:18 "+tooltip)
				append pstrArr pstr
				continue
			)
			--得到控件宽度
			pre_len = (GetTextExtent itemsText[i-1])[1]+10
			cur_len = (GetTextExtent itemsText[i])[1]+10
			if i == 8 then
			(
				cur_len += 60
				px += pre_len-10
				pstr = ("pos:["+ px as string+","+ py as string+"] width:"+ cur_len as string+" height:18 items:#(\"A\",\"B\",\"C\",\"D\") "+ tooltip)
				append pstrArr pstr
				continue
			)
			if i == 9 then
			(
				px += pre_len+70
				pstr = ("pos:["+ px as string+","+ py as string+"] width:"+ cur_len as string+" height:18 "+tooltip)
				append pstrArr pstr
				continue
			)
			
			px += pre_len+10

			pstr = ("pos:["+ px as string+","+ py as string+"] width:"+ cur_len as string+" height:18 " +tooltip)
			append pstrArr pstr
		)-- end for
		
		--向工具栏添加控件
		SouthToolbar.addControl #button #btn_option "选项" paramStr:(pstrArr[1])
		SouthToolbar.addControl #button #btn_mainPanel "主界面" paramStr:(pstrArr[2])
		SouthToolbar.addControl #button #btn_objEdit "物体编辑" 	paramStr:(pstrArr[3])
		SouthToolbar.addControl #button #btn_matNeaten "材质整理" paramStr:(pstrArr[4])
		SouthToolbar.addControl #button #btn_checkTools "检查工具" paramStr:(pstrArr[5])
		SouthToolbar.addControl #button #btn_renameTools "命名工具" paramStr:(pstrArr[6])
		SouthToolbar.addControl #label #setWordZone "设置工作区域：" paramStr:(pstrArr[7])
		SouthToolbar.addControl #dropdownlist #btn_workZone "" paramStr:(pstrArr[8])
		SouthToolbar.addControl #button #btn_batchTools "批处理工具" paramStr:(pstrArr[9])
		SouthToolbar.addControl #button #btn_help "帮助" paramStr:(pstrArr[10])
		SouthToolbar.addControl #button #btn_close "关闭" paramStr:(pstrArr[11])
		
		--给控响应事件处理
		/*选项*/
		SouthToolbar.addHandler #btn_option #pressed filter:on codeStr:"print \"ok\""
		/*主界面*/
		SouthToolbar.addHandler #btn_mainPanel #pressed filter:on codeStr:"CreateDialog main_SouthTools style:#(#style_titlebar,#style_border,#style_resizing, #style_sysmenu,#style_minimizebox,#style_maximizebox)"
		/*物体编辑*/
		SouthToolbar.addHandler #btn_objEdit #pressed filter:on codeStr:"print \"ok\""
		/*材质整理*/
		SouthToolbar.addHandler #btn_matNeaten #pressed filter:on codeStr:"print \"ok\""
		/*检查工具*/
		SouthToolbar.addHandler #btn_checkTools #pressed filter:on codeStr:"print \"ok\""
		/*命名工具*/
		SouthToolbar.addHandler #btn_renameTools #pressed filter:on codeStr:"print \"ok\""
		/*设置工作区域：*/
		--SouthToolbar.addHandler #workZone #pressed filter:on codeStr:""
		
		SouthToolbar.addHandler #btn_batchTools #pressed filter:on codeStr:"print \"ok\""
		SouthToolbar.addHandler #btn_help #pressed filter:on codeStr:"print \"ok\""
		SouthToolbar.addHandler #btn_close #pressed filter:on codeStr:"ToolbarFuncStruct.Yvi_closeToolbar()"

						
		SouthToolbar.end()
		
		dockSt =
		(
			if(toolbartype == #top) then
				#cui_dock_top 
			else 
				#cui_dock_bottom
		)
		createDialog SouthToolbar
		cui.RegisterDialogBar SouthToolbar style:#(dockSt, #cui_floatable, #cui_handles,#cui_max_sized)
		cui.DockDialogBar SouthToolbar dockSt
	)
)
SoutFunc = ToolbarFuncStruct() -- call it like this "SouthFunc.fn param"


struct MainUiFuncStruct
(
	function Yvi_anChorMainUICtrl size item =
	(
		m_size = size - _size
		case ClassOf item of
		(
			buttonControl :
					(
						item.pos.x = (m_size.x+90)/2 + 4
						break
					)
			SpinnerControl :()
			ColorPickerControl :()
			CheckBoxControl : ()
			CheckButtonControl : ()
			RadioControl : ()
			EdittextControl :
					(
						item.pos.x = (m_size.x+90)/2 + 4
						break
					)
			ComboBoxControl :()
			LabelControl :
					(
						item.pos.x = (m_size.x+90)/2 + 4
						item.pos.y = (m_size.y+358)
						break
					)
			MapButtonControl :()
			MtlButtonControl :()
			PickerControl :()
			SliderControl :					
					(
						item.pos.x = (m_size.x+80)/2
						break
					)
			LinkControl :
					(
						item.pos.x = (m_size.x+80)/2
						break
					)
			
			ListBoxControl :
					(
						item.width = m_size.x + 140
						item.height = m_size.y + 140
					)
			GroupBoxControl :
					(
						item.width = m_size.x + 190
						item.height = m_size.y + 265
					)
			Default:(print "Default")
		)

	)
	
)
--///////////////////////////////////////////////////// UI /////////////////////////////////////////////////////////////////
try(DestroyDialog main_SouthTools)catch()
rollout main_SouthTools "SouthTool"-- width:320 height:200
(
	Button btn_DockToTop "停靠顶部"
	Button btn_1 "物体编辑"
	Button btn_2 "材质整理"
	Button btn_3 "检查工具"
	Button btn_4 "命名工具"
	Button btn_5 "批处理工具"
	Button btn_6 "帮助"
	Button btn_7 "关闭"
	listBox lb_1 "" 
	label lb_yvi "Power By Yvi" width:63 height:13

	
	on main_SouthTools open do
	(
		clearListener()
		
		--get main ui size
		_size = getDialogSize main_SouthTools
	)
	
	on main_SouthTools resized size do
	(
		items = main_SouthTools.controls
		for i in items do
		(
			
			MainUiFuncStruct.Yvi_anChorMainUICtrl size i
		)
	)
	on btn_DockToTop pressed do
	(
		DestroyDialog main_SouthTools
		SoutFunc.Yvi_createToolbar #top
		
	)
)
CreateDialog main_SouthTools style:#(#style_titlebar,#style_border,#style_resizing, #style_sysmenu,#style_minimizebox,#style_maximizebox)

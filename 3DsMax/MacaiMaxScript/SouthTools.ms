global SouthToolbar,main_SouthTools
global MainUiFuncStruct,ToolbarFuncStruct
global _size = 0
--�����˵��������������ṹ
struct ToolbarFuncStruct
(	
		--������
	function Yvi_closeToolbar =	
	try 
	(
		cui.UnRegisterDialogBar  SouthToolbar
		destroydialog SouthToolbar
	)catch(),
	
	-- ��ȡ���¶�ͣ��������
	function Yvi_getToolbarWidth type:#top =
	(
		global toolbarWidth
		r = rolloutCreator "toolbarWidth" "���ڴ���������..."
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
	
	--��ȡͣ����߻����ұߵĸ߶�
	function Yvi_getToolbarHeight type:undefined =
	(	
		global toolbarHeight
		r = rolloutCreator "toolbarHeight" "���ڴ���������..."
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
		
		local itemsText = #("ѡ��", "������", "����༭", "��������", "��鹤��", "��������", "���ù�������","", "����������", "����", "�ر�") --11
		local pstrArr = #()
		
		-- Start creating the rollout
		SouthToolbar.begin()
		px=0;py=2
		for i = 1 to itemsText.count do
		(		
			tooltip = ("tooltip:\""+itemsText[i] +"\"")
			if i == 1 then
			(
				--�õ��ؼ�����
				cur_len = (GetTextExtent itemsText[i])[1]+10
				px = 0
				pstr = ("pos:["+ px as string+","+ py as string+"] width:"+ cur_len as string+" height:18 "+tooltip)
				append pstrArr pstr
				continue
			)
			--�õ��ؼ�����
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
		
		--�򹤾������ӿؼ�
		SouthToolbar.addControl #button #btn_option "ѡ��" paramStr:(pstrArr[1])
		SouthToolbar.addControl #button #btn_mainPanel "������" paramStr:(pstrArr[2])
		SouthToolbar.addControl #button #btn_objEdit "����༭" 	paramStr:(pstrArr[3])
		SouthToolbar.addControl #button #btn_matNeaten "��������" paramStr:(pstrArr[4])
		SouthToolbar.addControl #button #btn_checkTools "��鹤��" paramStr:(pstrArr[5])
		SouthToolbar.addControl #button #btn_renameTools "��������" paramStr:(pstrArr[6])
		SouthToolbar.addControl #label #setWordZone "���ù�������" paramStr:(pstrArr[7])
		SouthToolbar.addControl #dropdownlist #btn_workZone "" paramStr:(pstrArr[8])
		SouthToolbar.addControl #button #btn_batchTools "����������" paramStr:(pstrArr[9])
		SouthToolbar.addControl #button #btn_help "����" paramStr:(pstrArr[10])
		SouthToolbar.addControl #button #btn_close "�ر�" paramStr:(pstrArr[11])
		
		--������Ӧ�¼�����
		/*ѡ��*/
		SouthToolbar.addHandler #btn_option #pressed filter:on codeStr:"print \"ok\""
		/*������*/
		SouthToolbar.addHandler #btn_mainPanel #pressed filter:on codeStr:"CreateDialog main_SouthTools style:#(#style_titlebar,#style_border,#style_resizing, #style_sysmenu,#style_minimizebox,#style_maximizebox)"
		/*����༭*/
		SouthToolbar.addHandler #btn_objEdit #pressed filter:on codeStr:"print \"ok\""
		/*��������*/
		SouthToolbar.addHandler #btn_matNeaten #pressed filter:on codeStr:"print \"ok\""
		/*��鹤��*/
		SouthToolbar.addHandler #btn_checkTools #pressed filter:on codeStr:"print \"ok\""
		/*��������*/
		SouthToolbar.addHandler #btn_renameTools #pressed filter:on codeStr:"print \"ok\""
		/*���ù�������*/
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
	Button btn_DockToTop "ͣ������"
	Button btn_1 "����༭"
	Button btn_2 "��������"
	Button btn_3 "��鹤��"
	Button btn_4 "��������"
	Button btn_5 "����������"
	Button btn_6 "����"
	Button btn_7 "�ر�"
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
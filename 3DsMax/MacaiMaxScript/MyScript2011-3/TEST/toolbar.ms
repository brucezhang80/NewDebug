

try(DestroyDialog rl_toolbar)catch()
rollout rl_toolbar "toolbar" width:138 height:62
(
	dotNetControl toolbar "System.Windows.Forms.ToolBar"

	button btn_01 "buuton"
	button toolBarButton1 ""
	button toolBarButton2 ""
	button toolBarButton3 ""
	
	on rl_toolbar  open do
	(
		toolBarButton1.Text = "Open"
		toolBarButton2.Text = "Save"
		toolBarButton3.Text = "Print"

		toolbar.Buttons.Add(toolBarButton1)
		toolbar.Buttons.Add(toolBarButton2)
		toolbar.Buttons.Add(toolBarButton3)
	)
	
)
createDialog rl_toolbar style:#(#style_toolwindow, #style_sysmenu, #style_resizing) 
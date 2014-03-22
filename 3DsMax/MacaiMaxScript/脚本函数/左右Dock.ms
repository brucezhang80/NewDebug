rollout dockTab "Dock/Undock" width:138 height:62
(
		button unDockB "²»¿¿±ß" pos:[32,8] width:75 height:17 enabled:false toolTip:"Un-Docks the toolbar if it's docked."
		button dockL "¿¿×ó" pos:[8,8] width:25 height:17 toolTip:"Dock Toolbar to Left."
		button dockR "¿¿ÓÒ" pos:[112,8] width:25 height:17 toolTip:"Dock Toolbar to Right."
		on unDockB pressed do
		(
				cui.UnRegisterDialogBar dockTab
				unDockB.enabled = false
				DockStatus =  false
		)
		on dockL pressed do
		(
				cui.RegisterDialogBar dockTab
				cui.DockDialogBar dockTab #cui_dock_left 
				unDockB.enabled = true
				DockStatus =  true
		)
		on dockR pressed do
		(
				cui.RegisterDialogBar dockTab
				cui.DockDialogBar dockTab #cui_dock_right
				unDockB.enabled = true
				DockStatus =  true
		)
)
createDialog dockTab style:#(#style_toolwindow, #style_sysmenu, #style_resizing) 
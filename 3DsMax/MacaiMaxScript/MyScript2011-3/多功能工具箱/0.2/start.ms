
try(DestroyDialog rl_Start)catch()
Rollout rl_Start "插件使用说明" width:300 height:150
(
	hyperLink hl_0x00 "插件已运行...请从 " align:#left color:Green enabled:false
	hyperLink hl_0x1 "\" Customize(自定义) -> "align:#left color:yellow enabled:false
	hyperLink hl_0x2 "Customize User Interface(自定义用户界面) -> "align:#left color:yellow enabled:false
	hyperLink hl_0x3 "Toolbars(工具栏) -> Category(类别) -> "align:#left color:yellow enabled:false
	hyperLink hl_0x4 "下拉框选择\"3D虚拟建模工具箱-v3.1 \"  拖到工具栏" align:#left color:Green enabled:false
	hyperLink hl_0x5 "的任何位置即可!" align:#left color:Green enabled:false
	
	Button btn_Close "关闭"
	
	on btn_Close pressed do
	(
		DestroyDialog rl_Start
	)
	on rl_Start open do
	(
		try
		(
			fileIn ((getdir #Scripts)+"\3DTools\mainMcr.mcr")
			colorman.reInitIcons()--重新加载图标
		)
		catch(throw())
	)
)
CreateDialog rl_Start
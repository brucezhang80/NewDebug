
try(Destroydialog Yvi_FindOutNo4Face) catch()
Rollout Yvi_FindOutNo4Face "找出非四面物体"
(
	Button btn_findit "查找" height:50 width:160
	label lb_info "" align:#left height:50
	
	on btn_findit pressed do
	(
		sel = selection as array
		if sel.count == 0 then
		(
			Messagebox "请选择物体!" title:"找出非四面物体"
			return 0
		)
		Clearselection()
		local i = 0
		for obj in sel where(classof obj == Editable_Mesh)do
		(
			--num = obj.numfaces
			num = getNumFaces obj
			--num = meshop.getNumFaces obj
			print num
			if num != 4 then
			(
				i += 1
				selectMore obj
				actionMan.executeAction 0 "311"
				max zoomext sel all
			)
			else
			(
				
			)
		)
		if i > 0 then
		(
			lb_info.text = "检查完毕!\n存在 ["+ i as string + "] 个非四面物体!\n已经选中!"
		)
		else if i == 0 then
		(
			lb_info.text = "检查完毕!\n全部物体都为四面体!"
		)
	)
)CreateDialog Yvi_FindOutNo4Face

try(DestroyDialog Detach2Elements)catch()
rollout Detach2Elements "分离元素(Detach)"
(
	local srcObjs = #() -- 源对象物体
	local tmpObjs = #() -- 中间临时物体
	local endObjs = #() -- 转换成Mesh
	local eleGroups = #() -- 解组
	
	--函数
	fn DetachToElements obj cPivot &endObjs num:1 = 
	(
		while obj.getNumFaces() != 0 do 
		(
			polyop.setFaceSelection obj #{1}
			obj.selectElement()
			ele = polyop.getFaceSelection obj
			newName = (num as string)
			num += 1
			polyop.detachFaces obj ele asNode:true name:newName
			newObj = getNodeByName newName
			append endObjs newObj
			attachObjects obj newObj
		)
		if cPivot do centerPivot obj.children
	)
	
	mapped fn renameChildren ch = 
	(
		ch.name = ch.parent.name + ch.name
	)
	
	group "基本设置"
	(
		checkBox keepSource "保留源对象" checked:true
	)
	group "新对象设置"
	(
		checkBox outMesh "转换成Mesh" checked:false
		checkBox isGroup "Detach后打组" checked:false
		checkBox cPivot "轴心归中" checked:true
	)
	group "操作"
	(
		button detach "Detach" width:140
		label feed "..."
		progressbar progBar color:green
	)

	fn runEntirely = 
	(
		feed.text = "正在Detach，请稍等..."
		local total = (tmpObjs.count as string) -- 总对象数
		for i = 1 to tmpObjs.count do (
			feed.text = "Detaching... " + (i as string) + "/" + total -- 进度条
			DetachToElements tmpObjs[i] cPivot.state endObjs
			grp = group tmpObjs[i].children name:(srcObjs[i].name + "_Elements")
			append eleGroups grp -- 解组
			attachObjects srcObjs[i] grp move:false
			renameChildren grp.children --重命名
			progBar.value = 100. * i / tmpObjs.count
		)
		progBar.value = 0 -- 重设进度条
		feed.text = "完成..."
		eleGroups = for g in eleGroups where isValidNode g collect g -- 重要!
		if not isGroup.state do
		(
			if not keepSource.state do 
			( -- 删除原始的
				local n = 1
				for g in eleGroups do 
				(
					holder = point pos:g.pos name:g.name
					g.parent = holder 
					progBar.value = 100. * n / eleGroups.count
					n += 1
				)
			)
			progBar.value = 0
			ungroup eleGroups
		)
		if not keepSource.state do (delete srcObjs)
	)

	
	on detach pressed do
	(
		try
		(
			feed.text = "正在初始化..."
			srcObjs = for i in selection where canConvertTo i Editable_Poly collect i
			if srcObjs.count == 0 then 
			(
				feed.text = "#先选定对象#"
			)
			else 
			(
				disableSceneRedraw()
				feed.text = "就绪..."
				max create mode
				setWaitCursor()
				TimeStart = timestamp()
				numObjs = objects.count
				snapshot srcObjs --拷贝
				tmpObjs = 
				(
					for i = (numObjs + 1) to objects.count collect objects[i]
				)
				convertTo tmpObjs Editable_Poly
				
				runEntirely()
				delete tmpObjs
				---------------------
				endObjs = for i in endObjs where isValidNode i collect i -- 重要
				if outMesh.state do convertToMesh endObjs
				select endObjs
				feed.text = "完成!"
				TimeEnd = (timestamp() - TimeStart) / 1000.
				setArrowCursor() --------------------
				enableSceneRedraw()
				redrawViews()
				format "Time:\t%sec.\n" TimeEnd
			) -- end (Runtime)
		)catch()
	) -- end (on detach pressed)
	
------------------------------------------------------------
	on Detach2Elements open do
	(
		if selection.count == 0 then
		(
			feed.text = "#未选定任何对象#"
		)
		else
		(
			feed.text = "当前选择了 " + selection.count as string + " 个对象！"
		)
	)
	
) -- end of rollout
	fn Fn_SelCountObj =
	(
		 try
		 (
			TheSelection = selection as array
			if TheSelection.count == 0 then
			(
				Detach2Elements.feed.text = "#未选定任何对象#"
			)
			else
			(
				Detach2Elements.feed.text = "当前选择了 " + TheSelection.count as string + " 个对象!"
			)
		)
		catch()
		
	)
	Callbacks.AddScript #SelectionSetChanged "Fn_SelCountObj()" ID:#SelCountName
	
createDialog Detach2Elements
	
	
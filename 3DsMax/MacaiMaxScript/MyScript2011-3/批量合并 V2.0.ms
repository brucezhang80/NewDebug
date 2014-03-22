--批量合并
--Yvi
--2011-04-01
--------------------------------------------------------------------------------------------
rollout BatchMerger "批量合并" width:212 height:480
(
	-----------------------定义变量-----------------------------------
	--global getPath
	global bm_files = #()
	global getFilepath
	
	---------------------------控件------------------------------------
	button btn_browse "浏览" pos:[160,20] width:44 height:22 toolTip:"打开选择文件目录对话框"
	label lb_text "粘贴目录：(按Enter)" pos:[5,10]
	editText edit_filepath "" pos:[5,24] width:154 height:15 --text:("粘贴目录...")
	multilistbox list_files "文件列表：" pos:[4,41] width:193 height:13  -- 注意这里不要使用listbox，否则不能多选
	button btn_selectall "全选" offset:[0,-21] pos:[8,244] width:48 height:21
	button btn_selinvert "反选" offset:[0,-21] pos:[80,244] width:48 height:21 enabled:false
	button btn_unselect "不选" offset:[0,-21] pos:[153,244] width:48 height:21 
	
	button btn_merger "开始合并" pos:[113,280] width:85 height:60
	checkbox chk_autoRename "自动重命名" pos:[5,278] width:96 height:24 checked:true
	checkbox chk_merger2group "合并成组" pos:[5,300] width:96 height:20 checked:true
	checkbox chk_ShMaxName "上海用(另存为地块名-前4位数)" pos:[5,320] width:110 height:30 checked:true
	--progressBar pb_progress "进度条" pos:[9,309] width:201 height:12
	progressBar pgb_mergeProgress "进度条" pos:[4,350] width:200 height:12

	----------------------------控件事件------------------------------- 
	-------------------------------------------------------------------

	on btn_browse pressed  do --浏览
	(
		if (dir = getSavePath caption:"选择您要合并的文件（*.MAX）的目录...")!= undefined then
		(
			if (dir.count) > 28 then edit_filePath.text = "..."+(substring dir (dir.count-28) dir.count) 
			else edit_filePath.text = dir
			
			bm_files = getFiles (dir+"/*.max")
			list_files.items = for f in bm_files collect (filenameFromPath f)
		)
	)

	-------------------------------------------------------------------
	
-- 	on edit_filePath click do
-- 	( 
-- 		edit_filepath.text = ""
-- 	)
	
	on edit_filePath entered text do --文件路径
	(
		if (edit_filePath.text) != undefined do
		(
			if (edit_filePath.text.count) > 28 then edit_filePath.text = "..."+(substring edit_filePath.text (edit_filePath.text.count-28) edit_filePath.text.count) 
			else edit_filePath.text = edit_filePath.text

			bm_files = getFiles (edit_filePath.text+"/*.max")
			list_files.items = for f in bm_files collect (filenameFromPath f)
		)
	)
	
-- 	on list_files rightClick do--文件列表
-- 	(

-- 	)
	---------------------------------------------------------------------
	on btn_selectAll pressed  do  --全选
	(
		all = #{1..(list_files.items.count)}
		list_files.selection = all
	)
	on btn_unselect pressed  do   --不选
	(

		list_files.selection = #{}
	
	)
	
	on btn_selinvert pressed do  --反选
	(
		
	)
	
	----------------------------------------------------------------------------
	on btn_merger pressed  do     --合并
	(
		local toMerge = #()--合并对象的数组
		local sel = (list_files.selection as array)
			
		for i = 1 to sel.count do append toMerge bm_files[sel[i]]
		
			--自动重命名
			if chk_autoRename.checked then 
			(
				DialogMonitorOPS.unRegisterNotification id:#autoRaname
				DialogMonitorOPS.RegisterNotification checkDialog id:#autoRaname
				DialogMonitorOPS.Enabled = true
				
				--actionMan.executeAction 0 "40195"  -- File: 合并文件
				for i in objects  do i.name = ( uniquename i.name)
			)
		
		for i=1 to toMerge.count do 
		(
			--合并
			local mergedObjects = #() 
			
			if (mergeMAXFile toMerge[i] #noRedraw #select #mergeDups #renameMtlDups #neverReparent quiet:true) == false 
				then 
				(
					importFile toMerge[i] #noPrompt
				)
				
			mergedObjects = getCurrentSelection()
			
			--如果合并成组的复选框被勾选，则组成一个新的对象
			if chk_merger2group.checked then 
				group mergedObjects name:(getFilenameFile toMerge[i])
			
		
			--更新对象合并中的进度条
			pgb_mergeProgress.value = 100.*i / toMerge.count
		)
			
			DialogMonitorOPS.Enabled = false
			DialogMonitorOPS.unRegisterNotification id:#autoRaname
		
			-- 合并完成，进度条归0
			pgb_mergeProgress.value = 0
-------------------------------------------------------------------------------		
		--2011.6.23
		--杨祎增加
		--合并完成后另存为，主要是上海项目专用，临时增加。
		if bm_files[1] ==undefined then 
		(
			messagebox "文件列表为空！"
		)
		else
		(
			filepath = getFilenamePath bm_files[1]
			filename = getFilenameFile bm_files[1]
			--获取前四位
			pre4name = substring filename 1 4

			themaxname = (filepath+pre4name + ".max")
			print ("themaxname:"+themaxname)
			
			if (chk_ShMaxName.checked)then
			(
				if (not (doesfileexist themaxname)) or 
				querybox ("文件  "+themaxname +" 已经存在， 是否要覆盖?") then 
				( 
					saveMaxfile themaxname
					true 
				) 
				else false 
				
			)
		)
	)
-------------------------------------------------------------------------------
	---------------------------结束------------------------------
)
--createdialog BatchMerger
----建立浮动的窗口
--global theMergFloater

if theMergFloater != undefined then closeRolloutFloater theMergFloater
theMergFloater = newRolloutFloater "批量合并 v2.0" 220 400 
addRollout BatchMerger theMergFloater

--批量合并
--Yvi
--2011-04-01
--------------------------------------------------------------------------------------------
	
rollout BatchMerger "批量合并" width:212 height:460
(
	-----------------------定义变量-----------------------------------
	--global getPath
	global bm_files = #()
	global getFilepath

	---------------------------控件------------------------------------
	button btn_browse "浏览" pos:[165,10] width:44 height:22 toolTip:"打开选择文件目录对话框"
	editText edit_filepath "" pos:[8,10] width:154 height:22 --text:("粘贴目录...")
	multilistbox list_files "文件列表：" pos:[9,41] width:193 height:13  -- 注意这里不要使用listbox，否则不能多选
	button btn_selectall "全选" offset:[0,-21] pos:[8,244] width:48 height:21
	button btn_selinvert "反选" offset:[0,-21] pos:[80,244] width:48 height:21 enabled:false
	button btn_unselect "不选" offset:[0,-21] pos:[153,244] width:48 height:21 
	
	button btn_merger "开始合并" pos:[8,280] width:90 height:21
	checkbox chk_merger2group "合并成组" pos:[113,282] width:96 height:20 checked:true
	checkbox chk_autoRename "自动重命名" pos:[113,300] width:96 height:24 checked:true
	--progressBar pb_progress "进度条" pos:[9,309] width:201 height:12
	progressBar pgb_mergeProgress "进度条" pos:[9,330] width:190 height:12

	----------------------------控件事件------------------------------- 
	-------------------------------------------------------------------
	on btn_browse pressed  do --浏览
	(
		if (dir = getSavePath caption:"选择您要合并的文件（*.MAX）的目录...") != undefined 
				then
				(
					if (dir.count) > 28 then edit_filePath.text = "..."+(substring dir (dir.count-28) dir.count) 
					else edit_filePath.text = dir
					
					bm_files = getFiles (dir+"/*.max")
					list_files.items = for f in bm_files collect (filenameFromPath f)
				)

	)
	
	-------------------------------------------------------------------
	--on edit_filePath pressed do( edit_filePath.text1 = "")
	
	
	on edit_filePath entered text do --文件路径
	(
		
		if(edit_filePath.text != "")
		then (edit_filePath.text = "")
		
		if (edit_filePath.text) != undefined do
		(
			if (edit_path.count) > 28 then edit_filePath.text = "..."+(substring edit_path (edit_path.count-28) edit_path.count) 
			else edit_filePath.text = edit_path
				
			bm_files = getFiles (edit_path+"/*.max")
			list_files.items = for f in bm_files collect (filenameFromPath f)
		)
	)
	--on list_files selected sel do--文件列表
	--(
	
	--)
	
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
	)

	---------------------------结束------------------------------
)
--createdialog BatchMerger
----建立浮动的窗口
global theMergFloater

if theMergFloater != undefined then closeRolloutFloater theMergFloater
theMergFloater = newRolloutFloater "批量合并 v1.0" 220 380
addRollout BatchMerger theMergFloater

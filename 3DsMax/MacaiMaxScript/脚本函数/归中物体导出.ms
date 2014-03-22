try destroydialog export catch()

rollout export "归中导物体"(
	local numgeom = #()
	button btn_maxPath "浏览" width:40 height:20 pos:[205,3]	
	edittext edt_maxPath "路径:" width:195 pos:[5,5]	
	button btn_bacthExp "批量导出" width:95 height:25 pos:[150,26]	
	edittext edt_preFix "前綴名(可不填):" pos:[5,30] width:140 
	label lb_message "就绪中……" color:blue pos:[40,60]
	progressbar pgb_expProgress color:green  pos:[5,80] width:240
	button btn_fresh "场景刷新" pos:[120,55] height:20 width:55
	button btn_openRsc "打开文件夹" pos:[180,55] height:20 width:65
-------
	
on export open do
(
	numgeom = geometry as array
	lb_message.caption = "共有"+ (numgeom.count as string) +"个待导出物体"	
)
	
-----------
	
	on btn_fresh pressed do	
	(		
		numgeom = geometry as array
		lb_message.caption = "共有"+ (numgeom.count as string) +"个待导出物体"	
	)
		
-----------
		
	on btn_openRsc pressed do 
	(
		if edt_maxPath.text != "" then 
		(		
			cmd = "start" + " " + edt_maxPath.text		
			DOSCommand cmd
		)
		else messagebox "还没有指定路径"
	)
----------
	on btn_maxPath pressed do 
	(
		savepath = getSavePath caption:"请选择保存目录"		
		try edt_maxPath.text = savepath + "\\" catch()
	)
------------
on btn_bacthExp pressed do 
		
	if edt_maxPath.text != "" then 
	(
		for i = 1 to numgeom.count do
		(
			a = numgeom[i].pos.x
			b = numgeom[i].pos.y
			c = numgeom[i].pos.z
			
			numgeom[i].pivot.x = numgeom[i].center.x
			numgeom[i].pivot.y = numgeom[i].center.y
			numgeom[i].pivot.z = numgeom[i].min.z
			move numgeom[i] [-a,-b,-c]
			
			saveNodes numgeom[i]  (edt_maxPath.text + edt_preFix.text + numgeom[i].name )
			
			move numgeom[i] [a,b,c]
			pgb_expProgress.value = 100*i/numgeom.count 
		)
		if pgb_expProgress.value == 100 then lb_message.caption = "已导出"+numgeom.count as string + "个物体"
	)
	else messagebox "请先指定导出保存路径"
)
createdialog export 250 100

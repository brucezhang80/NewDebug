--------------------------------------------------------------------------------
--------------------------------------------------------------------
----------------------by 红小豆 2011.11.1------------------------------
-------------------------QQ：393689534--------------------------------
--------------------------------------------------------------------
--------------------------------------------------------------------
basemeshvert=#()
basemesh=undefined
global CreatePoint,PTArray,basemesh,PT,basepos

fn CreatePoint obj=
(
	basepos=undefined
	basepos=#()
	PT.pos= obj.pos
	basemeshvert=obj.selectedVerts
	for i in 1 to basemeshvert.count do
	(
		PPT=copy PT
		PPT.pos=basemeshvert[i].pos
		append PTArray PPT
		append basepos basemeshvert[i].pos
	)
	PTArray.parent=PT
	hide PTArray
	select PT 
)


rollout PointTransform "模型顶点变换工具" width:184 height:152
(
	Timer tmr1 "Timer" pos:[8,104] width:24 height:24 interval:100
	button btn1 "创建轴点" pos:[16,8] width:152 height:24
	checkbox chk1 "设置轴点位置模式" pos:[32,40] width:128 height:16
	button btn2 "完成" pos:[16,64] width:152 height:24
	button btn3 "取消" pos:[16,96] width:152 height:24	
	label lbl1 "【红小豆作品 2011.11.1】" pos:[26,128] width:168 height:16


	on tmr1 tick do
	(
		--try (select $MESHPT) catch()
		
		try
		(
			if basemeshvert.count!=0 and PTArray.count==basemeshvert.count then
			(
				for i in 1 to basemeshvert.count do basemeshvert[i].pos=PTArray[i].pos
				update basemesh
			)
		)
		catch()
		
	)
	on btn1 pressed do
	(
		if $MESHPT==undefined then
		(
			PT=point()
			PT.name="MESHPT"
		)
		
		PTArray=undefined
		PTArray=#()
		if selection.count==1 then
		(
			if(classof $)==Editable_mesh then 
			(
				basemesh=$
				CreatePoint selection[1]
			)
			else messagebox"被操作的对象必须为Editable_mesh物体！"
		)
		else messagebox"每次只能对一个物体进行操作！"
	)
	on chk1 changed theState do
	(
		if chk1.state==on then 
		(
			try(PTArray.parent=undefined) catch()
		)
		else
		(
			try(PTArray.parent=$MESHPT) catch()
		)
	)
	on btn2 pressed do
	(
		--try (delete $MESHPT0*) catch()
		delarray=undefined
		delarray=$MESHPT* as array
		for i in delarray do 
		(
			if i.name.count!=6 then delete i
		)
		delete $MESHPT
	)
	on btn3 pressed do
	(
		--try (delete $MESHPT0*) catch()
		delarray=undefined
		delarray=$MESHPT* as array
		for i in delarray do 
		(
			if i.name.count!=6 then delete i
		)
		delete $MESHPT
		
		for i in 1 to basemeshvert.count do
		(
			basemeshvert[i].pos=basepos[i]
		)
		update basemesh
	)
)
createdialog PointTransform
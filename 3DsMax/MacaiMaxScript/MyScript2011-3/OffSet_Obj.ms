--------------------------------------------------------------------------------
--------------------------------------------------------------------
----------------------by ��С�� 2011.11.1------------------------------
-------------------------QQ��393689534--------------------------------
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


rollout PointTransform "ģ�Ͷ���任����" width:184 height:152
(
	Timer tmr1 "Timer" pos:[8,104] width:24 height:24 interval:100
	button btn1 "�������" pos:[16,8] width:152 height:24
	checkbox chk1 "�������λ��ģʽ" pos:[32,40] width:128 height:16
	button btn2 "���" pos:[16,64] width:152 height:24
	button btn3 "ȡ��" pos:[16,96] width:152 height:24	
	label lbl1 "����С����Ʒ 2011.11.1��" pos:[26,128] width:168 height:16


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
			else messagebox"�������Ķ������ΪEditable_mesh���壡"
		)
		else messagebox"ÿ��ֻ�ܶ�һ��������в�����"
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
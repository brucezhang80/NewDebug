tool oneTreePlant
(
	on mousePoint clickno do
	(
		createPolygon worldPoint 
	)
)
tool polylineTreePlant
(
	local new_spline
	local firstPoint
	local clickCount=#()
    on mousePoint clickno do
	(
		if clickno==2 then 
		(
			append clickCount worldPoint
			firstPoint=worldPoint
			new_spline = splineShape()-- pos:worldPoint
			addNewSpline new_spline
			addKnot new_spline 1 #corner  #line  worldPoint
		)
		else if clickno>1 then
		(
			append clickCount worldPoint
			addKnot new_spline 1 #corner  #line  worldPoint
			updateShape new_spline
			select new_spline
		)
	)
)
tool polygonTreePlant
(
	local new_spline
	local firstPoint
	local clickCount=#()
    on mousePoint clickno do
	(
		if clickno==2 then 
		(
			append clickCount worldPoint
			firstPoint=worldPoint
			new_spline = splineShape()-- pos:worldPoint
			addNewSpline new_spline
			addKnot new_spline 1 #corner  #line  worldPoint
		)
		else if clickno<5 and clickno>1 then
		(
			append clickCount worldPoint
			addKnot new_spline 1 #corner  #line  worldPoint
			updateShape new_spline
		)
		else  if clickno==5 then
		(
			append clickCount worldPoint
			addKnot new_spline 1 #corner  #line  worldPoint
			updateShape new_spline
			close new_spline 1
			--addKnot new_spline 1 #corner  #line  firstPoint
			updateShape new_spline
			select new_spline
			#stop
		)
	)
)
try
(
	destroydialog TreePlantWayRollout
)
catch
(
)
rollout TreePlantWayRollout "��ֲ����" width:200 height:398
(
	local pTrees=#(),pPlanted=#(),pCount,pPolygon,pScale=1,pAngle=0,pClick=1
	
	GroupBox grp3 "ѡ��" pos:[5,2] width:190 height:50
	GroupBox grp4 "��ֲ" pos:[5,125] width:190 height:254
	button oneTreePlant_Btn "��������" pos:[10,159] width:100 height:20 enabled:false
	button drawLineToPlant_btn "��������" pos:[10,250] width:100 height:20 enabled:false
	button drawPolygonToPlant_btn "��������" pos:[10,291] width:100 height:20 enabled:false
	pickbutton btn15 "ѡ������" pos:[9,225] width:100 height:20 enabled:false
	pickbutton selectPolygonToPlant_btn "ѡ������" pos:[10,316] width:100 height:20 enabled:false
	spinner spn2 "" pos:[125,248] width:61 height:16 range:[0,100,5]
	button selectTrees_btn "ѡȡ" pos:[10,22] width:100 height:20
	label lbl1 "��ѡȡ0����" pos:[115,24] width:75 height:15
	dropdownList ddl1 "" pos:[121,158] width:70 height:22
	slider sld1 "" pos:[15,341] width:170 height:25 range:[100,1000,0] orient:#horizontal tick:0 enabled:false
	checkbox chk1 "������С" pos:[10,74] width:76 height:14
	checkbox chk2 "��������" pos:[86,97] width:76 height:14
	label lbl2 "By SouthGIS" pos:[133,380] width:61 height:13
	checkbox chk3 "�����Ƕ�" pos:[10,95] width:76 height:14
	spinner spn3 "" pos:[87,75] width:40 height:16 range:[0,1,0.8]
	GroupBox grp6 "ѡ��" pos:[5,55] width:190 height:66
	button btn7 "��ѡ��˳������" pos:[10,185] width:100 height:20 enabled:false
	label lbl3 "--------------------------------------------" pos:[10,142] width:180 height:13
	label lbl4 "--------------------------------------------" pos:[10,210] width:180 height:13
	label lbl5 "���ࣺ" pos:[125,227] width:57 height:15
	label lbl6 "--------------------------------------------" pos:[13,275] width:180 height:13
	spinner spn4 "" pos:[151,75] width:40 height:16 range:[1,2,1.2]
	label lbl25 "��" pos:[134,74] width:14 height:19
	
	on oneTreePlant_Btn pressed do
	(
		pPlanted.count=0
		if pTrees.count>0 then
		(
			oneTree=pTrees[ddl1.selection]
			p=pickPoint()
			while (classof p==point3) do 
			(
				
				if chk1.checked then
				(
					pScale=random (spn3.value) (spn4.value)
				) 
				if chk3.checked then
				(
					pAngle=random 0 360
				)
			
				theTree=copy oneTree
				
				oneTree.pos=p
				scale theTree ([1,1,1]*pScale)
				rotate theTree (angleaxis pAngle [0,0,1])
				append pPlanted theTree
				p=pickPoint()
			)
		)
	)
	on drawLineToPlant_btn pressed do
	(
	    startTool polylineTreePlant
		pLine=$
		pD=spn2.value
		obj=pTrees[ddl1.selection]
		p=curveLength pLine
		pPercent=(pD/p)*100
		treeCount=p/pD as integer +1
		theTrees=#()
		
		--print obj.pos.controller
		for i=1 to treeCount do 
		(
			if chk2.checked then
			(
				obj=pTrees[random 1 pTrees.count]
			) 
			
			if chk1.checked then
			(
				pScale=random (spn3.value) (spn4.value)
			) 
			if chk3.checked then
			(
				pAngle=random 0 360
			)
			theTree=copy obj
			theTree.pos.controller =Path_Constraint()
			theTree.pos.controller.path=pLine
			theTree.pos.controller.percent=(i-1)*pPercent
			theTree.pos.controller.follow=true
			scale theTree ([1,1,1]*pScale)
			rotate theTree (angleaxis pAngle [0,0,1])
			append theTrees theTree
		)
		
		for t in theTrees do 
		(
			 t.pos.controller=Position_XYZ()
		) 
		delete pLine
	    
	)
	on drawPolygonToPlant_btn pressed do
	(
		pPlanted.count=0
		clearselection()
	    starttool polygonTreePlant
		pPolygon=$
		convertto pPolygon Editable_poly
		CenterPivot pPolygon
		pCount=100
		i=1
		theTree=pTrees[ddl1.selection]
		while i<=pCount do
		(
			x=random pPolygon.min.x pPolygon.max.x
			y=random pPolygon.min.y pPolygon.max.y
			z=pPolygon.max.z+10
			
			pPivot=[x,y,z]
			dir=[0,0,-1]
			pRay= ray pPivot dir
			newRay=intersectRay pPolygon pRay
			
			if chk2.checked then
			(
				theTree=pTrees[random 1 pTrees.count]
			) 
			if chk1.checked then
			(
				pScale=random (spn3.value) (spn4.value)
			) 
			if chk3.checked then
			(
				pAngle=random 0 360
			)
			if newRay!=undefined then
			(	
				
				tree=copy theTree
				tree.pos=newRay.pos
				scale tree ([1,1,1]*pScale)
				append pPlanted tree
				rotate theTree (angleaxis pAngle [0,0,1])
			    i=i+1
			)
		)
	)
	on btn15 picked obj do
	(
		pLine=obj
		pD=spn2.value
		pobj=pTrees[ddl1.selection]
		p=curveLength pLine
		pPercent=(pD/p)*100
		treeCount=p/pD as integer +1
		
		theTrees=#()
		for i=1 to treeCount do 
		(
			if chk2.checked then
			(
				obj=pTrees[random 1 pTrees.count]
			) 
			
			
			if chk1.checked then
			(
				pScale=random (spn3.value) (spn4.value)
			) 
			if chk3.checked then
			(
				pAngle=random 0 360
			)
			theTree=copy obj
			
			theTree.pos.controller =Path_Constraint()
			theTree.pos.controller.path=pLine
			theTree.pos.controller.percent=(i-1)*pPercent
			theTree.pos.controller.follow=true
			scale theTree ([1,1,1]*pScale)
			rotate theTree (angleaxis pAngle [0,0,1])
		    append theTrees theTree
		)
		
		for t in theTrees do 
		(
			 t.pos.controller=Position_XYZ()
		) 
		
	)
	on selectPolygonToPlant_btn picked obj do
	(
		pPlanted.count=0
		pPolygon=obj
		centerPivot obj
		pCount=100
		i=1
		theTree=pTrees[ddl1.selection]
		while i<=pCount do
		(
			x=random obj.min.x obj.max.x
			y=random obj.min.y obj.max.y
			z=obj.max.z+10
			
			pPivot=[x,y,z]
			dir=[0,0,-1]
			pRay= ray pPivot dir
			newRay=intersectRay obj pRay
			
			if chk2.checked then
			(
				theTree=pTrees[random 1 pTrees.count]
			) 
			if chk1.checked then
			(
				pScale=random (spn3.value) (spn4.value)
			) 
			if chk3.checked then
			(
				pAngle=random 0 360
			)
			if newRay!=undefined then
			(	
				
				tree=copy theTree
				tree.pos=newRay.pos
				scale tree ([1,1,1]*pScale)
				rotate theTree (angleaxis pAngle [0,0,1])
				append pPlanted tree
			    i=i+1
			)
		)
	)
	on selectTrees_btn pressed do
	(
		pTrees.count=0
		for obj in selection do 
		(
			append pTrees obj
		)
		lbl1.caption="��ѡȡ"+(pTrees.count as string)+"����"
		pTreesString=#()
		for i=1 to pTrees.count do 
		(
			append pTreesString pTrees[i].name
		)
		ddl1.items=pTreesString
		
		oneTreePlant_Btn.enabled=true
		drawLineToPlant_btn.enabled=true
		drawPolygonToPlant_btn.enabled=true
		selectPolygonToPlant_btn.enabled=true
		btn15.enabled=true
		btn7.enabled=true
		sld1.enabled=true
	)
	on sld1 changed val do
	(
		
		pCount=pPlanted.count
		if pCount!=0 then
		(
		obj=pPolygon
		theCount=sld1.value
		i=1
		theTree=pTrees[ddl1.selection]
		pChange=theCount-pCount
		
		if pChange>0 then
		(
		while i<=pChange as integer do
		(
			x=random obj.min.x obj.max.x
			y=random obj.min.y obj.max.y
			z=obj.max.z+10
			
			pPivot=[x,y,z]
			dir=[0,0,-1]
			pRay= ray pPivot dir
			newRay=intersectRay obj pRay
			
			if chk2.checked then
			(
				theTree=pTrees[random 1 pTrees.count]
			) 
			if chk1.checked then
			(
				pScale=random (spn3.value) (spn4.value)
			) 
			if chk3.checked then
			(
				pAngle=random 0 360
			)
			if newRay!=undefined then
			(	
				
				tree=copy theTree
				tree.pos=newRay.pos
				scale tree ([1,1,1]*pScale)
				rotate theTree (angleaxis pAngle [0,0,1])
				append pPlanted tree
			    i=i+1
			)
	  	)
	    )
		else
		(
			while i<=abs (pChange as integer) do 
			(
				pIndex=random 1 pCount
				delete pPlanted[pIndex]
				deleteitem pPlanted pIndex
				pCount=pPlanted.count
				pChange=theCount-pCount
				i=i+1
			)
		)
		pCount=pPlanted.count
	    )
	)	
	on btn7 pressed do 
	(
		pPlanted.count=0
		if pTrees.count>0 then
		(
			p=pickPoint()
			oneTree=pTrees[pClick]
			while (classof p==point3) do 
			(
				
				if chk1.checked then
				(
					pScale=random (spn3.value) (spn4.value)
				) 
				if chk3.checked then
				(
					pAngle=random 0 360
				)
			
				theTree=copy oneTree
				
				oneTree.pos=p
				scale theTree ([1,1,1]*pScale)
				rotate theTree (angleaxis pAngle [0,0,1])
				append pPlanted theTree
				p=pickPoint()
				pClick=pClick+1
				pClick=(mod pClick pTrees.count) as integer
				if pClick==0 then
				(
					pClick=pTrees.count
				)
				oneTree=pTrees[pClick]
			)
		)
		
	)
)
createdialog TreePlantWayRollout pos:[10,100]
clearlistener()

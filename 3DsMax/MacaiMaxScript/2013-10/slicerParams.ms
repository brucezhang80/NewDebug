

--��õ�ǰ��ϵͳ��λ
function yviCurSystemUnitIndex =
(
	local unit = units.SystemType;
	local index = 6;	-- default is meters

	case unit of
	(
		#Inches: 		index = 1;
		#Feet:			index = 2;
		#Miles:			index = 3;
		#Millimeters:	index = 4;
		#Centimeters:	index = 5;
		#Meters:		index = 6;
		#Kilometers:	index = 7;
		default:		index = 8;
	)

	return index;
)

function yviSetCurSytemUnits index =
(
	case index of
	(
		1: 		units.SystemType = #Inches;
		2:		units.SystemType = #Feet;
		3:		units.SystemType = #Miles;
		4:		units.SystemType = #Millimeters;
		5:		units.SystemType = #Centimeters;
		6:		units.SystemType = #Meters;
		7:		units.SystemType = #Kilometers;
		default:units.SystemType = #Meters;
	)                              
)

function yviCollectSystemUnits =
(
	local unitArr = #("Inches��Ӣ�磩",
						"Feet��Ӣ�ߣ�",
						"Miles��Ӣ�",
						"Millimeters�����ף�",
						"Centimeters�����ף�",
						"Meters���ף�",
						"Kilometers��ǧ�ף�",
						"Unknow��δ֪��λ��");
	return unitArr;
)

--�ֶβü�������
function yviThickness obj lenght axis =
(
	local j = 0
	undo on
	(
		--sliceObject obj myThickness.value myAxis.state
		
		local pObj = obj;
		local pThickness = lenght;
	
		objCenter = pObj.center;
		pObj.pivot = [0,0,0] -- ����е���������
		resetXform pobj -- ȷ������������ȷ��
		convertToPoly pobj

		local startPos = case axis of --��ʼ��Ƭƽ���λ��
		(
			1 : [pObj.min.x,0,0]
			2 : [0,pObj.min.y,0]
			3 : [0,0,pObj.min.z]
		)
	 
		local endPos = case axis of -- ������Ƭƽ���λ��
		(
			1 : [pObj.max.x,0,0]
			2 : [0,pObj.max.y,0]
			3 : [0,0,pObj.max.z]
		)
				
		local lMod = mod (distance endPos startPos) pThickness;
		local lNumSlices; -- ��Ƭ����

		if (lMod < pThickness/2) then 
			lNumSlices = (((distance endPos startPos)/pThickness) as integer)-1;
		else 
			lNumSlices = ((distance endPos startPos)/pThickness) as integer; -- ���һƬ������һ�����Ƭ���

		for i = 1 to lNumSlices do -- Slicing
		(
			addmodifier pObj (slicemodifier());
			pObj.modifiers[1].slice_plane.rotation = case axis of -- ͨ����ת����Ƭƽ��
			(
				1 : (quat -0.5 -0.5 -0.5 -0.5);
				2 : (quat 0.707107 0 0 0.707107);
				3 : (quat 0 0 0 1);
			)
		
			lSliceOffset = i * pThickness;
		
			pObj.modifiers[1].slice_plane.pos = case axis of
			(
				1: startPos + [lSliceOffset,0,0];
				2: startPos + [0,lSliceOffset,0];
				3: startPos + [0,0,lSliceOffset];
			)
			
			OldObj = copy pObj
			oldObj.name = pObj.name + "_" + i as string
		
			OldObj.modifiers[1].Slice_Type = case axis of
			(
				1 : 2
				2 : 3
				3 : 2
			)
			
			pObj.modifiers[1].Slice_Type = case axis of
			(
				1 : 3
				2 : 2
				3 : 3
			)
			
			pObj.pivot = objCenter;
			convertToPoly pObj;
			--resetXform pObj;
			
		)--for plice
	)--button
	--pgb_Progress1.value = 0
	--pgb_Progress1.value = 0
)--�ֶγ��Ȳü�����
	
global offsetX = 37, offsetY = 18;
try(DestroyDialog slicerParams)catch()
rollout slicerParams "�и���Ƭ"
(
	Group "��������/����/��λ"
	(
		label lb_axis "��:" align:#left 
		radiobuttons rbtn_axis "" labels:#("X", "Y", "Z") default:2 align:#left offset:[offsetX,-offsetY]
		
		--label lb_length "���ȣ�" across:2 align:#left
		spinner spn_thickness "���ȣ�" width:100 range:[0,10E9,3] type:#float align:#left
		
		label lb_unit "��λ��"align:#left
		DropDownList dlist_unit "" offset:[offsetX,-offsetY] width:107 --selection:(yviCurSystemUnitIndex())
	)

	Group "�и����"
	(
		button btn_doSlice "�и�" width:140 height:22
		label lb_1 "�ռ����ȣ� " align:#left
		progressBar pgb_Progress1 "" width:140 height:8 color:red
		label lb_2 "�и���ȣ� " align:#left
		progressBar pgb_Progress2 ""  width:140 height:8 color:Green
	)
----------------------------------------------------------------------------------------

---------------------
	-- initialize
	on slicerParams open do
	(
		dlist_unit.items = yviCollectSystemUnits();
		local index = yviCurSystemUnitIndex();
		dlist_unit.selection = index;
	)
	
	-- change system unit
	on dlist_unit selected index do
	(
		yviSetCurSytemUnits index;
	)
	
	-- do slicing object
	on btn_doSlice pressed do 
	(
		local len = spn_thickness.value;
		local axis = rbtn_axis.state;
		for obj in selection do
		(
			
			yviThickness obj len axis;
		)
	)
	
)createDialog slicerParams
-- end rollout
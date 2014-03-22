

--获得当前的系统单位
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
	local unitArr = #("Inches（英寸）",
						"Feet（英尺）",
						"Miles（英里）",
						"Millimeters（毫米）",
						"Centimeters（厘米）",
						"Meters（米）",
						"Kilometers（千米）",
						"Unknow（未知单位）");
	return unitArr;
)

--分段裁剪函数数
function yviThickness obj lenght axis =
(
	local j = 0
	undo on
	(
		--sliceObject obj myThickness.value myAxis.state
		
		local pObj = obj;
		local pThickness = lenght;
	
		objCenter = pObj.center;
		pObj.pivot = [0,0,0] -- 轴居中到世界坐标
		resetXform pobj -- 确保本地轴是正确的
		convertToPoly pobj

		local startPos = case axis of --开始切片平面的位置
		(
			1 : [pObj.min.x,0,0]
			2 : [0,pObj.min.y,0]
			3 : [0,0,pObj.min.z]
		)
	 
		local endPos = case axis of -- 结束切片平面的位置
		(
			1 : [pObj.max.x,0,0]
			2 : [0,pObj.max.y,0]
			3 : [0,0,pObj.max.z]
		)
				
		local lMod = mod (distance endPos startPos) pThickness;
		local lNumSlices; -- 切片数量

		if (lMod < pThickness/2) then 
			lNumSlices = (((distance endPos startPos)/pThickness) as integer)-1;
		else 
			lNumSlices = ((distance endPos startPos)/pThickness) as integer; -- 最后一片不少于一半的切片厚度

		for i = 1 to lNumSlices do -- Slicing
		(
			addmodifier pObj (slicemodifier());
			pObj.modifiers[1].slice_plane.rotation = case axis of -- 通过旋转轴切片平面
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
)--分段长度裁剪函数
	
global offsetX = 37, offsetY = 18;
try(DestroyDialog slicerParams)catch()
rollout slicerParams "切割面片"
(
	Group "设置轴向/长度/单位"
	(
		label lb_axis "轴:" align:#left 
		radiobuttons rbtn_axis "" labels:#("X", "Y", "Z") default:2 align:#left offset:[offsetX,-offsetY]
		
		--label lb_length "长度：" across:2 align:#left
		spinner spn_thickness "长度：" width:100 range:[0,10E9,3] type:#float align:#left
		
		label lb_unit "单位："align:#left
		DropDownList dlist_unit "" offset:[offsetX,-offsetY] width:107 --selection:(yviCurSystemUnitIndex())
	)

	Group "切割操作"
	(
		button btn_doSlice "切割" width:140 height:22
		label lb_1 "收集进度： " align:#left
		progressBar pgb_Progress1 "" width:140 height:8 color:red
		label lb_2 "切割进度： " align:#left
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
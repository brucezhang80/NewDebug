--macroScript Gridify category:"Bobo_s Tools"
--rollout Gridify "切片工具_(单物体)V1.1"
(


--Gridify
--Version 0.45
--Edited:  2/01/01
--by Borislav Petrov
--http://gfxcentral.com/bobo/
--------------------------------------------------


global grid_float
local object_to_slice


rollout Grid_roll "切片平面控制_(单物体)v0.1"
(
local slice_x_array = #()
local slice_y_array = #()
local slice_z_array = #()
local update_slices

local old_x = 2
local old_y = 2
local old_z = 2

local mesh_selected_yes = false


fn check_for_mesh obj = (superclassof obj == geometryClass and classof obj != targetObject)

pickButton pick_object ">选择物体" width:150 filter:check_for_mesh align:#center 
checkbox split_elements "分离Mesh到元素" checked:false

	
	
Group "X Axis"
	(
	checkbox X_yes "开" checked:false  across:3 align:#left
	Spinner X_segs "#" range:[2,20,2] fieldwidth:30 align:#center type:#integer
	Spinner X_off "" range:[-99,99,0] fieldwidth:30 align:#right type:#integer
	
	checkbutton X_b_min "MIN" width:40 height:16 across:3 align:#left
	Spinner X_bias "" range:[1,99,50] fieldwidth:40 align:#center type:#integer
	checkbutton X_b_max "MAX" width:40 height:16 align:#right
)

Group "Y Axis"
	(
	
	checkbox Y_yes "开" checked:false across:3
	Spinner Y_segs "#" range:[2,20,2] fieldwidth:30 align:#center  type:#integer
	Spinner Y_off "" range:[-99,99,0] fieldwidth:30 align:#right type:#integer
	
	checkbutton Y_b_min "MIN" width:40 height:16 across:3 align:#left
	Spinner Y_bias "" range:[1,99,50] fieldwidth:40 align:#center type:#integer
	checkbutton Y_b_max "MAX" width:40 height:16 align:#right

)
Group "Z Axis"
	(
		
	checkbox Z_yes "开" checked:false across:3
	Spinner Z_segs "#" range:[2,20,2] fieldwidth:30 align:#center type:#integer
	Spinner Z_off "" range:[-99,99,0] fieldwidth:30 align:#right type:#integer
	
	checkbutton Z_b_min "MIN" width:40 height:16 across:3 align:#left
	Spinner Z_bias "" range:[1,99,50] fieldwidth:40 align:#center type:#integer
	checkbutton Z_b_max "MAX" width:40 height:16 align:#right
)
	
	Button Slice_object "Bake Grid 到 Mesh" width:150 height:30 enabled:false align:#center \
	tooltip:"Collapse Mesh to incorporate Grid into Mesh..."
	

On pick_object picked obj do
	(
	object_to_slice = obj
	pick_object.text = obj.name
	Slice_object.enabled = true
	)
	
	
fn ensure_selection =	
(
	if modpanel.getCurrentObject()== undefined then
		(
		select object_to_slice
		max modify mode
		)
)
	
On slice_object pressed do
	(
	undo on
	(
	converttomesh object_to_slice
	X_yes.checked = false
	Y_yes.checked = false
	Z_yes.checked = false
	)
	)
	
	
on X_segs changed value do
	(
	ensure_selection ()
if X_Yes.checked then
	(	
		for i = 2 to old_x do
			(
			deletemodifier object_to_slice slice_x_array[i]
			)
	
		for i = 2 to value do
			(
			s_m = sliceModifier ()
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			s_m.Faces___Polygons_Toggle = 0 
			slice_x_array[i]= s_m
			modpanel.addmodtoselection slice_x_array[i]			
			)
		update_slices x_segs.value y_segs.value z_segs.value true false false
		old_x = value
	)	
)

on X_bias changed value do
(
	ensure_selection ()

if X_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value true false false
	)	
)

on X_b_min changed state do
(
	ensure_selection ()

	if X_b_max.checked and state then X_b_max.checked = false

if X_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value true false false
	)	
)

on X_b_max changed state do
(
	ensure_selection ()

	if X_b_min.checked and state then X_b_min.checked = false
if X_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value true false false
	)	
)

on X_off changed value do
(
ensure_selection ()

if X_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value true false false
	)	
)


on Y_segs changed value do
(
ensure_selection ()

	if Y_yes.checked then
	(
		for i = 2 to old_y do
			(
			deletemodifier object_to_slice slice_y_array[i]
			)
	
		for i = 2 to value do
			(
			s_m = sliceModifier  ()
			s_m.Faces___Polygons_Toggle = 0 
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			slice_y_array[i]= s_m
			modpanel.addmodtoselection slice_y_array[i]
			)
		update_slices x_segs.value y_segs.value z_segs.value false true false
		old_y = value
	)	
)

on Y_bias changed value do
(
ensure_selection ()

	if Y_yes.checked then
	(
		update_slices x_segs.value y_segs.value z_segs.value false true false
	)	
)


on Y_b_min changed state do
(
ensure_selection ()

	if Y_b_max.checked and state then Y_b_max.checked = false

if Y_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value false true false 
	)	
)

on Y_b_max changed state do
(
ensure_selection ()

	if Y_b_min.checked and state then Y_b_min.checked = false
if Y_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value false true false
	)	
)


on Y_off changed value do
(
ensure_selection ()

	if Y_yes.checked then
	(
		update_slices x_segs.value y_segs.value z_segs.value false true false
	)	
)



on Z_segs changed value do
(
ensure_selection ()

if Z_yes.checked then
	(
		for i = 2 to old_z do
			(
			deletemodifier object_to_slice slice_z_array[i]
			)
	
		for i = 2 to value do
			(
			s_m = sliceModifier  ()
			s_m.Faces___Polygons_Toggle = 0 
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			slice_z_array[i]= s_m
			modpanel.addmodtoselection slice_z_array[i]		
			)
		update_slices x_segs.value y_segs.value z_segs.value false false true
		old_z = value
	)	
)

on Z_bias changed value do
(
ensure_selection ()

if Z_yes.checked then
	(
		update_slices x_segs.value y_segs.value z_segs.value false false true
	)	
)


on Z_b_min changed state do
(
ensure_selection ()

	if Z_b_max.checked and state then Z_b_max.checked = false

if Z_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value false false true 
	)	
)

on Z_b_max changed state do
(
ensure_selection ()

	if Z_b_min.checked and state then Z_b_min.checked = false
if Z_Yes.checked then
	(	
		update_slices x_segs.value y_segs.value z_segs.value false false true
	)	
)


on Z_off changed value do
(
ensure_selection ()

if Z_yes.checked then
	(
		update_slices x_segs.value y_segs.value z_segs.value false false true
	)	
)


on X_yes changed state do
(
ensure_selection ()

		if state then
		(
		for i = 2 to x_segs.value do
			(
			s_m = sliceModifier  ()
			s_m.Faces___Polygons_Toggle = 0 
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			slice_x_array[i]= s_m
			modpanel.addmodtoselection slice_x_array[i]		
			)
		update_slices x_segs.value y_segs.value z_segs.value true false false
		)
		else
		(
		for i = 2 to x_segs.value do
			(
			deletemodifier object_to_slice slice_x_array[i]
			)
		
		)
	)
	





on Y_yes changed state do
(
ensure_selection ()
		if state then
		(
		for i = 2 to y_segs.value do
			(
			s_m = sliceModifier  ()
			s_m.Faces___Polygons_Toggle = 0 
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			slice_y_array[i]= s_m
			modpanel.addmodtoselection slice_y_array[i]
			)
		update_slices x_segs.value y_segs.value z_segs.value false true false
		)
		else
		(
		for i = 2 to y_segs.value do
			(
			deletemodifier object_to_slice slice_y_array[i]
			)
		
		)
	)

on Z_yes changed state do
(
ensure_selection ()
		if state then
		(
		for i = 2 to z_segs.value do
			(
			s_m = sliceModifier  ()
			s_m.Faces___Polygons_Toggle = 0 
			if split_elements.checked then s_m.Slice_Type = 1 else s_m.Slice_Type = 0
			slice_z_array[i]= s_m
			modpanel.addmodtoselection slice_z_array[i]
			)
		update_slices x_segs.value y_segs.value z_segs.value false false true
		)
		else
		(
		for i = 2 to z_segs.value do
			(
			deletemodifier object_to_slice slice_z_array[i]
			)
		
		)
	)




fn update_slices x_sl y_sl z_sl upd_x upd_y upd_z =
	( 
	
	ensure_selection()
	disableSceneRedraw()
	
	if X_yes.checked and upd_x then
	(		
	for i = 2 to X_sl do
		(
		bbox_min = GetModContextBBoxMin object_to_slice slice_x_array[i]
		bbox_max = GetModContextBBoxMax object_to_slice slice_x_array[i]
	
		offset_x = (X_off.value as float/100) * ((bbox_max.x-bbox_min.x)/X_sl)
		
		if X_b_min.checked then
		(
		bbox_mid = [offset_x+bbox_min.x+((((100-X_bias.value)as float/100)*(i-1))*(bbox_max.x-bbox_min.x)/X_sl)*(i-1),bbox_min.y,bbox_min.z]
		)
		
		if X_b_max.checked then
		(
		bbox_mid = [-offset_x+bbox_max.x-((((100-X_bias.value)as float/100)*(i-1))*(bbox_max.x-bbox_min.x)/X_sl)*(i-1),bbox_min.y,bbox_min.z]
		)
		
		if not X_b_max.checked and not X_b_min.checked then
		(
		bbox_mid = [offset_x+bbox_min.x+((bbox_max.x-bbox_min.x)/X_sl)*(i-1),bbox_min.y,bbox_min.z]
		)

		slice_x_array[i][1][1].value = bbox_mid		
		slice_x_array[i][1][2].value = quat 90 [0,1,0]
		)--end i loop
	)
	
	if Y_yes.checked and upd_y then
	(
	for i = 2 to Y_sl do
		(
		
		bbox_min = GetModContextBBoxMin object_to_slice slice_y_array[i]
		bbox_max = GetModContextBBoxMax object_to_slice slice_y_array[i]
		
		offset_y = (Y_off.value as float /100) * ((bbox_max.y-bbox_min.y)/Y_sl)

		if Y_b_min.checked then
		(
		bbox_mid = [bbox_min.x,offset_y+bbox_min.y+((((100-Y_bias.value)as float/100)*(i-1))*(bbox_max.y-bbox_min.y)/Y_sl)*(i-1),bbox_min.z]
		)
		
		if Y_b_max.checked then
		(
		bbox_mid = [bbox_min.x, -offset_y+bbox_max.y-((((100-Y_bias.value)as float/100)*(i-1))*(bbox_max.y-bbox_min.y)/Y_sl)*(i-1),bbox_min.z]
		)
		
		if not Y_b_max.checked and not Y_b_min.checked then
		(
		bbox_mid = [bbox_min.x,offset_y+bbox_min.y+((bbox_max.y-bbox_min.y)/Y_sl)*(i-1),bbox_min.z]
		)



		slice_y_array[i][1][1].value = bbox_mid
		slice_y_array[i][1][2].value = quat 90 [1,0,0]		
		)--end i loop
	)
	
	
		if Z_yes.checked and upd_z then
	(
	for i = 2 to Z_sl do
		(

		bbox_min = GetModContextBBoxMin object_to_slice slice_z_array[i]
		bbox_max = GetModContextBBoxMax object_to_slice slice_z_array[i]

		offset_z = (Z_off.value as float /100) * ((bbox_max.z-bbox_min.z)/Z_sl)


		if Z_b_min.checked then
		(
		bbox_mid = [bbox_min.x,bbox_min.y,offset_z+bbox_min.z+((((100-Z_bias.value)as float/100)*(i-1))*(bbox_max.z-bbox_min.z)/Z_sl)*(i-1)]
		)
		
		if Z_b_max.checked then
		(
		bbox_mid = [bbox_min.x,bbox_min.y, -offset_z+bbox_max.z-((((100-Z_bias.value)as float/100)*(i-1))*(bbox_max.z-bbox_min.z)/Z_sl)*(i-1)]
		)
		
		if not Z_b_max.checked and not Z_b_min.checked then
		(
		bbox_mid = [bbox_min.x,bbox_min.y, offset_z+bbox_min.z+((bbox_max.z-bbox_min.z)/Z_sl)*(i-1)]
		)

		slice_z_array[i][1][1].value = bbox_mid
		)--end i loop
	)
enableSceneRedraw()
	
)--end fn		


)--end rollout


if grid_float != undefined then closerolloutfloater grid_float 
grid_float = newrolloutfloater "切片 v0.1" 190 340 100 100
addrollout grid_roll grid_float 
--createDialog Gridify
	
)--end util

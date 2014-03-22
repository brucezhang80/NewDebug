global functions
try(
	struct temp
	(
		val = 0 --不要更改这个
		,
		gIs = fn gIs Category Key File:"Settings" =  
		(
			local type, val, key1
			
 			registry.createKey HKEY_CURRENT_USER ("Software\\McSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
			
			registry.createKey HKEY_CLASSES_ROOT (".imr\\mc01\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
			
		)--END getIs FN
		,
		sIs = fn sIs Category Key Val File:"Settings" del:false = 
		(
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CURRENT_USER ("Software\\McSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
				if not del then
				(
					registry.setvalue key1 (Key as string) #REG_SZ (Val as string)
				)
				else
				(
					registry.deleteKey key1
				)
			)
			catch
			(	
				print (registry.getLastError())	
			)
			
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CLASSES_ROOT (".imr\\mc01\\"+File as string + "\\" + Category as string ) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
				if not del then
				(
					registry.setvalue key1 (Key as string) #REG_SZ (Val as string)
				)
				else
				(
					registry.deleteKey key1
				)
			)
			catch
			(	
				print (registry.getLastError())	
			)
			
		)--END saveIs FN
		,
		saveMCSettings = fn saveMCSettings MC ilimited =
		(
			local items = MC.controls
			for i in items do
			(
				--print (classOf i)  --uncommend to find out the classes of the UI items	不推荐找出UI项的类型
				case classOf i of
				(
					SpinnerControl : (functions.sIs MC.name i.name i.value)
					ColorPickerControl : (functions.sIs MC.name i.name i.color)
					CheckBoxControl : (functions.sIs MC.name i.name i.checked)
					CheckButtonControl : (functions.sIs MC.name i.name i.checked)
					RadioControl : (functions.sIs MC.name i.name i.state)
					ListBoxControl : (with printAllElements true functions.sIs MC.name i.name i.items)
					LabelControl : (functions.sIs MC.name i.name (if i.name == "lb_reg" do i.text = ilimited as string))
					Default:(functions.sIs MC.name "value1" (flag as string))
 
				)--END classes cases
			)--END items loop
		)--END saveMCSettings FN
		,
		getMCSettings = fn getMCSettings MC =
		(
			global items = MC.controls
			for i in items do
			(
				case classOf i of
				(
					SpinnerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer do i.value = val
					)
					ColorPickerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == color do i.color = val
					)
					CheckBoxControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)
					CheckButtonControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)					
					RadioControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer  do i.state = val
					)					
					ListBoxControl :
					(
						local val = functions.gIs MC.name i.name 
						if val != undefined AND classOf val == String do val = execute val 
						if classOf val == array do i.items = val
					)
					LabelControl :
					(
						local val = functions.gIs MC.name i.name
						if val != undefined AND classOf val == String do val = execute val
						if (classOf val == integer)  And (i.name == "lb_reg")then
						(
							if(val == 1)then
							(
								MessageBox "只剩下最后一次使用了！需要继续使用请注册！"
								i.text = (val) as String
							)
							else if(val > 1 and val <= 10)then
							(
								if i.name == "lb_reg" do
									i.text = val as String
							)
							else if (val > 10)then
							(
								if i.name == "lb_reg" do
								(
									i.text = val as String
									--i.pos = [200,300]
								)
								
							)
							else if(val <= 0)do
							(
								--关闭回调.
								callbacks.removeScripts id:#Fn_MymtlRefAdded01
								MessageBox "使用期限已到，需要继续使用请注册！"
								if i.name == "lb_reg" do
									i.text = 0 as string + " 未授权脚本已经过期"
							)
							if val ==  undefined then
							(
								val = 10
								if i.name == "lb_reg" do
									i.text = val as String
							)
						)
					)
				)
			)
		)
	)-- struct
	functions = temp val:1	
)catch()	
---------------------------------------------------------------------------------------				
	global Fn_MymtlRefAdded
	
	function Fn_MymtlRefAdded material =
	(
		try
		(
			--if material.Diffusemap.filename != undefined then
			(
				material.Diffusemap.name = (getFileNamefile material.Diffusemap.filename)
				material.name = material.Diffusemap.name
			)
			--else
			(
				--print "error mtl"
			)     
		)catch()
	)
global ilimited
global rl_regWin
global rl_mat

Rollout rl_regWin "Enter Code -By Yvi" width:200
(
	EditText edt_code "输入激活码:" 
	--Button btn_regit "好"
	try(
	on edt_code entered text do
	(
		if (text == (execute "12319597*3")) then
		(
			ilimited = 100
			rl_mat.lb_reg.text = ilimited as string
			functions.saveMCSettings rl_mat ilimited
			DestroyDialog rl_regWin
			
			callbacks.addScript #mtlRefAdded "Fn_MymtlRefAdded (callbacks.notificationParam())" id:#Fn_MymtlRefAdded01
		)
		else if(text == "12319597")then
		(
			ilimited = 10
			rl_mat.lb_reg.text = ilimited as string
			functions.saveMCSettings rl_mat ilimited
			DestroyDialog rl_regWin
			
			callbacks.addScript #mtlRefAdded "Fn_MymtlRefAdded (callbacks.notificationParam())" id:#Fn_MymtlRefAdded01
		)
		else
		(
			MessageBox ("错误的激活码!")
		)
	)
	
)catch()
	on rl_regWin close do
	(
		
		rl_mat.chb_register.checked = false
	)
)

try(DestroyDialog rl_mat)catch()
Rollout rl_mat "AutoNameMat By Yvi" width:200 height:94
(
	CheckButton chb_register "" width:4 height:4 pos:[2,4] 
	Button btn_close "Close" tooltip:"关闭本插件"
	label lb_info "保持本插件运行,拖图片到材质球上时,自动为你命名材质球名称和材质名称.(使用贴图名命名)" align:#left width:176 height:40
	CheckButton chb_moreinfo "----------------------------------------------"  height:10 pos:[0,84] tooltip:"展开其它信息"
	Group ""
	(
		label lb_copyright "@Copyright 2011 \nPower By Yvi QQ:12319597" align:#left width:155 height:25
		HyperLink hlk_link "Click link" Color:Green Address:"http://www.macai.co.cc"
		label lb_reg "10" height:10 align:#right
	)
	
	on chb_register changed state do
	(
		--DestroyDialog rl_mat
		if state == true then
		(
			CreateDialog rl_regWin
		)
		else
		(
			DestroyDialog rl_regWin
		)
	)
	on chb_moreinfo changed state do
	(
		if state == true then
		(
			for h = rl_mat.height to 200 by 3 do
			(
				rl_mat.height = h
			)
		)
		else
		(
			for h = rl_mat.height to 100 by -3 do
			(
				rl_mat.height = h
			)
		)
	)
	on btn_close pressed do
	(
		DestroyDialog rl_mat
	)
	
	on rl_mat open do
	(
		functions.getMCSettings rl_mat
	)
	
	on rl_mat close do
	(
		callbacks.removeScripts id:#Fn_MymtlRefAdded01
		--更新到注册表
		ilimited = lb_reg.text as integer
		if ilimited == undefined do return false
		ilimited -= 1
		lb_reg.text = ilimited as string
		functions.saveMCSettings rl_mat ilimited
	)
	
)CreateDialog rl_mat style:#(#style_minimizebox, #style_titlebar,#style_border, #style_sysmenu,#style_sunkenedge)

callbacks.addScript #mtlRefAdded "Fn_MymtlRefAdded (callbacks.notificationParam())" id:#Fn_MymtlRefAdded01

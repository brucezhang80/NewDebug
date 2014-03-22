global pluginpath = ((GetDir #Scripts) + "\\3DTools")
global mainPlugin = ((GetDir #Scripts) + "\\3DTools\\3D虚拟建模工具箱-3.1.mse")

global functions
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
			
			registry.createKey HKEY_CLASSES_ROOT (".imr\\mc03\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
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
				registry.createKey HKEY_CLASSES_ROOT (".imr\\mc03\\"+File as string + "\\" + Category as string ) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
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
			--if MC == undefined then return 0
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
								fileIn mainPlugin
							)
							else if(val > 1 and val <= 10)then
							(
								if i.name == "lb_reg" do
									i.text = val as String
								fileIn mainPlugin
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
								MessageBox "使用期限已到，需要继续使用请注册！"
								createDialog rl_regWin
								--return 0
							)
							if val ==  undefined then
							(
								val = 5
								if i.name == "lb_reg" do
									i.text = val as String
								fileIn mainPlugin
							)
						)
					)
				)
			)
		)
	)-- struct
	functions = temp val:1
--------------------------------------------------------------------------------------------------------------------------------------------------------
	
	
/*MyTimer 返回一个时间数*/
fn MyDownTimer numbers =
(
	valDown = (numbers as integer) - 1
	--if(valDown > 0)then
	--(
		return valDown
	--)
	--else return 0
)
global ilimited
global rl_regWin
global rl_Welcome


Rollout rl_regWin "Enter Code -By Yvi" width:200
(
	EditText edt_code "输入激活码:" 
	--Button btn_regit "好"
	
	on edt_code entered text do
	(
		if (text == (execute "12319597*3")) then
		(
			ilimited = 100
			--rl_Welcome.lb_reg.text = ilimited as string
			functions.saveMCSettings rl_Welcome ilimited
			DestroyDialog rl_regWin
			try
			(
				--fileIn mainPlugin
				functions.getMCSettings rl_Welcome
				
			)catch(Messagebox "启动脚本插件出错,未能找到主插件程序 \"3D虚拟建模工具箱-3.1\"";throw())
			
		)
		else if(text == "12319597")then
		(
			ilimited = 10
			--rl_Welcome.lb_reg.text = ilimited as string
			functions.saveMCSettings rl_Welcome ilimited
			DestroyDialog rl_regWin
			try
			(
				--fileIn mainPlugin
				functions.getMCSettings rl_Welcome
				
			)catch(Messagebox "启动脚本插件出错,未能找到主插件程序 \"3D虚拟建模工具箱-3.1\"")
		)
		else
		(
			MessageBox ("错误的激活码!")
			return 0
		)
	)
	
)

rollout rl_Welcome "欢迎使用 \"3D虚拟建模工具箱 - v3.1\"" width:700 height:230
(

		timer clock "My Timer" interval:100
		Bitmap bmp_Welcome "Welcome" fileName:"welcome.bmp" width:740 height:200--bitmap:<bitmap> 
	
		
		progressBar pgb_Progress ""width:702 height:8	color:green value:0 pos:[-2,203]
		label lb_text "100" pos:[-100,214]
		label lb_reg "5" pos:[300,220]
	
		label lb_txt1 "正在启动插件："pos:[0,214] --width:100
		label lb_load "" pos:[100,214] align:#left
	
		on clock tick do
		(
			myclock = MyDownTimer lb_text.text
				--print ("time:"+ myclock as string)
			if myclock > 80 then
			(
				lb_load.text = "正在初始化插件文件..."
			)
			else if (myclock < 80 and myclock > 50 )then
			(
				lb_load.text = "正在加载插件文件..."
			)
			else if (myclock < 50 and myclock > 30 )then
			(
				if ( (doesFileExist (pluginpath + "\\3D虚拟建模工具箱-3.1.mse")) == false )then
				(
					lb_load.text = "正在安装插件文件..."
				)
				else if ( (doesFileExist (pluginpath + "\\3D虚拟建模工具箱-3.1.mse")) == true )then
				(
					lb_load.text == "正在验证插件完整性..."
				)
			)
			else if (myclock < 30 and myclock > 20 )then
			(
				if ( (doesFileExist (pluginpath + "\\3D虚拟建模工具箱-3.1.mse") )== false and  (doesFileExist  (pluginpath + "\\functions.mse") ) == false)then
				(
					lb_load.text = "安装完成..."
				)
				else if ( (doesFileExist (pluginpath + "\\3D虚拟建模工具箱-3.1.mse") )== true  and (doesFileExist  (pluginpath + "\\functions.mse")) == true)then
				(
					lb_load.text == "插件完整..."
				)
			)
			else if (myclock < 20 and myclock > 10 )then
			(
				lb_load.text = "正在加载主插件程序文件..."
			)
			else if (myclock < 10 and myclock > 0 )then
			(
				lb_load.text = "插件加载完成,正在启动..."
			)
			
			if(myclock > 0)then
			(
				lb_text.text = myclock as string
			)
			else
			(
				--wel_pos = GetDialogPos rl_Welcome
				

			--	try
			--	(
					--fileIn mainPlugin
					functions.getMCSettings rl_Welcome
					
			--	)catch(Messagebox "启动脚本插件出错,未能找到主插件程序 \"3D虚拟建模工具箱-3.1\"")
				
				ilimited = lb_reg.text as integer
				if ilimited == undefined do return false
				ilimited -= 1
				lb_reg.text = ilimited as string
				functions.saveMCSettings rl_Welcome ilimited
				
				destroyDialog rl_Welcome
			)
			
			pgb_Progress.value = 100.0/(myclock)
		)

		on rl_Welcome open do
		(
			--functions.getMCSettings rl_Welcome	
		)
)createDialog rl_Welcome
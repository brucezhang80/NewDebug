
global pluginpath = ((GetDir #Scripts) + "\\3DTools")
global mainPlugin = ((GetDir #Scripts) + "\\3DTools\\3D虚拟建模工具箱-3.1.mse")


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


rollout rl_Welcome "欢迎使用 \"3D虚拟建模工具箱 - v3.1\"" width:700 height:230
(

		timer clock "My Timer" interval:100
		Bitmap bmp_Welcome "Welcome" fileName:"welcome.bmp" width:740 height:200--bitmap:<bitmap> 
	
		
		progressBar pgb_Progress ""width:702 height:8	color:green value:0 pos:[-2,203]
		label lb_text "100" pos:[-100,214]
	
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
				if ( (doesFileExist (pluginpath + "\\3D虚拟建模工具箱-3.1.mse") ) and  (doesFileExist  (pluginpath + "\\functions.mse") ))then
				(
					lb_load.text = "安装完成..."
				)
				else
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
				destroyDialog rl_Welcome
				try
				(
					fileIn mainPlugin
					
				)catch(Messagebox "启动脚本插件出错,未能找到主插件程序 \"3D虚拟建模工具箱-3.1\"")
			)
			pgb_Progress.value = 100.0/(myclock)
		)

		on rl_Welcome open do
		(
			--functions.getMCSettings rl_Welcome	
		)
)createDialog rl_Welcome

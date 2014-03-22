
macroScript yy_3DTool
category:"3D虚拟建模工具箱" 
toolTip:"3D虚拟建模工具箱-v3.1" 
buttontext:"工具箱"
Icon:#("YY3Dlogo",1)
(	

		try
		(
			FileIn ("$Scripts\3DTools\3D虚拟建模工具箱-3.1.mse") quiet:true
		)catch( Messagebox "由于文件 \"3D虚拟建模工具箱-3.1\" 丢失无法启动,请重新安装!")
		

)--end macroScript
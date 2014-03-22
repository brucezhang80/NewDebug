			
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


global rl_mat

try(DestroyDialog rl_mat)catch()
Rollout rl_mat "AutoNameMat By Yvi" width:200 height:94
(
	Button btn_close "Close" tooltip:"关闭本插件"
	label lb_info "保持本插件运行,拖图片到材质球上时,自动为你命名材质球名称和材质名称.(使用贴图名命名)" align:#left width:176 height:40
	CheckButton chb_moreinfo "" width:1000 height:10 pos:[0,84] tooltip:"展开其它信息"
	Group ""
	(
		label lb_copyright "@Copyright 2011 \nPower By Yvi QQ:12319597" align:#left width:155 height:25
		HyperLink hlk_link "Click link" Color:Green Address:"http://www.macai.co.cc"

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
	
	
	on rl_mat close do
	(
		callbacks.removeScripts id:#Fn_MymtlRefAdded01
	)
	
)CreateDialog rl_mat style:#(#style_minimizebox, #style_titlebar,#style_border, #style_sysmenu)

callbacks.addScript #mtlRefAdded "Fn_MymtlRefAdded (callbacks.notificationParam())" id:#Fn_MymtlRefAdded01

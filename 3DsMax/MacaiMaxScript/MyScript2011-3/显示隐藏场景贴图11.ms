	function Yvi_showMat objs=
	(
		for obj in objs do
		(
			m = obj.Material
			if m == undefined then continue
			if(classof m == MultiMaterial)then
			(
				for i = 1 to m.numsubs do
				(
					showTextureMap m m[i] true
				)
			)
			if(classof m == Standard)then
			(
				showTextureMap m m true
			)
			gc()
			freeSceneBitmaps()
			--hide obj
		)
	)
	
	function Yvi_hideMat objs=
	(
		for obj in objs do
		(
			m = obj.Material
			if m == undefined then continue
			if(classof m == MultiMaterial)then
			(
				for i = 1 to m.numsubs do
				(
					showTextureMap m m[i] false
				)
			)
			if(classof m == Standard)then
			(
				showTextureMap m m false
			)
			gc()
			freeSceneBitmaps()
			--hide obj
		)
	)

try(DestroyDialog rl_hideOrShowMat)catch()
rollout rl_hideOrShowMat "ÏÔÊ¾/Òþ²ØÌùÍ¼" width:162 height:137
(
	button btn_show "Show" pos:[14,10] width:125 height:20
	button btn_hide "Hide" pos:[14,40] width:125 height:20
	checkbox chk_sel "in selection only." pos:[15,74] width:127 height:14
	label lbl1 "Copyright(c)2012 Power By Yvi    QQ:12319597" pos:[4,105] width:156 height:32

	on btn_show pressed do
	(
		if chk_sel.checked then
		(
			selObj = selection as array
			try(Yvi_showMat selObj)catch(throw())
		)
		else
		(
			allObj = Objects
			try(Yvi_showMat allObj)catch(throw())
		)
	)
	
	on btn_hide pressed do
	(
		if chk_sel.checked then
		(
			selObj = selection as array
			try(Yvi_hideMat selObj)catch(throw())
		)
		else
		(
			allObj = Objects
			try(Yvi_hideMat allObj)catch(throw())
		)
	)
)
CreateDialog rl_hideOrShowMat

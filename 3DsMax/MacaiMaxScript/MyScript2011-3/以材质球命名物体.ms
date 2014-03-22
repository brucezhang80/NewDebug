
try(DestroyDialog ReName)catch()
rollout ReName "以材质球命名物体" width:206
(
	group "命名物体："
	(
		button btn_Mater2Obj"以材质球命名物体" pos:[6,20] width:140 height:24 toolTip:"将材质球名字匹配到物体的名字"
		HyperLink hlk_info "." pos:[151,25] width:60 color:RED enabled:false
		label lab1 "" pos:[8,50] style_sunkenedge:true width:194 height:16
		
		
	)
	group "命名材质"
	(
		button btn_Obj2Mater"以物体命名材质球" pos:[6,95] width:140 height:24 toolTip:"将材质球名字匹配到物体的名字"
		HyperLink hlk_info2 "." pos:[151,100] width:60 color:RED enabled:false
		label lab2 "" pos:[8,126] style_sunkenedge:true width:194 height:16
	)
	on btn_Mater2Obj pressed do
	(
		try
		(
			for o in geometry where o.material != undefined do 
			( 
				old_name = o.name
				o.name = o.material.name
				lab1.text = "Old: "+old_name + " -> " + "New: "+ o.name
				format "Old Objects Name:%   New Object name: %   Material name: % \n" old_name o.name o.material.name
				sleep 0.2
			)
			hlk_info.caption = "completed!"
		)catch(MessageBox "未知的系统错误……")
	)

	on btn_Obj2Mater pressed do
	(
		--try
		(
			undo on
			(
				for o in geometry where o.material != undefined do 
				( 
					if classof o.Material == standardMaterial then
					(
						myMaterial = o.Material -- medit.GetCurMtl()
						numberOfSubMats = myMaterial.numsubs
						print numberOfSubMats
						for i = 1 to numberOfSubMats do
						(
							try
							(
								myMaterial[i].name = o.name + "_"+ i as string
								print myMaterial[i].name
								--myMaterial.names[i] = o.name 
							)
							catch 
							(
								--print ("can no name:" + myMaterial[i].name)
							)
						)
					)
					else
					(
						
					)
				)
				hlk_info2.caption = "completed!"
			)
		)--catch(MessageBox "未知的系统错误……")
	)
	
)
createdialog ReName style:#(#style_minimizebox, #style_titlebar, \
							#style_border, #style_sysmenu,#style_sunkenedge,\
							#style_resizing)

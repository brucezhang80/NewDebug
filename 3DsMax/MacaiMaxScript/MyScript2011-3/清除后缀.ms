
try destroydialog del catch()
rollout del "清除后缀" width:250 height:140
(
	button btn_del "清除后缀" pos:[48,34] width:153 height:53
	
	on btn_del pressed  do
	(
		max select all
		
		for obj in selection do

		(

			--获得当前物体的材质

			myMaterial = obj.Material -- medit.GetCurMtl()
			if myMaterial == undefined then 
			(
				continue
			)

			--获得当前材质存在的材质数

			numberOfSubMats = myMaterial.numsubs

			for i = 1 to numberOfSubMats do

			(
				try
				(

					--获取子材质名不带后缀

					bitmapName = getFilenameFile myMaterial[i].diffusemap.filename

					--去掉材质名后缀

					myMaterial[i].name = bitmapName

					--print myMaterial[i].name

					myMaterial.names[i] =bitmapName

					--print myMaterial.names[i]

				)

				catch 
				(

					print ("can no name:" + myMaterial[i].name)

				)

			)
			messagebox "完成"

		)

		max select none
		)
)

createdialog  del
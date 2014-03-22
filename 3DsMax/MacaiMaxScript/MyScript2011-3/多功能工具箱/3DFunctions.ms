
global Fn_deleteLM
global Fn_yyShowInViewport
global Fn_CloseLM
global Fn_OpenLM
global Fn_collectSubMats
global Fn_handleAllSubMats
global Fn_fixLightingMapNameExt
global Fn_test
-------------------------function
	/*2011.10.20号前未修改代码，添加了一个opacityMap通道，并开了自发光*/
	/*修改代码（注释掉的）去掉添加的材质并关掉自发光*/
	fn Fn_yyShowInViewport = 
	(
		local sel = #()
		sel = Selection as Array
		for theNode in sel where ((superClassOf theNode) == GeometryClass) do
		(
			local theMaterial = theNode.material
			
			if ((classOf theMaterial) == Standard) then
			(    
				--theMaterial.selfIllumAmount = 0 --100 关掉自发光
				if ((classOf theMaterial.diffuseMap) == BitmapTexture) then
				(
					/*theMaterial.opacityMap = theMaterial.diffuseMap*/--去透明通道
					--theMaterial.opacityMap.monoOutput = 1
					/*2011.10.20新加*/
					theMaterial.shaderType = 1
					theMaterial.adTextureLock = on
					showTextureMap theMaterial on
					/**/
				)
				showTextureMap theMaterial true
			)
			/*
			else if ((classOf theMaterial) == MultiMaterial) then
			(
				for theSubMat in theMaterial.materialList where ((classOf theSubMat) == Standard) do
				(
					theSubMat.selfIllumAmount = 0 --100 关掉自发光

					if ((classOf theSubMat.diffuseMap) == BitmapTexture) then
					(
						--theSubMat.opacityMap = theSubMat.diffuseMap--去透明通道
						theSubMat.opacityMap.monoOutput = 1
					)

					showTextureMap theSubMat true
				)
			)
			--这是处理标准材质的。不需要判断多维材质处理
			*/
		)
		--clearselection()
		updateToolbarButtons()

	)
---------------------------------------------
--         清除选中物体的LightingMap       --
--              2011.9.30 杨祎             --
---------------------------------------------
	fn Fn_deleteLM mat = 
	(
		if ClassOf mat == Multimaterial then 
		(
			for m in mat do 
			(
				Fn_deleteLM m
			)
			--mat.numsubs = 0
		)
		else if (ClassOf mat == Standard) then 
		(
			selName = mat.name
			if mat.selfillummap != undefined then
			(
				mat.selfillummap = undefined
				mat.name = selName
			)
		)
		mat = undefined
	)
---------------------------------------------
--         关闭\打开选中物体的自发光             --
--              2011.9.30 杨祎             --
---------------------------------------------
	function Fn_CloseLM = 
	(
		local sel = #()
		sel = Selection as Array
		if sel.count == 0 then 
		(
			Messagebox "请先选择一个以上的物体！"title:"错误！"
			return 0
		)
		for theNode in sel do --where ((superClassOf theNode) == GeometryClass) do
		(
			local theMaterial = theNode.material
			
			if ((classOf theMaterial) == Standard) then
			(    
				theMaterial.selfIllumAmount = 0 --100 
				theMaterial.useSelfIllumColor = off--关掉自发光
				showTextureMap theMaterial true

			)
			else if ((classOf theMaterial) == MultiMaterial) then
			(
				for theSubMat in theMaterial.materialList where ((classOf theSubMat) == Standard) do
				(
					theSubMat.selfIllumAmount = 0 --100 
					theSubMat.useSelfIllumColor = off--关掉自发光
					showTextureMap theSubMat true
					
				)
			)
		)
		--clearselection()
		updateToolbarButtons()
		Messagebox "自发光已经关闭完成！" title:"完成"
	)
	
	---- 打开自发光
	function Fn_OpenLM = 
	(
		local sel = #()
		sel = Selection as Array
		if sel.count == 0 then 
		(
			Messagebox "请先选择一个以上的物体！"title:"错误！"
			return 0
		)
		for theNode in sel do --where ((superClassOf theNode) == GeometryClass) do
		(
			local theMaterial = theNode.material
			
			if ((classOf theMaterial) == Standard) then
			(    
				theMaterial.selfIllumAmount = 100 
				theMaterial.useSelfIllumColor = on--打开自发光
				showTextureMap theMaterial true

			)
			else if ((classOf theMaterial) == MultiMaterial) then
			(
				for theSubMat in theMaterial.materialList where ((classOf theSubMat) == Standard) do
				(
					theSubMat.selfIllumAmount = 100 
					theSubMat.useSelfIllumColor = on--打开自发光
					showTextureMap theSubMat true
					
				)
			)
		)
		--clearselection()
		updateToolbarButtons()
		Messagebox "自发光已经打开！" title:"完成"
	)
	
	
---------------------------------------------
--                 材质贴图处理             --
--              2011.12.30 杨祎             --
---------------------------------------------
	---------贴图名处理
	function Fn_collectSubMats mat mats = 
	(
		if ( classof mat == Multimaterial) then 
		(
			for i = 1 to mat.numsubs do
			(
				Fn_collectSubMats mat[i] mats
			)
		)
		else if(classof mat == standard) then 
		(
			append  mats mat
		)
		else
		(
			messagebox ("出现了不是多维材质或者标准材质的材质 "+ mat.name)
		)
		return mats
	)
	-- 
	function Fn_handleAllSubMats obj =
	(
		local allsubmats = #()
		--这里清除全局数组
		g_oldfilenames=#()
		g_newfilenames=#()
		
		mat = obj.material
		if mat == undefined then return 0
		allsubmats = Fn_collectSubMats  mat allsubmats
		for mat in allsubmats do 
		(
			if mat.diffuseMap == undefined then 
			(
				messagebox ("物体: "+ obj.name + "中的材质: "+ mat.name as string +" 错误或者没有 Diffuesmap --已略过...") title:"注意"
				format "物体: % 中的材质: % 错误或者没有 \"Diffuesmap\" --已略过...\n" obj.name mat.name as string
				continue
			)
			filepathname = mat.diffuseMap.fileName
			--获取文件名字，带后缀
			filename = filenameFromPath filepathname
			-- 获得贴图路径
			filepath = getFilenamePath filepathname
			--获取文件后缀
			ext = getfilenametype filename
			
			--查找在数组里面是否已经存在这个文件名
			local index = findItem g_oldfilenames filename
			if (index == 0) then 
			(
				--这里修改贴图的名字，就是文件名字，不要后缀
				mat.diffusemap.name = getFileNameFile filename --+ "_temp"
				mat.name  = mat.diffusemap.name
			)
			try
			(
				if(mat.opacityMap != undefined)then
				(
					mat.opacityMap.filename = mat.diffusemap.filename
					mat.opacityMap.name = mat.diffusemap.name
				)
			)catch(format "non-OpacityMap...")
		)
	)
	
---------------------------------------------
--        修改自发光贴图后缀的函数           --
--              2011.12.31 杨祎            --
---------------------------------------------
	--配合收集材质的函数使用
	function Fn_fixLightingMapNameExt obj exName ext =
	(
		local allsubmats = #()

		mat = obj.material
		if mat == undefined then return 0
			
		allsubmats = Fn_collectSubMats  mat allsubmats
		
		for mat in allsubmats do 
		(
-- 			try
-- 			(
				if(mat.SelfIllumMap != undefined)then
				(
					lmName = (getFilenameFile mat.SelfIllumMap.fileName)
					lmpath = (getFilenamePath mat.SelfIllumMap.fileName)
					index = findString lmName "LightingMap"
					if index == 0 then --找不到 名字有 "LightingMap", 添加进去
					(
						mat.SelfIllumMap.filename = lmpath + lmName + exName + "." + ext
						mat.SelfIllumMap.name = lmName
					)
					else-- 找到有不添加
					(
						exName = ""
						mat.SelfIllumMap.filename = lmpath + lmName + exName + "." + ext
						mat.SelfIllumMap.name = lmName
					)
				)
-- 			)catch
-- 			(
-- 				format "\nnon-LightingMap..."
-- 				throw()
			--)
		)
	)
	
	function Fn_test =
	(
		MessageBox "KKKK"
	)
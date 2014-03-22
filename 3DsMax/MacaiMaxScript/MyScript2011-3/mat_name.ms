-- 1.	材质球名 == 贴图名(无后缀)
-- 2.	多维材质的子材质的命名显示:
-- 3.	优化材质时保持对象选择 (X)
-- 4.	一个对象一个多维,多维材质不嵌套多维材质.仅标准材质

--------------------------------------------------------------------------------------------------------
---------贴图名处理
--这里仅收集多维子材质
function Fn_collectSubMultiMats arr_mats mat = 
(
	if (classof mat  == Multimaterial) then 
	(
		for i = 1 to mat.numsubs do 
		(
			Fn_collectSubMultiMats arr_mats mat[i]
		)
	)
	--收集多维子材质中的材质
	else if (classof mat == MultiSubMaterial) then
	(
		format "发现存在多维子材质: %"  mat.name
		for j = 1 to mat.numsubs do
		(
			Fn_collectSubMultiMats arr_mats mat[i]
		)
		--收集完这个多维子材质后,删除之
		--这个多维子材质内的标准材质将放到上一级多维材质中
		mat = undefined
	)
	else if(classof mat == Standard)then
	(
		append arr_mats mat
	)
)
	
--这里收集场景中的所有子材质,包括多维子材质中的材质
function Fn_collectAllSubMats = 
(
	allSubMats=#()
	for mat in scenematerials do 
	(
	   Fn_collectSubMultiMats allsubMats mat
	)
	
	return allSubMats
)

-- 重命名贴图
function Fn_handleAllSubMats allsubmats =
(
	oldfilenames = #()
	for mat in allsubmats do 
	(
		if mat.diffuseMap == undefined then 
		(
			messagebox ("出错了！ "+ mat.name as string +" 错误或者没有 Diffuesmap,请检查!")
			continue
		)
		filepathname = mat.diffuseMap.fileName
		--获取文件名字，带后缀
		filename = filenameFromPath filepathname
		-- 获得贴图路径
		filepath = getFilenamePath filepathname
		-- 获取文件后缀
		ext = getfilenametype filename
		
		-- 查找在数组里面是否已经存在这个文件名
		index = findItem oldfilenames filename
		if (index == 0) then 
		(
			append oldfilenames filename
			-- 这里修改贴图的名字，就是文件名字，不要后缀
			mat.diffusemap.name = getFileNameFile filename
			mat.name  = mat.diffusemap.name
		)
		try
		(
			if(mat.opacityMap != undefined)then
			(
				mat.opacityMap.filename= mat.diffusemap.filename
				mat.opacityMap.name = mat.diffusemap.name
			)
		)catch(format "non-OpacityMap...")
	)
)


try(DestroyDialog rl_No1MatName)catch()
Rollout rl_No1MatName "N.o.1_子材质修复"
(
	Button btn_MatRename "修复子材质是多维的"
	
	on btn_MatRename pressed do
	(
		local allsubMats = #()
		allsubMats = Fn_collectAllSubMats()
		print allsubMats
		
		Fn_handleAllSubMats allsubMats
	)
	
)
CreateDialog rl_No1MatName
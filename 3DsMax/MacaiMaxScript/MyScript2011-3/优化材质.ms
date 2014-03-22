--获取场景中物体所用的所有材质
--这些材质应当少于scenematerials
	fn GetAllMatUsedByScene = 
	(
		allmats=#()
		for obj in objects do (
			if obj.material!=undefined then (
				append allmats obj.material
				if classof obj.material==Multimaterial then (
					for x in obj.material.materialList do (
						if x!=undefined then
							append allmats x
					)
				)
			)
		)
		return allmats
	)
	
	fn clearUnusedMaterial = (
		
		allmats = GetAllMatUsedByScene()
		count=scenematerials.count
		for i =count to 1 by -1 do (
			index = findItem allmats (scenematerials[i])
			if index==0 then
			(deleteItem scenematerials i)
		)
		
		--把面板清空
		count = scenematerials.count
		for i =1 to meditmaterials.count do (
			if i<=count then
			(   
				setMeditMaterial i scenematerials[i] 
				--messagebox scenematerials[i].name 
			)
			else
			( 
				setMeditMaterial i (standardMaterial())
			)
		)
	)

	global errorfin
	fn openErrorFile = (
		errorfin = openfile "errorInfo.txt:" mode:"at"
		if (errorfin == undefined)do
		(
			createFile "errorInfo.txt"
			errorfin = openfile "errorInfo.txt" mode:"at"
		)

		if (errorfin == undefined)do
		(
			messagebox "打开文件失败！"
			exit
		)
	)         

        --把错误信息写到文件 
    fn writeErrorInfo info= 
	(	
		format info  to:errorfin
		format "\n"  to:errorfin
	)  
	
	fn   closeErrorFile = (
		 close errorfin	
	)
	
	   fn getTrueSubMatID  multiMat matID = (
		  -- print ("matID=" + (matID as string) )
		   if multiMat==undefined then
		   (  
		     -- messagebox "空材质?"
		      return matID
			)
		   if  not (classof multiMat == Multimaterial) then (
					return matID
			)
		    --当前子材质的文件名字
			
			if multiMat[matID]==undefined then (
				writeErrorInfo (multiMat.name+"有问题")
				return matID
			)
			
			if (classof (multiMat[matID]) == Multimaterial) then
			(
				 writeErrorInfo (multiMat.name+"的第"+(matID as string) +"个子材质是多维材质")
				return matID
			)
			
			IF multiMat[ matID ].diffusemap == undefined then
			 (
				 ss = multimat.name
				 ss1 = matID as string
				 writeErrorInfo ("材质"+ss+"的第"+ss1+ "子材质有问题")
				  return matID
				 )
			
		    curMatDiffuseMap = multiMat[ matID ].diffusemap.filename
			
			if (matID>1) and (matID<=multiMat.numsubs) then (
				for i = 1 to (matID-1) do 
				(
					if multiMat[i]==undefined then
						continue
					
					if multiMat[ i ].diffusemap==undefined then
						continue
					
					pre = multiMat[ i ].diffusemap.filename
					
					if (pre == curMatDiffuseMap) then
					(	
						--print ("newmatID=" + (i as string) )
						return i
					)
				)				
		    )
			else if(matID>multiMat.numsubs) then(
				print ("错误 matID=" + (matID as string) )
			)
			return matID
	   )
	   
	   --老ID 新ID
	   struct ID_NID(ID,nID)
	   
	   --这里我们获取一个新老ID数组
	   fn getTrueSubMatIDs  multiMat = (
		   IDS=#()
		   num = multiMat.numsubs
		   for i=1 to num do (
			  -- print "dl"
			   newID = getTrueSubMatID multiMat i
			  -- print "dl----------"
			   Id_id = ID_NID ID:i nID:newID
			   append IDS Id_id
		   )
		   return IDS
	   )
	
	   --处理同名材质:有可能存在这种情况
	   fn UpdateMeshMatID obj = (
		        
				curmat = obj.material
	
		        --只要处理多维材质
				if classof curmat != Multimaterial then (
					writeErrorInfo (obj.name+"的材质不是多维材质")
					return false 
				)
				
				 
				minMatID = 1
				maxMatID = curmat.numsubs
				
				--老ID对应每个新ID,形成一个数组
				IDS = getTrueSubMatIDs(curmat)
				
				--这里收集所有用到的子材质ID
				rr=true
				
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--这个面用的材质ID
					faceMatID = getFaceMatID obj faceIndex
					
					if faceMatID>maxMatID then
					(
					    errorstr = obj.name+"第"+(faceIndex as string)+"个面材质ID指定错误,指定的材质ID为"+(faceMatID as String)
						writeErrorInfo errorstr  
						rr = false
                        continue						
					)
					
					if (faceMatID>IDS.count)  then
					(
						faceMatID = faceMatID
					)
					else (
						for ii in IDS do (
						     if (faceMatID==ii.ID) then
							       (
									   faceMatID == ii.nID
									   exit
								   )
						)
					)
					--getTrueSubMatID curmat faceMatID
					
					setFaceMatID obj faceIndex faceMatID
				)
				
				return rr
	   )
	   
	  fn deleteMaterial mat = (
		    if classof mat == Multimaterial then (
				for m in mat do (
				  deleteMaterial m
				)
				mat.numsubs = 0
			)
			else if (classof mat == Standard) then (
				mat.diffusemap = undefined
				mat.opacitymap = undefined
				mat.selfillummap = undefined
				mat.name = ""
			)
		
			
			mat = undefined
	   )

       fn handleMesh obj = (
		        rr = UpdateMeshMatID obj
			    if rr==false then (
					return undefined  
				)
		   
				
				newmat =  Multimaterial()
				curmat = obj.material
				newmat.name = obj.name
				


                 minMatID = 1
                 maxMatID = 1
				if classof curmat == Multimaterial then (
					maxMatID = curmat.numsubs
				)
				
                                  			
				--这里收集所有用到的子材质ID
				IDS =#()
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--这个面用的材质ID
					faceMatID = getFaceMatID obj faceIndex

					kk = findItem IDS faceMatID
					if kk==0 then 
					(
						a = faceMatID<minMatID
						b = faceMatID>maxMatID
						if ( a or b ) then (
							errorstr = obj.name+"第"+(faceIndex as string)+"个面材质ID指定错误,指定的材质ID为"+(faceMatID as String)
							writeErrorInfo errorstr
							return undefined
						)
						else        
							append IDS faceMatID
					)
				)
				--现在，用到的材质ID全部放在了IDS里面	

				--排序
				sort IDS  
				
				--设置新材质的子材质个数
				newmat.numsubs = IDS.count

				
				--复制原来的材质到新材质中
				for i=1 to IDS.count do (
				   if classof curmat == Multimaterial then (
					   if (curmat[ IDS[i] ]!=undefined) then 
					   (
							newmat[i] = copy curmat[ IDS[i] ]
							newmat[i].name = curmat[ IDS[i] ].name
					   )
					   else 
					   (
						   newmat[i] = standard()
						   newmat[i].name ="请指定贴图"
						   writeErrorInfo (curmat.name + "第"+(i as string)+"个子材质有问题")
						)
				   )
				   else 
				   (
					   newmat[i] = copy curmat
					   newmat[i].name = curmat.name
				   )
				   
				   if newmat[i].diffusemap==undefined then 
				   (
					   writeErrorInfo ("材质"+newmat[i].name+"错误，没有指定diffusemap" as string) 
				   )
				   else 
				  (
					 if newmat[i].diffusemap.filename.count==0 then 
						   writeErrorInfo ("材质"+newmat[i].name+"错误，没有指定diffusemap对应的贴图" as string)   
				  )
				)
				
				
				--面的材质ID应当有所变化
				numfaces = getNumFaces(obj)
				for faceIndex=1 to numfaces do 
				(
					oldID = getFaceMatID obj faceIndex
					--oldID = getTrueSubMatID curmat oldID
					
					--老ID对应的新ID
					newID = findItem IDS oldID
					
					setFaceMatID obj faceIndex newID
				)
				
				--先把新材质赋值过去
				obj.material = newmat
				
				--删除老材质
				deleteMaterial curmat
       )

	--删除不需要的子材质
	fn clearUnusedSubMaterial = (
    
		for obj in objects do (
			writeErrorInfo ("开始处理物体"+obj.name)
			if obj.material==undefined then
			(
			    writeErrorInfo ("物体"+obj.name+"没有材质" as string)
				
				mymesh = converttomesh obj
				if mymesh==undefined then 
				(
					writeErrorInfo ("物体"+obj.name+"不能转变成mesh" as string)
					delete obj
				)
				
				continue
			)
			
			if classof obj == Editable_mesh then
			(
				handleMesh obj
				--messagebox obj.name
			)
			else
			(
				myname = obj.name
				mymesh = converttomesh obj
				if mymesh!=undefined then 
				(
					mymesh.name = obj.name
					handlemesh mymesh   
				)
				else (
					writeErrorInfo ("物体"+obj.name+"不能转变成mesh" as string)
					delete obj
				)
			)
			writeErrorInfo ("---------------------------")
		)
		
		--最后，完全重画
		completeRedraw()
	)
	

	--fn collectAllSubMats = 
	--(
	--	allSubMats=#()
	--	for m=1 to scenematerials.count do 
	--	(
	--	    mat = sceneMaterials[m]
	--		if classof mat == Multimaterial do (
	--			for submat in sceneMaterials[m] do		
	--			    append allsubmats submat				 
	--		)
	--		if classof mat  == standard do (
	--				append allSubMats mat
	--		)
	--	)		
		
	--	for smat in allSubMats do (
									--删除环境贴图
	--		if smat!=undefined then (
	--			smat.ambientmap=undefined
	--		)
	--	)
			
	--	return allSubMats
	--)
	
	fn CollectSubMats mat mats = 
	(
		if mat == undefined then return undefined
	     if ( classof mat  == Multimaterial) then (
		      for i=1 to mat.numsubs do (
				 collectSubMats mat[i] mats
			  )
		 )
		 else if(classof mat == standard) then (
			 append  mats mat
		 )
		 else (
		     writeErrorInfo ("出现了不是多维材质或者标准材质的材质"+mat.name)
		 )
	)
	
	--这里收集场景中的所有子材质
	fn collectAllSubMats = (
		allSubMats=#()
		for mat in scenematerials do 
		(
		   CollectSubMats mat allsubMats
		)
		
		for smat in allSubMats do (
									--删除环境贴图
			if smat!=undefined then (
				smat.ambientmap=undefined
			)
		)
		
		return allSubMats
	)
	
	
	fn main = 
	(
			allSubMats = #()
			allSubMats = collectAllSubMats()
			clearUnusedMaterial()
			print("clearUnusedMaterial")
			clearUnusedSubMaterial()
			print("clearUnusedSubMaterial end")
	)
	
	openErrorFile()
	main()
	closeErrorFile()
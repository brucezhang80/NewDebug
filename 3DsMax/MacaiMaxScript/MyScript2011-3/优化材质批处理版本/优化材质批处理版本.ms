--获取场景中物体所用的所有材质
--这些材质应当少于scenematerials
	fn GetAllMatUsedByScene = 
	(
		allmats=#()
		max select all
		for obj in Selection do (
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
		max select none
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

        global errorInfoFile="errorInfo.txt"
         
        --把错误信息写到文件 
        fn writeErrorInfo info= 
	(	
		centerFile = openfile "errorInfo.txt:" mode:"at"
		if (centerFile == undefined)do
		(
			createFile "errorInfo.txt"
			centerFile = openfile "errorInfo.txt" mode:"at"
		)
		
		if (centerFile == undefined)do
		(
			messagebox "打开文件失败！"
			exit
		)
		
		format info  to:centerFile
		format "\n"  to:centerFile
		close centerFile			
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
			--return matID
			IF multiMat.materialList[ matID ].diffusemap == undefined then
			 (
				 ss = multimat.name
				 ss1 = matID as string
				 writeerrorinfo  ("材质"+ss+"的第"+ss1+ "子材质有问题")
				  return matID
				 )
		    curMatDiffuseMap = multiMat.materialList[ matID ].diffusemap.filename
			--messagebox curmatdiffusemap
		   
			if matID>1 then (
				for i = 1 to (matID-1) do (
					if multiMat.materialList[ i ].diffusemap==undefined then
						continue
					
					pre = 	 multiMat.materialList[ i ].diffusemap.filename
					--messagebox pre
					if (pre == curMatDiffuseMap) then
					(	
						   --messagebox  pre 
						print ("newmatID=" + (pre as string) )
							return i
					)
				)				
		    )
			return matID
	   )
	
	   --处理同名材质:有可能存在这种情况
	   fn UpdateMeshMatID obj = (
		    
				curmat = obj.material
 
		        --只要处理多维材质
				if classof curmat != Multimaterial then (
					return undefined 
				)
				 
				--这里收集所有用到的子材质ID
				IDS =#()
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--这个面用的材质ID
					faceMatID = getFaceMatID obj faceIndex
					faceMatID = getTrueSubMatID curmat faceMatID
					setFaceMatID obj faceIndex faceMatID
				)
	   )
	   

       fn handleMesh obj = (
		   UpdateMeshMatID obj
		   --messagebox "UpdateMeshMatID finished"
				newmat =  Multimaterial()
				curmat = obj.material
				--oldname = curmat.name
				--curmat.name = oldname+"temp"
				newmat.name = obj.name

                 minMatID = 1
                 maxMatID = 1
				if classof curmat == Multimaterial then (
					maxMatID = curmat.numsubs
				)
				
				--messagebox (minMatID as string)
                                  				

				--这里收集所有用到的子材质ID
				IDS =#()
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--这个面用的材质ID
					faceMatID = getFaceMatID obj faceIndex
					--messagebox (faceMatID as string)
					
					--faceMatID = getTrueSubMatID curmat faceMatID
					--if faceMatID!=faceMatID0 then
					--	messagebox "有同名子材质"

					--messagebox (faceMatID as string)
					kk = findItem IDS faceMatID
					if kk==0 then 
					(
						a = faceMatID<minMatID
						b = faceMatID>maxMatID
						if ( a or b ) then (
							--errorstr = obj.name+"第"+(faceIndex as string)+"个面材质ID指定错误,指定的材质ID为"+(faceMatID as String)
							--messagebox errorstr
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
				       newmat.materialList[i] = copy curmat.materialList[ IDS[i] ]
					   newmat.materialList[i].name = curmat.materialList[ IDS[i] ].name

				   )
				   else (
					   newmat.materialList[i] = copy curmat
					   newmat.materialList[i].name = curmat.name
				   )
                                       if newmat.materialList[i].diffusemap==undefined then 
                                       (
                                           writeErrorInfo ("材质"+newmat.materialList[i].name+"错误，没有指定diffusemap" as string) 
                                       )
                                       else 
                                          (
                                             if newmat.materialList[i].diffusemap.filename.count==0 then 
                                                   writeErrorInfo ("材质"+newmat.materialList[i].name+"错误，没有指定diffusemap对应的贴图" as string)   
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
				index = findItem scenematerials curmat
				for i = 1 to scenematerials.count do
				(
					if i == index then 
					(
						deleteItem scenematerials index
						break
					)
				) 
       )

	   
	
         

	--删除不需要的子材质
	fn clearUnusedSubMaterial = (
	--选择这个max的所有对象
		max select all
           
		for obj in Selection do (
			if obj.material==undefined then
			(
			    writeErrorInfo ("物体"+obj.name+"没有材质" as string)
			    exit
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
                                if mymesh!=undefined then (
                                mymesh.name = obj.name
                                handlemesh mymesh   
                                )
			)
		)
		
		--最后，完全重画
		completeRedraw()
		print("completeRedraw done")
	)
	

	fn collectAllSubMats = 
	(
		allSubMats=#()
		for m=1 to scenematerials.count do 
		(
		    mat = sceneMaterials[m]
			if classof mat == Multimaterial do (
				for submat in sceneMaterials[m] do		
				    append allsubmats submat				 
			)
			if classof mat  == standard do (
					append allSubMats mat
			)
		)		
		return allSubMats
	)
	
	global allSubMats = #()
	
	-----------------------------------------------
	global allmaxFileNames =#()

allmaxFilename=openFile "maxFiles.txt"
while not(eof allmaxFilename) do(
	    line = readLine allmaxFilename
        append allmaxFileNames line
)
close allmaxFilename

for maxfile in allmaxFileNames do
( 
	loadmaxfile maxfile
	
	writeErrorInfo (maxfile)
	
	max select all

	allSubMats = collectAllSubMats()
	
	clearUnusedMaterial()	
	clearUnusedSubMaterial()
	
	savemaxfile maxfile
	
	resetMaxFile #noPrompt
	
	writeErrorInfo ("\n")
)

quitMax #noPrompt
	
	
	
	--savemaxfile (maxFilePath+maxfilename)
	
	
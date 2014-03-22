
--在max7下面，没有toUpper这个函数,因此我们自己写一下这个函数
	fn MyToUpper str = (
	   for  x=1 to str.count do (
	        i = str[x] as string
			if i=="a" then  str[x]= "A"
			if i=="b" then  str[x]= "B"
			if i=="c" then  str[x]= "C"
			if i=="d" then  str[x]= "D"
			if i=="e" then  str[x]= "E"
			if i=="f" then  str[x]= "F"
			if i=="g" then  str[x]= "G"
			if i=="h" then  str[x]= "H"
			if i=="i" then  str[x]= "I"
			if i=="j" then  str[x]= "J"
			if i=="k" then  str[x]= "K"
			if i=="l" then  str[x]= "L"
			if i=="m" then  str[x]= "M"
			if i=="n" then  str[x]= "N"
			if i=="o" then  str[x]= "O"
			if i=="p" then  str[x]= "P"
			if i=="q" then  str[x]= "Q"
			if i=="r" then  str[x]= "R"
			if i=="s" then  str[x]= "S"
			if i=="t" then  str[x]= "T"
			if i=="u" then  str[x]= "U"
			if i=="v" then  str[x]= "V"
			if i=="w" then  str[x]= "W"
			if i=="x" then  str[x]= "X"
			if i=="y" then  str[x]= "Y"
			if i=="z" then  str[x]= "Z"
	   )
	   return str
	)
	
	
	--在max7下面，没有toUpper这个函数,因此我们自己写一下这个函数
	fn MyToLower str = (
	   for  x=1 to str.count do (
	        i = str[x] as string
			if i=="A" then  str[x]= "a"
			if i=="B" then  str[x]= "b"
			if i=="C" then  str[x]= "c"
			if i=="D" then  str[x]= "d"
			if i=="E" then  str[x]= "e"
			if i=="F" then  str[x]= "f"
			if i=="G" then  str[x]= "g"
			if i=="H" then  str[x]= "h"
			if i=="I" then  str[x]= "i"
			if i=="J" then  str[x]= "j"
			if i=="K" then  str[x]= "k"
			if i=="L" then  str[x]= "l"
			if i=="M" then  str[x]= "m"
			if i=="N" then  str[x]= "n"
			if i=="O" then  str[x]= "o"
			if i=="P" then  str[x]= "p"
			if i=="Q" then  str[x]= "q"
			if i=="R" then  str[x]= "r"
			if i=="S" then  str[x]= "s"
			if i=="T" then  str[x]= "t"
			if i=="U" then  str[x]= "u"
			if i=="V" then  str[x]= "v"
			if i=="W" then  str[x]= "w"
			if i=="X" then  str[x]= "x"
			if i=="Y" then  str[x]= "y"
			if i=="Z" then  str[x]= "z"
	   )
	   return str
	)
----------------------------------------------------------------------------
	
	global g_tilefilename = ""
	
	global g_bitmapfilepath =#()
	
	--定义全局的数组，用来保存文件名字
	global g_oldfilenames=#()
	global g_newfilenames=#()
	
	global g_old_selfillummap_filenames=#()
	global g_new_selfillummap_filenames=#()
	
	global g_old_opacitymap_filenames=#()
	global g_new_opacitymap_filenames=#()

	--获取Bitmap和Xref路径，对应3ds max里Configure Paths对话框里Bitmaps和Xref选项卡
	fn GetAllBitmapPaths = 
	(
		allSubPaths=#()
		num = mappaths.count()
		for m=1 to num do (
		     path2 = mapPaths.get(m)
			append allSubPaths path2
		)
		
		return allSubPaths
	)
	
	fn findImageTruePath imagefname = (
	   imgpaths = GetAllBitmapPaths()
	   --裸文件名字
	   s3 = filenameFromPath imagefname 
	   for x in imgpaths do (
	       fname = x+"\\"+s3
		   fname = mytolower fname
	        bexistfile  =  doesFileExist fname 
		  -- print fname 
		 --  print bexistfile
		   if bexistfile==true then
		   (   
		      -- messagebox ( ("找到文件"+fname) as string )

		      return fname
	      )
	   )
	   print ( ("找不到文件"+imagefname) as string)
	   return ""
	)
	
	fn GetBitmapFilePath bitmapfile =
	(
		return getFilenamePath bitmapfile
	)
	
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
	
	fn clearUnusedMaterial = 
	(
		
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
	
	--删除不需要的子材质
	fn clearUnusedSubMaterial = (
	   --选择这个max的所有对象
		max select all

		for obj in Selection do (
			if obj.material==undefined then
			(
				messagebox ("物体"+obj.name+"没有材质" as string)
			    exit
			)
			
			
			if classof obj == Editable_mesh then
			(
				newmat =  Multimaterial()
				curmat = obj.material
				oldname = curmat.name
				curmat.name = oldname+"temp"
				newmat.name = oldname
				
				--这里收集所有用到的子材质ID
				IDS =#()
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--这个面用的材质ID
					faceMatID = getFaceMatID obj faceIndex
					
					kk = findItem IDS faceMatID
					if kk==0 then (
						append IDS faceMatID
					)
				)
				--现在，用到的材质ID全部放在了IDS里面	
				--排序
				sort IDS  
				
				--设置新材质的子材质个数
				newmat.numsubs = IDS.count

				--messagebox "dslf1"
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
				)
				--messagebox "dslf12"
				
				--面的材质ID应当有所变化
				numfaces = getNumFaces(obj)
				for faceIndex=1 to numfaces do 
				(
					oldID = getFaceMatID obj faceIndex
					
					--老ID对应的新ID
					newID = findItem IDS oldID
					
					setFaceMatID obj faceIndex newID
				)
				
				--先把新材质赋值过去
				obj.material = newmat
				
				--删除老材质
				index = findItem scenematerials curmat
				deleteItem scenematerials index
			)
			else
			(
				messagebox "不是mesh"
			)
		)
		
		--最后，完全重画
		completeRedraw()
		
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
	
	fn isTGA filetype = 
	(
         return getFilenameType filetype == ".tga" or getFilenameType filetype == ".TGA"
	)

	fn isShadowMap filename=
	(
		filename0 = MyToLower(getFilenameFile filename)
		strcom = (findstring filename0 "completemap")
		strlig = (findstring filename0 "lightingmap")
		
		if(strcom != undefined )then
		(
			--print "存在有*completemap.tga贴图！"
			return true
		)
		else if (strlig != undefined)then
		(
			--print "存在有*lightingmap.tga贴图！"
			return true
		)
		else return false
	)
	
	fn HandleDiffuseMap mat =
	(
		--处理diffusemap
		
		filepathname = mat.diffusemap.filename
		if filepathname==undefined then 
				print ("filepathname:"+filepathname)	
		
		--获取子材质名带后缀
		bitmapName = filenameFromPath filepathname
		
		------------------------------------------
		--如果不是阴影贴图，我们先查找是否阴影贴图里面内涵字符串 _alpha
		  --如果不包含，表示我们的名称有错，我们需要改名字
		  --如原来是:xx.tga,改成xx_alpha.tga
		  --如果文件名已经含了_alpha，表示我们命名正确，我们不理了		   
		------------------------------------------
		flag = isShadowMap bitmapName
		bTGA =  isTGA  bitmapName
		
		if((not flag) and bTGA )then
		(
			 --messagebox "dfs"
			 alphastr = (findstring bitmapName "_alpha") 
			
			--文件名字不含_alpha
			if(alphastr ==undefined ) then
			(
				 --保存原始文件名字
				oldname  = findImageTruePath  bitmapName
				append g_oldfilenames oldname
				oldpath = getFilenamePath oldname
				
				filename0 = ((getFileNameFile oldname) + "_alpha" + (getFilenameType oldname))
				newfilename = MyToUpper filename0
				
				 --保存对应的新文件名字
				append g_newfilenames  newfilename 
				
				--修改材质名,变成大写
				mat.diffusemap.filename = oldpath + newfilename
				mat.diffusemap.name =  (getFileNameFile newfilename ) + ".tga"
				mat.name  = mat.diffusemap.name

					--print ("mat.diffusemap.name(not 'compeletemap and lightingmap':" + mat.diffusemap.name)
				--这里同步修改 透明贴图
				if mat.opacitymap!=undefined then 
				(
				   opmap = mat.opacitymap
				   opmap.filename = mat.diffusemap.filename
				   opmap.name = mat.diffusemap.name
				)  
				
				--messagebox oldname
				--messagebox newfilename 
			)
		)
		else if((flag) and bTGA)then--找到存在completemap和lightingmap，转成jpg
		(
			--如果是阴影贴图，文件名去一次"_alpha"，防止文件名里面不小心带上了_alpha
			--文件名改后缀成jpg 
			--文件本身去alpha通道
			
			--如果包含有_alpha
			--那么，去掉_alpha字串
			srcfname = MyToLower (findImageTruePath bitmapName)
			removestr = "_alpha"
			tonewname = ""
			noalphaname = (substituteString srcfname removestr tonewname)

			tgastr = (findstring noalphaname ".tga") 
			if(tgastr !=undefined)do
			(
				--调用外部程序把tga格式的贴图转换到jpg格式 
				cmd = "D:\\tga2jpg.exe"+ " "+noalphaname
				DOSCommand cmd
			)
			
			--将jpg贴图重新附上
			oldname  =getFilenameFile bitmapName
			newfilename = MyToLower oldname
			
			--修改材质名
			mat.diffusemap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
			mat.diffusemap.name =  (getFileNameFile newfilename ) + ".jpg"
			mat.name  = mat.diffusemap.name

		)
	)
	
	--透明贴图
	fn Handleopacitymap  mat =
	(
		--处理opacitymap
		filepathname = mat.opacitymap.filename
		if(filepathname!=undefined) then
		(
			--print ("opacitymap filepathname:"+filepathname)	
			
			--获取子材质名带后缀
			bitmapName = filenameFromPath filepathname

			flag = isShadowMap bitmapName--找不到completemap和lightingmap，返回flag=false
			bTGA =  isTGA  bitmapName
			
			if( flag and bTGA )then--找到存在completemap和lightingmap，转成jpg
			(
				srcfname = findImageTruePath bitmapName
				tgastr = (findstring srcfname ".tga") --前面已经转成jpg。以防cmd提示输入文件名停下等待
				if(tgastr !=undefined)do
				(
					--调用外部程序把tga格式的贴图转换到jpg格式 
					cmd = "D:\\tga2jpg.exe"+ " "+srcfname
					DOSCommand cmd
				)
				
				--将jpg贴图重新附上
				oldname  =getFilenameFile bitmapName
				newfilename = MyToLower oldname
				
				--修改材质名
				mat.opacitymap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
				mat.opacitymap.name =  (getFileNameFile newfilename ) + ".jpg"
				mat.name  = mat.opacitymap.name
					
				print ("这是mat.opacitymap.name 和 mat.opacitymap.filename： " +mat.opacitymap.name+" " +mat.opacitymap.filename)
				
			)
			else if (flag and (not bTGA)) then
			(
				--alphastr = (findstring bitmapName "_alpha") 
				
				--文件名字不含_alpha
				--if(alphastr ==undefined ) then
				--(
					srcfname = findImageTruePath bitmapName
					
					--将jpg贴图重新附上
					oldname  = filenameFromPath  srcfname-- returns: "myImage.jpg"
					newfilename = MyToLower oldname
					
					--修改材质名
					mat.opacitymap.filename = (maxFilePath +"maps\\") + newfilename --+ ".jpg"
					mat.opacitymap.name =  (getFileNameFile newfilename ) + ".jpg"
					mat.name  = mat.opacitymap.name
				
				print ("这是mat.opacitymap.name 和 mat.opacitymap.filename： " +mat.opacitymap.name+ " " +mat.opacitymap.filename)
				--)
			)
		)
	)
	
	--自发光贴图
	fn Handleselfillummap  mat =
	(
		--处理selfillummap
		filepathname = mat.selfillummap.filename
		if (filepathname!=undefined )then
		(
			--print ("selfillummap filepathname:"+filepathname)	
		
			--获取子材质名带后缀
			bitmapName = filenameFromPath filepathname

			flag = isShadowMap bitmapName----找不到completemap和lightingmap，返回flag=false
			bTGA =  isTGA  bitmapName
			
			if( flag and bTGA )then--找到存在completemap和lightingmap，转成jpg
			(
				
				srcfname = findImageTruePath bitmapName
				tgastr = (findstring srcfname ".tga") 
				if(tgastr !=undefined)do
				(
					--调用外部程序把tga格式的贴图转换到jpg格式 
					cmd = "D:\\tga2jpg.exe"+ " "+srcfname
					DOSCommand cmd
				)
				
				--将jpg贴图重新附上
				oldname  =getFilenameFile bitmapName
				newfilename = MyToLower oldname
				
				--修改材质名
				mat.selfillummap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
				mat.selfillummap.name =  (getFileNameFile newfilename ) + ".jpg"
				mat.name  = mat.selfillummap.name
				
				print ("这是mat.selfillummap.name 和 mat.selfillummap.filename： " +mat.selfillummap.name+" " + mat.selfillummap.filename)
			)
			else
			(
				 alphastr = (findstring bitmapName "_alpha") 
			
				--文件名字不含_alpha
				if(alphastr ==undefined ) then
				(
					 --保存原始文件名字
					oldname  = findImageTruePath  bitmapName
					oldpath = getFilenamePath oldname
					
					filename0 = (filenameFromPath oldname) --+ "_alpha" + (getFilenameType oldname))
					newfilename = MyToUpper filename0
					
					--修改材质名,变成大写
					mat.selfillummap.filename = oldpath + newfilename
					mat.selfillummap.name =  (getFileNameFile newfilename )+ ".jpg"
					mat.name  = mat.selfillummap.name
				
					print ("这是mat.selfillummap.name 和 mat.selfillummap.filename： " +mat.selfillummap.name+" " + mat.selfillummap.filename)
				)
			)
		)
	)
	

	
	fn ChangeAllSubMatsFilePathAndName allSubMats = 
	(
		tilepath = (maxfilepath+"maps\\")--???		
		--tilepath = (g_bitmapfilepath[1])		
		for mat in allSubMats do
		(
			--diffusemap			
		    if mat.diffusemap==undefined then (
			   --print ("材质"+mat.name+"缺少diffuse贴图") 
		    )
		    else (
			     HandleDiffuseMap  mat
			)
			
			--selfillummap
			if mat.selfillummap==undefined then (
			   --print ("材质"+mat.name+"缺少selfillummap贴图") 
			
			)
			else
			(
			    Handleselfillummap  mat
			)
			
			--opacitymap
			if mat.opacitymap==undefined then (
			  -- print ("材质"+mat.name+"缺少opacitymap贴图") 
			   
			)
			else
			(
			    Handleopacitymap  mat
			)
			
			
		)
	)
	
	--获取带路径的文件名字
	fn GetFullPathName newpath  filename0 = 
	(
		filename = MyToUpper filename0
		s = newpath+filename		
		return s
	)
	
	--处理硬盘上的图片文件，我们认为所有文件都在当前目录下
	fn HandleHardDiskFiles = 
	(
		path1 = getFilenamePath g_tilefilename--..\maps的路径
		
		for i=1 to g_oldfilenames.count do 
		(
		     --messagebox ("old_file" + g_oldfilenames[i])
			old0  = GetFullPathName path1 (g_oldfilenames[i])
			old1  = findImageTruePath old0	

			new  =  GetFullPathName path1 (g_newfilenames[i])
			--print ("new_file" + new)
			
			renameFile old1  new
		)
		
		--selfillummap_filenames
		for i=1 to g_old_selfillummap_filenames.count do 
		(
			--print ("old_file" + g_old_selfillummap_filenames[i]);
			old0  = GetFullPathName path1 (g_old_selfillummap_filenames[i])
			old1  = findImageTruePath old0
			
			--print ("new_file" + g_new_selfillummap_filenames[i]);
			new  = GetFullPathName path1 (g_new_selfillummap_filenames[i])

			renameFile old1  new
			
		)
		
		--opacitymap_filenames
		for i=1 to g_old_opacitymap_filenames.count do 
		(
			--print ("old_file" + g_old_opacitymap_filenames[i]);
			old0  = GetFullPathName path1 (g_old_opacitymap_filenames[i])
			old1 = findImageTruePath old0
			
			--print ("new_file" + g_new_opacitymap_filenames[i]);
			new  = GetFullPathName path1 (g_new_opacitymap_filenames[i])
			
			renameFile old1  new
			
		)
	)

	--clearUnusedMaterial()
	--clearUnusedSubMaterial ()
	
	global allSubMats = #()
	max select all
	if selection.count!=0 then
		ungroup $selection
	allSubMats = collectAllSubMats()
	--allSubMats = GetAllMatUsedByScene()
	for obj in selection do
	(
		isAllMesh = true
		if (classof obj != Editable_mesh)then
		(
			isAllMesh = false
			exit
		)
		if(isAllMesh == false)then
		(
			print "文件中存在物体不是Mesh"
		    return undefined	
		)
	)
	
	ChangeAllSubMatsFilePathAndName  allSubMats

	g_tilefilename = (maxFilePath +"maps\\")
	--g_tilefilename = g_bitmapfilepath
	
	HandleHardDiskFiles()

	--clearUnusedMaterial()
	--clearUnusedSubMaterial()

	max select none
	
	savemaxfile	(maxFilePath+maxfilename)
	resetMaxFile #noPrompt
	
	
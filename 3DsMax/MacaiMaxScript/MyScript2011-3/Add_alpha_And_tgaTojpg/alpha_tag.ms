
--��max7���棬û��toUpper�������,��������Լ�дһ���������
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
	
	
	--��max7���棬û��toUpper�������,��������Լ�дһ���������
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
	
	--����ȫ�ֵ����飬���������ļ�����
	global g_oldfilenames=#()
	global g_newfilenames=#()
	
	global g_old_selfillummap_filenames=#()
	global g_new_selfillummap_filenames=#()
	
	global g_old_opacitymap_filenames=#()
	global g_new_opacitymap_filenames=#()

	--��ȡBitmap��Xref·������Ӧ3ds max��Configure Paths�Ի�����Bitmaps��Xrefѡ�
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
	   --���ļ�����
	   s3 = filenameFromPath imagefname 
	   for x in imgpaths do (
	       fname = x+"\\"+s3
		   fname = mytolower fname
	        bexistfile  =  doesFileExist fname 
		  -- print fname 
		 --  print bexistfile
		   if bexistfile==true then
		   (   
		      -- messagebox ( ("�ҵ��ļ�"+fname) as string )

		      return fname
	      )
	   )
	   print ( ("�Ҳ����ļ�"+imagefname) as string)
	   return ""
	)
	
	fn GetBitmapFilePath bitmapfile =
	(
		return getFilenamePath bitmapfile
	)
	
	--��ȡ�������������õ����в���
	--��Щ����Ӧ������scenematerials
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
		
		--��������
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
	
	--ɾ������Ҫ���Ӳ���
	fn clearUnusedSubMaterial = (
	   --ѡ�����max�����ж���
		max select all

		for obj in Selection do (
			if obj.material==undefined then
			(
				messagebox ("����"+obj.name+"û�в���" as string)
			    exit
			)
			
			
			if classof obj == Editable_mesh then
			(
				newmat =  Multimaterial()
				curmat = obj.material
				oldname = curmat.name
				curmat.name = oldname+"temp"
				newmat.name = oldname
				
				--�����ռ������õ����Ӳ���ID
				IDS =#()
				for faceIndex=1 to getNumFaces(obj) do 
				(
					--������õĲ���ID
					faceMatID = getFaceMatID obj faceIndex
					
					kk = findItem IDS faceMatID
					if kk==0 then (
						append IDS faceMatID
					)
				)
				--���ڣ��õ��Ĳ���IDȫ��������IDS����	
				--����
				sort IDS  
				
				--�����²��ʵ��Ӳ��ʸ���
				newmat.numsubs = IDS.count

				--messagebox "dslf1"
				--����ԭ���Ĳ��ʵ��²�����
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
				
				--��Ĳ���IDӦ�������仯
				numfaces = getNumFaces(obj)
				for faceIndex=1 to numfaces do 
				(
					oldID = getFaceMatID obj faceIndex
					
					--��ID��Ӧ����ID
					newID = findItem IDS oldID
					
					setFaceMatID obj faceIndex newID
				)
				
				--�Ȱ��²��ʸ�ֵ��ȥ
				obj.material = newmat
				
				--ɾ���ϲ���
				index = findItem scenematerials curmat
				deleteItem scenematerials index
			)
			else
			(
				messagebox "����mesh"
			)
		)
		
		--�����ȫ�ػ�
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
			--print "������*completemap.tga��ͼ��"
			return true
		)
		else if (strlig != undefined)then
		(
			--print "������*lightingmap.tga��ͼ��"
			return true
		)
		else return false
	)
	
	fn HandleDiffuseMap mat =
	(
		--����diffusemap
		
		filepathname = mat.diffusemap.filename
		if filepathname==undefined then 
				print ("filepathname:"+filepathname)	
		
		--��ȡ�Ӳ���������׺
		bitmapName = filenameFromPath filepathname
		
		------------------------------------------
		--���������Ӱ��ͼ�������Ȳ����Ƿ���Ӱ��ͼ�����ں��ַ��� _alpha
		  --�������������ʾ���ǵ������д���������Ҫ������
		  --��ԭ����:xx.tga,�ĳ�xx_alpha.tga
		  --����ļ����Ѿ�����_alpha����ʾ����������ȷ�����ǲ�����		   
		------------------------------------------
		flag = isShadowMap bitmapName
		bTGA =  isTGA  bitmapName
		
		if((not flag) and bTGA )then
		(
			 --messagebox "dfs"
			 alphastr = (findstring bitmapName "_alpha") 
			
			--�ļ����ֲ���_alpha
			if(alphastr ==undefined ) then
			(
				 --����ԭʼ�ļ�����
				oldname  = findImageTruePath  bitmapName
				append g_oldfilenames oldname
				oldpath = getFilenamePath oldname
				
				filename0 = ((getFileNameFile oldname) + "_alpha" + (getFilenameType oldname))
				newfilename = MyToUpper filename0
				
				 --�����Ӧ�����ļ�����
				append g_newfilenames  newfilename 
				
				--�޸Ĳ�����,��ɴ�д
				mat.diffusemap.filename = oldpath + newfilename
				mat.diffusemap.name =  (getFileNameFile newfilename ) + ".tga"
				mat.name  = mat.diffusemap.name

					--print ("mat.diffusemap.name(not 'compeletemap and lightingmap':" + mat.diffusemap.name)
				--����ͬ���޸� ͸����ͼ
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
		else if((flag) and bTGA)then--�ҵ�����completemap��lightingmap��ת��jpg
		(
			--�������Ӱ��ͼ���ļ���ȥһ��"_alpha"����ֹ�ļ������治С�Ĵ�����_alpha
			--�ļ����ĺ�׺��jpg 
			--�ļ�����ȥalphaͨ��
			
			--���������_alpha
			--��ô��ȥ��_alpha�ִ�
			srcfname = MyToLower (findImageTruePath bitmapName)
			removestr = "_alpha"
			tonewname = ""
			noalphaname = (substituteString srcfname removestr tonewname)

			tgastr = (findstring noalphaname ".tga") 
			if(tgastr !=undefined)do
			(
				--�����ⲿ�����tga��ʽ����ͼת����jpg��ʽ 
				cmd = "D:\\tga2jpg.exe"+ " "+noalphaname
				DOSCommand cmd
			)
			
			--��jpg��ͼ���¸���
			oldname  =getFilenameFile bitmapName
			newfilename = MyToLower oldname
			
			--�޸Ĳ�����
			mat.diffusemap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
			mat.diffusemap.name =  (getFileNameFile newfilename ) + ".jpg"
			mat.name  = mat.diffusemap.name

		)
	)
	
	--͸����ͼ
	fn Handleopacitymap  mat =
	(
		--����opacitymap
		filepathname = mat.opacitymap.filename
		if(filepathname!=undefined) then
		(
			--print ("opacitymap filepathname:"+filepathname)	
			
			--��ȡ�Ӳ���������׺
			bitmapName = filenameFromPath filepathname

			flag = isShadowMap bitmapName--�Ҳ���completemap��lightingmap������flag=false
			bTGA =  isTGA  bitmapName
			
			if( flag and bTGA )then--�ҵ�����completemap��lightingmap��ת��jpg
			(
				srcfname = findImageTruePath bitmapName
				tgastr = (findstring srcfname ".tga") --ǰ���Ѿ�ת��jpg���Է�cmd��ʾ�����ļ���ͣ�µȴ�
				if(tgastr !=undefined)do
				(
					--�����ⲿ�����tga��ʽ����ͼת����jpg��ʽ 
					cmd = "D:\\tga2jpg.exe"+ " "+srcfname
					DOSCommand cmd
				)
				
				--��jpg��ͼ���¸���
				oldname  =getFilenameFile bitmapName
				newfilename = MyToLower oldname
				
				--�޸Ĳ�����
				mat.opacitymap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
				mat.opacitymap.name =  (getFileNameFile newfilename ) + ".jpg"
				mat.name  = mat.opacitymap.name
					
				print ("����mat.opacitymap.name �� mat.opacitymap.filename�� " +mat.opacitymap.name+" " +mat.opacitymap.filename)
				
			)
			else if (flag and (not bTGA)) then
			(
				--alphastr = (findstring bitmapName "_alpha") 
				
				--�ļ����ֲ���_alpha
				--if(alphastr ==undefined ) then
				--(
					srcfname = findImageTruePath bitmapName
					
					--��jpg��ͼ���¸���
					oldname  = filenameFromPath  srcfname-- returns: "myImage.jpg"
					newfilename = MyToLower oldname
					
					--�޸Ĳ�����
					mat.opacitymap.filename = (maxFilePath +"maps\\") + newfilename --+ ".jpg"
					mat.opacitymap.name =  (getFileNameFile newfilename ) + ".jpg"
					mat.name  = mat.opacitymap.name
				
				print ("����mat.opacitymap.name �� mat.opacitymap.filename�� " +mat.opacitymap.name+ " " +mat.opacitymap.filename)
				--)
			)
		)
	)
	
	--�Է�����ͼ
	fn Handleselfillummap  mat =
	(
		--����selfillummap
		filepathname = mat.selfillummap.filename
		if (filepathname!=undefined )then
		(
			--print ("selfillummap filepathname:"+filepathname)	
		
			--��ȡ�Ӳ���������׺
			bitmapName = filenameFromPath filepathname

			flag = isShadowMap bitmapName----�Ҳ���completemap��lightingmap������flag=false
			bTGA =  isTGA  bitmapName
			
			if( flag and bTGA )then--�ҵ�����completemap��lightingmap��ת��jpg
			(
				
				srcfname = findImageTruePath bitmapName
				tgastr = (findstring srcfname ".tga") 
				if(tgastr !=undefined)do
				(
					--�����ⲿ�����tga��ʽ����ͼת����jpg��ʽ 
					cmd = "D:\\tga2jpg.exe"+ " "+srcfname
					DOSCommand cmd
				)
				
				--��jpg��ͼ���¸���
				oldname  =getFilenameFile bitmapName
				newfilename = MyToLower oldname
				
				--�޸Ĳ�����
				mat.selfillummap.filename = (maxFilePath +"maps\\") + newfilename + ".jpg"
				mat.selfillummap.name =  (getFileNameFile newfilename ) + ".jpg"
				mat.name  = mat.selfillummap.name
				
				print ("����mat.selfillummap.name �� mat.selfillummap.filename�� " +mat.selfillummap.name+" " + mat.selfillummap.filename)
			)
			else
			(
				 alphastr = (findstring bitmapName "_alpha") 
			
				--�ļ����ֲ���_alpha
				if(alphastr ==undefined ) then
				(
					 --����ԭʼ�ļ�����
					oldname  = findImageTruePath  bitmapName
					oldpath = getFilenamePath oldname
					
					filename0 = (filenameFromPath oldname) --+ "_alpha" + (getFilenameType oldname))
					newfilename = MyToUpper filename0
					
					--�޸Ĳ�����,��ɴ�д
					mat.selfillummap.filename = oldpath + newfilename
					mat.selfillummap.name =  (getFileNameFile newfilename )+ ".jpg"
					mat.name  = mat.selfillummap.name
				
					print ("����mat.selfillummap.name �� mat.selfillummap.filename�� " +mat.selfillummap.name+" " + mat.selfillummap.filename)
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
			   --print ("����"+mat.name+"ȱ��diffuse��ͼ") 
		    )
		    else (
			     HandleDiffuseMap  mat
			)
			
			--selfillummap
			if mat.selfillummap==undefined then (
			   --print ("����"+mat.name+"ȱ��selfillummap��ͼ") 
			
			)
			else
			(
			    Handleselfillummap  mat
			)
			
			--opacitymap
			if mat.opacitymap==undefined then (
			  -- print ("����"+mat.name+"ȱ��opacitymap��ͼ") 
			   
			)
			else
			(
			    Handleopacitymap  mat
			)
			
			
		)
	)
	
	--��ȡ��·�����ļ�����
	fn GetFullPathName newpath  filename0 = 
	(
		filename = MyToUpper filename0
		s = newpath+filename		
		return s
	)
	
	--����Ӳ���ϵ�ͼƬ�ļ���������Ϊ�����ļ����ڵ�ǰĿ¼��
	fn HandleHardDiskFiles = 
	(
		path1 = getFilenamePath g_tilefilename--..\maps��·��
		
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
			print "�ļ��д������岻��Mesh"
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
	
	
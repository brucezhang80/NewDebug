
rollout ShanghaiCheck "SH_地块检查器" width:221 height:500
(
	button btn_checkObjName "检查物体命名" pos:[28,13] width:162 height:31 
	button btn_autoNameMat "自动材质名称处理" pos:[28,380] width:162 height:31  
	multiListBox lbx_obj "错误名字：" pos:[2,50] width:108 height:15
	multiListBox cust3 "所有名字：" pos:[113,50] width:108 height:15
	button btn_DoMat "优化材质" pos:[28,340] width:162 height:31
	button btn5 "删除无用材质球" pos:[28,300] width:162 height:31 enabled:false
	button btn_ObjName2MatName "材质名等于物体名" pos:[28,420] width:162 height:31 
	hyperlink lbl_Tuby "广州市拓比信息科技有限公司" pos:[60,480] width:200 height:16 color:green hoverColor:red visitedColor:green address:"http://www.tuby.cc"
	GroupBox grp1 "材质处理"pos:[8,280] width:200 height:190
	
	--定义全局的数组，用来保存文件名字
	global g_oldfilenames=#()
	global g_midfilenames=#()
	global g_newfilenames=#()
	global g_tilefilename=""  
	
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
	
    --global	MyToUpper = MyToUpper
	
	fn numbertostr i =
	(
	   if i==0 then return "0"
	   if i==1 then return "1"
	   if i==2 then return "2"
	   if i==3 then return "3"
	   if i==4 then return "4"
	   if i==5 then return "5"
	   if i==6 then return "6"
	   if i==7 then return "7"
	   if i==8 then return "8"
	   if i==9 then return "9"
	   if i==10 then return "A"
	   if i==11 then return "B"
	   if i==12 then return "C"
	   if i==13 then return "D"
	   if i==14 then return "E"
	   if i==15 then return "F"
	   if i==16 then return "G"
	   if i==17 then return "H"
	   if i==18 then return "I"
	   if i==19 then return "J"
	   if i==20 then return "K"
	   if i==21 then return "L"
	   if i==22 then return "M"
	   if i==23 then return "N"
	   if i==24 then return "O"
	   if i==25 then return "P"
	   if i==26 then return "Q"
	   if i==27 then return "R"
	   if i==28 then return "S"
	   if i==29 then return "T"
	   if i==30 then return "U"
	   if i==31 then return "V"
	   if i==32 then return "W"
	   if i==33 then return "X"
	   if i==34 then return "Y"
	   if i==35 then return "Z"
	   return "0"
	)
	
	--数字变成36进制文字，总共4位
	fn CreateImageName i = (
	   a = (mod i   36) as integer
	   i = i-a
	   i = i/36
	   
	   b = (mod i  36 ) as integer
	   i = i-b
	   i = i/36
	   
	   c = (mod i  36) as integer
	   i = i-c
	   i = i/36
	   
	   d = (mod i  36 ) as integer
	   s = numbertostr(d) + numbertostr(c) +numbertostr(b) + numbertostr(a) 
	   
	   return s
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
	--清除没有使用的材质球
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
	
	
	--这里收集场景中的所有子材质
	fn collectAllSubMats = (
		allSubMats=#()
		for mat in scenematerials do 
		(
		  if ( classof mat  == Multimaterial) then (
			  for i=1 to mat.numsubs do (

				if mat[i]!=undefined then (
					--收集每一个子材质
					append allSubMats mat[i]
				)
				else (
					messagebox "不应当出现无定义材质"
				)
			  )
		  )
		  else (
		      --普通材质，我们先打印一下他的名字
			 messagebox "在场景材质中，不应当出现普通材质"
		  )
		)
		
		return allSubMats
	)
	
	fn ChangeAllSubMatsFilePathAndName allsubmats = 
	(
		 tilepath = maxfilepath
		
		
		 for mat in allsubmats do
		(
            --带路径的文件名 
			filepathname = mat.diffusemap.filename
			
			--获取文件名字
			filename0 = filenameFromPath filepathname
			filename = MyToUpper filename0
			 
			--这里修改子材质的对应的diffusemap的文件名字(包括路径)
			mat.diffusemap.filename= tilepath+filename

			--这里修改贴图的名字，就是文件名字，不要后缀
			mat.diffusemap.name = getFileNameFile filename
			mat.name  = mat.diffusemap.name 
		 )
	)
	
	
	--处理场景中的所有子材质
	--我们收集每一个子材质的文件名字
	--形成一个列表
	--这个列表里面，每一个老的文件名字对应一个中间态的文件名字，和一个新的文件名字
	--然后统一：删除所有中间态的文件名字，把老的文件拷贝一份成中间态文件，
	--删除老的文件，删除新的文件，把中间态的文件改名成新的文件名
	fn HandleAllSubMats allsubmats = (
	
		--获取地块的名字，如7H2B
		tileName0 = getFilenameFile maxFileName
		tileName = MyToUpper tileName0
		
		tilepath = maxFilePath
		
		--debug = newScript()
		--alloldfilenames=#()
		--allmidfilenames=#()
		--allnewfilenames=#()
		
		--这里清除我们的全局数组
		g_midfilenames=#()
		g_oldfilenames=#()
		g_newfilenames=#()
		
		--我们假定文件名是正确的,并且不包括路径
		uniqueFile=0
        for mat in allsubmats do (
		
		    filepathname = mat.diffusemap.filename
			
			--获取文件名字
		    filename0 = filenameFromPath filepathname
			filename = MyToUpper filename0
			
			--获取文件后缀
			ext0  = getfilenametype filename	
	        ext = MyToUpper ext0 
			
		    --查找在数组里面是否已经存在这个文件名
		    index = findItem g_oldfilenames filename
			
			--文件名字不存在，我们加到数组里面去
			if index==0 then (
			    uniqueFile=uniqueFile+1
				
				--原始文件名字
				append g_oldfilenames  filename
					
				--中间态名字
				midname =  "YY"+tileName+CreateImageName(uniqueFile)+ext
				append g_midfilenames midname
				
				--最终态名字
				newname =  tileName+CreateImageName(uniqueFile)+ext
				append g_newfilenames newname
				
				
				--这里修改子材质的对应的diffusemap的文件名字(包括路径)
				mat.diffusemap.filename= tilepath+newname

				--这里修改贴图的名字，就是文件名字，不要后缀
				mat.diffusemap.name = getFileNameFile newname
                mat.name  = mat.diffusemap.name
			)
			else(
			    --表示同名的一个图片已经处理过了
				--我们只需要修改材质就ok了
				--filename = filename
				--midname = findItem g_midfilenames index
				
				newname = g_newfilenames[index]
				
				--这里修改子材质的对应的diffusemap的文件名字(包括路径)
				mat.diffusemap.filename= tilepath+newname

				--这里修改贴图的名字，就是文件名字，不要后缀
				mat.diffusemap.name = getFileNameFile newname
                mat.name  = mat.diffusemap.name
			)
        ) 

       	--改变透明贴图通道
		for mat in allsubmats do (
			if(mat.opacityMap != undefined)then
			(
				mat.opacityMap.filename= mat.diffusemap.filename
				mat.opacityMap.name = mat.diffusemap.name
			)
		)
	)
	
	--获取带路径的文件名字
	--主要是因为不知道什么原因，g_midfilenames里面带上了路径
	fn GetFullPathName newpath oldfilename = (
		filename0 = filenameFromPath oldfilename
		filename = MyToUpper filename0
		
		s = newpath+filename
		return s
	)
	
	--处理硬盘上的图片文件，我们认为所有文件都在当前地块目录下
	fn HandleHardDiskFiles = (
		-- debug = newScript()
		
		 path1 = getFilenamePath g_tilefilename
		
		 for i=1 to g_oldfilenames.count do (
			 old  = GetFullPathName path1 (g_oldfilenames[i])
			 mid  = GetFullPathName path1 (g_midfilenames[i])
			 
			--print old to:debug
			--print mid to:debug 
			 
			--print ( 
				deleteFile mid 
			--	) to:debug
			--print ( 
				copyFile old  mid 
			--	)to:debug
			--print ( 
				deleteFile old 
			--	) to:debug
			--print "--------------------------" to:debug  
		)
		--print "--------------------------" to:debug
		--print "--------------------------" to:debug
		--print "--------------------------" to:debug
	    for i=1 to g_oldfilenames.count do (
			mid  = GetFullPathName path1 (g_midfilenames[i])
			new  = GetFullPathName path1 (g_newfilenames[i])
			--print mid to:debug
			--print new to:debug 
			--print ( 
				deleteFile new 
				--) to:debug
			--print ( 
				renameFile mid  new 
			--	) to:debug
			--print "--------------------------" to:debug  
		)
	)
	
	--物体名字检测,检测长度，前缀，中间
	fn ObjectNameCheck objname = (
	    --第一步,是不是8位
		lenValid = (objname.count==8)
		
		--第二部，前缀是否正确
		PrefixValid=false
		Prefix = substring objname 1 2
		if Prefix=="CA" then PrefixValid=true
		if Prefix=="CB" then PrefixValid=true
		if Prefix=="CC" then PrefixValid=true
		if Prefix=="CD" then PrefixValid=true
		if Prefix=="CE" then PrefixValid=true
		
		--第三部，后缀是否正确,后缀一定是一个36进制的数字
		Suffix  = substring objname (objname.count-1) 2
		--数字太大了，我们认为错误？？
		
		--第四步，中间是否正确
		middle = substring objname 3 4
		--获取地块的名字，如7H2B
		tileName0 = filenameFromPath maxFileName
		tileName = MyToUpper tileName0
		midValid = (middle==tileName)
		
		rr = lenValid and midValid and PrefixValid
		return rr
	)
	
	fn charTo36BaseNumber c = (
		if c=="0" then return  0
		if c=="1" then return  1
		if c=="2" then return  2
		if c=="3" then return  3
		if c=="4" then return  4
		if c=="5" then return  5
		if c=="6" then return  6
		if c=="7" then return  7
		if c=="8" then return  8
		if c=="9" then return  9
		if c=="A" then return  10
		if c=="B" then return  11
		if c=="C" then return  12
		if c=="D" then return  13
		if c=="E" then return  14
		if c=="F" then return  15
		if c=="G" then return  16
		if c=="H" then return  17
		if c=="I" then return  18
		if c=="J" then return  19
		if c=="K" then return  20
		if c=="L" then return  21
		if c=="M" then return  22
		if c=="N" then return  23
		if c=="O" then return  24
		if c=="P" then return  25
		if c=="Q" then return  26
		if c=="R" then return  27
		if c=="S" then return  28
		if c=="T" then return  29
		if c=="U" then return  30
		if c=="V" then return  31
		if c=="W" then return  32
		if c=="X" then return  33
		if c=="Y" then return  34
		if c=="Z" then return  35
	)
	
	--字符串变到36进制数字
	fn  StrTo36BaseNumber str = (
	    result=0
	    rr = 1
	    count = str.count
		for i=count to 1 by (-1) do(
		     tmp = charTo36BaseNumber str[i]
			 result = rr*tmp + result
			 rr= 36*rr
		)
		return result
	)
	
	--物体名字排序函数
	fn sortObjectNameFunction name1 name2 = (
	   --获取后缀
	    Suffix1  = substring name1 (name1.count-1) 2
	    Suffix2  = substring name2 (name2.count-1) 2
	   
	    num1 = StrTo36BaseNumber suffix1
	    num2 = StrTo36BaseNumber suffix2
	   
		if num1<num2 then return -1
		if num1>num2 then return 1
		else return 0
	)
	
	--获取物体的序号
	fn getObjectIndex name1 = (
	   Suffix1  = substring name1 (name1.count-1) 2
	   num1 = StrTo36BaseNumber suffix1

	   return num1
	)
	


	on btn_checkObjName pressed do
	(
		--检查物体名字
		--检查物体名字是否是8位
		--前面两位必须是CB，CA,CC,CD
		--中间四位必须是地块编号：
		--最后两必须是36进制的数字，并且应当是从01开始
		--物体名字必须都是大写
	
		lbx_obj.items = #()
		allNames = #()
		errorNames = #()
		max select all
		for i in selection do 
		(
			if i.name != undefined then
			(
				strName = i.name as string
				uppName = MyToUpper strName
				i.name = uppName
				
				validName = ObjectNameCheck uppName
				
				if (validName==false)then
				(
				   append errorNames uppName
				)
				
				append allNames uppName
			) 
			else
			(
			    messagebox "存在无名物体"
			)
		)
		
		--if ch_errName!=undefined then
		(lbx_obj.items = errorNames )
		
		--上面已经把所有名字收集起来
		--我们按照名字大小排序
		qsort allNames sortObjectNameFunction
		
		cust3.items =  allNames
		
		--检查是否连贯
		count = allNames.count
		for i=1 to count do (
		    index = getObjectIndex (allNames[i])
			if i!=index then (
			   messagebox ("名字不连贯" + allNames[i] )
			   exit
			)
		)
	)
	on btn_autoNameMat pressed do
	(
	--首先判断是否指定了地块文件
	if maxFileName.count==0 then 
	(
	    messagebox "请先选择地块"
	    return undefined
	)
	name = getFileNameFile(maxFileName)
	if name.count!=4 then (
	   messagebox "打开的max文件名长度不是4，可能不是地块文件"
	   return undefined
	)
	
	isAllMesh = true
	max select all
	for obj in selection do
	(
		if (classof obj != Editable_mesh)then
		(
			isAllMesh = false
			exit
		)
	)
	max select none
	if(isAllMesh == false)then
	(
		messagebox "文件中存在物体不是Mesh"
	   return undefined	
	)
	
	--我们应当去掉多余的材质
	if true then (
	  clearUnusedMaterial()
	  clearUnusedSubMaterial()
	  --再次清除
	  clearUnusedMaterial() 
	)
	
	--收集所有的子材质	   
	allSubMats=collectAllSubMats()
	
	--这里修改一下,做测试而已
	ChangeAllSubMatsFilePathAndName allSubMats
	
	--处理所有的材质,包括材质改名
	
	--并且修改全局的存放文件名的变量，以便后续修改
	
	HandleAllSubMats allsubmats
	
	--这里我们保存并且关闭文件
	savemaxfile (maxFilePath+maxFileName)
	g_tilefilename = maxFilePath+maxFileName
	
	--关闭文件
	resetMaxFile #noPrompt
	
	--处理硬盘上的文件
	HandleHardDiskFiles()
	
	--再次打开这个文件	
	loadMaxFile(g_tilefilename )
	messagebox "材质名称处理完成！请不要在当前文件重复使用此功能！"
	)
	on btn4 pressed do
	(
	     clearUnusedSubMaterial()
	)
	on btn5 pressed do
	(
	     clearUnusedMaterial()
	)
	
	on btn_ObjName2MatName pressed do
	(
		max select all
		for obj in selection do
		(
			mat = obj.material
			if (mat !=undefined)then
			(
				mat.name = obj.name
			)
		)
		messagebox "已完成！"
		max select none
	)
	
	on lbx_obj doubleClicked select do
	(
		(
			rollout edit_rename "重命名"
			(
				editText edt_rename "重命名"
				
				on edt_rename entered text do
				(
					text1 = edt_rename.text
				
				)
			)createdialog edit_rename
			
						
		)
	)
)

createdialog ShanghaiCheck




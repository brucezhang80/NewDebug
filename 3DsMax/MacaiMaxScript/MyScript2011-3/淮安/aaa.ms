fn writeCenter2File iCenter height fileName= 
	(
		
		centerFile = openfile "iCenter.txt" mode:"at"
		if (centerFile == undefined)do
		(
			createFile "iCenter.txt"
			centerFile = openfile "iCenter.txt" mode:"at"
		)
		
		if (centerFile == undefined)do
		(
			messagebox "打开文件失败！"
			exit
		)
		
		s0 = formattedPrint iCenter.x format:"19.14g" 
		s1 = formattedPrint iCenter.y format:"19.14g" 
		
		s2 = formattedPrint height format:"19.14g" 
		
		s3 = filenameFromPath fileName 
		
		str = fileName + " " + s0  + " " + s1 + " "+s3+" "+s2
		format str to:centerFile
		format "\n" to:centerFile
		
		close centerFile			
	)

        --轴心归底
	fn CenterPiv i =
	(
		CenterPivot i  --轴心归中心
		i.pivot.z = i.min.z  --轴心的Z轴等于物体最底点
        )
		--移动物体到中原点
        fn moveObjectToZero obj = (
              center = obj.center
              minz   = obj.min.z
              move obj [-center.x,-center.y,-minz]
              
              mpos = [-center.x,-center.y,-minz]
              return mpos
        )
	--只移动Z轴到原点
	 fn moveObjectToZeroOnlyZ obj = (
              center = obj.center
              minz   = obj.min.z
              move obj [0,0,-minz] 
             
        )

	
	fn FormatObj obj=
	(
		if (classof obj == Editable_mesh)do
		(
			update obj geometry:true topology:true normals:true
		)
	)

   	fn HandleMesh obj=
	(
		--select obj
		
                --修改轴
		CenterPiv obj

                --更新物体
		FormatObj obj

                --物体移动到原点
                moveObjectToZero obj
		
                 
		name1=obj.name
		filepath = maxfilepath
		
        --输出这个文件成一个.x文件	
		exportfile (filepath + name1+".x") #noprompt 

         --selectedOnly:true
		--saveNodes obj (filepath + name1+".max")
		
		--无需再移动回来
		--obj.pos = pos
		
		--clearselection()
	)

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
	
	if (objects.count == 1)then
	(
	   obj = objects[1]
		
	   iCenter = obj.center
		height = obj.max.z - obj.min.z
	   writeCenter2File iCenter height maxfile
	   HandleMesh obj
	)
	else 
	(
	  max select all
	  for mymesh in selection do (
			FormatObj mymesh
		moveObjectToZeroOnlyZ mymesh
	  )
	   

	   Group selection name:"g1"
	   clearSelection()

	   obj = $g1
		
	   iCenter = obj.center
	   height = obj.max.z - obj.min.z
	   writeCenter2File iCenter height maxfile
	   HandleMesh obj	
		
	   ungroup $g1	

		
		
	   --messagebox ("文件中物体数目不是1，请检查该文件："+maxfile)
	)
	
	savemaxfile maxfile
	
	 resetMaxFile #noPrompt
)

quitMax #noPrompt




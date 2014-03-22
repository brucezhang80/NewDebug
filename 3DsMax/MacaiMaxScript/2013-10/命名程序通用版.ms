--命名程序
fn getSeries num _base _digit _step=
(
	a=((num-1)*_step+_base) as string
	for k=1 to _digit-a.count do a="0"+a
	return a
)
try 
(
	destroydialog renameRollout
)
catch
()
rollout renamerollout "命名程序通用版" width:155 height:300
(
	local mapfiles=#()
	edittext edt1 "" pos:[25,25] width:120 height:20
	button btn1 "命名" pos:[15,245] width:125 height:37
	edittext edt2 "" pos:[25,75] width:120 height:20
	edittext edt3 "" pos:[25,125] width:120 height:20
	spinner spn1 "" pos:[70,156] width:75 height:16 range:[0,100000,1] type:#integer scale:1
	spinner spn3 "" pos:[70,215] width:75 height:16 range:[1,100000,1] type:#integer scale:1
	label lbl1 "基础名称:" pos:[5,5] width:67 height:18
	label lbl2 "前缀:" pos:[5,55] width:67 height:18
	label lbl3 "后缀:" pos:[5,105] width:67 height:18
	label lbl4 "起始值:" pos:[5,155] width:53 height:18
	label lbl6 "步数:" pos:[5,215] width:53 height:18
	spinner spn2 "" pos:[70,185] width:75 height:16 range:[2,100000,3] type:#integer scale:1
	label lbl5 "位数:" pos:[5,185] width:53 height:18
	
	on renamerollout open do
	(
		/*version=(maxversion())[1]
		if version==9000 then
		(
			spn1.value=0
		)*/
		edt1.text=getfilenamefile maxfilename
	)
	on btn1 pressed do
	(
		mapfiles.count=0
		pString=edt2.text+edt1.text+edt3.text
		
		AllMaps=getClassInstances BitmapTexture
		AllMapsFileName=#()
		for i =1 to AllMaps.count do 
		(
			append AllMapsFileName AllMaps[i].filename
		)
		(
			fn addmap mapfile =
			(
				mapfileN=mapfile as name
				index=finditem mapfiles mapfileN
				if index == 0 do append mapfiles mapfileN
			)
			enumeratefiles addmap
			sort mapfiles
			
			pFilename=#()
			k=1
			version=(maxversion())[1]
			if version==9000 then
		    (
			k=2
		     )

			for i=k to mapfiles.count do 
			(
				if doesFileExist (mapfiles[i] as string) then
				(
					append pFilename (mapfiles[i] as string)
				)
			)
			for i=1 to pFilename.count do 
			(
				pSeries=getSeries i (spn1.value) (spn2.value) (spn3.value)
				newfile=(getfilenamepath pFilename[i])+(pString)+(pSeries)+(getfilenametype pFilename[i])
				--tempfile=(getfilenamepath (mapfiles[i] as string))+"temp"+(pString)+(pSeries)+(getfilenametype (mapfiles[i] as string))
				tindex=finditem pFilename newfile
				if tindex==0 then
				(
					index=finditem AllMapsFileName pFilename[i]
					while index !=0 do
					(
						AllMaps[index].filename=newfile
						AllMapsFileName[index]=newfile
						index=finditem AllMapsFileName pFilename[i]
					)
					try
					(
						renamefile pFilename[i] newfile
					)
					catch
					()
			    )
				else
				(
					temp=pFilename[i]
				     pFilename[i]=pFilename[tindex]
					pFilename[tindex]=temp
					
				)
			)
		)
		messagebox "完成"
	)
)
createdialog renameRollout pos:[100,100]
clearlistener()
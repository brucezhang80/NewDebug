global yviCompareBitmapInpixRL;

Function yviGetMaxobjectBitmapInstance &bitmap_Instances =
(
	local bmpInst_temp = #();
	bmpInst_temp = getClassInstances BitmapTexture;
	local i = 0;
	for bi in bmpInst_temp where(isProperty bi #filename) do
	(
		append bitmap_Instances bi;
		
		yviCompareBitmapInpixRL.pb_status1.value = 100.*(i+=1)/bmpInst_temp.count;
	)
)

Function yviSetImageToSamePix &bitmap1 &bitmap2 =
(
	local _width = 64;
	local _height = 64;
	outputImg1 = bitmap _width _height;
	copy bitmap1 outputImg1;
	
	outputImg2 = bitmap _width _height;
	copy bitmap2 outputImg2;
	
	bitmap1 = outputImg1;
	bitmap2 = outputImg2;
)

Function yviFilenameFromPath file =
(
	filename = getFilenameFile file + getFilenameType file;
	filename;
)
	
Function yviIsSameImage img1 img2 =
(
	if( (img1 as string) != (img2 as string) )do 
	(
		local bTga = false;
		if(getFilenameType img1 == ".tga" or getFilenameType img2 == ".tga") do bTga = true;
		-- 需要修改成相同大小的图片，再进行比较朴素
		--format "%\t%\n" img1 img2;
		bitmap1 = openBitMap img1;
		bitmap2 = openBitMap img2;
		yviSetImageToSamePix &bitmap1 &bitmap2 ;

		ret = CompareBitmaps bitmap1 bitmap2 1 5 useAlpha:bTga;
		if( ret) then--same file.
		(
			return true;
		)
	)
	
	return false;
)

Function yviSeqSearchBitmap bitmapInstanceTable &duplite_bitmapInstanceTable =
(
	local i = 2;--first bitmapinstance has used.
	local end = 1;
	local counts = bitmapInstanceTable.count;
	-- search a file in bitmap instance array is same file.
	while(end < counts)do
	(
		if( yviIsSameImage (bitmapInstanceTable[i].filename) (bitmapInstanceTable[end].filename))do
		(
			if( (findItem duplite_bitmapInstanceTable bitmapInstanceTable[end]) == 0 )do
				append duplite_bitmapInstanceTable bitmapInstanceTable[end];
			append duplite_bitmapInstanceTable bitmapInstanceTable[i];
		)
		if(i == counts)do
		(
			end +=1;
			i = end;
		)
		i+=1;
	)
)

Function yviRenameDupUsingFile filename replacedFile = 
(
	_path = getFilenamePath filename;
	newName = _path + getFilenameFile replacedFile + "("+ getFilenameFile filename + ")" + getFilenameType filename;
	_filename = substituteString filename "\\" "/";
	_newName = substituteString newName "\\" "/";
	--format "--%\t%\n" _filename _newName
	if ( not (renameFile _filename _newName) )do 
		format  "rename failed: \n" _filename;
)

Function yviDoUniqueSceneBitmap bitmapInstanceTable =
(
	local i = 2;--first bitmapinstance has used.
	local end = 1;
	local counts = bitmapInstanceTable.count;
	-- search a file in bitmap instance array is same file.
	local duplite_bitmapInstanceTable = #();
	while(end < counts)do
	(
		if( yviIsSameImage (bitmapInstanceTable[i].filename) (bitmapInstanceTable[end].filename))do
		(
			if( (findItem duplite_bitmapInstanceTable bitmapInstanceTable[end]) == 0 )do
				append duplite_bitmapInstanceTable bitmapInstanceTable[end];
			if( (findItem duplite_bitmapInstanceTable bitmapInstanceTable[i]) == 0 )do
				append duplite_bitmapInstanceTable bitmapInstanceTable[i];
		)
		if(i == counts)do
		(
			if(duplite_bitmapInstanceTable.count != 0)do 
			(
				local lbxName = #();
				local stand_name = duplite_bitmapInstanceTable[1].filename;
				append lbxName stand_name;
				
				for i=2 to duplite_bitmapInstanceTable.count do 
				(
					replaced_name = duplite_bitmapInstanceTable[i].filename
					--format "%\t%\n" stand_name replaced_name;
					duplite_bitmapInstanceTable[i].filename = stand_name;
					--print dup.filename;
					--yviRenameDupUsingFile replaced_name stand_name;
					
					yviCompareBitmapInpixRL.pb_status2.value = 100.*end/duplite_bitmapInstanceTable.count;
	
					if( (findItem lbxName replaced_name) == 0)do
						append lbxName replaced_name;
				)
				append lbxName (end as string+".--------------------------------------");
				yviCompareBitmapInpixRL.lbx_dupBitmap.items += lbxName;
				duplite_bitmapInstanceTable = #();
			)
			end +=1;
			i = end;
		)
		i+=1;
		yviCompareBitmapInpixRL.pb_status2.value = 100.*end/counts;
	)
)

Function main =
(
	local bitmap_Instances = #();
	yviGetMaxobjectBitmapInstance bitmap_Instances;
	
	if(bitmap_Instances.count != 0)do
		yviDoUniqueSceneBitmap bitmap_Instances;
	
	print "Successful.";
	ok;
)

---------------


try(DestroyDialog yviCompareBitmapInpixRL)catch()
rollout yviCompareBitmapInpixRL "重复贴图唯一化" width:300 height:247
(
	button btn_setUniqueBitmap "贴图唯一化" --pos:[31,22] width:138 height:25
	progressBar pb_status1 "ProgressBar"
	progressBar pb_status2 "ProgressBar" --pos:[17,67] width:178 height:8
	listBox lbx_dupBitmap "重复贴图：" --width:208 height:9
	
	on btn_setUniqueBitmap pressed  do
	(
		if(lbx_dupBitmap.items.count != 0)then lbx_dupBitmap.items = #();

		pb_status1.value = 0.;
		pb_status2.value = 0.;
		main()
		
		if( lbx_dupBitmap.items.count == 0) do messageBox "全部贴图唯一";
	)
)
CreateDialog yviCompareBitmapInpixRL style:#(#style_titlebar, #style_border, #style_sysmenu, #style_minimizebox);
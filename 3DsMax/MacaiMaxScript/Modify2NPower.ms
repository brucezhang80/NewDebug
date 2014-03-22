


--路径 文件类型如："*.max"
function Yvi_getFilesRecursive root ext =
(
	local all_Files = #()
	local TheAllSubFolder = #()
	
	TheAllSubFolder = GetDirectories (root+"*")
	for s in TheAllSubFolder do
	(
		join TheAllSubFolder (GetDirectories (s + "*"))
	)

	for f in TheAllSubFolder do
	(
		join all_Files (getFiles (f  + ext))
	)
	join all_Files (getFiles (root  + ext))
	return all_Files
)
	
function Yvi_setImgPath =
(
	img_path = getSavePath caption:"选择图片所在的目录...";
	if(img_path == undefined)then
	(
		return undefined;
	)
	
	return img_path;
)

/********************************************************************/
/* 检查贴图是否是2的N次幂，如果不是，修改其最接近2次幂的数值        */
/********************************************************************/
----------------------------------------------------------------------
-- 是否是2的N次
function Yvi_isTwoPower num =
(
	
	return ( (num >= 2) and ( ( bit.and num (num - 1) ) == 0) );-- 2的n次幂肯定大于0, bit.and 位与运算
)
	
function Yvi_isBitmap2Power width height =
(
	return ( (Yvi_isTwoPower  width) and (Yvi_isTwoPower  height) );
)

-- 获取接近2的N次幂的值（向上接近或向下接近）
function Yvi_getCloser2Power num =
(
	oldNum = num;

	-- 我们使用位与运算
	while( (bit.and num (num-1) ) != 0 )do
	(
		num = bit.and num (num-1);
	)
	-- 位左移一位（如果-1，右移一位）
	high = bit.shift num 1; -- 得到 当前 num 向上接近 2的N次的值
	low = num;				-- 得到 当前 num 向下接近 2的N次的值
	
	-- 比较得到的两个向上 向下 的数值，与原始的数值差，取得差值小的。
	if( abs(oldNum-low) < abs(high-oldNum) )then
	(
		return low;
	)

	return high;
)


function Yvi_ModifyBitmapCloser2Power scrBitmap =
(
	sourceImg = openBitmap scrBitmap;
	local _width = sourceImg.width;
	local _height = sourceImg.height;
	
	-- 如果都是2的N次，不处理。
	if ( Yvi_isBitmap2Power _width _height ) then
	(
		return 0;
	)
	_width = Yvi_getCloser2Power _width;
	_height = Yvi_getCloser2Power _height;
	-- 需要对不同类型的图片进行不同方式来处理
	ext = getFilenameType scrBitmap;
	if( ext == ".jpg")then
	(
		outputImg = bitmap _width _height;
		copy sourceImg outputImg;
		outputImg.fileName = scrBitmap;
		JPEG.setQuality 100;
		save outputImg;
	)
	else if(ext == ".bmp")then
	(
		type = bmp.ibmpio.getType();
		outputImg = bitmap _width _height;
		copy sourceImg outputImg;
		outputImg.fileName = scrBitmap;
		bmp.ibmpio.setType type;
		save outputImg;
	)
)


Try(DestroyDialog rl_Modify2NPower)Catch()
rollout rl_Modify2NPower "Modify2NPower" width:228 height:169
(
	Group "Modify 2N Power"
	(
		editText edt_BitmapPath "" pos:[4,20] width:180 height:18
		button btn_BitmapPath "..." pos:[183,20] width:32 height:18
		checkbox ckb_ChkJpg "JPG" pos:[14,44] width:47 height:14
		checkbox ckb_ChkBmp "BMP" pos:[76,44] width:47 height:14
		button btn_CheckImg "Check No 2N Power" pos:[8,62] width:100 height:18
		button btn_ModifyImg "Modify To 2N Power" pos:[108,62] width:100 height:18 enabled:false
		button btn_About "?" pos:[210,62] width:13 height:18
		
		label lb_about "\nVersion:\t1.1\nMax Require: Max 9.0 + \n(c) 2012 Yvi, All Rights Reserved" height:100  width:208 pos:[8,80]
		HyperLink hpl_link "www.macai.co.cc" color:BLUE hoverColor:green visitedColor:(COLOR 111 7 44) address:"www.macai.co.cc" pos:[8,135]
		progressBar pb_Progressbar "ProgressBar" pos:[4,154] width:220 height:25 color:( random (color 0 0 0) (color 255 255 255) )
	)
	
	
	/*****************************************************/
	/*图片文件2的N次幂检查与修正                        */
	/****************************************************/
	global g_ImgFiles = #();
	
	on btn_BitmapPath pressed do
	(
		dir = Yvi_setImgPath();
		if( dir == undefined )then
		(
			edt_BitmapPath.text = edt_BitmapPath.text;
			return 0;
		)
		edt_BitmapPath.text = dir;
	)
	
	on btn_CheckImg pressed do
	(
		local theJpgFiles = #();
		local theBmpFiles = #();
		local srcPath = edt_BitmapPath.text;
		g_ImgFiles = #()
		------------------
		if( srcPath.count == 0 )then
		(
			MessageBox "指定路径" title:rl_Modify2NPower.title;
			return 0;
		)
		if( srcPath[srcPath.count] != "\\" )then
		(
			srcPath += "\\";
		)
		if(ckb_ChkJpg.checked == off and ckb_ChkBmp.checked == off)then
		(
			MessageBox ("选择其中一种图片类型") title:rl_Modify2NPower.title;
			return 0;
		)
		if( ckb_ChkJpg.checked)then
		(
			ext = "*.jpg";
			theJpgFiles = Yvi_getFilesRecursive srcPath ext;
		)
		if( ckb_ChkBmp.checked )then
		(
			ext = "*.bmp";
			theBmpFiles = Yvi_getFilesRecursive srcPath ext;
		)
		-----------------
		g_ImgFiles = theJpgFiles + theBmpFiles;
		local imgCounts = g_ImgFiles.count;
		local b = true;
		if( imgCounts != 0)then
		(
			local i = 0;
			local counts = 0;
			for img in g_ImgFiles do
			(
				
				sourceImg = openBitmap img;
				try
				(
					--if( sourceImg != undefined )then
					(
						b = Yvi_isBitmap2Power sourceImg.width sourceImg.height;
					)
				)Catch(/*throw()*/)
					

				if( b == false)then
				(
					counts += 1;
				)
				---------------
				--pb_Progressbar.color =( random (color 0 0 0) (color 255 255 255) );
				pb_Progressbar.value = 100.*(i+=1)/imgCounts;
				
			)
			--pb_Progressbar.color = GREEN;
			if( counts != 0 )then
			(
				btn_ModifyImg.enabled = true;
				MessageBox ("发现 "+ counts as string + " 个不符合2的N次幂标准的图片。") title:rl_Modify2NPower.title;
			)
			else
			(
				btn_ModifyImg.enabled = false;
				MessageBox ("全部图片符合2的N次幂标准！") title:rl_Modify2NPower.title;
			)
		)
		GC();
	)
	/*****************************************
	修改图片文件为2的N次幂
	******************************************/
	on btn_ModifyImg pressed do
	(
		if(ckb_ChkJpg.checked == off and ckb_ChkBmp.checked == off)then
		(
			MessageBox ("选择其中一种图片类型") title:rl_Modify2NPower.title;
			return 0;
		)
		
		local i = 0;
		for img in g_ImgFiles where(g_ImgFiles.count != 0)do
		(
			try
			(
				Yvi_ModifyBitmapCloser2Power img;
			)Catch()
			--pb_Progressbar.color =( random (color 0 0 0) (color 255 255 255) );
			pb_Progressbar.value = 100.*(i+=1)/g_ImgFiles.count;
		)
		--pb_Progressbar.color = GREEN;
		MessageBox ("完成") title:rl_Modify2NPower.title;
		GC();
	)
	
	
		/*****************************************
	关于或帮助
	******************************************/
	on btn_About pressed do
	(
		try(DestroyDialog rl_About)Catch()
		rollout rl_About "关于" width:389 height:180
		(
			groupBox grp1 "About" pos:[10,6] width:368 height:150
			label lbl1 "" pos:[21,20] width:355 height:130
			button btn1 "OK" pos:[168,158] width:60 height:20
			
			on rl_About open  do
			(
				lbl1.text = "修改尺寸：\n  
	1.指定图片输入目录，选择图片类型（JPEG、BMP）。
	2.检查是否存在非2次幂的图片。
	3.如果有非2次幂图片，则修改为2次幂图片按钮可用。
	※[PS:图片输入目录包含当前目录下的子目录]\n
			-------------------	            \n\t\t\t\tDesigned by Yvi @2012-07-06
				Email:12319597@qq.com"
			)
			on btn1 pressed  do
			(
				DestroyDialog rl_About;
			)
		)CreateDialog rl_About;
	)
)
CreateDialog rl_Modify2NPower style:#(#style_sysmenu, #style_minimizebox, #style_titlebar)
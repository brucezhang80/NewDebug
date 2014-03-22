
function Yvi_compressJpegImage srcImg desImg quality:0 =
(
	sourceImg = openBitmap srcImg;
	outputImg = bitmap sourceImg.width sourceImg.height;
	copy sourceImg outputImg;
	outputImg.fileName = desImg;
	JPEG.setQuality quality;
	save outputImg;

)

--enums: {#paletted|#true24|#true48|#gray8|#gray16}
/*
	paletted ; 0 - 255
*/
function Yvi_compressBmpImage srcImg desImg type:0 =
(
	sourceImg = openBitmap srcImg;

	outputImg = bitmap sourceImg.width sourceImg.height;
	copy sourceImg outputImg;
	outputImg.fileName = desImg;
	bmp.ibmpio.setType type;
	save outputImg;
)


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


Try(DestroyDialog rl_CompressImage)Catch()
rollout rl_CompressImage "Compress&Modity Image" width:245 height:350
(
	EditText edt_Browse "" width:170 across:2 align:#left pos:[0, 6]
	button btn_Browse "Browse..." tooltip:"设置图片所在路径..." align:#right height:18 width:60
	EditText edt_Saveas "" width:170 across:2 align:#left pos:[0, 27]
	button btn_Saveto "Save as..." tooltip:"设置保存压缩后图片存放路径..." align:#right  height:18 width:60
	
	Group "Set Parameter"
	(
		checkBox ckb_Jpg "JPEG" checked:false
		slider sld_Quality ""width:170 height:10  range:[0,100,1] type:#integer ticks:1 across:2 tooltip:"设置图片压缩质量" enabled:false
		spinner spn_Quality "" range:[0,100,0] type:#integer scale:1 pos:[175,100] tooltip:"设置图片压缩质量" enabled:false
		
		checkBox ckb_Bmp "BMP"	checked:false
			--{#paletted|#true24|#true48|#gray8|#gray16}
			radiobuttons rdb_BmpType "" labels:#( "paletted(recommend)", "Color24", "noType") align:#left default:1 columns:2 enabled:false
	)
	button btn_CompressNow "Compress" width:240 height:22 tooltip:"设置完毕开始压缩图片."
	
	Group "Image 2N"
	(
		EditText edt_BitmapPath "" width:168 across:2 align:#left pos:[2, 235]
		button btn_BitmapPath "Browse..." tooltip:"设置图片所在路径..." align:#right height:18 width:60
		checkBox ckb_ChkJpg "JPG"	checked:true across:2
		checkBox ckb_ChkBmp "BMP"	checked:true
		button btn_CheckImg "Check" width:230 height:22 tooltip:"检查2N次幂."
		button btn_ModifyImg "Modity To 2N" width:230 height:22 tooltip:"自动修正2N次幂." enabled:false
	)
	progressBar pb_Progressbar "ProgressBar"width:230 height:16 pos:[0, 335] across:2
	button btn_About "?" pos:[230, 334] width:12
/*****************************************
界面控件逻辑
******************************************/
	on sld_Quality changed val do
	(
		spn_Quality.value = val;
	)
	
	on spn_Quality changed val do
	(
		sld_Quality.value = val;
	)
	
	on ckb_Bmp changed state do
	(
		if (state)then
		(
			rdb_BmpType.enabled = true;
		)
		else
		(
			rdb_BmpType.enabled = false;
		)
	)
	on ckb_Jpg changed state do
	(
		if (state)then
		(
			sld_Quality.enabled = true;
			spn_Quality.enabled = true;
		)
		else
		(
			sld_Quality.enabled = false;
			spn_Quality.enabled = false;
		)
	)
----------------------------------------------
	-- set img path
	on btn_Browse pressed do
	(
		btnTitle = Yvi_setImgPath();
		if( btnTitle == undefined )then
		(
			edt_Browse.text = edt_Browse.text;
			return 0;
		)
		edt_Browse.text = btnTitle;
	)
	
	on btn_Saveto pressed do
	(
		btnTitle = Yvi_setImgPath();
		if( btnTitle == undefined )then
		(
			edt_Saveas.text = edt_Saveas.text;
			return 0;
		)
		edt_Saveas.text = btnTitle;
	)
	
/*****************************************
压缩图片存储
******************************************/
	on btn_CompressNow pressed do
	(
		local theJpgFiles = #();
		local theBmpFiles = #();
		local theImgFiles = #();
		local theQuality = sld_Quality.value;
		local srcPath = edt_Browse.text;
		local desPath = edt_Saveas.text;
		
---------------------------
		if( srcPath.count == 0 or desPath.count == 0  )then
		(
			MessageBox "指定路径" title:rl_CompressImage.title;
			return 0;
		)
		if( desPath[desPath.count] != "\\" )then
		(
			desPath += "\\";
		)
		
		if( ckb_Jpg.checked)then
		(
			ext = "*.jpg";
			theJpgFiles = Yvi_getFilesRecursive srcPath ext;
		)
		if( ckb_Bmp.checked )then
		(
			ext = "*.bmp";
			theBmpFiles = Yvi_getFilesRecursive srcPath ext;
		)
-------------------------------
		
		theImgFiles = theJpgFiles + theBmpFiles;
		local imgCounts = theImgFiles.count;
		if( imgCounts != 0)then
		(
			local i = 0;
			for img in theImgFiles do
			(
				fileName = filenameFromPath img;
				ext = getFilenameType img;
				desFile = desPath + fileName;
				if( ext == ".jpg")then
				(
					Yvi_compressJpegImage img desFile quality:theQuality;
				)
				else if( ext == ".bmp")then
				(
					_bmpType = case rdb_BmpType.state of
					(
						1: 0 --#noType
						2: 2 --#paletted
						3: 8 --#true24
					)
					Yvi_compressBmpImage img desFile type:_bmpType;
				)
				---------------
				pb_Progressbar.value = 100.*(i+=1)/imgCounts;
			)
		)
	)
	
	
/*****************************************************/
/*图片文件2的N次幂检查与修正                        */
/****************************************************/
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
	
	global g_ImgFiles = #();
	on btn_CheckImg pressed do
	(
		local theJpgFiles = #();
		local theBmpFiles = #();
		local srcPath = edt_BitmapPath.text;
		g_ImgFiles = #()
		------------------
		if( srcPath.count == 0 )then
		(
			MessageBox "指定路径" title:rl_CompressImage.title;
			return 0;
		)
		if( srcPath[srcPath.count] != "\\" )then
		(
			srcPath += "\\";
		)
		if(ckb_ChkJpg.checked == off and ckb_ChkBmp.checked == off)then
		(
			MessageBox ("选择其中一种图片类型") title:rl_CompressImage.title;
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
				pb_Progressbar.value = 100.*(i+=1)/imgCounts;
			)
			if( counts != 0 )then
			(
				btn_ModifyImg.enabled = true;
				MessageBox ("发现 "+ counts as string + " 个不符合2的N次幂标准的图片。") title:rl_CompressImage.title;
			)
			else
			(
				btn_ModifyImg.enabled = false;
				MessageBox ("全部图片符合2的N次幂标准！") title:rl_CompressImage.title;
			)
		)
	)
/*****************************************
修改图片文件为2的N次幂
******************************************/
	on btn_ModifyImg pressed do
	(
		local i = 0;
		for img in g_ImgFiles where(g_ImgFiles.count != 0)do
		(
			try
			(
				Yvi_ModifyBitmapCloser2Power img;
			)Catch()
			pb_Progressbar.value = 100.*(i+=1)/g_ImgFiles.count;
		)
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
			label lbl1 "" pos:[21,20] width:353 height:130
			button btn1 "OK" pos:[168,158] width:60 height:20
			
			on rl_About open  do
			(
				lbl1.text = "图片压缩：\n  1.指定图片输入目录，指定压缩后保存的目录。
  2.选择压缩图片类型(JPEG、BMP)，选择压缩jpg质量、bmp类型。
修改尺寸：\n  1.指定图片输入目录，选择图片类型（JPEG、BMP）。
  2.检查是否存在非2次幂的图片。
  3.如果有非2次幂图片，则修改为2次幂图片按钮可用。
※[PS:图片输入目录包含当前目录下的子目录]\n-------------------------------------------------
		\t\t\tDesigned by Yvi @2012-6-29"
			)
			on btn1 pressed  do
			(
				DestroyDialog rl_About;
			)
		)CreateDialog rl_About;
	)
)

CreateDialog rl_CompressImage style:#(#style_sysmenu, #style_minimizebox, #style_titlebar)





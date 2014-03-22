
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

function Yvi_getMaxVersion =
(
	local version = (filterString ( (MaxVersion() )[1] as string) "0")[1] as integer;
)
/************************************************************/
/*UI 消息处理事件								 */
/************************************************************/
Try(DestroyDialog rl_CompressImage)Catch()

rollout rl_CompressImage "Compress&2NPower" width:245 height:414
(
	checkbutton chbtn_EN "EN" pos:[55,7] width:25 height:14 highlightColor:( random (color 0 0 0) (color 255 255 255) ) across:2 checked:true toolTip:"Switch to english language"
	checkbutton chbtn_CN "CN" pos:[164,7] width:25 height:14 toolTip:"切换到英文界面"
	
	GroupBox grb_1 "Compress Image" pos:[3,25] width:238 height:241
	
	label lbl1 "Set Path:" pos:[10,42] width:116 height:14
	edittext edt_Browse "" pos:[4,57] width:173 height:17 across:2
	button btn_Browse "Browse..." pos:[176,57] width:60 height:18 toolTip:"Set the source image files where the path..."
	edittext edt_Saveas "" pos:[4,77] width:174 height:17 across:2
	button btn_Saveto "Save as..." pos:[176,77] width:60 height:18 toolTip:"Set save igmage files where the path..."
		
	checkbox ckb_Jpg "JPEG" pos:[8,100] width:51 height:15 checked:false tooltip:"Compression *.jpg image files"
	slider sld_Quality "Set Quality:" pos:[19,119] width:168 height:44 enabled:false range:[0,100,1] type:#integer ticks:1 across:2 toolTip:"Set quality(the higher value, the lower compression rat.)"
	spinner spn_Quality "" pos:[179,145] width:57 height:16 enabled:false range:[0,100,0] type:#integer scale:1 toolTip:"Set quality(the higher value, the lower compression rat.)"
	checkbox ckb_Bmp "BMP" pos:[7,165] width:47 height:15 checked:false tooltip:"Compression *.bmp image files"
	--{#paletted|#true24|#true48|#gray8|#gray16}
	radiobuttons rdb_BmpType "" pos:[23,183] width:156 height:48 enabled:false labels:#("paletted(recommend)", "Color24", "noType") default:1 columns:1
	button btn_CompressNow "Compress" pos:[8,236] width:230 height:22 toolTip:"Now compression."


	groupBox grb_2 "Modity Image 2N Power" pos:[4,278] width:237 height:112
	
	label lbl2 "Set Images Path:" pos:[10,298] width:117 height:15
	edittext edt_BitmapPath "" pos:[5,312] width:170 height:17 across:2
	button btn_BitmapPath "Browse..." pos:[175,312] width:60 height:18 toolTip:"Set image files where the path..."
	checkbox ckb_ChkJpg "JPG" pos:[10,340] width:44 height:15 checked:true across:2 toolTip:"Check for *.jpg image files"
	checkbox ckb_ChkBmp "BMP" pos:[119,340] width:47 height:15 checked:true toolTip:"Check for *.bmp image files"
	button btn_CheckImg "Check No 2N Power" pos:[7,362] width:115 height:22 toolTip:"Check 2N power"
	button btn_ModifyImg "Modity To 2N Power" pos:[122,362] width:115 height:22 enabled:false toolTip:"Auto modity image files to 2N power"
	
	progressBar pb_Progressbar "ProgressBar" pos:[1,398] width:231 height:16 across:2
	button btn_About "?" pos:[231,397] width:13 height:17
		
	/*****************************************/
	/*中英文切换*/
	/****************************************/
	on chbtn_EN changed state do
	(
		chbtn_CN.checked = off;
		chbtn_EN.highlightColor = ( random (color 0 0 0) (color 255 255 255) );
		if state == true then 
		(
			chbtn_EN.caption = "EN";chbtn_EN.toolTip = "Switch to english language";
			chbtn_CN.caption = "CN";chbtn_CN.toolTip = "切换到英文界面";
			grb_1.caption = "Compress Image";
			lbl1.text = "Set Path:";
			btn_Browse.caption = "Browse...";btn_Browse.toolTip = "Set the source image files where the path...";
			btn_Saveto.caption = "Save as...";btn_Saveto.toolTip = "Set save igmage files where the path...";
							
			-- higher 2009
			if( Yvi_getMaxVersion() > 9 )then
			(
				ckb_Jpg.tooltip = "Compression *.jpg image files"
				ckb_Bmp.tooltip = "Compression *.bmp image files"
			)
		
			sld_Quality.caption = "Set Quality:";
			--higher 2010
			if( Yvi_getMaxVersion() >= 12 )then
			(	
				sld_Quality.toolTip = "Set quality(the higher value, the lower compression rat.)";
				spn_Quality.toolTip = "Set quality(the higher value, the lower compression rat.)"
			)
			btn_CompressNow.caption = "Compress";btn_CompressNow.toolTip = "Now compression.";
			
			grb_2.caption = "Modity Image 2N Power";
			lbl2.text = "Set Images Path:";
			btn_BitmapPath.caption = "Browse...";btn_BitmapPath.toolTip = "Set image files where the path...";
									
			-- higher 2009
			if( Yvi_getMaxVersion() > 9 )then
			(
				ckb_ChkJpg.toolTip = "Check for *.jpg image files"
				ckb_ChkBmp.toolTip = "Check for *.bmp image files"
			)
			
			btn_CheckImg.caption = "Check No 2N Power";btn_CheckImg.toolTip = "Check 2N power";
			btn_ModifyImg.caption = "Modity To 2N Power";btn_ModifyImg.toolTip = "Auto modity image files to 2N power";
			
			rl_CompressImage.title = "Compress&2NPower";
		)
		else
		(
			chbtn_EN.checked = on;
		)
	)
	on chbtn_CN changed state do
	(
		chbtn_EN.checked = off;
		chbtn_CN.highlightColor = ( random (color 0 0 0) (color 255 255 255) );
		if state == true then 
		(
			chbtn_EN.caption = "英";chbtn_EN.toolTip = "Switch to english language";
			chbtn_CN.caption = "中";chbtn_CN.toolTip = "切换到英文界面";
			grb_1.caption = "压缩图片";
			lbl1.text = "设置路径:";
			
			btn_Browse.caption = "浏览...";btn_Browse.toolTip = "设置图片所在路径...";
			btn_Saveto.caption = "另存...";btn_Saveto.toolTip = "设置保存压缩后图片存放路径...";
									
			-- higher 2009
			if( Yvi_getMaxVersion() > 9 )then
			(
				ckb_Jpg.tooltip = "压缩 *.jpg 图片文件"
				ckb_Bmp.tooltip = "压缩 *.bmp 图片文件"
			)
			
			sld_Quality.caption = "压缩质量:";
			if ( Yvi_getMaxVersion() >= 12 ) then
			(
				sld_Quality.toolTip = "设置图片压缩质量(值越大,压缩率越低)";
				spn_Quality.toolTip = "设置图片压缩质量(值越大,压缩率越低)"
			)
			btn_CompressNow.caption = "压缩";btn_CompressNow.toolTip = "设置完毕开始压缩图片";
			
			grb_2.caption = "检查/修改2N幂";
			lbl2.text = "设置图片路径:";
			btn_BitmapPath.caption = "浏览...";btn_BitmapPath.toolTip = "设置图片所在路径...";
				
			-- higher 2009
			if( Yvi_getMaxVersion() > 9 )then
			(			
				ckb_ChkJpg.toolTip = "检查 *.jpg 图片文件"
				ckb_ChkBmp.toolTip = "检查 *.bmp 图片文件"
			)
			
			btn_CheckImg.caption = "检查非2N次幂";btn_CheckImg.toolTip = "检查2N次幂";
			btn_ModifyImg.caption = "修改为2N次幂";btn_ModifyImg.toolTip = "自动修正接近2N次幂";
			rl_CompressImage.title = "图片压缩&修改2N幂";
		)
		else
		(
			chbtn_CN.checked = on;
		)
	)	
	/*****************************************
	界面控件逻辑
	******************************************/
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
			label lbl1 "" pos:[21,20] width:355 height:130
			button btn1 "OK" pos:[168,158] width:60 height:20
			
			on rl_About open  do
			(
				lbl1.text = "图片压缩：\n  1.指定图片输入目录，指定压缩后保存的目录。
	2.选择压缩图片类型(JPEG、BMP)，选择压缩jpg质量、bmp类型。
	修改尺寸：\n  1.指定图片输入目录，选择图片类型（JPEG、BMP）。
	2.检查是否存在非2次幂的图片。
	3.如果有非2次幂图片，则修改为2次幂图片按钮可用。
	※[PS:图片输入目录包含当前目录下的子目录]\n-----------------------------	            \n\t\t\t\tDesigned by Yvi @2012-6-29"
			)
			on btn1 pressed  do
			(
				DestroyDialog rl_About;
			)
		)CreateDialog rl_About;
	)
)
CreateDialog rl_CompressImage style:#(#style_sysmenu, #style_minimizebox, #style_titlebar)

--
-- try(closeRolloutFloater theFloater)catch()
-- theFloater = newRolloutFloater "" 305 420 
-- addRollout rl_CompressImage theFloater
-- addRollout rl_ModifyImg2NPower theFloater rolledUp:true



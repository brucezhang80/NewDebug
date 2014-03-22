


--·�� �ļ������磺"*.max"
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
	img_path = getSavePath caption:"ѡ��ͼƬ���ڵ�Ŀ¼...";
	if(img_path == undefined)then
	(
		return undefined;
	)
	
	return img_path;
)

/********************************************************************/
/* �����ͼ�Ƿ���2��N���ݣ�������ǣ��޸�����ӽ�2���ݵ���ֵ        */
/********************************************************************/
----------------------------------------------------------------------
-- �Ƿ���2��N��
function Yvi_isTwoPower num =
(
	
	return ( (num >= 2) and ( ( bit.and num (num - 1) ) == 0) );-- 2��n���ݿ϶�����0, bit.and λ������
)
	
function Yvi_isBitmap2Power width height =
(
	return ( (Yvi_isTwoPower  width) and (Yvi_isTwoPower  height) );
)

-- ��ȡ�ӽ�2��N���ݵ�ֵ�����Ͻӽ������½ӽ���
function Yvi_getCloser2Power num =
(
	oldNum = num;

	-- ����ʹ��λ������
	while( (bit.and num (num-1) ) != 0 )do
	(
		num = bit.and num (num-1);
	)
	-- λ����һλ�����-1������һλ��
	high = bit.shift num 1; -- �õ� ��ǰ num ���Ͻӽ� 2��N�ε�ֵ
	low = num;				-- �õ� ��ǰ num ���½ӽ� 2��N�ε�ֵ
	
	-- �Ƚϵõ����������� ���� ����ֵ����ԭʼ����ֵ�ȡ�ò�ֵС�ġ�
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
	
	-- �������2��N�Σ���������
	if ( Yvi_isBitmap2Power _width _height ) then
	(
		return 0;
	)
	_width = Yvi_getCloser2Power _width;
	_height = Yvi_getCloser2Power _height;
	-- ��Ҫ�Բ�ͬ���͵�ͼƬ���в�ͬ��ʽ������
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
	/*ͼƬ�ļ�2��N���ݼ��������                        */
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
			MessageBox "ָ��·��" title:rl_Modify2NPower.title;
			return 0;
		)
		if( srcPath[srcPath.count] != "\\" )then
		(
			srcPath += "\\";
		)
		if(ckb_ChkJpg.checked == off and ckb_ChkBmp.checked == off)then
		(
			MessageBox ("ѡ������һ��ͼƬ����") title:rl_Modify2NPower.title;
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
				MessageBox ("���� "+ counts as string + " ��������2��N���ݱ�׼��ͼƬ��") title:rl_Modify2NPower.title;
			)
			else
			(
				btn_ModifyImg.enabled = false;
				MessageBox ("ȫ��ͼƬ����2��N���ݱ�׼��") title:rl_Modify2NPower.title;
			)
		)
		GC();
	)
	/*****************************************
	�޸�ͼƬ�ļ�Ϊ2��N����
	******************************************/
	on btn_ModifyImg pressed do
	(
		if(ckb_ChkJpg.checked == off and ckb_ChkBmp.checked == off)then
		(
			MessageBox ("ѡ������һ��ͼƬ����") title:rl_Modify2NPower.title;
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
		MessageBox ("���") title:rl_Modify2NPower.title;
		GC();
	)
	
	
		/*****************************************
	���ڻ����
	******************************************/
	on btn_About pressed do
	(
		try(DestroyDialog rl_About)Catch()
		rollout rl_About "����" width:389 height:180
		(
			groupBox grp1 "About" pos:[10,6] width:368 height:150
			label lbl1 "" pos:[21,20] width:355 height:130
			button btn1 "OK" pos:[168,158] width:60 height:20
			
			on rl_About open  do
			(
				lbl1.text = "�޸ĳߴ磺\n  
	1.ָ��ͼƬ����Ŀ¼��ѡ��ͼƬ���ͣ�JPEG��BMP����
	2.����Ƿ���ڷ�2���ݵ�ͼƬ��
	3.����з�2����ͼƬ�����޸�Ϊ2����ͼƬ��ť���á�
	��[PS:ͼƬ����Ŀ¼������ǰĿ¼�µ���Ŀ¼]\n
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
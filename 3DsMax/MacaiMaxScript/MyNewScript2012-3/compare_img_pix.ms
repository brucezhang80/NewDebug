rl_test
(
	Struct imageStrct
	(
		image_file = undefined,
		bSame = false,
		bChanged = false
	)

	Function doCompareImages img &imgsArr &sameImgArr = 
	(
		if(imgsArr.count != 0)then
		(
			for img2 in imgsArr do 
			(
				ret = CompareBitmaps img img2 10 128 ;-- useAlpha:bTga;
				if(true == ret) then
				(				
					--在这里做贴图的修改
					--。。。将img贴图修改成img2贴图
					appendIfUnique sameImgArr img;
					break;
				)
				else
				(
					appendIfUnique imgsArr img;
				)
			)
		)
		else
			append imgsArr img;
	)
	
	Function collectSceneImage &sameImgArr=
	(
		imagesArr = #();
		for m in  scenematerials do 
		(
			if classOf m == MultiMaterial then
			(
				for i = 1 to m.numsubs do 
				(
					-- 收集的时候就进行比较图片
					img = m[i].diffuseMap.filename;
					doCompareImages img imagesArr sameImgArr;
					--appendIfUnique  imgArr m[i].diffuseMap.filename;
				)
			)
			else
			(
				img = m.diffuseMap.filename;
				doCompareImages img imagesArr sameImgArr;			
			)
		)
		
		freeSceneBitmaps();
		gc();
	)

	Function createImageObject &imgArr &imgObjArr=
	(
		for img in imgArr do 
		(
			imageObject = imageStrct image_file:img bSame:false bChanged:false;
			append imgObjArr imageObject;
		)
	)

	Function doCompareImage imgObjArr &sameImgArr=
	(
		counts = imgObjArr.count;
		bTga = false;
		for i=1 to counts-1 where(false == imgObjArr[i].bChanged and false == imgObjArr[i].bSame) do 
		(
			ext0 = getFilenameType imgObjArr[i].image_file;
			for j=i+1 to counts where(false == imgObjArr[j].bChanged and false == imgObjArr[j].bSame) do 
			(
				ext1 = getFilenameType imgObjArr[j].image_file;
				if(ext0 == ".tga" and ext1 == ".tga")then
					bTga = true;
				else
					bTga = false;
				
				ret = CompareBitmaps imgObjArr[i].image_file imgObjArr[j].image_file 10 128 useAlpha:bTga;
				if(true == ret) do 
				(
					imgObjArr[j].bSame = true;
					
					--change same image1 filename to image0
					--imgObjArr[j].image_file = imgObjArr[i].image_file;
					imgObjArr[j].bChanged = true;
					--print (imgObjArr[j].image_file +" \tand\t "+ imgObjArr[i].image_file);
					
					appendIfUnique sameImgArr imgObjArr[i].image_file;
				)
				
			)
			
			rl_test.pgb_do.value = 100.* i/counts;
		)
		rl_test.pgb_do.value = 100.;
		freeSceneBitmaps();
		gc();
		ok;
	)
	
	
	Rollout rl_test "rl_test"
	(
		button btn_do "do" 
		progressBar pgb_do ""
		
		on btn_do pressed do 
		(
			imageArr = #();
			imageObjArr = #();
			sameImageArr = #();
			
			collectSceneImage sameImageArr;
			--createImageObject imageArr imageObjArr;
			--doCompareImage imageObjArr sameImageArr;		
			freeSceneBitmaps();
			gc();
			
			print sameImageArr.count;
		)
	)CreateDialog rl_test

	

)
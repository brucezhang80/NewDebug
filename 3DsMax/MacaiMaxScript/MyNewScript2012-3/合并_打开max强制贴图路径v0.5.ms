-- 合并/打开max文件，强制贴图路径为当前模型文件路径

/*
*2013-2-20使用方案1：以材质类型修改贴图路径，工作量大，而不确定所使用的具体材质类型有哪些，相同类型材质的使用的材质的方式又不同，所以该方案被抛弃。
*2013-2-22使用方案2：修改贴图追踪器的路径（项目路径）的方法，打开文件时能解决，但合并时无法解决丢失的问题。并且效率很低。 
*2013-2-25使用第3方案：使用方案3修正丢失贴图，效率明显提高。
*2013-2-26修复bug:修复部分贴图丢失的bug
*2013-2-27修复bug:修复部分定义了filename属性的材质但未给予贴图名，以致于得到的filename属性是undefined而出错。增加了广告，动态切换功能。修复部分操作提示有误的bug。
*2013-2-28修复bug：修复VR代理材质（贴图）丢失的bug
*/
global yviSetBitmmapNewPathRL;

-- 	Function yviShowMissingFilesDlg missingFiles =
-- 	(
-- 		global missFileList = #();
-- 		missFileList = missingFiles
-- 		try(DestroyDialog yviMissBitmapRL)catch()
-- 		Rollout yviMissBitmapRL "Missing Files" width:680
-- 		(
-- 			Label lb_miss "丢失的文件：" align:#left
-- 			ListBox lb_missFiles "" width:500
-- 			Label lb_found "找到的文件：" align:#left
-- 			ListBox lb_foundFiles "" width:500
-- 			button btn_previewFile ""  width:140 height:140 border:false offset:[255,-146]
-- 			
-- 			on yviMissBitmapRL open do 
-- 			(
-- 				mainPos = GetDialogPos yviSetBitmmapNewPathRL;
-- 				SetDialogPos yviMissBitmapRL [mainPos.x+310, mainPos.y];
-- 				lb_miss.text += missFileList.count as string;
-- 				lb_missFiles.items = missFileList;
-- 			)
-- 			
-- 			on lb_foundFiles doubleClicked index do
-- 			(
-- 				item = lb_foundFiles.items[index];
-- 				str = "/e,/select,\"" + item + "\"";
-- 				ShellLaunch "explorer.exe" str;
-- 			)
-- 			
-- 			on lb_foundFiles selected index do
-- 			(
-- 				item = lb_foundFiles.items[index];
-- 				if(not doesFileExist item)do
-- 				(
-- 					btn_previewFile.text = "文件已丢失"
-- 					return 0;
-- 				)
-- 				btn_previewFile.images = #(item, undefined, 1, 1, 1, 1, 1)
-- 			)
-- 		)
-- 		CreateDialog yviMissBitmapRL ;
-- 	)
	
	-- 取得子文件夹下的文件
	Function yviGetFilesRecursive root ext =
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
			join all_Files (getFiles (f + ext))
		)
		join all_Files (getFiles (root + ext))
			
		all_Files
	)
	
	-- 转换成小写
	Function yviStringToLower instr =
	(
		local upper, lower, outstring;
		upper="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		lower="abcdefghijklmnopqrstuvwxyz";

		outstr = copy instr;
		for i=1 to outstr.count do
		( 
			j = findString upper outstr[i];
			if (j != undefined) do outstr[i] = lower[j];

		)
		
		outstr;
	)
	
	-- 得到的文件名都转成小写
	Function yviFilenameFromPath file =
	(
		filename = yviStringToLower (getFilenameFile file + getFilenameType file);
		filename;
	)

	global missing_sceneMap_files = #();
	Function yviGetSceneMissingMap =
	(	
		missing_sceneMap_files = #();
		-- defined the function to be called by enumeratefiles.
		Function yviAddSceneMap mapfile =
		(
			local mapfileN = mapfile as name;
			local index = finditem missing_sceneMap_files mapfileN;
			if index == 0 do append missing_sceneMap_files mapfileN;
		)
		
		enumeratefiles yviAddSceneMap #missing;
		--sort missing_sceneMap_files;
		--missing_sceneMap_files;
	)
	
	Function yviSetOpenModelBitmapPath bmpPath =
	(
		yviSetBitmmapNewPathRL.lb_stateBar.text = "正在重设丢失贴图的路径......";
		local all_Files = #();
		all_Files = yviGetFilesRecursive bmpPath "*.*";

		local all_file_tripname = #();
		for f in all_Files do append all_file_tripname (yviFilenameFromPath f);
		
		--print all_file_tripname;
		-- 贴图/明暗器 丢失处理
		local bitmap_Instance = #();
		local bkgroun_files = #();
		local isMissingBkground = false;
		bitmap_Instance = getClassInstances BitmapTexture;
		if ( not(doesFileExist(backgroundImageFilename)) ) then
		(
			--format "backgroundImageFilename:%\n" backgroundImageFilename;
			isMissingBkground = true;
			append bitmap_Instance backgroundImageFilename;
		)

		-- 取得磁盘中所有文件的文件名
		for bi in bitmap_Instance do 
		(
			--当前场景中使用的材质是否有filename属性
			if( isProperty bi #filename )do 
			(
				if(bi.filename != undefined)do
				(
					local bi_tripname = yviFilenameFromPath bi.filename;
					local index = findItem all_file_tripname bi_tripname;
					
					if( index != 0 )then
					(
						bi.filename = all_Files[index];
					)
				)
			)
		)
		
		---灯光
		local web_points_free = for l in lights where  matchPattern (classof l as string) pattern:"Free_*" collect l;
		local web_points_target = for l in lights where  matchPattern (classof l as string) pattern:"Target_*" collect l;
		local all_webPoint = #();
		join all_webPoint web_points_free
 		join all_webPoint web_points_target;
		
		local missing_web_points = for wp in all_webPoint while wp.webfile != "" where (not doesFileExist wp.webfile) collect wp;
		for w in missing_web_points do
		(	
			local webfile_tmp = w.webfile;
			local webfile_tripname = yviFilenameFromPath webfile_tmp;
			if( (index = findItem all_file_tripname webfile_tripname) != 0 )then 	-- check if the current webfile is missing
					w.webfile = all_Files[index];		-- Relinks the current webfile to the found file
		)

		-- 环境
		if( isMissingBkground )do 
		(
			for bk in bitmap_Instance do 
			(
				local bk_tripname = yviFilenameFromPath (bk as string);
				local index = findItem all_file_tripname bk_tripname;
				if( index != 0 )do 
					backgroundImageFilename = all_Files[index];-- Relink the background
			)
		)
		
		-- VR代理
		local vr_proxy = for vro in geometry where(classOf vro == VR代理 or classOf vro == VRaryProxy) collect vro;
		for vrp in vr_proxy do 
		(
			if(isProperty vrp #filename)do 
			(
				if(vrp.filename != undefined)do
				(
					local vrp_tripname = yviFilenameFromPath vrp.filename;
					local index = findItem all_file_tripname vrp_tripname;
					
					if( index != 0 )then
					(
						vrp.filename = all_Files[index];
					)
				)
			)
		)
		
 		ATSOps.ReFresh();--refresh assert Track Tracking System
		actionMan.executeAction -841213575 "2"
		ret = ATSOps.ResolveSelectionRelativeToProjectFolder();
		if(not ret)then print "Failed."else print "Successful."
	)

	-- 合并时，确保原始的贴图路径不被修改
	-- 
	Function yviSetMergeModelBitmapPath bmpPath =
	(
		yviGetSceneMissingMap();
		
		--print missing_sceneMap_files;
		
		yviSetBitmmapNewPathRL.lb_stateBar.text = "正在重设丢失贴图的路径......";
		local all_Files = #();
		all_Files = yviGetFilesRecursive bmpPath "*.*";

		local all_file_tripname = #();
		for f in all_Files do append all_file_tripname (yviFilenameFromPath f);
		
		local bitmap_Instance = #();
		bitmap_Instance = getClassInstances BitmapTexture;
		
		-- 贴图/明暗器 丢失处理
		local bkgroun_files = #();
		local isMissingBkground = false;
		
		if ( not(doesFileExist(backgroundImageFilename)) ) then
		(
			isMissingBkground = true;
			append missing_sceneMap_files backgroundImageFilename;
		)

		-- 取得磁盘中所有文件的文件名
		for miss in bitmap_Instance do 
		(
			if(isProperty miss #filename)do
			(
				if(miss.filename != undefined)do
				(
					local miss_tripname = yviFilenameFromPath miss.filename;
					local index = findItem all_file_tripname miss_tripname;
					if( index != 0 )then
					(
						miss.filename = all_Files[index];
					)
				)
			)
		)
				
		---灯光
		local web_points_free = for l in lights where  matchPattern (classof l as string) pattern:"Free_*" collect l;
		local web_points_target = for l in lights where  matchPattern (classof l as string) pattern:"Target_*" collect l;
		local all_webPoint = #();
		join all_webPoint web_points_free
 		join all_webPoint web_points_target;
		
		local missing_web_points = for wp in all_webPoint while wp.webfile != "" where (not doesFileExist wp.webfile) collect wp;

		for w in missing_web_points do
		(	
			local webfile_tmp = w.webfile;
			local webfile_tripname = yviFilenameFromPath webfile_tmp;
			if( (index = findItem all_file_tripname webfile_tripname) != 0 )then 	-- check if the current webfile is missing
					w.webfile = all_Files[index];		-- Relinks the current webfile to the found file
		)

		-- 环境
		if( isMissingBkground )do 
		(
			for bk in bitmap_Instance do 
			(
				local bk_tripname = yviFilenameFromPath (bk as string);
				local index = findItem all_file_tripname bk_tripname;
				if( index != 0 )do 
					backgroundImageFilename = all_Files[index];-- Relink the background
			)
		)
		-- VR代理
		local vr_proxy = for vro in geometry where(classOf vro == VR代理 or classOf vro == VRaryProxy) collect vro;
		for vrp in vr_proxy do 
		(
			if(isProperty vrp #filename)do 
			(
				if(vrp.filename != undefined)do
				(
					local vrp_tripname = yviFilenameFromPath vrp.filename;
					local index = findItem all_file_tripname vrp_tripname;
					
					if( index != 0 )then
					(
						vrp.filename = all_Files[index];
					)
				)
			)
		)
		
 		ATSOps.ReFresh();--refresh assert Track Tracking System
		actionMan.executeAction -841213575 "2"
		ret = ATSOps.ResolveSelectionRelativeToProjectFolder();
		if(not ret)then print "Failed."else print "Successful."
	)
	
	Function yviUniqueObjname =
	(
		for obj in Objects do 
			obj.name = ( uniquename obj.name);
	)
	
	Function yviMergeModel maxfile bitmapPath=
	(
		if( not (isMaxFile maxfile) )do
		(
			messageBox "请选择3dsmax文件。";
			return 0;
		)
		if(not doesFileExist maxfile)do
		(
			messageBox ("目标文件不存在:"+maxfile);
			return 0;
		)

		yviSetBitmmapNewPathRL.lb_stateBar.text = "正在合并......";
		clearSelection();
		
		-- start auto rename
-- 		DialogMonitorOPS.unRegisterNotification id:#autoRename
-- 		DialogMonitorOPS.RegisterNotification checkDialog id:#autoRename
-- 		DialogMonitorOPS.Enabled = true
		
		bSuccess = mergeMAXFile maxfile #noRedraw #select #mergeDups #renameMtlDups #neverReparent quiet:true;--#neverReparent #noRedraw
		if (  not bSuccess ) do 
		(
			--importFile maxfile #noPrompt;
			yviSetBitmmapNewPathRL.lb_stateBar.text = "合并失败。此模型高于当前3dsmax版本";
			return false;
		)
		
-- 		DialogMonitorOPS.Enabled = false
-- 		DialogMonitorOPS.unRegisterNotification id:#autoRename
		
		yviSetMergeModelBitmapPath bitmapPath;
		
		yviSetBitmmapNewPathRL.lb_stateBar.text = "合并已完成。";
	)
	
	Function yviOpenModel maxfile bitmapPath =
	(
		if( not (isMaxFile maxfile) )do
		(
			messageBox "请选择3dsmax文件。";
			return 0;
		)
		if(not doesFileExist maxfile)do
		(
			messageBox ("目标文件不存在:"+maxfile);
			return 0;
		)
		yviSetBitmmapNewPathRL.lb_stateBar.text = "正在打开......";
		if ( checkForSave() ) do
		(
			resetMaxFile #noPrompt;
			ret = loadMaxFile maxfile useFileUnits:true quiet:true;
			if(not ret)do
			(
				yviSetBitmmapNewPathRL.lb_stateBar.text = "打开失败。此模型高于当前3dsmax版本";
				return false;
			)
			yviSetOpenModelBitmapPath bitmapPath;
			yviSetBitmmapNewPathRL.lb_stateBar.text = "打开已完成。";
		)		
	)

	Function yviIsLowMaxversion = 
	(
		local version = maxVersion();
		if(version[1] < 9000)then
		(
			yviSetBitmmapNewPathRL.lb_stateBar.text = "当前3dsmax版本过低，请选择3dsmax 9.0以上版本。";
			messageBox "当前3dsmax版本过低，请选择3dsmax 9.0以上版本。";
			return true;
		)
		
		return false;
	)
	-- get the "C:\WINDOWS\system32\reg.ini" or "C:\WINDOWS\sysWOW64\reg.ini" dose exsits
	Function yviIsLicensing =
	(
		licens32 = "C:/WINDOWS/system32/reg.ini";
		licens64 = "C:/WINDOWS/sysWOW64/reg.ini";
		
		if(doesFileExist licens32 or doesFileExist licens64)do
		(
			return true;
		)
		
		return false;
	)
	
	Function yviRegWindow =
	(
		try(DestroyDialog yviRegWindowRl)catch()	
		Rollout yviRegWindowRl "www.jinke3d.com" width:200 height:70
		(
			Label lb_msg "请在已注册金科图库的电脑上使用！";
			Label lb_web "金科图库网：" offset:[10,0] align:#left 
			HyperLink hlk_web "www.jinke3d.com" color:blue hoverColor:red visitedColor:(color 127 0 127) address:"http://www.jinke3d.com" offset:[80,-18]
			
			Button btn_close "  ok  "
			
			on btn_close pressed do
			(
				DestroyDialog yviRegWindowRl;
			)
		)
		createDialog yviRegWindowRl modal:true;
	)
	
	Function yviGetHtmlStr =
	(
		local file = (getDir #downloads +"\\index.html");
		if(not doesFileExist file)do
		(
			--MessageBox "创建广告栏数据失败：广告位图片尚未添加，请在“广告设置”中添加地址和图片地址。"
			return undefined;
		)
		local fstream = openFile file mode:"r";
		if(fstream == undefined)do 
		(
			Messagebox "创建广告栏数据失败。"
			return undefined;
		)
		local str = "MSHTML:";
		while(not eof fstream)do
		(
			str+=readLine fstream;
		)
		close fstream;
		str;
	)
	
	Function yviMakeHtmlStr linkArr imgArr titleArr bgColor=
	(
		thmlStr1 = "<html xmlns=\"http://www.w3.org/1999/xhtml\">
					<head>
					<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />
					<title>ads</title>
					</head>
					<script language=\"javascript\">
						var nowFrame=1;
						var maxFrame=4;	

						function picPlay()
						{
							var next=nowFrame+1 ;
							if(next==maxFrame+1)
							{
								nowFrame=maxFrame;
								next=1;
							}
							document.getElementById('pic'+next).style.display=\"block\";
							document.getElementById('pic'+nowFrame).style.display=\"none\";
							if(nowFrame==maxFrame)
							{
								nowFrame=1;
							}
							else
							{
								nowFrame++;
							}	
							setTimeout('picPlay()',2000);
						}	

					</script>
					<body bgColor=\""+ bgColor +"\">
					<body onload=\"picPlay()\">
						<table border=\"0\" align=\"left\" >
							<tr>
						<td align=\"left\">\n";
			local linkStr="";
			for i=1 to linkArr.count do
			(
				if(i==1)then
					linkStr += "<a href=\"" + linkArr[i] + "\"><img src=\""+ imgArr[i] + "\" width=\"290\" height=\"120\" id=\"pic"+i as string + "\" title=\"" + titleArr[i]+ "\" style=\"display:block\"/></a>\n";
				else
					linkStr += "<a href=\"" + linkArr[i] + "\"><img src=\""+ imgArr[i] + "\" width=\"290\" height=\"120\" id=\"pic"+i as string + "\" title=\"" + titleArr[i]+ "\" style=\"display:none\"/></a>\n";

			)
			
		thmlStr3 =	"		</td>
							</tr>
						</table>
					</body>
					</html>";

		local thmlStr = thmlStr1 + linkStr + thmlStr3;

		filename = (getDir #downloads +"/index.html");
		fstream = CreateFile filename;
		if(fstream == undefined)do
		(
			MessageBox "创建文件失败。";
			return undefined;
		)
		
		inFile = openFile filename mode:"r+";
		if(inFile == undefined)do
		(
			MessageBox "打开文件失败。";
			return undefined;
		)
		format "%" thmlStr to:inFile;
		
		flush inFile;
		close inFile;
	)
	
	Function yviGetWindowBgColor= 
	(
		r = (((colorMan.getColor #background )[1])*255) as string;
		g = (((colorMan.getColor #background )[2])*255) as string;
		b = (((colorMan.getColor #background )[3])*255) as string;
		clr = "rgb(" + r +"," + g +"," +b +")";
		clr;
	)
	
	Function yviGetAdrAndImg &addrList &imgList=
	(
		local file = (getDir #downloads +"\\index.html");
		local fstream = openFile file mode:"r";
		if(fstream == undefined)do 
		(
			Messagebox "打开文件失败。"
			return undefined;
		)
		local str = "";
		while(not eof fstream)do
		(
			str =readLine fstream;
			
			filterStr = filterstring str "><"
			if(filterStr.count > 1 )do
			(
				i = findString filterStr[1] "http:";
				
				if(i == undefined )then 
					continue;
				else
				(
					j = findString filterStr[1] ".com";
					if(j == undefined)do
						continue;
					addr = substring filterStr[1] i (j-i+4);
				
					k = findString filterStr[2] "http:";
					l = findString filterStr[2] ".jpg";
					if(l == undefined)then
					(
						l = findString filterStr[2] ".png";
						if(l == undefined)then
						(
							l = findString filterStr[2] ".bmp";
							if(l == undefined)then
							(
								l = findString filterStr[2] ".gif";
								if(l == undefined)then
								(
									continue;
								)
							)
						)
					)
					img = substring filterStr[2] k (l-k+4);
					append addrList addr;
					append imgList img;
				)
			)
			
		)
		close fstream;
	)
	
	linkArr = #("http://www.jinke3d.com/?action-viewnews-itemid-20", 
				"http://www.jinke3d.com/?action-viewnews-itemid-21",
				"http://www.jinke3d.com/?action-viewnews-itemid-22"
				);
	imgArr = #("http://www.jinke3d.com/attachments/ad/gg01.gif",
				"http://www.jinke3d.com/attachments/ad/gg02.gif",
				"http://www.jinke3d.com/attachments/ad/gg03.gif"
				);
	titleArr =#("广告1","广告2","广告3");
				

		global clr = yviGetWindowBgColor();
		--global memHtml = yviMakeHtmlStr linkArr imgArr titleArr clr;
		--global html = yviGetHtmlStr();	
		--global fHtml = "file://\\"+(getDir #downloads + @"/index.html");
		--"file://C:\\Users\\Yvi\\Documents\\3dsMax\\downloads\\index.html"
	--/***********************************************************************************************/
	try(DestroyDialog yviSetBitmmapNewPathRL)catch()
	Rollout yviSetBitmmapNewPathRL "金科智能MAX贴图路径追踪器" width:300
	(	
		Group ""
		(
			--www.jinke3d.com 金科图库
			EditText edt_maxPath "选择3dsmax文件：" labelOnTop:true width:240
			Button btn_open "..." offset:[120,-25] tooltip:"指定合并/打开并强制修改贴图路径的3dsmax文件"
			Button btn_mergeModel "合并模型" across:2 tooltip:"合并当前指定的3dsmax文件到当前场景" --enabled:false
			Button btn_openModel "打开模型" tooltip:"打开当前指定的3dsmax文件模型"
			Label lb_description "彻底解决打开/合并3D模型位图/光度学路径丢失问题。"offset:[0,0]
			Label lb_web "版权所有：金科图库网：" offset:[20,0] align:#left 
			HyperLink hlk_web "www.jinke3d.com" color:blue hoverColor:red visitedColor:(color 127 0 127) address:"http://www.jinke3d.com" offset:[150,-18]
			--progressBar pgb_state1 "" value:0 height:10 color:green
			--progressBar pgb_state2 "" value:0 height:10 color:green
		)
		--GroupBox group_ads "" width:290 height:1 align:#left Offset:[-8,0]
		--(	
		activeXControl ax "MSHTML:
<html xmlns=\"http://www.w3.org/1999/xhtml\">
<head>
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />
<title>ads</title>
</head>
<script language=\"javascript\">
	var nowFrame=1;
	var maxFrame=3;	

	function picPlay()
	{
		var next=nowFrame+1 ;
		if(next==maxFrame+1)
		{
			nowFrame=maxFrame;
			next=1;
		}
		document.getElementById('pic'+next).style.display=\"block\";
		document.getElementById('pic'+nowFrame).style.display=\"none\";
		if(nowFrame==maxFrame)
		{
			nowFrame=1;
		}
		else
		{
			nowFrame++;
		}	
		setTimeout('picPlay()',3000);
	}	

</script>
<body bgcolor=\"rgb(86,86,86)\">
<body onload=\"picPlay()\">
	<table border=\"0\" align=\"left\" >
		<tr>
			<td align=\"left\">
			<a href=\"http://www.jinke3d.com/?action-viewnews-itemid-20\" target=\"_blank\"><img src=\"http://www.jinke3d.com/attachments/ad/gg01.gif\" width=\"290\" height=\"155\" id=\"pic1\" title=\"jinke3d1\" style=\"display:block\"/></a>
			<a href=\"http://www.jinke3d.com/?action-viewnews-itemid-21\" target=\"_blank\"><img src=\"http://www.jinke3d.com/attachments/ad/gg02.gif\" width=\"290\" height=\"155\" id=\"pic2\" title=\"jinke3d2\" style=\"display:none\" /></a>
			<a href=\"http://www.jinke3d.com/?action-viewnews-itemid-22\" target=\"_blank\"><img src=\"http://www.jinke3d.com/attachments/ad/gg03.gif\" width=\"290\" height=\"155\" id=\"pic3\" title=\"jinke3d3\"style=\"display:none\"/></a>
			</td>
		</tr>
	</table>
</body>
</html>" height:180 width:320 align:#center releaseOnClose:true; prop1:#bgColor prop2:#textContent

		--button btn_setting "!" align:#right width:20 height:10 tooltip:"广告设置" offset:[0, -5]
		--)
		Label lb_stateBar "就绪." style_sunkenedge:true width:290 height:18

		Timer tm_setBgClr "" interval:10 active:false
----------------------------------------------------
		global maxfile = undefined;
		global bLowMaxVersion = false;
		
		on btn_open pressed do 
		(
			if(yviIsLowMaxversion())do
			(
				return 0;
			)
			
			yviSetBitmmapNewPathRL.lb_stateBar.text = "选择需要打开/合并的3dsmax文件...";
			
			maxfile = getOpenFileName caption:"选择3dsmax文件:" filename:"*.max"\
			types:"3ds Max (*.max)|*.max|All (*.*)|*.*|"
			if( undefined == maxfile ) do 
			(
				yviSetBitmmapNewPathRL.lb_stateBar.text = "未选择3dsmax文件。";
				return 0;
			)
			edt_maxPath.text = maxfile as string;
			
			yviSetBitmmapNewPathRL.lb_stateBar.text = "3dsmax文件已选定，请选择合并/打开模型按钮。";
		)
		
		on btn_mergeModel pressed do 
		(
			if(yviIsLowMaxversion())do
			(
				return 0;
			)
			--yviSetBitmmapNewPathRL.pgb_state1.value = 0.;
			--yviSetBitmmapNewPathRL.pgb_state2.value = 0.;

			bmpPath = getFilenamePath edt_maxPath.text;
			yviMergeModel edt_maxPath.text bmpPath;
		)
		
		on btn_openModel pressed do 
		(
			if(yviIsLowMaxversion())do
			(
				return 0;
			)
			--yviSetBitmmapNewPathRL.pgb_state1.value = 0.;
			--yviSetBitmmapNewPathRL.pgb_state2.value = 0.;
			
			bmpPath = getFilenamePath edt_maxPath.text;
			yviOpenModel edt_maxPath.text bmpPath;
			
		)
		
		on btn_setting pressed do
		(
			local isModify = false;
			try(DestroyDialog yviAdSettingRL)catch()
			Rollout yviAdSettingRL "广告设置" width:500
			(
				CheckBox ckb_ad1 ""align:#left across:4 
				label lb_ad1 "广告1" align:#left   offset:[-95, 0] enabled:false
				editText edt_adr1 "地址：" align:#left offset:[-165, 0] width:200 text:"http://" enabled:false
				editText edt_img1 "图片：" align:#left offset:[-70, 0] width:200 text:"http://"  enabled:false

				CheckBox ckb_ad2 ""align:#left across:4 
				label lb_ad2 "广告2" align:#left offset:[-95, 0] enabled:false
				editText edt_adr2 "地址："align:#left offset:[-165, 0] width:200 text:"http://" enabled:false
				editText edt_img2 "图片："align:#left offset:[-70, 0] width:200 text:"http://"  enabled:false
                              
				CheckBox ckb_ad3 ""align:#left across:4
				label lb_ad3 "广告3" align:#left offset:[-95, 0] enabled:false
				editText edt_adr3 "地址："align:#left offset:[-165, 0] width:200 text:"http://" enabled:false
				editText edt_img3 "图片："align:#left offset:[-70, 0] width:200 text:"http://"  enabled:false
				      
				CheckBox ckb_ad4 ""align:#left across:4
				label lb_ad4 "广告4" align:#left offset:[-95, 0] enabled:false
				editText edt_adr4 "地址：" align:#left offset:[-165, 0] width:200 text:"http://" enabled:false
				editText edt_img4 "图片：" align:#left offset:[-70, 0] width:200 text:"http://"  enabled:false
                
				CheckBox ckb_ad5 ""align:#left across:4 				
				label lb_ad5 "广告5" align:#left offset:[-95, 0] enabled:false
				editText edt_adr5 "地址：" align:#left offset:[-165, 0] width:200 text:"http://" enabled:false
				editText edt_img5 "图片：" align:#left offset:[-70, 0] width:200 text:"http://"  enabled:false
                                                       
				label lb_adbout "地址：点击时连接到网站的地址。图片：显示的图片（本地或网络图片）"-- pos:[300, 40]
				
				Button btn_changedAds "确定"
				
				on yviAdSettingRL open do
				(
					local addrList = #(), imgList = #();
					yviGetAdrAndImg &addrList &imgList;
					
					if(addrList.count == 1)then
					(
						edt_adr1.text = addrList[1];
						edt_img1.text = imgList[1];
					)
					if(addrList.count == 2)then
					(
						edt_adr1.text = addrList[1];
						edt_adr2.text = addrList[2];
						
						edt_img1.text = imgList[1];
						edt_img2.text = imgList[2];
					)
					if(addrList.count == 3)then
					(
						edt_adr1.text = addrList[1];
						edt_adr2.text = addrList[2];
						edt_adr3.text = addrList[3]
						
						edt_img1.text = imgList[1];
						edt_img2.text = imgList[2];
						edt_img3.text = imgList[3];
					)
					if(addrList.count == 4)then
					(
						edt_adr1.text = addrList[1];
						edt_adr2.text = addrList[2];
						edt_adr3.text = addrList[3];
						edt_adr4.text = addrList[4];
						
						edt_img1.text = imgList[1];
						edt_img2.text = imgList[2];
						edt_img3.text = imgList[3];
						edt_img4.text = imgList[4];
					)
					if(addrList.count == 5)then
					(
						edt_adr1.text = addrList[1];
						edt_adr2.text = addrList[2];
						edt_adr3.text = addrList[3];
						edt_adr4.text = addrList[4];
						edt_adr5.text = addrList[5];
						
						edt_img1.text = imgList[1];
						edt_img2.text = imgList[2];
						edt_img3.text = imgList[3];
						edt_img4.text = imgList[4];
						edt_img5.text = imgList[5];
					)
				)
				on btn_changedAds pressed do
				(
					local addrList = #();
					local imgList = #();
					local titleList = #();
					
					append addrList edt_adr1.text;
					append addrList edt_adr2.text;
					append addrList edt_adr3.text;
					append addrList edt_adr4.text;
					append addrList edt_adr5.text;
					
					append imgList edt_img1.text;
					append imgList edt_img2.text;
					append imgList edt_img3.text;
					append imgList edt_img4.text;
					append imgList edt_img5.text;
					
					append titleList edt_adr1.text;
					append titleList edt_adr2.text;
					append titleList edt_adr3.text;
					append titleList edt_adr4.text;
					append titleList edt_adr5.text;
					               
					yviMakeHtmlStr addrList imgList titleList clr;
					html=yviGetHtmlStr();
					DestroyDialog yviSetBitmmapNewPathRL;
					CreateDialog yviSetBitmmapNewPathRL;
					DestroyDialog yviAdSettingRL;
				)
				
				on ckb_ad1 changed state do
				(
					if(state)then
					(
						lb_ad1.enabled = state;
						edt_img1.enabled = state;
						edt_adr1.enabled = state;
					)
					else
					(
						lb_ad1.enabled = state;
						edt_img1.enabled = state;
						edt_adr1.enabled = state;
					)
				)  
				
				on ckb_ad2 changed state do
				(   
					if(state)then
					(
						lb_ad2.enabled = state;
						edt_img2.enabled = state;
						edt_adr2.enabled = state;
					)
					else
					(
						lb_ad2.enabled = state;
						edt_img2.enabled = state;
						edt_adr2.enabled = state;
					)
				)
				on ckb_ad3 changed state do
				(
					if(state)then
					(
						lb_ad3.enabled = state;
						edt_img3.enabled = state;
						edt_adr3.enabled = state;
					)
					else
					(
						lb_ad3.enabled = state;
						edt_img3.enabled = state;
						edt_adr3.enabled = state;
					)
				)
				on ckb_ad4 changed state do
				(
					if(state)then
					(
						lb_ad4.enabled = state;
						edt_img4.enabled = state;
						edt_adr4.enabled = state;
					)
					else
					(
						lb_ad4.enabled = state;
						edt_img4.enabled = state;
						edt_adr4.enabled = state;
					)
				)
				on ckb_ad5 changed state do
				(
					if(state)then
					(
						lb_ad5.enabled = state;
						edt_img5.enabled = state;
						edt_adr5.enabled = state;
					)
					else
					(
						lb_ad5.enabled = state;
						edt_img5.enabled = state;
						edt_adr5.enabled = state;
					)
				)
			)
			CreateDialog yviAdSettingRL modal:true;
		)
		
		on yviSetBitmmapNewPathRL open do 
		(
			tm_setBgClr.active = on;
			--在这里修改ax背景颜色没有效果，不知为何。
			--setIndexedProperty yviSetBitmmapNewPathRL.ax.body #bgColor 1 clr;
			-- get licens 
-- 			if ( not yviIsLicensing() )do
-- 			(
-- 				btn_mergeModel.enabled = false;
-- 				btn_openModel.enabled = false;
-- 				yviRegWindow();
-- 				DestroyDialog yviSetBitmmapNewPathRL;
-- 				return false;
-- 			)
			yviIsLowMaxversion();
		)
		
		on tm_setBgClr tick do
		(
			setIndexedProperty yviSetBitmmapNewPathRL.ax.body #bgColor 1 clr;
			tm_setBgClr.active = off;
		)

	)
	CreateDialog yviSetBitmmapNewPathRL style:#(#style_titlebar, #style_border, #style_sysmenu, #style_minimizebox);
	

-----------------------------------------------------
--程序功能：递归获取目录下包括子目录所有max文件。
--作者：Yvi.麻菜
--联系：Email 12319597@qq.com
--日期：2011 - 10 - 11
-----------------------------------------------------

try(DestroyDialog getAllFiles)catch()
rollout getAllFiles "getAllFiles" width:203 height:300
(
	Button btn_OpenRoot "浏览..."pos:[6,10] width:65 height:20 tooltip:"浏览打开一个存有max文件的目录"
	editText edt_strPath "" pos:[70,12] width:130 height:16
	
	MultilistBox lbx_AllFiles "" pos:[6,40] width:190 height:18
	
	--函数
	--递归获取子目录文件
	--getFilesRecursive 路径 文件类型如："*.max"
	fn getFilesRecursive root ext =
	(
		local 	all_Files = #()
		local TheAllSubFolder = #()
		--获取root目录下的所有子目录
		TheAllSubFolder = GetDirectories (root+"/*")
		for d in TheAllSubFolder do
		(
			--递归的将所有子目录下的子目录append到TheAllSubFolder
			--这时TheAllSubFolder装入了root目录下的所有子目录，包括root目录
			join TheAllSubFolder (GetDirectories (d+"/*"))
		)
	
		for f in TheAllSubFolder do
		(
			--在每个目录下遍历
			join all_Files (getFiles (f + ext))
		)
		join all_Files (getFiles (root + "/" +  ext))
		return all_Files 
	)--函数结束
	
	--Handle
	on btn_OpenRoot pressed do
	(
		local AllFiles = #()
		try
		(
			if (root = getSavePath caption:"选择一个存有MAX文件的目录...") != undefined then
			(
				edt_strPath.text = root
				
				AllFiles = getFilesRecursive root "*.max"

				lbx_AllFiles.items = for f in AllFiles collect(filenameFromPath f)
			)
			else
			(
				return 0
			)
		)catch( print "Unknow System Error!")
	)

)CreateDialog getAllFiles


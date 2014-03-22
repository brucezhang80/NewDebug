--Yvi.麻菜
--2011.6.2
--QQ12319597

global rn_files=#()
global dir

rollout ReName "命名工具 v1.0" width:300 height:480
(
	subrollout ReName1 "Offset1"
	subrollout ReName2 "Offset2"
)

rollout ObjName2MaterialName "物体名到材质名" --width:265 height:383
(
	button btn_Obj2Mater "物体名等于材质名" pos:[140,28] width:111 height:24 toolTip:"将物体的名字匹配到材质球名字"
	checkbox chk_selall "选中所有物体" pos:[13,34] width:119 height:20 checked:false
	groupBox grp1 "材质命名" pos:[6,12] width:253 height:100
	hyperlink lb_Note1 "提示：" pos:[13,90] width:235 height:18 color:green enabled:false
	
	button btn_Mater2Map "材质名等于贴图名" pos:[140,60] width:111 height:24 --enabled:false
	
	on chk_selall changed state do
	(
		if chk_selall.checked then
		(
			max select all
			lb_Note1.caption ="提示：选择所有物体!"
		)
		else clearSelection()
		if(selection.count == 0)do
		(
			lb_Note1.caption ="提示：没有选择物体!"
		)
	)
	
	on btn_Obj2Mater pressed  do----材质名等于物体名
	(
		if chk_selall.checked then
		(
			for i in objects do 
			(
				if i.material != undefined then
				(
					i.material.name =i.name 
					lb_Note1.caption="提示：转材质名等于物体名成功!"
				) 
				else lb_Note1.caption ="没有材质!"
			)
		)
		else lb_Note1.caption ="提示：没有选择物体! 命名失败！"
			
	)
	
	--材质名等于贴图名
	on btn_Mater2Map pressed do
	(
		if chk_selall.checked then
		(
			for i in sceneMaterials do 
			(
				try(i.name=(getFilenameFile i.diffuseMap.fileName))catch()
			)
			lb_Note1.caption="提示：成功转到 材质名等于贴图名!"
		)
		else
		(
			messagebox "请勾选 “选择所有物体”复选框！"
			lb_Note1.caption="提示：失败!"
		)
	)
)
createdialog ReName
addSubRollout ReName.ReName1 ObjName2MaterialName
ReName. ReName1.height += 120
ReName. ReName1.pos += [0,0]
	
rollout Lower2Upper "贴图文件名转换" --width:116 height:20
(
	groupBox grp2 "大小写转换" pos:[6,6] width:254 height:235
	button btn_bitmap "贴图路径" pos:[21,23] width:228 height:20
	multilistbox listBox "" pos:[24,50] width:221 height:12
	checkbox chk_selallflies "选择所有" pos:[25,220] width:70 height:16
	checkbox chk_UpperOrLower  "转成大写" pos:[100,219] width:70 height:20 checked:true
	button btn_LowerOrUpper "转换大小写" pos:[182,216] width:70 height:20
	hyperlink lb_Note2 "提示："pos:[6,270] width:235 height:18 color:green enabled:false
	
	radioButtons rdo_pix72 "" pos:[25,245] width:103 height:16 labels:#("72 x 72") enabled:false default:0
	radioButtons rdo_pix512 "" pos:[100,245] width:103 height:16 labels:#("512 x 512") enabled:false default:0
	button btn_toPix "转换像素" pos:[182,242] width:70 height:20 enabled:false

	on btn_bitmap pressed do--jpg贴图的路径
	(
		if (dir = getSavePath caption:"选择目录...") != undefined 
		then
		(
			if (dir.count) > 28 then btn_bitmap.text = "..."+(substring dir (dir.count-28) dir.count) 
			else btn_bitmap.text = dir
			
			rn_files = getFiles (dir+"/*.*")
			listBox.items = for f in rn_files collect (filenameFromPath f)
		)
	)
	
	on btn_LowerOrUpper pressed  do--jpg贴图文件名大小写转换
	(
		local UpperOrlower = #()
		local sel = (listBox.selection as array)
		local fileName
		local newFileName
		
		for i = 1 to sel.count do append UpperOrlower rn_files[sel[i]]
		
		if chk_selallflies.checked then 
		(
			if chk_UpperOrLower.checked  then
			(
				try
				(
					if(UpperOrlower.count != 0) then
					(
						for i = 1 to UpperOrlower.count do
						(
							--fileName = (getFilenamefile upper[i]) + (getFilenameType upper[i])
							--renameFile ((getFilenamePath upper[i])+"\\"+ fileName) ((getFilenamePath upper[i])+"\\"+  toUpper fileName)
							
							fileName = (getFilenameFile UpperOrlower[i])
							renameFile ((getFilenamePath UpperOrlower[i])+"\\"+ fileName + (getfilenametype UpperOrlower[i])) ((getFilenamePath UpperOrlower[i])+"\\"+  (toUpper fileName) + (getfilenametype UpperOrlower[i]))
						)
						lb_Note2.caption = "提示：转换成功！"
						rn_files = getFiles (dir+"/*.*")
						listBox.items = for f in rn_files collect (filenameFromPath f)
						update listBox
					)
					else 
					(
						messagebox "没有选择文件！"
						lb_Note2.caption = "提示：失败！"
					)
				)catch()
			)
			else 
			(
				try
				(
					if(UpperOrlower.count != 0) then
					(					
						for i = 1 to UpperOrlower.count do
						(
							--fileName = (getFilenamefile upper[i]) + (getFilenameType upper[i])
							--renameFile ((getFilenamePath upper[i])+"\\"+ fileName) ((getFilenamePath upper[i])+"\\"+  toUpper fileName)
							
							fileName = (getFilenameFile UpperOrlower[i])
							renameFile ((getFilenamePath UpperOrlower[i])+"\\"+ fileName + (getfilenametype UpperOrlower[i])) ((getFilenamePath UpperOrlower[i])+"\\"+  (toLower fileName) + (getfilenametype UpperOrlower[i]))
						)
						lb_Note2.caption = "提示：转换成功！"
						
						rn_files = getFiles (dir+"/*.*")
						listBox.items = for f in rn_files collect (filenameFromPath f)
						update listBox
					)
					else 
					(
						messagebox "没有选择文件！"
						lb_Note2.caption = "提示：失败！"
					)	
				)catch()
			)
		)--chk_selallflies.checked结束
		
		else --当chk_selallflies.checked = false时
		(
			if chk_UpperOrLower.checked  then
			(
				try
				(
					if(UpperOrlower.count != 0) then
					(
						for i = 1 to UpperOrlower.count do
						(
							--fileName = (getFilenamefile upper[i]) + (getFilenameType upper[i])
							--renameFile ((getFilenamePath upper[i])+"\\"+ fileName) ((getFilenamePath upper[i])+"\\"+  toUpper fileName)
							
							fileName = (getFilenameFile UpperOrlower[i])
							renameFile ((getFilenamePath UpperOrlower[i])+"\\"+ fileName + (getfilenametype UpperOrlower[i])) ((getFilenamePath UpperOrlower[i])+"\\"+  (toUpper fileName) + (getfilenametype UpperOrlower[i]))
						)
						lb_Note2.caption = "提示：转换成功！"
						rn_files = getFiles (dir+"/*.*")
						listBox.items = for f in rn_files collect (filenameFromPath f)
						update listBox
					)
					else 
					(
						messagebox "没有选择文件！"
						lb_Note2.caption = "提示：失败！"
					)
				)catch()
			)
			else --当chk_UpperOrLower.checked = false时
			(
				try
				(
					if(UpperOrlower.count != 0) then
					(					
						for i = 1 to UpperOrlower.count do
						(
							--fileName = (getFilenamefile upper[i]) + (getFilenameType upper[i])
							--renameFile ((getFilenamePath upper[i])+"\\"+ fileName) ((getFilenamePath upper[i])+"\\"+  toUpper fileName)
							
							fileName = (getFilenameFile UpperOrlower[i])
							renameFile ((getFilenamePath UpperOrlower[i])+"\\"+ fileName + (getfilenametype UpperOrlower[i])) ((getFilenamePath UpperOrlower[i])+"\\"+  (toLower fileName) + (getfilenametype UpperOrlower[i]))
						)
						lb_Note2.caption = "提示：转换成功！"
						rn_files = getFiles (dir+"/*.*")
						listBox.items = for f in rn_files collect (filenameFromPath f)
						update listBox
					)
					else 
					(
						messagebox "没有选择文件！"
						lb_Note2.caption = "提示：失败！"
					)	
				)catch()
			)
			--else --
			--(
			--	lb_Note2.caption = "提示：没有文件被选择！"
			--	messagebox "文件列表没有文件或没有选择文件！"
			--)
		)
	)
	
	on chk_selallflies changed state  do  --全选
	(
		if chk_selallflies.checked then
		(
			if (listBox.items.count != 0)then
			(
				listBox.selection = #{1..(listBox.items.count)}
				lb_Note2.caption = "提示：选择了所有文件！"
			)
			else
			(
				lb_Note2.caption = "提示：没有文件被选择！"
				messagebox "文件列表没有文件！"
			)
		)
		else
		(
			listBox.selection = #{}
			lb_Note2.caption = "提示：没有选择文件！"
		)
	)
	
	on chk_UpperOrLower changed state do --大写小写切换
	(
		if chk_UpperOrLower.checked then
		(
			chk_UpperOrLower.caption = "转成大写"
			lb_Note2.caption = "提示：转成大写字母的文件名！"
		)
		else
		(
			chk_UpperOrLower.caption = "转成小写"
			lb_Note2.caption = "提示：转成小写字母的文件名！"
		)
	)
	
	
	-----------------------------像素处理---------------------------------------
	on btn_toPix pressed do
	(
		
	
	)
	
)
createdialog ReName
addSubRollout ReName.ReName2  Lower2Upper
ReName. ReName2.height += 310
ReName. ReName2.pos += [0,128]


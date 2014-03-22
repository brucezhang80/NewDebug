
---global value
global g_arr_lineContent = #()
global g_arr_posAndObjname = #()
struct s_posAndName (pos, objName, changed)
-- 打开配置文件
function Yvi_openConfigeFile =
(
		
	try
	(
		in_configefile= getOpenFileName caption:"打开配置文件..." types:"配置文件(*.TXT)|*.txt|All|*.*|" --initialDir:
		if in_configefile == undefined then
		(
			Messagebox "打开文件失败!"
			return 0
		)
	)catch( Messagebox "打开文件失败！")

	return in_configefile
)

-- function
function Yvi_readConfigeFile filename =
(
	if filename == undefined or filename == "0" then
	(
		Messagebox "获取文件失败!"
		return 0
	)
	in_file = openFile filename
	if (in_file == undefined )then return 0
	while not(eof in_file) do
	(
		iline = readLine in_file
		append g_arr_lineContent iline
	)
	close in_file
)

function Yvi_compareFN v1 v2 =
(
	if v1.pos.x < v2.pos.x then return 1
	if v1.pos.x > v2.pos.x then return -1
	else return 0
)
function Yvi_formatContent =
(
	if g_arr_lineContent.count == 0 then
	(
		MessageBox ("无法读取配置文件!")
		return 0
	)
	for i = 1 to g_arr_lineContent.count do 
	(
		local l_arr_eachLine = #()
		l_arr_eachLine = filterString g_arr_lineContent[i] ","
		_pos = [ l_arr_eachLine[1] as integer, l_arr_eachLine[2] as integer, l_arr_eachLine[3] as integer ] as point3
		posAndName = s_posAndName pos:_pos objName:l_arr_eachLine[4] changed:false

		append g_arr_posAndObjname posAndName
		qsort g_arr_posAndObjname Yvi_compareFN
		--print ("g_arr_posAndObjname:  " + g_arr_posAndObjname as string)
	)
)
function Yvi_creatfile =
(
	out_file = CreateFile "D:\\infer.txt" 

	return out_file
)
function Yvi_writeInferFile file val1 val2 val3 val4 =
(
	if file != undefined then
	(
		--_file = openFile "D:\\infer.txt" mode:"at"
		format "old:% old_pos:% \t new_obj:% new_pos:%" val1 val2 val3 val4 to:file
	)
	close file
)
-- this function is called by next function Yvi_handleObject
function Yvi_getObjCenterPos obj =
(
	_x = (obj.center.x as integer) + 0.0
	_y = (obj.center.y as integer) + 0.0
	_z = (obj.center.z as integer) + 0.0
	_center = [_x, _y, _z] as point3
	print ("_center: " + _center as string)
	return _center
)

-- 分块查找,仅仅对比 x 坐标
function Yvi_binSearch arr key = -- arr 从大到小排列
(
	local mid
	local low = 1
	local high = arr.count--查找区间的上中下界
	
	while(low <= high) do
	(
		mid = (low + high)/2

		print ("arr[mid].pos.x: "+arr[mid].pos.x as string + " " + arr[mid].pos.y as string + " " + arr[mid].pos.z as string)

		if(key.x == arr[mid].pos.x and key.y == arr[mid].pos.y and key.z == arr[mid].pos.z) then
		(
			return mid
		)
		else if(key.x > arr[mid].pos.x) then
		(
			high = mid-1
		)
		else
		(
			low = mid+1
		)
	)
--	return -1  -- 不知道为什么最后总是返回 -1
)

function Yvi_handleObject obj =
(
	if g_arr_posAndObjname.count == 0 then
	(
		MessageBox ("获取数据失败")
		return 0
	)
	_filename = Yvi_creatfile()
	--for i = 1 to g_arr_posAndObjname.count do
	(
		_center = Yvi_getObjCenterPos obj
		
		index = Yvi_binSearch g_arr_posAndObjname _center --
		print index
		--if (index != -1) then
		(
			if (g_arr_posAndObjname[index].changed == false) then
			(
				Yvi_writeInferFile _filename obj.name obj.center g_arr_posAndObjname[index].objName g_arr_posAndObjname[index].pos
				print (index as string + ": " + obj.name + obj.center as string + g_arr_posAndObjname[index].objName + g_arr_posAndObjname[index].pos as string + "\n")
				obj.name = g_arr_posAndObjname[index].objName
				
				g_arr_posAndObjname[index].changed = true
			)
		)
-- 		else
-- 		(
-- 			print "next..."
-- 		)
	)
	
)

----------------
function main =
(
	clearListener()
	Yvi_readConfigeFile (Yvi_openConfigeFile())
	Yvi_formatContent()
	
	--obj = selection[1]
	for obj in Geometry do
	(
		Yvi_handleObject obj
	)
	g_arr_lineContent = #()
	g_arr_posAndObjname = #()
)

main()




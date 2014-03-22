
--打开保存txt记录坐标文件的对话框
try(
	out_filename= getSaveFileName caption:"选择保存的位置..." initialDir:"$export" filename:"坐标.txt"
	out_file = createFile out_filename
)catch("创建\"坐标.txt\"文件失败！")

global i =0
for obj in geometry do 
(
	i= i + 1
	--转成mesh
	Y_mesh = convertToMesh obj
	--Y_mesh = snapshotAsMesh obj
	format "%-物体名: %\n" i obj.name to:out_file
	format "   X Pos, Y Pos, Z Pos\n" to:out_file
	
	--获取顶点数遍历
	for v = 1 to getNumVerts obj do 
	(
		--获取当前索引顶点在场景世界中的Point3坐标
		vert = getVert Y_mesh v
		format "%: %, %, %\n" v vert.x vert.y vert.z to:out_file
	)
	format "\n" to:out_file
	--delete Y_mesh
)
close out_file


if(out_filename == undefined)then
(
	messagebox "已退出，文件 \"坐标.txt\" 没有创建！" title:"错误!"
)
else
	edit out_filename

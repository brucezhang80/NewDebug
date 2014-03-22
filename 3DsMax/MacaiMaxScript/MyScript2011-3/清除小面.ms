/*
	作者:Yvi
	功能:删除(Mesh操作)中不可用的面
	日期:2012-2-15-17
*/

-- 求向量之间的夹角
function Yvi_getEachFaceAngle dot1 dot2 dot3 =
(
	a0 = dot3.x - dot1.x
	b0 = dot3.y - dot1.y
	c0 = dot3.z - dot1.z

	a1 = dot3.x - dot2.x
	b1 = dot3.y - dot2.y
	c1 = dot3.z - dot2.z

	_angle = (a0*a1+b0*b1+c0*c1)/(sqrt(a0^2+b0^2+c0^2) * sqrt(a1^2+b1^2+c1^2)) -- 求向量a ,b之间的夹角

	return acos(_angle)
)

-- 求两点之间的距离
function Yvi_getVertexDistence dot1 dot2 =
(
	a = dot1.x - dot2.x
	b = dot1.y - dot2.y
	c = dot1.z - dot2.z
	
	return sqrt(a^2 + b^2 + c^2)
)

---------------------------------------------------------------------------------------------

-- 删除两点距离最小的的面
function Yvi_deleteTwoVertsMinDistance _meshObj _minDistance =
(
	local arr_minDistenceVertexIndex = #()
	for v = 1 to _meshObj.numverts do
	(
		minDistence = meshop.minVertexDistanceFrom _meshObj v #{_meshObj.numverts}
		if (minDistence != 0. and minDistence < _minDistance) then
		(
			format "索引点 % 到其中一个点最小的距离为: % , 该距离小于 %, 该面被删除...\n" v minDistence _minDistance
			append arr_minDistenceVertexIndex v
		)
	)
	if (arr_minDistenceVertexIndex.count == 0)then
	(
		format "物体 % 所有点距离符合要求,不存在过小距离的点.\n" _meshObj.name
		return 0;
	)
	meshop.deleteVerts _meshObj arr_minDistenceVertexIndex
	update _meshObj --update the mesh
)


--删除最小面积的面
function Yvi_deleteMinAreaInMeshFace _meshObj _minArae = 
(
	local arr_minAreaFaceIndex = #()

	for f = 1 to _meshObj.numfaces do
	(
		minArae = meshop.getFaceArea _meshObj #{f}
		--得到每个面的面积
		if ( minArae < _minArae ) then
		(
			format "索引面 % 的面积为: % , 该面积小于 %,该面被删除...\n" f minArae _minArae
			append arr_minAreaFaceIndex f
		)
	)
	if (arr_minAreaFaceIndex.count == 0)then
	(
		format "物体 % 所有三角面的面积符合要求,不存在过小面积的三角面.\n" _meshObj.name
		return 0;
	)
	meshop.deleteFaces _meshObj arr_minAreaFaceIndex delIsoVerts:true -- 删除独立的点
	update _meshObj --update the mesh
)

-- 删除角度最小的的面
function Yvi_deleteMinAngleInMeshFace _meshObj _minAngle =
(
	local vertsArr = #()
	local arr_validTrangleFaceIndex = #()
	numface = getNumFaces _meshObj

	for f = 1 to numface do --遍历每个三角面的顶点
	(
		verts = (meshop.getVertsUsingFace _meshObj #{f}) as array
		for v = 1 to verts.count do
		(
			_vert = meshop.getVert _meshObj verts[v]
			append vertsArr _vert
		)
		-- 计算三角面的三个角的角度
		_angle0 = Yvi_getEachFaceAngle vertsArr[1] vertsArr[2] vertsArr[3]
		_angle1 = Yvi_getEachFaceAngle vertsArr[1] vertsArr[3] vertsArr[2]
		_angle2 = Yvi_getEachFaceAngle vertsArr[2] vertsArr[3] vertsArr[1]
		--print (f as string + ": " + _angle0 as string + " , " + _angle1 as string + " , " + _angle2 as string)
		if( _angle0 < _minAngle or _angle1 < _minAngle or _angle2 < _minAngle) then
		(
			case  of
			(
				(_angle0 < _minAngle): format "物体 % 索引号 % 的面上的其中一个角为 % ,小于 % 度,该面被删除...\n" _meshObj.name f _angle0 _minAngle
				(_angle1 < _minAngle): format "物体 % 索引号 % 的面上的其中一个角为 % ,小于 % 度,该面被删除...\n" _meshObj.name f _angle1 _minAngle
				(_angle2 < _minAngle): format "物体 % 索引号 % 的面上的其中一个角为 % ,小于 % 度,该面被删除...\n" _meshObj.name f _angle2 _minAngle
			)
			append arr_validTrangleFaceIndex f
		)
		vertsArr = #()
	)
	if (arr_validTrangleFaceIndex.count == 0)then
	(
		format "物体 % 所有三角面的角度符合要求,不存在过小角度的三角面.\n" _meshObj.name
		return 0;
	)
	meshop.deleteFaces _meshObj arr_validTrangleFaceIndex delIsoVerts:false -- 删除三角面
	
	update _meshObj --update the mesh
)
-----------------------------------------------------------------------------------------------------------

function main =
(
	clearListener()
	local minAngle = 30
	local minDistance = 30
	local minArea = 30
	
	format "Starting...\n"
	obj = selection[1]
	if (classOf obj != Editable_Mesh) then
	(
		if(canConvertTo obj Editable_Mesh)then
		(
			convertToMesh obj
		)
	)
	Yvi_deleteMinAngleInMeshFace obj minAngle
	Yvi_deleteTwoVertsMinDistance obj minDistance
	Yvi_deleteMinAreaInMeshFace obj minArea
	format "---------------------------------------\n"
	format "The end.\n"
)

-- main
main()



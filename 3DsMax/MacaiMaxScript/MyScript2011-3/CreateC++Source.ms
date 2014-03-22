-----------------------------------------------------------
-- mesh2cpp.ms
-- By Simon Feltman
-----------------------------------------------------------
-- macroScript Mesh2CPP
--    category:"SDK工具"
--    tooltip:"Mesh 生成 CPP"
Rollout Mesh2CPP "SDK工具:Mesh生成CPP" width:200 height:40
(
	Button btn_CreateCPP "生成C++文件"
	
   fn ExportCPP obj stream =
   (
		format "\tmesh.setNumVerts(%);\n" obj.numverts to:stream

		format "\tmesh.setNumFaces(%);\n" obj.numfaces to:stream

		for i = 1 to obj.numverts do
		(
			pnt = getvert obj i
			format "\tmesh.setVert(%,size*Point3(%,%,%));\n" (i-1) pnt.x pnt.y pnt.z to:stream
		)

		for i = 1 to obj.numfaces do
		(
			pnt = getface obj i
			format "\tmesh.faces[%].setVerts(%, %, %);\n" \
				(i-1) \
				(pnt.x as integer - 1) \
				(pnt.y as integer - 1) \
				(pnt.z as integer - 1) \
				to:stream

		format "\tmesh.faces[%].setEdgeVisFlags(%,%,%);\n"\
            (i-1) \
            (if (getedgevis obj i 1) then 1 else 0) \
            (if (getedgevis obj i 2) then 1 else 0) \
            (if (getedgevis obj i 3) then 1 else 0) \
            to:stream

		format "\tmesh.faces[%].setSmGroup(%);\n" \
            (i-1) (getfacesmoothgroup obj i) to:stream
		)
   )

   on btn_CreateCPP pressed do
   (
	   if selection[1] != undefined and (classof selection[1]) == Editable_mesh then
	   (
		    try
			(
			    local fname = getsavefilename types:"头文件 (*.h)|*.h|"
			    local stream = createfile fname
			    if stream != undefined then
			   (
				    ExportCPP (selection[1]) stream
				    close stream
			    )
			    else
				    messagebox ("打开文件 \"" + fname + "\" 失败")
			)catch(format "未知的系统错误!")
	   )
	   else
		    messagebox "你必须选择一个可编辑网络对象."
   )
)
CreateDialog Mesh2CPP

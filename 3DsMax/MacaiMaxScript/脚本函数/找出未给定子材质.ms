
global IDS =#()

--这里收集所有用到的子材质ID
fn CellectIds obj =
(
	minMatID = 1
	maxMatID = 1
	
	for faceIndex=1 to (getNumFaces obj) do 
	(
		--这个面用的材质ID
		faceMatID = getFaceMatID obj faceIndex

		it = findItem IDS faceMatID
		if it==0 then 
		(
			a = faceMatID<minMatID
			b = faceMatID>maxMatID
			if ( a or b ) then (
				errorstr = obj.name+"第"+(faceIndex as string)+"个面材质ID指定错误,指定的材质ID为"+(faceMatID as String)
				print errorstr
				return undefined
			)
			else        
				append IDS faceMatID
		)
	)
)

fn HandleMesh obj =
(

	curmat = obj.material
	
	for i=1 to curmat.numsubs do 
	(
		if classof curmat == Multimaterial then 
		( 
			if curmat[i] == standard do
				print ("材质："+curmat[i].name +"有问题，请指定贴图")
		)

		if curmat[i].diffusemap==undefined then 
		(		  
			print (obj.name + "材质"+curmat[i].name+"错误，子材质ID号为 【"+i as string+"】的没有指定diffusemap" as string)
		)
	)
)


--main
for o in objects do
(
	ConvertToMesh o
	CellectIds o
	HandleMesh o
)
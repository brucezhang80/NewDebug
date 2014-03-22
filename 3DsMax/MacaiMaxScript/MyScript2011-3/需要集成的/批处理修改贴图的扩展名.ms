 rollout Modifier_KZM "贴图扩展名批量修改器" width:328 height:312
 (
 	label rend "修改选择物体的贴图扩展名 " pos:[25,20] width:197 height:13
    
 
   
 	edittext KZM_mingzhen "修改后的扩展名：   " pos:[13,55] width:160 height:17 fieldwidth:20 labelOnTop:false  	
	--- label 	aaa "   "  
 	
 	button quedingxiugai "确定"  pos:[70,82] width:60 height:20
 	GroupBox shuoming "使用说明：" pos:[7,119] width:185 height:169

 	label l00bl2 "1. 选择物体，在文本框中输入贴图扩展名,如TGA，按下确定后其选择物体材质的贴图扩展名将被修改。" pos:[24,140]	width:150 height:269

	label l00bl3 "2. 只对三种材质有效：标准材质；子材质为标准材质的多维材质；子材质为标准材质和多维材质的外壳材质。" pos:[24,197]	width:150 height:269

 	
 	on quedingxiugai pressed do
	(
---********************************************************************执行内容：
	
global kuozhanming="."+KZM_mingzhen.text
 for  obj_select in selection do
(
  ---meditMaterials[1]=obj_select.material
  	case classof obj_select.material of
		(
	---------------------------------------------------------------------------------------------------------------		
			Standardmaterial:
				---ClassstandMat kuozhanming
				if obj_select.material.diffusemap != undefined then
		            (
				    fileN = obj_select.material.diffuseMap.filename
			         temp = getfilenamepath fileN + getfilenamefile fileN + kuozhanming
			         obj_select.material.diffuseMap.filename= (Temp as string)
			
			         if obj_select.material.opacityMap != undefined then			
                    	(
			     	   obj_select.material.opacityMap = Bitmaptexture fileName:(Temp as string)
                    	   obj_select.material.opacityMap.monoOutput = 1-----通道为ALPHA	

			              )
		             )	
---------------------------------------------------------------------------------------------------------------------------------------------
		     Multimaterial:
			(
			nob=obj_select.material.materialIDList.count 
			for i = 1 to nob do
	            (
		          if obj_select.material.materialList[i].diffusemap != undefined then
		            (
				    fileN = obj_select.material.materialList[i].diffuseMap.filename
			         temp = getfilenamepath fileN + getfilenamefile fileN + kuozhanming
			         obj_select.material.materialList[i].diffuseMap.filename= (Temp as string)
			
			           if obj_select.material.materialList[i].opacityMap != undefined then			
                    	   (
			          	obj_select.material.materialList[i].opacityMap = Bitmaptexture fileName:(Temp as string)	
                            obj_select.material.materialList[i].opacityMap.monoOutput = 1-----通道为ALPHA	
			            )
		             )
	            )	

			)
-----------------------------------------------------------------------------------------------------------------------------------			
			Shell_Material:
			(
			   if classof obj_select.material.bakedMaterial == Standardmaterial then 
			       (
				     	if obj_select.material.bakedMaterial.diffusemap != undefined then
		                    (
				            fileN = obj_select.material.bakedMaterial.diffuseMap.filename
			                 temp = getfilenamepath fileN + getfilenamefile fileN + kuozhanming
			                 obj_select.material.bakedMaterial.diffuseMap.filename= (Temp as string)
                             )	

				   )
			  if classof obj_select.material.originalMaterial == Standardmaterial then 
			       (
				     	if obj_select.material.originalMaterial.diffusemap != undefined then
		                    (
				            fileN = obj_select.material.originalMaterial.diffuseMap.filename
			                 temp = getfilenamepath fileN + getfilenamefile fileN + kuozhanming
			                 obj_select.material.originalMaterial.diffuseMap.filename= (Temp as string)
                             )	

				   )
                if classof obj_select.material.originalMaterial == multimaterial then
				   (
				        nob=obj_select.material.originalMaterial.materialIDList.count 
			             for i = 1 to nob do
	                       (
		                         if obj_select.material.originalMaterial.materialList[i].diffusemap != undefined then
		                               (
				                        fileN = obj_select.material.originalMaterial.materialList[i].diffuseMap.filename
			                             temp = getfilenamepath fileN + getfilenamefile fileN + kuozhanming
			                             obj_select.material.originalMaterial.materialList[i].diffuseMap.filename= (Temp as string)
			
			                              if obj_select.material.originalMaterial.materialList[i].opacityMap != undefined then			
                    	                       (
			          	                     obj_select.material.originalMaterial.materialList[i].opacityMap = Bitmaptexture fileName:(Temp as string)	
                                                 obj_select.material.originalMaterial.materialList[i].opacityMap.monoOutput = 1-----通道为ALPHA	
			                                )
		                               )
	                       )	

				   )
			   
			)
				
-----------------------------------------------------------------------------------------------------------------------------------------------------------				
	
				
		)
  
)
Messagebox("处理完毕")

---**************************************************************** 执行内容结束
 	)
 ) createDialog modifier_KZM 200 300
 --create a Dialog from the rollout

 
--******************************************************************************

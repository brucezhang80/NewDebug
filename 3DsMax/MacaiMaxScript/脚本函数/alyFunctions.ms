
--function library
-----------fn--------------

fn exist_f the_path = try((getFiles the_path).count == 1)catch(undefined)
fn existFile fname = (getfiles fname).count != 0	
fn exist_path the_node = try(the_node.filename)catch(undefined)

fn getMisPathes mat = 
(
	local	theTexPath = try(mat.filename)catch(undefined)
	if theTexPath != undefined and findItem misPathes theTexPath == 0  do if exist_f theTexPath == false do append misPathes theTexPath		
	try(	if mat.numsubs != 0 do
							(
								for i = 1 to mat.numsubs do
									(
										if findItem misPathes theTexPath == 0 and theTexPath != undefined do if exist_f theTexPath == false do append misPathes theTexPath
										
										getMisPathes mat[i]
									)
							)		)catch()
	return misPathes
	)


fn getMisPathesLocal mat controlPath = 
(
	local	theTexPath = try(mat.filename)catch(undefined)
	if theTexPath != undefined and theTexPath == controlPath do if exist_f theTexPath == false do sw_tf = true

	try(	
			if mat.numsubs != 0 do 
			(
				for i = 1 to mat.numsubs do	
				(
					if theTexPath == controlPath  do if theTexPath != undefined and theTexPath == controlPath do if exist_f theTexPath == false do sw_tf = true
					getMisPathesLocal mat[i] controlPath
					)
				)
			)catch()
	)	

fn KillMisTex mat	=
(
	local	theTexPath = try(mat.filename)catch(undefined)
	if theTexPath != undefined and exist_f theTexPath == false do 
	(
		local theTexNameMat = fileNameFromPath theTexPath
		for folder in all_dir do
		(
			local theFiles = getFiles (folder + "*")
			for file in theFiles do if (fileNameFromPath file) == theTexNameMat do	mat.filename = file
			)
		)		
	try(	if mat.numsubs != 0 do
							(
								for i = 1 to mat.numsubs do
									(
										if theTexPath != undefined and exist_f theTexPath == false do 
										(
												local theTexNameMat = fileNameFromPath theTexPath
												for folder in all_dir do
												(
													local theFiles = getFiles (folder + "*")
													for file in theFiles do if (fileNameFromPath file) == theTexNameMat do	mat.filename = file
													)
											)	
										KillMisTex mat[i]
									)
							)		)catch()
	return misPathes
	)

fn the_Helper the_Array the_Date = if findItem the_Array the_Date == 0 do append the_Array the_Date

fn getAllSceenMaterial = 
(
		local AllMaxMat = #()	
	
		for i = 1 to meditMaterials.count do if findItem AllMaxMat meditMaterials[i] == 0 do	append AllMaxMat meditMaterials[i] 	
		for i=1 to currentMaterialLibrary.count do if findItem AllMaxMat currentMaterialLibrary[i] == 0 do	append AllMaxMat currentMaterialLibrary[i]
		if EnvironmentMap != undefined do if findItem AllMaxMat EnvironmentMap == 0 do	append AllMaxMat EnvironmentMap
		if numAtmospherics != 0 do for i=1 to numAtmospherics do	for j = 1 to  (getAtmospheric i).numsubs do	the_Helper AllMaxMat (getAtmospheric i)[j]
		if numEffects != 0 do
		(
			for i=1 to numEffects do
			(
					for j = 1 to  (getEffect i).numsubs do
					(
						the_Helper AllMaxMat (getEffect i)[j]
						for k = 1 to (getEffect i)[j].numsubs do the_Helper AllMaxMat (getEffect i)[j][k]
						)
				)
		)
		
		local theListProp = getPropNames renderers.current
		
		for i = 1 to theListProp.count do the_Helper AllMaxMat (getProperty renderers.current theListProp[i])
			
		for i in objects do
		(
			if i.material != undefined do	if findItem AllMaxMat i.material == 0 do append AllMaxMat i.material
			if i.modifiers.count != 0 do
			(
				for j = 1 to i.modifiers.count do
				(
					theListProp = getPropNames i.modifiers[j]
					for k =1 to theListProp.count do the_Helper AllMaxMat (getProperty  i.modifiers[j] theListProp[k])
					)
				)		
			theListProp = getPropNames i
			if theListProp != undefined do for j = 1 to theListProp.count do the_Helper AllMaxMat (getProperty i theListProp[j])		
		)--end loop in objects
return AllMaxMat
)	



fn getFolderPathes root = 
		(	
			local dir_array = GetDirectories (root+"*")
			join all_dir dir_array
			
			if dir_array.count != 0 do
			(
				for the_path in dir_array do
					(
						join dir_array (GetDirectories the_path)
						getFolderPathes the_path
						)
				)
			return all_dir
			)	

fn testSplineShape the_obj = 
(
	local nS
	nS = try(numSplines the_obj)catch(undefined)
	if classOf the_obj == NURBSCurveshape do nS = false
	return nS
	)
	
fn randomNormVec =
(
		local randV = random -90 90, 
				randH = random 0 360,
				vect = [cos(randV)*sin(randH),   cos(randV)*cos(randH),  sin(randV)]	
	return vect
	)	

fn spline_Length the_spline =
(	
	local summa = 0, 
			step = 0.001
	try(
			for ns = 1 to (numSplines the_spline) do
				(
					local s = curveLength the_spline ns
					summa = summa + s
					)
			return summa
		)catch(undefined)
	/*
	try(
			for ns = 1 to (numSplines the_spline) do
			(
					for i = 0.0 to 1.0 by step do
					(
							the_pos1 = lengthInterp the_spline ns i
							try(the_pos2 = lengthInterp the_spline ns (i-step))catch()
							if the_pos2 != undefined do (dist = distance the_pos1 the_pos2; summa = summa + dist)
						)
				)
				return summa	
			)catch(undefined)
	*/
	)

fn createMarkLines theArrayData SplineFragment DummyFragment theName= 
(
local		ArrayCount = theArrayData.count,
			theLength = 0, 
			switcher = true,
			step,
			num = 0
ss = SplineShape pos:[0,0,0] name: (uniqueName theName) 

			for i = 1 to ArrayCount do
			(
			step = try(length(theArrayData[i] - theArrayData[i-1]))catch(undefined)
			
			if step != undefined do theLength += step
			if switcher == true and theLength < SplineFragment do num+=1
			if switcher == true and theLength >= SplineFragment do
				(
					local bitS = addNewSpline ss
					addKnot ss bitS #corner #line theArrayData[i-num]
					addKnot ss bitS #corner #line theArrayData[i]
					theLength = 0
					switcher = not switcher
				) 
				
			if switcher == false and theLength >= DummyFragment do
				(
					theLength = 0
					switcher = not switcher
					num = 0
				)
				
			)
	updateShape ss
	RedrawViews()
)	


fn theChangeTex mat the_path the_misPath = 
	(
		try(
											theTexPath = mat.filename
											theNameTex = "\\" + (filenameFromPath theTexPath)
											theOutputPath = the_path + theNameTex
											
										if existFile theOutputPath == false then
										(
											format  "Copying: %\n" (filenameFromPath theTexPath)
											if (copyFile theTexPath theOutputPath) == true then
											(
												if var_update == true do (mat.filename = theOutputPath)
												) else (if (findItem the_misPath theTexPath)==0 do append the_misPath theTexPath)								
											)else (mat.filename = theOutputPath)	
				)catch()
			try(
													thePath_texmap_radius = mat.texmap_radius.filename
													theName_texmap_radius = "\\" + (filenameFromPath thePath_texmap_radius)
													theOutputPath = the_path + theName_texmap_radius
													
												if existFile theOutputPath == false then
												(
													format  "Copying: %\n" (filenameFromPath thePath_texmap_radius)
													if (copyFile thePath_texmap_radius theOutputPath) == true then
													(
														if var_update == true do (mat.texmap_radius.filename = theOutputPath)
														) else (if (findItem the_misPath thePath_texmap_radius)==0 do append the_misPath thePath_texmap_radius)								
													)else (mat.texmap_radius.filename = theOutputPath)												
							)catch()
			try(
													thePath_occluded_color = mat.texmap_occluded_color.filename
													theName_occluded_color = "\\" + (filenameFromPath thePath_occluded_color)
													theOutputPath = the_path + theName_occluded_color
													
												if existFile theOutputPath == false then
												(
													format  "Copying: %\n" (filenameFromPath thePath_occluded_color)
													if (copyFile thePath_occluded_color theOutputPath) == true then
													(
														if var_update == true do (mat.texmap_occluded_color.filename = theOutputPath)
														) else (if (findItem the_misPath thePath_occluded_color)==0 do append the_misPath thePath_occluded_color)								
													)else (mat.texmap_occluded_color.filename = theOutputPath)												
							)catch()
			try(
													thePath_unoccluded_color = mat.texmap_unoccluded_color.filename
													theName_unoccluded_color = "\\" + (filenameFromPath thePath_unoccluded_color)
													theOutputPath = the_path + theName_unoccluded_color
													
												if existFile theOutputPath == false then
												(
													format  "Copying: %\n" (filenameFromPath thePath_unoccluded_color)
													if (copyFile thePath_unoccluded_color theOutputPath) == true then
													(
														if var_update == true do (mat.texmap_unoccluded_color.filename = theOutputPath)
														) else (if (findItem the_misPath thePath_unoccluded_color)==0 do append the_misPath thePath_unoccluded_color)								
													)else (mat.texmap_unoccluded_color.filename = theOutputPath)												
							)catch()
			try(														
										if mat.numsubs != 0 do
															(
																for i = 1 to mat.numsubs do
																	(
																	theChangeTex mat[i] the_path the_misPath
																	)
															)
									
				)catch()
	)
-----------fn--------------

--macroscript bakersfield category:"Render" buttontext:"Bakersfield V1.24..."

(
	local bakefloat
	local bakersfieldroll, bakeobjectroll, lightmaproll, bakerenderroll, overwritedialog, blendmaproll, multiroll1, multiroll2, completemaproll, existchannelroll, log
	local nodes = #()
	local bakechannels = #()
	local nodebakeassign = #()

	on execute do
	(
		rollout bakeobjectroll "Object Settings" width:660 height:312
		(
			local channum = 0
			local chandialogroll
			local targetpath = ""
		
			function UIenable roll state = 
			(
				for i in roll.controls do
				(
					i.enabled = state
				)
			)
			
			function rollupall = 
			(
				lightmaproll.open = false
				UIenable lightmaproll false
				blendmaproll.open = false
				UIenable blendmaproll false
				multiroll1.open = false
				UIenable multiroll1 false
				multiroll2.open = false
				UIenable multiroll2 false
				completemaproll.open = false
				UIenable completemaproll false		
			)
				
			function buildnodenamelist = 
			(
				names = #()
				for i in nodes do
				(
					append names i.name
				)
				return names
				)
			
			function oktoaddnode node = 
			(
				if superclassof node != geometryclass do return false
				for i in nodes do
				(
					if node == i do (return false)
				)
				return true
			)
			
			function bitarrayAND btarray = 
			(
				res = btarray[1]
				for i = 1 to btarray.count do
				(
					res = res - -btarray[i]
				)
				return res
			)
			
			function fillbitarray start end boo =
			(
				a = #{}
				for i = start to end do
				(
					a[i] = boo
				)
				return a
			)
			
			function buildchannellist nodeindexarray = 
			(
				channelarray = #()
				for i in nodeindexarray do
				(
					append channelarray nodebakeassign[i]
				)
				channelarray = (bitarrayAND channelarray)as array
				
				bakeobjectroll.channellist.listitems.clear()
				for i in channelarray do
				(
					-- add name to channels list
					newlistnode = bakeobjectroll.channellist.listitems.add()
					newlistnode.text = (classof bakechannels[i]) as string
					newlistnode.checked = bakechannels[i].enabled
					sub1 = newlistnode.listsubitems.add()
					sub2 = newlistnode.listsubitems.add()
					sub1.text = i as string
					sub2.text = bakechannels[i].elementname
				)
			)

			multilistBox nodelist "Bake Enabled Nodes" pos:[14,4] width:258 height:19
			button addnode "Add Nodes" pos:[24,287] width:72 height:20
			button addselectednodes "Add Selected" pos:[103,286] width:72 height:20
			button delnode "Delete Nodes" pos:[182,286] width:72 height:20
			activeXControl channellist "MSComctlLib.ListViewCtrl" pos:[284,23] width:360 height:174
			spinner mapchannel "Map Channel" pos:[338,283] width:120 height:16 range:[1,99,1] type:#integer scale:1 fieldwidth:40
			spinner edgepadding "Edge Padding" pos:[476,283] width:120 height:16 range:[1,99,1] type:#integer scale:1 fieldwidth:40
			
			button addchannel "Add Channel" pos:[290,203] width:82 height:20
			button delchannel "Delete Channel" pos:[378,203] width:82 height:20
			button linkchannel "Link Channel" pos:[466,203] width:82 height:20
			button purgechannel "Purge Channel" pos:[554,203] width:82 height:20			
			label channame "Channels" pos:[282,4] width:57 height:14
			label Targetdir "<No Target Dir Selected>" pos:[319,257] width:324 height:18
			button pickdir "..." pos:[287,254] width:27 height:20
			
			on bakeobjectroll open do
				(
				-- initialize listview
				channellist.gridlines = true
				channellist.view = #lvwreport
				channellist.checkboxes = true
				channellist.columnheaders.add()
				channellist.columnheaders.add()
				channellist.columnheaders.add()
				channellist.columnheaders[1].text = "Channel Type"
				channellist.columnheaders[2].text = "Channel Index"
				channellist.columnheaders[3].text = "Channel Name"
				channellist.allowcolumnreorder = false
				channellist.fullrowselect = true
				channellist.LabelEdit = #lvwmanual
				
				-- build the index list of bakeelements and nodes
				
				nodes = #()
				bakechannels = #()
				nodebakeassign = #()		
				nodecount = 0
				channelcount = 0
		
				for i = 1 to objects.count do
					(
					if objects[i].numbakeelements() != 0 do
						(
						append nodes objects[i]
						nodecount += 1
						nodebakeassign[nodecount] = (fillbitarray 1 20 false)
						
						-- for every bake channel, see if it's in the index list. add if not or point assign to index.
						for t = 1 to objects[i].numbakeelements() do
							(
							bel = objects[i].getbakeelement t
		
							addelement = true
							elementindex = 0
							for n = 1 to channelcount do
								(
								if bel == bakechannels[n] do 
									(
									addelement = false
									elementindex = n
									)
								)
							
							if addelement then
								(
								-- append bakeelement to array
								append bakechannels bel
								channelcount += 1
								nodebakeassign[nodecount][channelcount] = true
								)
							else
								(
								nodebakeassign[nodecount][elementindex] = true
								)
							)
						)
					)
				
				-- check path in channels
				if bakechannels.count != 0 do
					(
					tdir = bakechannels[1].filename
					showdir = true
					for i in bakechannels do
						(
						if i.filename != tdir do
							(
							showdir = false
							exit
							)
						)
					
					if tdir == "" do showdir = false
					if showdir then
						(
						if tdir.count > 50 then
							(
							s = substring tdir 1 20
							e = substring tdir (tdir.count - 19) 20
							targetdir.text = "Target Dir : " + s + "..\\.." + e
							)
						else
							(
							targetdir.text = "Target Dir :" + tdir
							)

						targetpath = tdir
						)
					else
						(
						targetdir.text = "<No Target Dir Selected>"
						targetpath = ""							
						)
					)				
				nodelist.items = buildnodenamelist()
				)
			
			on pickdir pressed do
				(
				if (p = getsavepath()) != undefined do
					(
					targetdir.text = p + "\\"
					for i in bakechannels do
						(
						i.filename = p + "\\"
						)
					targetpath = p + "\\"
					)
				)
			
			on mapchannel buttonup do
				(
				val = mapchannel.value
				for i in nodelist.selection do
					(
					try
						(
						if (meshop.getmapsupport nodes[i].mesh val) then
							(
							nodes[i].bakechannel = val
							)
						else
							(
							messagebox ("Node " + nodes[i].name + " does not have map coords for channel " + val as string + "!") title:"Bakersfailed!"
							)
						)
					catch
						(
						messagebox ("Node " + nodes[i].name + " will not convert to a trimesh\nThis is a bad sign as they are probably not renderable!") title:"Bakersfailed!"
						)
					)
				)
			
			on mapchannel entered do
				(
				mapchannel.buttonup()
				)
				
			on edgepadding changed val do
				(
				for i in nodelist.selection do
					(
					nodes[i].ndilations = val
					)
				)
			
			on nodelist selectionend do
				(
				rollupall()
				sel = (nodelist.selection as array)
				if sel.count != 0 then
					(
					buildchannellist sel
					
					-- deal with != mapchannels
					tmpval = nodes[sel[1]].bakechannel
					hideval = false
					for i = 1 to sel.count do
						(
						if nodes[sel[i]].bakechannel != tmpval do (hideval = true; exit)
						)
						
					if hideval then
						(
						mapchannel.indeterminate = true
						)
					else
						(
						mapchannel.value = tmpval	
						)
					
					-- deal with != edge padding
					tmpval = nodes[sel[1]].ndilations
					hideval = false
					for i = 1 to sel.count do
						(
						if nodes[sel[i]].ndilations != tmpval do (hideval = true; exit)
						)
					
					if hideval then
						(
						edgepadding.indeterminate = true
						)
					else
						(
						edgepadding.value = tmpval	
						)
					)
				else
					(
					channellist.listitems.clear()
					)
				)
			
			on addnode pressed do
				(
				if (obj = selectbyname single:false title:"Pick Node" filter:(fn listfilter obj = (for i in nodes do (if ((obj == i) or (superclassof obj !=  geometryclass)) do return false); if (classof obj == Targetobject) do return false; return true))) != undefined do
					(
					for i in obj do
						(
						if (oktoaddnode i) do
							(
							append nodes i
							append nodebakeassign (fillbitarray 1 20 false)
							i.bakeenabled = true
							)
						)
					nodelist.items = buildnodenamelist()
					)
				)
			
			on addselectednodes pressed do
				(
				if selection.count != 0 do
					(
					for i = 1 to selection.count do
						(
						if (oktoaddnode selection[i]) do
							(
							append nodes selection[i]
							append nodebakeassign (fillbitarray 1 20 false)
							selection[i].bakeenabled = true
							)
						)
					nodelist.items = buildnodenamelist()
					)
				)
			
			on delnode pressed do
				(
				if nodelist.selection.count != 0 do
					(
					sel = nodelist.selection as array
					for i = sel.count to 1 by -1 do
						(
						nodes[sel[i]].removeallbakeelements()
						nodes[sel[i]].bakeenabled = false
						deleteitem nodes sel[i]
						deleteitem nodebakeassign sel[i]
						)
					nodelist.items = buildnodenamelist()
					channellist.listitems.clear()
					)
				)
			
			on addchannel pressed do
				(
				if (nodelist.selection as array).count != 0 then
					(
					rollout chandialogroll "Add Channel" width:150 height:209
						(
						listBox chantype "Channel Type" pos:[7,6] width:135 height:11 items:#("Complete Map", "Blend Map", "Shadow Map", "Alpha Map", "Diffuse Map", "Lighting Map", "Normals Map", "Specular Map")
						button clickok "OK" pos:[15,179] width:57 height:20
						button clickcancel "Cancel" pos:[78,179] width:57 height:20
						
						on chantype doubleclicked arg do
							(
							clickok.pressed()
							)
						
						on clickok pressed do
							(
							channum = chantype.selection
					
							-- add new bake channel and add index to nodeassign
							newchan = #("completemap", "blendmap", "shadowsmap", "alphamap", "diffusemap", "lightingmap", "normalsmap", "specularmap")
							BC = (execute (newchan[chantype.selection] + "()"))
							append bakechannels BC
							BC.elementname = ("New " + chantype.selected)
							BC.autosizeon = true
							BC.filename = targetpath
							sel = nodelist.selection as array
							bakecount = bakechannels.count
							for i in sel do
								(
								nodebakeassign[i][bakecount] = true
								nodes[i].addbakeelement bakechannels[bakecount]
								)
		
							-- add name to channels list
							newlistnode = bakeobjectroll.channellist.listitems.add()
							newlistnode.text = (classof BC) as string
							newlistnode.checked = true
							sub1 = newlistnode.listsubitems.add()
							sub2 = newlistnode.listsubitems.add()
							sub1.text = bakecount as string
							sub2.text = ("New " + chantype.selected)
							
							destroydialog chandialogroll
							)
						
						on clickcancel pressed do
							(
							destroydialog chandialogroll
							)
						)
					createdialog chandialogroll
					)
				else
					(
					messagebox "No nodes selected!" title:"Gibbon!"
					)
				)
				
			on linkchannel pressed do
				(
				if (nodelist.selection as array).count != 0 then
					(
					rollout existchannelroll "Existing Channels" width:380 height:210
						(
						activeXControl channellist "MSComctlLib.ListViewCtrl" width:360 height:173 align:#center
						button btnok "OK" width:57 height:20 across:2 offset:[55,0]
						button btncancel "Cancel" width:57 height:20 offset:[-55,0]
						
						on existchannelroll open do
							(
							-- initialize listview
							channellist.gridlines = true
							channellist.view = #lvwreport
							channellist.checkboxes = true
							channellist.columnheaders.add()
							channellist.columnheaders.add()
							channellist.columnheaders.add()
							channellist.columnheaders[1].text = "Channel Type"
							channellist.columnheaders[2].text = "Channel Index"
							channellist.columnheaders[3].text = "Channel Name"
							channellist.allowcolumnreorder = false
							channellist.fullrowselect = true
							channellist.LabelEdit = #lvwmanual							
							
							for i = 1 to bakechannels.count do
								(
								-- add name to channels list
								newlistnode = channellist.listitems.add()
								newlistnode.text = (classof bakechannels[i]) as string
								newlistnode.checked = bakechannels[i].enabled
								sub1 = newlistnode.listsubitems.add()
								sub2 = newlistnode.listsubitems.add()
								sub1.text = i as string
								sub2.text = bakechannels[i].elementname
								)								
							)
						
						on btnok pressed do
							(
							if (be = channellist.selecteditem) != undefined do
								(
								for i in nodelist.selection do
									(
									if not nodebakeassign[i][be.listsubitems[1].text as integer] do
										(
										nodebakeassign[i][be.listsubitems[1].text as integer] = true
										nodes[i].addbakeelement bakechannels[be.listsubitems[1].text as integer]
										)
									buildchannellist (nodelist.selection as array)
									rollupall()
									)							
								destroydialog existchannelroll
								)
							)
						
						on btncancel pressed do
							(
							destroydialog existchannelroll
							)
						)
					createdialog existchannelroll
					)
				else
					(
					messagebox "No nodes to link to!" title:"Gibbon!"
					)
				)
			
			on purgechannel pressed do
				(
				if (be = channellist.selecteditem) != undefined do
					(
					if (querybox "Really delete all references to this channel?") do
						(
						for i = 1 to nodes.count do
							(
							nodebakeassign[i][be.listsubitems[1].text as integer] = false
							nodes[i].removebakeelement bakechannels[be.listsubitems[1].text as integer]
							buildchannellist (nodelist.selection as array)
							rollupall()
							)
						deleteitem bakechannels (be.listsubitems[1].text as integer)
						be = undefined						
						)
					)
				)			
			
			on delchannel pressed do
				(
				if (be = channellist.selecteditem) != undefined do
					(
					for i in nodelist.selection do
						(
						nodebakeassign[i][be.listsubitems[1].text as integer] = false
						nodes[i].removebakeelement bakechannels[be.listsubitems[1].text as integer]
						buildchannellist (nodelist.selection as array)
						rollupall()
						)
					)
				)
			
			on channellist itemcheck item do
				(
				bakechannels[item.listsubitems[1].text as integer].enabled = item.checked
				case (item.text) of
					(
					"LightingMap": (lightmaproll.redrawroll (item.listsubitems[1].text as integer) item.index)
					"BlendMap": (blendmaproll.redrawroll (item.listsubitems[1].text as integer) item.index)
					"AlphaMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)
					"NormalsMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)
					"ShadowsMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)
					"diffuseMap": (multiroll2.redrawroll (item.listsubitems[1].text as integer) item.index)
					"specularMap": (multiroll2.redrawroll (item.listsubitems[1].text as integer) item.index)
					"CompleteMap": (completemaproll.redrawroll (item.listsubitems[1].text as integer) item.index)
					)		
				)
			
			on channellist itemclick item do
				(
				rollupall()
				case (item.text) of
					(
					"LightingMap": (lightmaproll.redrawroll (item.listsubitems[1].text as integer) item.index)
					"BlendMap": (blendmaproll.redrawroll (item.listsubitems[1].text as integer) item.index)
					"AlphaMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)
					"NormalsMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)
					"ShadowsMap": (multiroll1.redrawroll (item.listsubitems[1].text as integer) item.index)	
					"diffuseMap": (multiroll2.redrawroll (item.listsubitems[1].text as integer) item.index)
					"specularMap": (multiroll2.redrawroll (item.listsubitems[1].text as integer) item.index)
					"CompleteMap": (completemaproll.redrawroll (item.listsubitems[1].text as integer) item.index)		
					)
				)
			)
		
		--***********************************************
		--* map type rollouts							*
		--***********************************************
		
		rollout lightmaproll "LightMap Properties" width:660 height:84
			(
			local currentindex, listviewindex
		
			function redrawroll channelindex lvwindex = 
				(
				lightmaproll.open = true
				lightmaproll.channelname.text = bakechannels[channelindex].elementname
				lightmaproll.direct.checked = bakechannels[channelindex].directon
				lightmaproll.indirect.checked = bakechannels[channelindex].indirecton
				lightmaproll.shadows.checked = bakechannels[channelindex].shadowson
				lightmaproll.filter.checked = bakechannels[channelindex].filteron
				lightmaproll.renderwidth.value = bakechannels[channelindex].outputszX
				lightmaproll.renderheight.value = bakechannels[channelindex].outputszY
				lightmaproll.currentindex = channelindex
				lightmaproll.listviewindex = lvwindex
				bakeobjectroll.UIenable lightmaproll bakechannels[channelindex].enabled
				)
			
			label lbl1 "Channel Name" pos:[6,13] width:79 height:15 enabled:false
			editText channelname "" pos:[80,9] width:218 height:1 enabled:false
			checkbox direct "Render Direct Lighting" pos:[305,7] width:158 height:17 enabled:false
			checkbox indirect "Render Indirect Lighting" pos:[305,23] width:158 height:17 enabled:false
			checkbox shadows "Render Shadows" pos:[305,41] width:158 height:17 enabled:false
			checkbox filter "Enable Filtering" pos:[305,58] width:158 height:17 enabled:false
			spinner renderwidth "Width" pos:[19,39] width:99 height:16 range:[1,9999,256] type:#integer scale:1 enabled:false
			spinner renderheight "Height" pos:[19,58] width:99 height:16 range:[1,9999,256] type:#integer scale:1 enabled:false
			dropDownList presets "Presets" pos:[125,35] width:172 height:40 items:#("128 X 128", "256 X 256", "512 X 512", "1024 X1024", "2048 X 2048") enabled:false
			
			on presets selected arg do
				(
				size = #(128, 256, 512, 1024, 2048)
				renderwidth.value = size[arg]
				renderheight.value = size[arg]
				bakechannels[currentindex].outputszX = size[arg]
				bakechannels[currentindex].outputszY = size[arg]
				)

			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)				
			
			on channelname entered txt do
				(
				bakechannels[currentindex].elementname = txt
				bakeobjectroll.channellist.listitems[listviewindex].listsubitems[2].text = txt
				channelname.text = txt
				)
			
			on direct changed val do
				(
				bakechannels[currentindex].directon = val
				)
		
			on indirect changed val do
				(
				bakechannels[currentindex].indirecton = val
				)
		
			on shadows changed val do
				(
				bakechannels[currentindex].shadowson = val
				)
		
			on filter changed val do
				(
				bakechannels[currentindex].filteron = val
				)	
			)
		
		rollout blendmaproll "BlendMap Properties" width:660 height:84
			(
			local currentindex, listviewindex
		
			function redrawroll channelindex lvwindex = 
				(
				blendmaproll.open = true
				blendmaproll.channelname.text = bakechannels[channelindex].elementname
				blendmaproll.shadows.checked = bakechannels[channelindex].shadowson
				blendmaproll.filter.checked = bakechannels[channelindex].filteron
				blendmaproll.renderwidth.value = bakechannels[channelindex].outputszX
				blendmaproll.renderheight.value = bakechannels[channelindex].outputszY
				blendmaproll.ambient.checked = bakechannels[channelindex].ambienton
				blendmaproll.diffuse.checked = bakechannels[channelindex].diffuseon
				blendmaproll.specular.checked = bakechannels[channelindex].specularon
				blendmaproll.selfillum.checked = bakechannels[channelindex].emissionon
				blendmaproll.lighting.checked = bakechannels[channelindex].lightingon
				blendmaproll.reflection.checked = bakechannels[channelindex].reflectionon
				blendmaproll.refraction.checked = bakechannels[channelindex].refractionon
				blendmaproll.currentindex = channelindex
				blendmaproll.listviewindex = lvwindex
				bakeobjectroll.UIenable blendmaproll bakechannels[channelindex].enabled
				)
			
			editText channelname "" pos:[80,9] width:218 height:1 enabled:false
			checkbox ambient "Render Ambient" pos:[305,7] width:110 height:17 enabled:false
			checkbox diffuse "Render Diffuse" pos:[305,25] width:110 height:17 enabled:false
			checkbox specular "Render Specular" pos:[305,43] width:110 height:17 enabled:false
			checkbox filter "Enable Filtering" pos:[460,60] width:133 height:17 enabled:false
			spinner renderwidth "Width" pos:[19,39] width:99 height:16 range:[1,9999,256] type:#integer scale:1 enabled:false
			spinner renderheight "Height" pos:[19,58] width:99 height:16 range:[1,9999,256] type:#integer scale:1 enabled:false
			dropDownList presets "Presets" pos:[125,35] width:172 height:40 items:#("128 X 128", "256 X 256", "512 X 512", "1024 X1024", "2048 X 2048") enabled:false
			label lbl1 "Channel Name" pos:[6,13] width:79 height:15 enabled:false
			checkbox selfillum "Render SelfIllumination" pos:[460,8] width:133 height:17 enabled:false
			checkbox lighting "Render Lighting" pos:[305,61] width:110 height:17 enabled:false
			checkbox reflection "Render Reflection" pos:[460,26] width:133 height:17 enabled:false
			checkbox refraction "Render Refraction" pos:[460,43] width:133 height:17 enabled:false
			checkbox shadows "Render Shadows" pos:[305,79] width:110 height:17 enabled:false
			
			on presets selected arg do
				(
				size = #(128, 256, 512, 1024, 2048)
				renderwidth.value = size[arg]
				renderheight.value = size[arg]
				bakechannels[currentindex].outputszX = size[arg]
				bakechannels[currentindex].outputszY = size[arg]
				)

			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)
			
			on channelname entered txt do
				(
				bakechannels[currentindex].elementname = txt
				bakeobjectroll.channellist.listitems[listviewindex].listsubitems[2].text = txt
				channelname.text = txt
				)
			
			on ambient changed val do
				(
				bakechannels[currentindex].ambienton = val
				)
		
			on diffuse changed val do
				(
				bakechannels[currentindex].diffuseon = val
				)
		
			on specular changed val do
				(
				bakechannels[currentindex].specularon = val
				)
		
			on selfillum changed val do
				(
				bakechannels[currentindex].emissionon = val
				)
		
			on lighting changed val do
				(
				bakechannels[currentindex].lightingon = val
				)
		
			on shadows changed val do
				(
				bakechannels[currentindex].shadowson = val
				)
		
			on reflection changed val do
				(
				bakechannels[currentindex].reflectionon = val
				)		
		
			on filter changed val do
				(
				bakechannels[currentindex].filteron = val
				)
		
			on refraction changed val do
				(
				bakechannels[currentindex].refractionon = val
				)
			
			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)		
			)
			
		rollout multiroll1 "Alpha Map\\Shadow Map\\Normal Map Properties" width:660 height:84
			(
			local currentindex, listviewindex
		
			function redrawroll channelindex lvwindex = 
				(
				multiroll1.open = true
				multiroll1.channelname.text = bakechannels[channelindex].elementname
				multiroll1.filter.checked = bakechannels[channelindex].filteron
				multiroll1.renderwidth.value = bakechannels[channelindex].outputszX
				multiroll1.renderheight.value = bakechannels[channelindex].outputszY
				multiroll1.currentindex = channelindex
				multiroll1.listviewindex = lvwindex
				bakeobjectroll.UIenable multiroll1 bakechannels[channelindex].enabled
				)
			
			editText channelname "" pos:[80,9] width:218 height:1
			checkbox filter "Enable Filtering" pos:[309,7] width:158 height:17
			spinner renderwidth "Width" pos:[19,39] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			spinner renderheight "Height" pos:[19,58] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			dropDownList presets "Presets" pos:[125,35] width:172 height:40 items:#("128 X 128", "256 X 256", "512 X 512", "1024 X1024", "2048 X 2048")
			label lbl1 "Channel Name" pos:[6,13] width:79 height:15
		
			on presets selected arg do
				(
				size = #(128, 256, 512, 1024, 2048)
				renderwidth.value = size[arg]
				renderheight.value = size[arg]
				bakechannels[currentindex].outputszX = size[arg]
				bakechannels[currentindex].outputszY = size[arg]
				)

			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)
	
			on channelname entered txt do
				(
				bakechannels[currentindex].elementname = txt
				bakeobjectroll.channellist.listitems[listviewindex].listsubitems[2].text = txt
				channelname.text = txt
				)			
		
			on filter changed val do
				(
				bakechannels[currentindex].filteron = val
				)	
			)
		
		rollout multiroll2 "Diffuse Map\\Specular Map Properties" width:660 height:84
			(
			local currentindex, listviewindex
		
			function redrawroll channelindex lvwindex = 
				(
				multiroll2.open = true
				multiroll2.channelname.text = bakechannels[channelindex].elementname
				multiroll2.filter.checked = bakechannels[channelindex].filteron
				multiroll2.renderwidth.value = bakechannels[channelindex].outputszX
				multiroll2.renderheight.value = bakechannels[channelindex].outputszY
				multiroll2.currentindex = channelindex
				multiroll2.listviewindex = lvwindex
				bakeobjectroll.UIenable multiroll2 bakechannels[channelindex].enabled
				)	
			
			editText channelname "" pos:[80,9] width:218 height:1
			checkbox lighting "Render Lighting" pos:[305,9] width:158 height:17
			checkbox shadows "Render Shadows" pos:[305,27] width:158 height:17
			checkbox filter "Enable Filtering" pos:[305,44] width:158 height:17
			spinner renderwidth "Width" pos:[19,39] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			spinner renderheight "Height" pos:[19,58] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			dropDownList presets "Presets" pos:[125,35] width:172 height:40 items:#("128 X 128", "256 X 256", "512 X 512", "1024 X1024", "2048 X 2048")
			label lbl1 "Channel Name" pos:[6,13] width:79 height:15
		
			on presets selected arg do
				(
				size = #(128, 256, 512, 1024, 2048)
				renderwidth.value = size[arg]
				renderheight.value = size[arg]
				bakechannels[currentindex].outputszX = size[arg]
				bakechannels[currentindex].outputszY = size[arg]
				)

			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)
	
			on channelname entered txt do
				(
				bakechannels[currentindex].elementname = txt
				bakeobjectroll.channellist.listitems[listviewindex].listsubitems[2].text = txt
				channelname.text = txt
				)		
		
			on lighting changed val do
				(
				bakechannels[currentindex].lightingon = val
				)
		
			on shadows changed val do
				(
				bakechannels[currentindex].shadowson = val
				)
		
			on filter changed val do
				(
				bakechannels[currentindex].filteron = val
				)
			)
		
		rollout completemaproll "Complete Map Properties" width:660 height:84
			(
			local currentindex, listviewindex
		
			function redrawroll channelindex lvwindex = 
				(
				completemaproll.open = true
				completemaproll.channelname.text = bakechannels[channelindex].elementname
				completemaproll.filter.checked = bakechannels[channelindex].filteron
				completemaproll.renderwidth.value = bakechannels[channelindex].outputszX
				completemaproll.renderheight.value = bakechannels[channelindex].outputszY
				completemaproll.currentindex = channelindex
				completemaproll.listviewindex = lvwindex
				bakeobjectroll.UIenable completemaproll bakechannels[channelindex].enabled
				)
			
			editText channelname "" pos:[80,9] width:218 height:1
			checkbox shadows "Render Shadows" pos:[305,7] width:158 height:17
			checkbox filter "Enable Filtering" pos:[305,24] width:158 height:17
			spinner renderwidth "Width" pos:[19,39] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			spinner renderheight "Height" pos:[19,58] width:99 height:16 range:[1,9999,256] type:#integer scale:1
			dropDownList presets "Presets" pos:[125,35] width:172 height:40 items:#("128 X 128", "256 X 256", "512 X 512", "1024 X1024", "2048 X 2048")
			label lbl1 "Channel Name" pos:[6,13] width:79 height:15
		
			on presets selected arg do
				(
				size = #(128, 256, 512, 1024, 2048)
				renderwidth.value = size[arg]
				renderheight.value = size[arg]
				bakechannels[currentindex].outputszX = size[arg]
				bakechannels[currentindex].outputszY = size[arg]
				)

			on renderwidth changed val do
				(
				bakechannels[currentindex].outputszX = val
				)
		
			on renderheight changed val do
				(
				bakechannels[currentindex].outputszY = val
				)
	
			on channelname entered txt do
				(
				bakechannels[currentindex].elementname = txt
				bakeobjectroll.channellist.listitems[listviewindex].listsubitems[2].text = txt
				channelname.text = txt
				)			
			
			on shadows changed val do
				(
				bakechannels[currentindex].shadowson = val
				)
		
			on filter changed val do
				(
				bakechannels[currentindex].filteron = val
				)	
			)	
		
		--*******************************
		--*	fileoverwrite dialog roll	*
		--*******************************
		
		rollout overwritedialog "File Overwrite Confirm" width:270 height:122
			(
			local ticktime
			
			button okbutton "Overwrite File" pos:[8,74] width:121 height:20
			button skipbutton "Skip Channel" pos:[140,74] width:121 height:20
			timer tmr1 "Timer" pos:[235,97] width:24 height:24
			label timetoclose "Auto Skip in : 0 Seconds" pos:[9,99] width:218 height:20
			label inf1 "This file already exists!" pos:[80,16] width:110 height:15
			label inf2 "file:\\" pos:[8,43] width:254 height:15
			
			on overwritedialog open do
				(
				tmr1.active = true
				ticktime = 15
				
				inf2.text = filenamefrompath bakerenderroll.filoverwritename
				)
			
			on tmr1 tick do
				(
				ticktime -= 1
				timetoclose.text =  ("Auto Skip in : " + ticktime as string +  " Seconds")
		
				if ticktime == 0 do
					(
					bakerenderroll.fileoverwrite = false
					destroydialog overwritedialog
					)
				)
			
			on okbutton pressed do
				(
				bakerenderroll.fileoverwrite = true
				destroydialog overwritedialog
				)
		
			on skipbutton pressed do
				(
				bakerenderroll.fileoverwrite = false
				destroydialog overwritedialog
				)		
			)
		
		--***************************
		--*	render rollout			*
		--***************************
		
		rollout bakerenderroll "Render" width:660 height:41
			(
			local fileoverwrite = true
			local filoverwritename = ""
			local logfilename = undefined
			
			function oktorender = 
				(
				if nodes.count == 0 do return "You do not have any nodes set up!"
				if bakechannels.count == 0 do return "You do not have any channels set up!"
				if bakeobjectroll.targetpath == "" do return "You have not target picked a path yet!"
				
				return true
				)
			
			function oktooverwrite name = 
				(
				bakerenderroll.fileoverwrite = true
				bakerenderroll.filoverwritename = name
				
				createdialog overwritedialog modal:true
				return bakerenderroll.fileoverwrite
				)
			
			function parselocaltime = 
				(
				t = localtime
				
				for i = 1 to t.count do
					(
					if (t[i] == "/") or (t[i] == ":") do t[i] = "-"
					)
				return t
				)
			
			function parseboolean boo = 
				(
				case boo of
					(
					"true": return true
					"false": return false
					default: return false
					)
				)
			
			function secondstoHMS sec = 
				(
				d = (sec / 86400) as integer -- in to days
				dtmp = mod sec 86400 -- left over from days
				tmp = (sec / 60) -- in to minutes
				s = (mod sec 60) as integer -- left over from minutes
				h = (dtmp / 3600) as integer 
				m = (mod tmp 60) as integer
				return (d as string + "d:" +h as string + "h:" + m as string + "m:" + s as string + "s")
				)
			
			button dorender "Bake Textures" pos:[7,172] width:211 height:27
			progressBar pb1 "ProgressBar" pos:[229,178] width:335 height:15 color:(color 30 10 190)
			checkbox filewarning "Show File Overwrite Warnings" pos:[16,36] width:202 height:15
			checkbox showvfb "Show Virtual Frame Buffer" pos:[16,55] width:202 height:15
			label renderstatus "Status:Not in render state." pos:[9,209] width:642 height:17
			checkbox savelog "Save Log File" pos:[131,75] width:131 height:15
			groupBox grp1 "Filename Generation" pos:[276,9] width:371 height:148
			checkbox useprefix "Use Prefix" pos:[289,35] width:73 height:15
			checkbox usesuffix "Use Suffix" pos:[289,54] width:73 height:15
			checkbox usechanneltype "Use Channel Type" pos:[289,81] width:121 height:15
			checkbox usechannelindex "Use Channel Index" pos:[289,100] width:121 height:15
			checkbox usechannelname "Use Channel Name" pos:[289,119] width:121 height:15
			dropDownList fileext "Use Extension" pos:[539,32] width:83 height:40 items:#("TGA", "BMP", "TIF", "JPG", "PNG")
			groupBox grp3 "General Settings" pos:[11,9] width:258 height:148
			radioButtons rendertype "Render Type" pos:[24,118] width:231 height:30 labels:#("Set up nodes", "Selected nodes") columns:2
			editText prefixtext "" pos:[360,34] width:161 height:1
			editText suffixtext "" pos:[360,54] width:161 height:1
			label renderpercent "0%" pos:[572,178] width:81 height:15
			checkbox savefullrender "Save Full Render" pos:[17,94] width:108 height:15
			checkbox saveframes "Save Frames" pos:[131,94] width:131 height:15
			checkbox genlogfile "Generate Log File" pos:[16,75] width:108 height:15
			
			on bakerenderroll open do
				(
				-- read from ini
				if (getfiles (getdir #plugcfg + "\\bakersfield.ini")).count != 0 do
					(
					filewarning.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "filewarning")
					showvfb.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "showvfb")
					filewarning.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "filewarning")
					useprefix.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "useprefix")
					usesuffix.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usesuffix")
					usechanneltype.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechanneltype")
					usechannelname.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechannelname")
					usechannelindex.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechannelindex")
					genlogfile.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "genlogfile")
					rendertype.state = (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "rendertype") as integer
					savelog.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "savelog")
					saveframes.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "saveframes")
					savefullrender.checked = parseboolean (getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "savefull")
					prefixtext.text = getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "pre" 
					suffixtext.text = getinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "suff" 
					savelog.enabled = genlogfile.checked
					)
				)
		
			on bakerenderroll close do
				(
				-- write from ini
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "filewarning" (filewarning.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "showvfb" (showvfb.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "filewarning" (filewarning.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "useprefix" (useprefix.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usesuffix" (usesuffix.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechanneltype" (usechanneltype.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechannelname" (usechannelname.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "usechannelindex" (usechannelindex.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "genlogfile" (genlogfile.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "rendertype" (rendertype.state as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "savelog" (savelog.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "savefull" (savefullrender.checked as string)
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "pre" prefixtext.text
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "suff" suffixtext.text
				setinisetting (getdir #plugcfg + "\\bakersfield.ini") "settings" "saveframes" (saveframes.checked as string)

				bakefloat = undefined
				)
			
			on genlogfile changed val do
				(
				savelog.enabled = val
				)
				
			on dorender pressed do
				(
				try
					(
					numnodes = nodes.count
					if (mess = oktorender()) == true then
						(
						if genlogfile.checked do 
							(
							log = stringstream ""
							format ("<message> New render que started at " + localtime + "\n") to:log
							)
						stime = timestamp()
						renderscenedialog.close()
						
						nsel = #{}
						if rendertype.state == 1 then
							(
							nsel = #{1..nodes.count}
							)
						else
							(
							nsel = bakeobjectroll.nodelist.selection
							)
						
						totalnodestorender = (nsel as array).count
						currentrendernode = 0
						
						-- function to generate sequence string from number
						function digitstring digits number =
						(
							local str = ""
							local curdigit = digits-1
							local curthresh = 10^curdigit
							
							while (number < curthresh and curthresh > 1) do(
								str += "0"	
								curthresh /= 10
							)
							str += (number as string)
							
							return str
						)
						
	
					
						for i in nsel do
							(
							try
								(
								currentrendernode += 1
								if genlogfile.checked do format "<message> new node setup : %\n" nodes[i].name to:log
				
								numchannels = nodes[i].numbakeelements()
								if numchannels == 0 then
									(
									if genlogfile.checked do format "<message> WARNING : no bake channels found.  Skipping node.\n" to:log
									)
								else
									(
									if genlogfile.checked do format "<message> % bake channels found\n" numchannels to:log
									

									local curtime = sliderTime
									local starttime = curtime
									local endtime = curtime
									
									
									if (saveframes.checked) do (
										local range = animationRange
										starttime = range.start
										endtime = range.end
									)
									local totalframes = endtime.frame+1 - starttime.frame
									
								for curtime = starttime to endtime do (
									
									if (genlogfile.checked and saveframes.checked) do format "<message> % frame setup\n" curtime to:log
									sliderTime = curtime
									
									-- for every channel in node build full filename
									for n = 1 to numchannels do
										(
										chindex = nodebakeassign[i] as array
										be = nodes[i].getbakeelement n
										start = bakeobjectroll.targetpath 
										end = ""
										-- buildfilenames
										if (useprefix.checked) do start += (prefixtext.text + " ")
										if usechanneltype.checked do start += (classof be as string + " ")
										if usechannelindex.checked do start += (chindex[n] as string + " ")
										if usechannelname.checked do start += (be.elementname + " ")
										if (usesuffix.checked) do end += " " + suffixtext.text	
										if (saveframes.checked) do end+= " " + (digitstring 5 (curtime.frame as integer))
										end += "." + fileext.selected
					
										be.filetype = (start + nodes[i].name + end)
										if genlogfile.checked do format "<message> filename for channel % : %\n" n be.filetype to:log
										
										-- deal with file overwrites
										if filewarning.checked do
											(
											if (f = getfiles be.filetype).count != 0 do
												(
												be.enabled = (oktooverwrite be.filetype)
					
												if be.enabled then
													(
													if genlogfile.checked do format "<message> WARNING : Duplicate filename found, user overwrite.\n" to:log
													)
												else
													(
													if genlogfile.checked do format "<message> WARNING : Duplicate filename found, user skipped.\n" to:log
													)
												)
											)
										)
									
									-- select the node and do the render
									-- eta = (seconds rendered so far / numobjects done) * num objects left
									select nodes[i]
									eta = (((timestamp() - stime) / 1000.0) / currentrendernode) * (totalnodestorender - currentrendernode)
									
									renderstatus.text = ("Status : Rendering Node " + currentrendernode as string + "/" + totalnodestorender as string + " \"" + nodes[i].name + "\" " + " Time to go : " + (secondstoHMS eta) )
										
									if genlogfile.checked do format ("<message> render started at " + localtime + "\n") to:log

									pb1.value = ((((curtime.frame-starttime.frame) as float)*(currentrendernode as float)) / (totalnodestorender*totalframes)) * 100
									renderpercent.text = pb1.value as string + "%"
									bmap = render rendertype:#bakeselected outputsize:[nodes[i].renderwidth(), nodes[i].renderheight()] vfb:showvfb.checked
			
									if savefullrender.checked do
										(
										bmap.filename = bakeobjectroll.targetpath + " " + nodes[i].name + " Full Rendeder." + fileext.selected
										save bmap
										)
									undisplay bmap
									bmap = undefined
									
									if genlogfile.checked do format ("<message> render completed at " + localtime + "\n") to:log
									)
									)
								)
							catch
								(
								if genlogfile.checked do format ("<message> ERROR : a problem with this object has occured!!\n") to:log
								if bmap != undefined do
									(
									undisplay bmap
									bmap = undefined
									)
								)
							)
						pb1.value = 0.0
						renderpercent.text = "0%"
						renderstatus.text = "Status : Not in render state."
						
						-- close log file and display if needed
						if genlogfile.checked do 
							(
							format "<message> render que completed in % seconds\n" (secondstoHMS ((timestamp() - stime) / 1000.0)) to:log
							--close log
							
							if savelog.checked do
								(
								slog = createfile (getdir #plugcfg + "\\bakersfield renderlog" + parselocaltime() + ".txt")
							
								seek log 0
								while not (eof log) do
									(
									format "%\n" (readline log)  to:slog
									)
								close slog
								)
							
							seek log 0
							
							rollout displogfileroll "Render Log" width:651 height:314
								(
								listBox logfile "Last Render Log" pos:[6,9] width:639 height:21
								
								on displogfileroll open do
									(
									tmp = #()
									while not (eof log) do
										(
										append tmp (readline log)
										)
									logfile.items = tmp
									tmp = undefined
									)
								)
							createdialog displogfileroll

							)
						)
					else
						(
						messagebox mess title:"bakersfailed"
						)
					)
				catch
					(
					if log != undefined do close log
					messagebox "Program Error!"  title:"bakersfailed"
					)
				)
			)
		if bakefloat == undefined do
			(
			bakefloat = newrolloutfloater "Bakersfield V1.23" 675 700
			addrollout bakeobjectroll bakefloat
			addrollout blendmaproll bakefloat rolledup:true
			addrollout lightmaproll bakefloat rolledup:true
			addrollout multiroll1 bakefloat rolledup:true
			addrollout multiroll2 bakefloat rolledup:true
			addrollout completemaproll bakefloat rolledup:true
			addrollout bakerenderroll bakefloat
			)
		)
	)

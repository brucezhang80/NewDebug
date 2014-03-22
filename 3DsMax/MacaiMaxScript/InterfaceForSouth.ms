
global theRoot
-- //////////////////////////function//////////////////////////////////////////////
function Yvi_getViewportSize =
(
	local _height
	layout = viewport.getLayout()
	case layout of
	(
		#layout_1: -- 1 viewport
			(
				_size = getViewSize()
				_height = _size.y
				return _height
			)
		#layout_2v: -- 2 viewports, vertical split, both same size 
			(
				_size = getViewSize()
				_height = _size.y
				return _height
			)
		#layout_2h: -- 2 viewports, horizontal split, both same size
			(
				_size = getViewSize()
				_height = _size.y * 2
				return _height
			)
		#layout_2ht: -- 2 viewports, horizontal split, top smaller
			(
				if(viewport.activeViewport == 1)then -- top smaller viewport is active.
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				if(viewport.activeViewport == 2)then 
				(
					_size = getViewSize()
					_height = _size.y + (_size.y / 2)
				)
				
				return _height
			)
		#layout_2hb: -- 2 viewports, horizontal split, top larger
			(
				if(viewport.activeViewport == 1)then -- top larger viewport is active.
				(
					_size = getViewSize()
					_height = _size.y + (_size.y / 2)
					
				)
				if(viewport.activeViewport == 2)then 
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				
				return _height
			)
		#layout_3vl: -- 3 viewports, 2 on left, 1 on right
			(
				if(viewport.activeViewport == 1 or viewport.activeViewport == 2)then
				(
					_size = getViewSize()
					_height = _size.y * 2
					
				)
				if (viewport.activeViewport == 3) then
				(
					_size = getViewSize()
					_height = _size.y
				)
				
				return _height
			)
		#layout_3vr: -- 3 viewports, 1 on left, 2 on right
			(
				if(viewport.activeViewport == 2 or viewport.activeViewport == 3)then
				(
					_size = getViewSize()
					_height = _size.y * 2
					
				)
				if (viewport.activeViewport == 1) then
				(
					_size = getViewSize()
					_height = _size.y
				)
				
				return _height
			)
		#layout_3ht: -- 3 viewports, 2 on top, 1 on bottom
			(
				_size = getViewSize()
				_height = _size.y * 2
				
				return _height
			)
		#layout_3hb: -- 3 viewports, 1 on top, 2 on bottom
			(
				_size = getViewSize()
				height = _size.y * 2
				
				return _height
			)
		#layout_4: -- 4 viewports, all same size
			(
				_size = getViewSize()
				_height = _size.y * 2
				
				return _height
			)
		#layout_4vl: -- 4 viewports, 3 on left, 1 on right
			(
				
				if (viewport.activeViewport != 4) then
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				else
				(
					_size = getViewSize()
					_height = _size.y
				)
				
				return _height
			)
		#layout_4vr: -- 4 viewports, 1 on left, 3 on right
			(
				
				if (viewport.activeViewport != 1) then
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				else
				(
					_size = getViewSize()
					_height = _size.y
				)
				
				return _height
			)
		#layout_4ht: -- 4 viewports, 3 on top, 1 on bottom
			(
				if (viewport.activeViewport != 4) then
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				else
				(
					_size = getViewSize()
					_height = _size.y + (_size.y / 2)
				)
				
				return _height
			)
		#layout_4hb: -- 4 viewports, 1 on top, 3 on bottom
			(
				if (viewport.activeViewport != 1) then
				(
					_size = getViewSize()
					_height = _size.y * 3
				)
				else
				(
					_size = getViewSize()
					_height = _size.y + (_size.y / 2)
				)
				
				return _height
			)
			
		default:return 600
	)
)

function Yvi_addChildren tv theNode theChildren =
(
	for c in theChildren do
	(
		newNode = tv.Nodes.add theNode.index 4 "" c.name
		theNode.sorted = true
		newNode.checked = not c.isHidden
		newNode.forecolor = color c.wirecolor.b c.wirecolor.g c.wirecolor.r
		Yvi_addChildren tv newNode c.children
	)

)

function Yvi_initTreeView treeCtrl= 
(
	treeCtrl.Indentation = 28*15
	treeCtrl.LineStyle = #tvwRootLines 
	treeCtrl.checkboxes = true
	treeCtrl.sorted = true

	treeCtrl.Enabled = true
	treeCtrl.LabelEdit = #tvwAutomatic 
	treeCtrl.Scroll = true
	
	
	--theRoot = treeCtrl.Nodes.add()
	--theRoot.text = "Layer Root"
	rootNodes = for obj in Objects where obj.parent == undefined collect obj
	Yvi_addChildren treeCtrl theRoot rootNodes
)

function Yvi_fillInTreeView tv =
(

) 

function Yvi_getByGroupChild GroupObj = 
(
	local groupArr = #(), gchild = #()

	setGroupOpen GroupObj true
	append groupArr GroupObj --将组节点装入数组

	gchild = for j in GroupObj.children collect j --收集组内子节点
	setGroupOpen GroupObj false
	print gchild as string
	return gchild
)

--/////////////////////////////////////////////////////////////UI//////////////////////////////////////
global dlg_size = 0

try(DestroyDialog rl_TreeView)catch()
Rollout rl_TreeView "SceneTreeView" 
(
	activeXControl axc_layerTv "MSComctlLib.TreeCtrl" height:460 width:140 align:#left
	activeXControl axc_imgTv "MSComctlLib.ImageListCtrl" height:0 width:0


	on rl_TreeView open do
	(
		theRoot = axc_layerTv.Nodes.add()
		theRoot.text = "Layer Root"
		
		dlg_size = getDialogSize rl_TreeView
		
		_height = Yvi_getViewportSize()
		rl_TreeView.height = _height - 35
		
		Yvi_initTreeView axc_layerTv
	)
	
	on rl_TreeView resized size do
	(
		m_size = size - dlg_size
	)
	
	on rl_TreeView close do
	(
		callbacks.removeScripts id:#Yvi_createNode01
	)
)CreateDialog rl_TreeView style:#(#style_titlebar,#style_resizing,\
	#style_minimizebox,#style_maximizebox,#style_sysmenu) pos:[2,126]
	
	
--/////////////Call Back
function Yvi_createNode _node =
(

	if isValidObj _node then 
	(
		newNode = rl_TreeView.axc_layerTv.Nodes.add theRoot.index 4 "" _node.name
		theRoot.sorted = true
		theRoot.checked = not _node.isHidden
		newNode.forecolor = color _node.wirecolor.b _node.wirecolor.g _node.wirecolor.r
		Yvi_addChildren rl_TreeView.axc_layerTv newNode _node.children
			
		if(isGroupHead _node) then
		(
			local arrChildren = #()
			arrChildren = Yvi_getByGroupChild _node
			--print ( arrChildren as string )
			for obj in arrChildren do
			(
				print "---------------"
				print obj
				
				
				newNode = rl_TreeView.axc_layerTv.Nodes.add theRoot.index 4 "" _node.name
				theRoot.sorted = true
				theRoot.checked = not _node.isHidden
				newNode.forecolor = color _node.wirecolor.b _node.wirecolor.g _node.wirecolor.r
				Yvi_addChildren rl_TreeView.axc_layerTv newNode _node.children
			)
		)
	)
)
callbacks.addScript #nodeCreated "Yvi_createNode(callbacks.notificationParam())" ID:#Yvi_createNode01
	
	
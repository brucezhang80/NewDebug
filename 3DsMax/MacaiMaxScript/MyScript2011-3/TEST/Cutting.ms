rollout rl_Cutting "cut" 
(
	Button btn_getNumFace "getNumFaces"
	
	on btn_getNumFace pressed do
	(
		local sel = selection as array
		for s in sel do
		(
			num_face = getNumFaces s
			print num_face 
		)
	)
	
)CreateDialog rl_Cutting
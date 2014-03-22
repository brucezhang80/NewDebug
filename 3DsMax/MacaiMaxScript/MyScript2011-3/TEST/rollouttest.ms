
global dz = "" /*define global and init it to empty */
rollout c1 "¾íÕ¹À¸1" width:495 height:196
(
	edittext edt_dz "dz"
	
	on edt_dz changed text do /*or you can use "entered" to changes the text in the edt_dz box*/
	(
		dz = edt_dz.text /*when you've changed or entered the text in the edt_dx box, the global value "dz" equal to edt_dx box's text*/
	)
	
	on edt_dz entered val do /*when your pressed the keyborad "Enter", then the text equal to global value dz in the edt_c2dz */
	(
		c2.edt_c2dz.text = dz
	)
)


rollout c2 "¾íÕ¹À¸2" width:495 height:196
(
	edittext edt_c2dz "c2dz"
	
	on edt_c2dz entered text do /*if you enter some text in the edt_c2dz box and pressed keyborad "Enter" */
	(
		c1.edt_dz.text = dz + dz + text /* then the text in the edt_c2dz box is equal to global value "dz + dz + 'you entered the text'" */
	)
)
try (closerolloutfloater FloatTest) catch () 
FloatTest = newrolloutfloater "test rollout by yvi" 200 120
addrollout c1 FloatTest --add the c1 to RolloutFloater 
addrollout c2 FloatTest --radd the c2
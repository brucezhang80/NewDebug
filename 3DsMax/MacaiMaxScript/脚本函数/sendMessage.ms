global  testRollout
try(destroyDialog  testRollout) catch()
(
	toto = point name:"toto"
	animate on at time 100 $toto.position = [100 , 100 , 100]
  
	rollout testRollout "Collapse Reduced Keys" width:150 heig\ht:240
	 (
		 button btnPick "pick tracks" pos:[10,20] width:127 height:20	

		on  btnPick pressed do 
		(
			reduceKeys $toto.position.controller 0.5 1f
			hwnd = windows.getChildHWND 0 testRollout.title
			windows.sendMessage hwnd[1] 0x0112 0xF120 0 -- restore
		)
	 )
	createDialog testRollout 150 250
)
Try(DestroyDialog  RolTestDrag) Catch()
rollout RolTestDrag "Pos Change" 
(
    local mousedd = false
    local ThePos = [0,0]
    button BtnA "aaa"
    button BtnB "bbb"
    button BtnC "Close"
	
    on RolTestDrag lbuttonup pos do mousedd = false ;
    on RolTestDrag lbuttondown pos do (mousedd = true ; ThePos = pos)
    on RolTestDrag mousemove pos do  if mousedd do SetDialogPos   RolTestDrag  (mouse.screenpos - thePos)
    on BtnC pressed do DestroyDialog  RolTestDrag
)
CreateDialog RolTestDrag Style:#()
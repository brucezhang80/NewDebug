try(destroyDialog posChangeTest) catch()
rollout posChangeTest "Pos Change"
(
button bt1 "Close"

local mouseIsDown = false,thePos = [0,0]

on posChangeTest lbuttondown pos do
(mouseIsDown = true ; thePos = pos)
on posChangeTest lbuttonup pos do
mouseIsDown = false
on posChangeTest mousemove pos do if mouseIsDown do
setDialogPos posChangeTest (mouse.screenpos - thePos)

on bt1 pressed do destroyDialog posChangeTest
)
createDialog posChangeTest style:#()
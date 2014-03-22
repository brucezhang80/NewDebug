Fn GetViewportRootPos  =
(
	return Mouse.ScreenPos - Mouse.Pos
)

try(DestroyDialog FollowViewport)catch()
Rollout FollowViewport "FollowViewport" width:70 height:23
(
    Timer theTimer interval:100
	editText theText "" pos:[1,2]width:64 height:20

	
    on theTimer tick do
    (
		local thePos = GetViewportRootPos()
        SetDialogPos FollowViewport (thePos)
		theText.text = (thePos) as string
    )

)
CreateDialog FollowViewport
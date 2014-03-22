rollout CuiScript "清除opacityMap" width:162 height:80
( button btnCls "清除全部OM" pos:[29,14] width:105 height:33

on btnCls pressed  do (
for m = 1 to sceneMaterials.count do ( 

if classof sceneMaterials[m] == standard do (
sceneMaterials[m].opacityMap = undefined
)

)
print "ok"
)
-- end 
) CreateDialog CuiScript




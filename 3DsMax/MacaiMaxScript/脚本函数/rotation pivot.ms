struct OBBstruct
(
minPos=[0.0,0.0,0.0],
maxPos=[0.0,0.0,0.0],
size=[0.0,0.0,0.0],
center=[0.0,0.0,0.0],
angle=[0.0,0.0,0.0],
angleQuat
)

fn getSizeVerts theVertsPos theAngle = (
local minCoord=[theVertsPos[1].x,theVertsPos[1].y,theVertsPos[1].z]
local maxCoord=[theVertsPos[1].x,theVertsPos[1].y,theVertsPos[1].z]
for thisPos in theVertsPos do (
if thisPos.x<minCoord.x do minCoord.x=thisPos.x
if thisPos.x>maxCoord.x do maxCoord.x=thisPos.x
if thisPos.y<minCoord.y do minCoord.y=thisPos.y
if thisPos.y>maxCoord.y do maxCoord.y=thisPos.y
if thisPos.z<minCoord.z do minCoord.z=thisPos.z
if thisPos.z>maxCoord.z do maxCoord.z=thisPos.z
)
local sizeCoord=maxCoord-minCoord
local centerCoord=(minCoord+maxCoord)/2.0
(OBBstruct minPos:minCoord maxPos:maxCoord size:sizeCoord center:centerCoord angle:theAngle)
)

fn findOBB obj angleStep = (
local lim=0.00001
local objNumVerts=obj.numVerts
local theVerts=#{1..objNumVerts}
local polyOpGetVert=polyOp.getVert
local theVertsPos=for v in theVerts collect (polyOpGetVert obj v)
local firstOBB=getSizeVerts theVertsPos [0.0,0.0,0.0]
local centerCoord=firstOBB.center
for v in theVerts do theVertsPos[v]-=centerCoord
local OBBarrayLev1=#()
local OBBvolArrayLev1=#()
local maxAngle=90.0
for angleZ=-maxAngle to maxAngle by angleStep do (
local OBBarrayLev2=#()
local OBBvolArrayLev2=#()
for angleY=-maxAngle to maxAngle by angleStep do (
local OBBarrayLev3=#()
local OBBvolArrayLev3=#()
for angleX=-maxAngle to maxAngle by angleStep do (
local q=inverse(eulerToQuat (eulerAngles angleX angleY angleZ))
local theNewVertsPos=for v in theVerts collect ( theVertsPos[v]*q )
local currentAngle=[angleX,angleY,angleZ]
local currentOBB=getSizeVerts theNewVertsPos currentAngle
append OBBarrayLev3 currentOBB
if currentOBB.size.x<lim do currentOBB.size.x=lim
if currentOBB.size.y<lim do currentOBB.size.y=lim
if currentOBB.size.z<lim do currentOBB.size.z=lim
append OBBvolArrayLev3 (currentOBB.size.x + currentOBB.size.y + currentOBB.size.z)
-- append OBBvolArrayLev3 (currentOBB.size.x * currentOBB.size.y * currentOBB.size.z)
)
local idxLev3=findItem OBBvolArrayLev3 (amin OBBvolArrayLev3)
append OBBarrayLev2 OBBarrayLev3[idxLev3]
append OBBvolArrayLev2 OBBvolArrayLev3[idxLev3]
)
local idxLev2=findItem OBBvolArrayLev2 (amin OBBvolArrayLev2)
append OBBarrayLev1 OBBarrayLev2[idxLev2]
append OBBvolArrayLev1 OBBvolArrayLev2[idxLev2]
)
local idx=findItem OBBvolArrayLev1 (amin OBBvolArrayLev1)
local thisOBB=OBBarrayLev1[idx]
thisOBB.angleQuat=eulerToQuat (eulerAngles thisOBB.angle.x thisOBB.angle.y thisOBB.angle.z)
thisOBB.minPos=(thisOBB.minPos*thisOBB.angleQuat)+ firstOBB.center
thisOBB.maxPos=(thisOBB.maxPos*thisOBB.angleQuat)+ firstOBB.center
thisOBB.center+=firstOBB.center
thisOBB
)

clearListener()
obj=selection[1]
if isValidNode obj do (
local objName=obj.name

local thisOBB=findOBB obj 10.0
format "thisOBB=% \n" thisOBB

point pos:thisOBB.center wireColor:red name:(objName+"_OBB_center")
point pos:thisOBB.minPos wireColor:blue name:(objName+"_OBB_min")
point pos:thisOBB.maxPos wireColor:blue name:(objName+"_OBB_max")

local b=box length:thisOBB.size.y width:thisOBB.size.x height:thisOBB.size.z wireColor:green name:(objName+"_OBB")
b.pivot=[0,0,(thisOBB.size.z/2.0)]
b.rotation.controller.x_rotation=thisOBB.angle.x
b.rotation.controller.y_rotation=thisOBB.angle.y
b.rotation.controller.z_rotation=thisOBB.angle.z
b.pos=thisOBB.center
b.xray=true
select b

max views redraw
)
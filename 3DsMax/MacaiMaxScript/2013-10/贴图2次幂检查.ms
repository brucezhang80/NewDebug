maps=#()
try (destroydialog check_error)catch()
fn isDivided num1 num2 = 
(
   cc=true
   for k = 4 to 10 do
   (
      bb=pow(2)(k)
	  if num1==bb then
	  (
	    cc=false
	  )
	  if num2==bb then
	  (
	    cc=false
	  )
	)
	return cc
)
not2Nfile=#()
struct error1(pName,pPath,pError)
(

local mapfiles=#()

fn addmap mapfile =

(

local mapfileN=mapfile as name

local index=finditem mapfiles mapfileN

if index == 0 do append mapfiles mapfileN

)

enumeratefiles addmap

sort mapfiles

for i=1 to mapfiles.count do 
(
	if doesFileExist (mapfiles[i] as string) then
	(
		try(
         	m=openBitMap (mapfiles[i] as string)
         	if (isDivided (m.width) (m.height)) then
	        (
		         str=((m.width) as string)+"X"+((m.height) as string)
		         append not2Nfile (error1 (getfilenamefile (mapfiles[i] as string)+getfilenametype (mapfiles[i] as string) )  (mapfiles[i] as string) str)
	        )
	       )
		   catch()
	)	
)
if not2Nfile.count>0 then
(

	try (destroydialog check_error)catch()
	
	rollout check_error "¼ì²é"  width:300 height:300
	 (
	 dotNetControl lv "system.windows.forms.listView" pos:[0,10] width:500 height:290
	 fn initLv theLv=
	 (
		--Setup the forms view
		theLv.view=(dotNetClass "system.windows.forms.view").details
		theLv.FullRowSelect=true		--Set so full width of listView is selected and not just first column.
		theLv.GridLines=true			--Show lines between the items. 
		theLv.MultiSelect=true			--Allow for multiple selections. 
	)
	fn addColumns theLv columnsAr=
	(

		for x in 1 to  columnsAr.count do		--Loop through all the column names to be added. 
		(
		     if x==1 then
			(
			w=theLv.width*0.1
			theLv.columns.add columnsAr[x] w		--Add each new column to the listview control. 
			)
		      if x==2 then
			(
			w=theLv.width*0.2
			theLv.columns.add columnsAr[x] w		--Add each new column to the listview control. 
			)
			if x==3 then
			(
			w=theLv.width*0.7
			theLv.columns.add columnsAr[x] w		--Add each new column to the listview control. 
			)
			if x==4 then
			(
			w=theLv.width*0.0
			theLv.columns.add columnsAr[x] w		--Add each new column to the listview control. 
			)
		)
	)
     fn populateList theLv=
	(
		rows=#()		--Empty array to collect rows of data
		for x=1 to not2Nfile.count do		--Loop through all the objects in the scene. 
		(
			li=dotNetObject "System.Windows.Forms.ListViewItem"(x as string) --Create a listViewItem object and name it
			li.subitems.add ( not2Nfile[x].pName)
			li.subitems.add ( not2Nfile[x].pError)
			li.subitems.add ( not2Nfile[x].pPath)	
			append rows li		--Added the listViewItem to the rows array
		)
		theLv.items.addRange rows		--Add the array of rows to the listView control. 
	)
	on lv mouseDoubleClick arg do
	(
	 hit=(lv.HitTest (dotNetObject "System.Drawing.Point" arg.x arg.y))
	 --print hit.item.subItems.item[3].text
     shelllaunch hit.item.subItems.item[3].text ""
	)
     on check_error open do 
	 (
	 initLv lv
	 addColumns lv #("Index","Name","Error","File")
	 populateList lv
	 
	 )
	 
	 )
	 createdialog check_error pos:[170,100]
)
else
(
	messagebox "no error"
)

)


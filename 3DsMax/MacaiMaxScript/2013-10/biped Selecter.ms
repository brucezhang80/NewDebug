namespaceMacai
(
	global bipSelecterDialog;
	global resultArr = #();
	function travelObject pChildren &resultArr =
	(
		for cNode in pChildren where( cNode.children != undefined) do
		(
			--print cNode.name;
			appendIfUnique resultArr cNode.name;
			travelObject cNode.children &resultArr;
		)
	)

-- 	function getObjects =
-- 	(
-- 		local nodeArr = #();
-- 		for obj in Objects where(obj.parent == undefined) do
-- 		(
-- 			--append nodeArr obj.name;
-- 			travelObject obj nodeArr;
-- 		)
-- 		
-- 		nodeArr;
-- 	)
	
	try(DestroyDialog bipSelecterDialog)catch()
	rollout bipSelecterDialog "Biped Selecter"
	(
		MultiListBox mulList_objName ""
		dropdownlist comboxList_pNode "" selection:1

		on bipSelecterDialog open do
		(
			comboxList_pNode.items = for obj in Objects where(obj.parent == undefined) collect obj.name;
			
			local chArr = #();
			objName = comboxList_pNode.items[1];
			inode = getNodeByName objName;
			select inode;
			travelObject inode &chArr;
			mulList_objName.items = chArr;
		)
		
		on comboxList_pNode selected idx do
		(
			local chArr = #();
			mulList_objName.items = #();--使用前清空
			 
			objName = comboxList_pNode.items[idx];
			inode = getNodeByName objName;
			select inode;
			travelObject inode &chArr;
			mulList_objName.items = chArr;
			
			resultArr = #(); -- 清除全局变量数据
		)
		
		on mulList_objName selected idx do
		(
			selNode = getNodeByName mulList_objName.items[idx];
			selectMore selNode;
		)
		
	)CreateDialog bipSelecterDialog
)
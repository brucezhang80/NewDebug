(
	
	local multiArr =  #(#(1,3, 12),	    #(2, 5, 6, 8, 19, 20), 
						#(3, 12),	 	    #(2,6,8), 
						#(4, 16, 17), 		#(5, 6, 8, 19, 20), 
						#(6, 8, 19, 20), 	#(8, 19, 20)
				);
	
	
	function isSubSet bigArray lowArray =
	(
		local result = true;
		local tempArr = bigArray + lowArray;
		makeUniqueArray tempArr;
		
		--????????????
		--if( tempArr == bigArray ) then
		if(tempArr.count == bigArray.count ) then
		(
			-- ??????С????
			-- ??С????????
			sort tempArr;
			sort bigArray;
			
			for i = 1 to tempArr.count do
			(
				if ( (findItem bigArray i) == 0 ) then
					result = false;
			)
		)
		
		return result;
	)
	
	
	function sortByLength &multiArr =
	(
		function compareArrayFN subArr1 subArr2 =
		(
			
			local d = subArr1.count - subArr2.count;
			case of
			(
				(d < 0): 1	--?????С????
				(d > 0): -1
				default: 0
			)
		)
		
		qsort multiArr compareArrayFN;
	)
	
	-- ?????????У?????????????????????
	function removeSubArray &multiArray =
	(
		
		local cnt = multiArray.count;
		if( 0 == count ) then return false;

		local tempArray = deepCopy multiArray;
		local resultArray = #();
		
		-- ?????????鰴???????
		sortByLength &tempArray;
		format "result: %\n" tempArray;
		for big in tempArray do 
		(
			for i = tempArray.count  to 1 by -1 do
			(
				local low = tempArray[i];
				local ret = isSubSet big low;
				if( ret == true ) then
				(
					deleteItem tempArray (findItem tempArray low);
				)
			)
		)

		multiArray = #();
		multiArray = tempArray;
	)
	
	removeSubArray &multiArr;
	format "result: %\n" multiArr;

)


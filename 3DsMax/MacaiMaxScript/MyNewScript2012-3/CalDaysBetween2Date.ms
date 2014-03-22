
-- 构造日期格式 :年月日
struct dateStruct
(
	year,
	month,
	day
)

-- 计算两个日期之间的天数
struct calDaysStruct
(
	-- 判断是否是闰年
	function Yvi_isLeap year =
	(
		year = year as integer
		return ((mod year 4 == 0.0) and ( mod year 100 != 0.0) or (mod year 400 == 0.0))
	),
	
	function Yvi_witchDayInYear year month day =
	(
		local dayArr = #(0,31,28,31,30,31,30,31,31,30,31,30,31)
		year = year as integer
		month = month as integer
		day = day as integer
		
		if(myDays.Yvi_isLeap year )then
		(
			dayArr[3] = 29;
		)
		
		for i = 1 to month do
		(
			day += dayArr[i]
		)
		
		return day
	),
	
	-- 计算两个日期之间的天数.date(y,m,d)
	function Yvi_calDaysBetween2Date date1 date2 =
	(
		local d1,d2,d3=0
		local year1 = date1.year as integer, month1 = date1.month as integer,day1 = date1.day as integer
		local year2 = date2.year as integer, month2 = date2.month as integer,day2 = date2.day as integer
		-- 如果年月相同，返回day1 - day2的绝对值
		if(year1 == year2 and month1 == month2)then
		(
			--(date1.day > date2.day ? date1-date2 : date2-date1)
			return abs(day1-day2)
		)
		--如果年相同，月不同
		-- date1是该年的第几天
		-- date2是该年的第几天
		else if(year1 == year2 and month1 != month2)then
		(
			d1 = myDays.Yvi_witchDayInYear year1 month1 day1
			d2 = myDays.Yvi_witchDayInYear year2 month2 day2
			
			return abs(d1-d2)
		)
		-- 如果年份不同
		else
		(
			--确保date1年份早于date2
			if(year1 > year2)then
			(
				swap year1 year2
				swap month1 month2
				swap day1 day2
			)
			
-- 			if(myDays.Yvi_isLeap year1)then
-- 			(
-- 				d1 = 366 - myDays.Yvi_witchDayInYear year1 month1 day1
-- 			)
-- 			else
-- 			(
-- 				d1 = 365 - myDays.Yvi_witchDayInYear year1 month1 day1
-- 			)
			
-- 			if(myDays.Yvi_isLeap year1)then
-- 			(
-- 				d2 = 366 - myDays.Yvi_witchDayInYear year2 month2 day2
-- 			)
-- 			else
-- 			(
-- 				d2 = 365 - myDays.Yvi_witchDayInYear year2 month2 day2
-- 			)
			d1 = myDays.Yvi_witchDayInYear year1 month1 day1
			d2 = myDays.Yvi_witchDayInYear year2 month2 day2
			for y = year1 to year2-1 do
			(
				if(myDays.Yvi_isLeap y)then
				(
					d3 += 366
				)
				else
				(
					d3 += 365
				)
			)
			return (d1+d2+d3)
		)
	),
	

	function Yvi_removeWeekend days startWeek =
	(
		-- 日期天数与7整除，周末天数必定是2*(days/7)
		startWeek = startWeek as integer
		if(days >= 7 and (mod days 7) == 0.0)then
		(
			weekendDays = 2*(days/7)+(mod days 7) as integer
		)
		else
		(
			--周末数
			week_count = (days/7) as integer
			
			--周末剩余天数
			mod1 = (mod days 7)as integer -- 0,1,2,...6
			
			mod2 = (mod (startWeek + mod1) 7)as integer -- 0,1,2,...6
			local factor = 0
			-- 当MOD1 > 0 and  MOD2< 开始日是周几（X) 时，需要计算“修正因子（F)“.否则不用修正。修正因子（F)最大值为2
			if(mod1 > 0 and mod2 < startWeek)then -- need to modify bigest mod is 2
			(
				if(startWeek < 5)then
				(
					factor = mod2+1
					if(factor > 2)then
					(
						factor = 2
					)
				)
				else
				(
					factor = 1
				)
			)
			else -- neddn't to modify
			(
				factor = 0
			)

			format "% -- % -- % -- %\n"startWeek factor week_count mod2
			if(week_count == 0)then
			(
				weekendDays = (factor) as integer
			)
			else
				weekendDays = (2*week_count+factor) as integer
		)
		
		return weekendDays
	),
	
	-- 蔡勒（Zeller）公式
	-- W=（D + 2*M + 3*(M+1)/5 + Y + Y/4 - Y/100 + Y/400 ）MOD  7 
	-- D--日期
	-- M--月份
	-- Y--年份
	-- w = 0 为星期一，1为星期二.....6星期天
	function Yvi_getCurDayOfWeek date =
	(
		D = date.day
		M = date.month
		Y = date.year
		
		w = (mod (D + 2*M + 3*(M+1)/5 + Y + Y/4 - Y/100 + Y/400) 7) as integer
		
		return (w+1)
	)
)
myDays = calDaysStruct()


try(Destroydialog rl_CalDaysBetween2Date)catch()
rollout rl_CalDaysBetween2Date "Cal Days Between Two Date - Yvi" width:267 height:210
(
	label lb_datecpt "   选择日期：     年        /      月         /      日" align:#left
	label lb_date1 "日期1：" across:4
	DropdownList ddl_year1 "" items:(for y = 1900 to 2050 collect y as string) 
	DropdownList ddl_month1 "" items:(for m = 1 to 12 collect m as string)
	DropdownList ddl_day1 "" items:(for d = 1 to 31 collect d as string)
	--button btn_today "今天" align:#right
	
	label lb_date2 "日期2：" across:4
	DropdownList ddl_year2 "" items:(for y = 1900 to 2050 collect y as string) 
	DropdownList ddl_month2 "" items:(for m = 1 to 12 collect m as string)
	DropdownList ddl_day2 "" items:(for d = 1 to 31 collect d as string)
	
	CheckBox ckb_weekend "排除周末" across:2
	CheckBox ckb_holiday "排除假期"
	CheckBox ckb_preLeave "排除预计请假天数："across:2
	edittext edt_sickLeave "    "
	
	button btn_Runtime "计算" width:164 height:34 align:#center
	
	label lb_rmWeekend ""
	label lb_times ""
	
	on ddl_month1 selected item do
	(
		local year = ddl_year1.selected as integer
		local month = ddl_month1.items[item] as integer
		
		if( month == 2)then
		(
			if(myDays.Yvi_isLeap year)then
				ddl_day1.items = (for d = 1 to 29 collect d as string)
			else
				ddl_day1.items = (for d = 1 to 28 collect d as string)
		)
		else
		(
			if(month == 4 or month == 6 or month == 9 or month == 11)then
			(
				ddl_day1.items = (for d = 1 to 30 collect d as string)
			)
			else
			(
				ddl_day1.items = (for d = 1 to 31 collect d as string)
			)
		)
	)
	
	on ddl_year1 selected item do
	(
		local month = ddl_month1.selected as integer
		local year = ddl_year1.items[item] as integer

		if(month == 2)then-- 如果是闰年
		(
			if(myDays.Yvi_isLeap year) then 
				ddl_day1.items = (for d = 1 to 29 collect d as string)
			else
				ddl_day1.items = (for d = 1 to 28 collect d as string)
		)
		else 
		(
			if(month == 4 or month == 6 or month == 9 or month == 11)then
			(
				ddl_day1.items = (for d = 1 to 30 collect d as string)
			)
			else
			(
				ddl_day1.items = (for d = 1 to 31 collect d as string)
			)
		)
	)
	
	on ddl_month2 selected item do
	(
		local year = ddl_year2.selected as integer
		local month = ddl_month2.items[item] as integer
		
		if(month == 2)then
		(
			if(myDays.Yvi_isLeap year)then
				ddl_day2.items = (for d = 1 to 29 collect d as string)
			else
				ddl_day2.items = (for d = 1 to 28 collect d as string)
		)
		else
		(
			if(month == 4 or month == 6 or month == 9 or month == 11)then
			(
				ddl_day2.items = (for d = 1 to 30 collect d as string)
			)
			else
			(
				ddl_day2.items = (for d = 1 to 31 collect d as string)
			)
		)
	)
	on ddl_year2 selected item do
	(
		local month = ddl_month2.selected as integer
		local year = ddl_year2.items[item] as integer
		
		if(month  == 2 )then-- 如果是闰年
		(
			if(myDays.Yvi_isLeap year)then
				ddl_day2.items = (for d = 1 to 29 collect d as string)
			else
				ddl_day2.items = (for d = 1 to 28 collect d as string)
		)
		else
		(
			if(month == 4 or month == 6 or month == 9 or month == 11)then
			(
				ddl_day2.items = (for d = 1 to 30 collect d as string)
			)
			else
			(
				ddl_day2.items = (for d = 1 to 31 collect d as string)
			)
		)
	)
	
	on btn_Runtime pressed do
	(
		date1 = dateStruct year:(ddl_year1.selected as integer) month:(ddl_month1.selected as integer) day:(ddl_day1.selected as integer)
		date2 = dateStruct year:(ddl_year2.selected as integer) month:(ddl_month2.selected as integer) day:(ddl_day2.selected as integer)
		
		
		numDays = myDays.Yvi_calDaysBetween2Date date1 date2
		
		lb_rmWeekend.text = ""
		--除去周末
		if(ckb_weekend.checked)then
		(
			if(date1.year > date2.year)then
			(
				swap date1.year date2.year
				swap date1.month date2.month
				swap date1.day date2.day
			)
			weekStart = myDays.Yvi_getCurDayOfWeek date1
			weekendDays = myDays.Yvi_removeWeekend numDays weekStart
			numDays = numDays - weekendDays
			lb_rmWeekend.text = "除去周末天数：" + weekendDays as string + "（不含当天）"
		)
		-- 除去请假
		if(ckb_preLeave.checked)then
		(
			if((edt_sickLeave.text as integer) != undefined or (edt_sickLeave.text as integer) != 0)then
			(
				n = (edt_sickLeave.text as integer)
				if(n <= numDays)then
				(
					numDays = numDays - (edt_sickLeave.text as integer)
				)
				else
				(
					lb_times.text = "请假天数已超出总天数！"
					return 0
				)
			)
			else
			(
				return 0
			)
		)
		
		--除去假期
		if(ckb_holiday.checked)then
		(
			
		)
		-----///////////////////////////////////////
		format "日期 %/%/% 距 %/%/% 共有 % 天。\n" date1.year date1.month date1.day  date2.year date2.month date2.day numDays
		if date2.year == 2012 and date2.month == 12 and date2.day == 22 then
		(
			str = "今天 " +date1.year as string +"/"+ date1.month as string +"/"+ date1.day as string \
				+ " 距世界末日 " +  date2.year as string +"/"+ date2.month as string +"/"+ date2.day as string \
				+ " 还有 " + numDays as string + "天。"
		)
		else
		(
			str = "日期 " +date1.year as string +"/"+ date1.month as string +"/"+ date1.day as string \
				+ " 距 " +  date2.year as string +"/"+ date2.month as string +"/"+ date2.day as string \
				+ " 有 " + numDays as string + " 天。"
		)
		lb_times.text = str
	)
	
	on rl_CalDaysBetween2Date open do
	(
		local_time = getLocalTime() -- #(2013, 7, 4, 25, 16, 13, 25, 708) y m dw d h m s ms
		cur_time = dateStruct year:(local_time[1] as string) month:(local_time[2] as string) day:(local_time[4] as string)
		ddl_year1.selected = cur_time.year
		ddl_month1.selected = cur_time.month
		ddl_day1.selected = cur_time.day
		
		--test
		ddl_year2.selected = cur_time.year
		ddl_month2.selected = cur_time.month
		ddl_day2.selected = (cur_time.day as integer+3) as string
	)
)
CreateDialog rl_CalDaysBetween2Date

	
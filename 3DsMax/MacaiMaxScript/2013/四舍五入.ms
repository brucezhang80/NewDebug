
-- 小数四舍五入法
-- 参数1：num为输入的需要四舍五入的数值（小数）
-- 参数2：为输出结果保留多少位小数（位数不能超出小数点位数）
-- 注：由于3dsmax默认只能保留到小数点后5位，所以超出部分将被截断
function yviRound num step =
(
	result = 0.0;
	-- 如果是正数
	if ( num > 0.0 ) then
	(
		result = floor (num * (pow 10 step) + 0.5);
		result /= (pow 10 step);
	)
	-- 负数
	else
	(
		result = ceil (num * (pow 10 step) - 0.5);
		result /= (pow 10 step);
	)
	
	return result;
)

yviRound 2.55645852178 8;
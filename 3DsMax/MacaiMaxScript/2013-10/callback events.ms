
-- 作者：麻菜
-- Email:12319597@qq.com
-- 网站：www.newdebug.com
-- 版权：copyleft@ 2013, 开源，相互学习使用，请务用于商业交易。
-- 添加回调测试，尚未完善，未对回调事件做相关的事件处理。打算对所有回调事件进行
-- 一些场景操作的测试同时输出log信息的，暂时先不做。

global callbackEventsTester, eventsArr = #(), currentEvent="";
(
	-- 初始化ui界面数据
	-- 读取配置文件数据
	function yviInitializ = 
	(
		callbackEventsTester.ddl_callbacks.items = eventsArr;
	)

	-- 输出log信息
	function yviOutput str =
	(
		-- 添加一条新log前，如果当前信息列表已满20，删除旧的log
		local tempArr = callbackEventsTester.lb_output.items;
		local cnt = tempArr.count;
		if( cnt > 20 )then
		(
			deleteItem tempArr 1;
		)
		append tempArr str;
		callbackEventsTester.lb_output.items = tempArr;
		callbackEventsTester.lb_output.selection = tempArr.count;
	)
	
	-- 更新界面数据（当添加一条新的callback时）
	function yviUpdate =
	(
		callbackEventsTester.ddl_callbacks.items = eventsArr;
		callbackEventsTester.ddl_callbacks.selection = eventsArr.count;
	)
	
	-- 注册callback， event_type为可用的回调事件类型名称
	function yviRegisterEvent event_type =
	(
		event_id = (event_type as string +"0") as name;
		event_type = event_type as name;
		
		-- 由于添加回调函数没有返回值，无法确定是否添加功能，
		-- 所以只能使用try抛出的异常来判定添加的回调是否正确
		try
		(
			callbacks.addScript event_type "print(callbacks.notificationParam())" id:event_id;
			str = "Add callback evnet successful: " + event_type as string;
			yviOutput str;
-- 			currentEvent = event_type as string;
		)
		catch
		(
			-- 如果注册失败，输出到log列表
			str = "Add callback evnet failed: " + event_type as string;
			yviOutput str;
		)	
	)
	
	-- 注销回调事件
	function yviUnRegisterEvent event_type =
	(
		event_id = (event_type as string +"0") as name;
		event_type = event_type as name;
		
		callbacks.removeScripts id:event_id;
		str = "Callback event has been removed: " + event_type as string;
		yviOutput str;
	)
	
	-- 添加一个新的回调事件
	-- 弹出一个输入回调名称的对话框
	function yviAddCallbackEvent =
	(
		Rollout addEventDlg "Input a valid callback type name" width:260
		(
			EditText 	edt_name "Name: #" across:2 width:200
			Button		btn_add "Ok" offset:[40, -2]
			
			on btn_add pressed do
			(
				local _name = edt_name.text;
				if( _name == "") then
				(
					MessageBox "Invalid empty name.";
					return false;
				)
				append eventsArr _name;
				DestroyDialog addEventDlg;
				
				-- 添加成功后，更新界面数据，同时注册已经添加的新回调事件
				-- 添加新回调事件时，应该注销之前的回调，以确保当前只有一个回调事件是活动的
				-- 在这里不做处理
				yviUpdate();
				yviRegisterEvent _name;
				return true;
			)
			
		)createDialog addEventDlg;
		
		return false;
	)


	
	try(DestroyDialog callbackEventsTester)catch()
	Rollout callbackEventsTester "Callback Events Tester" width:240 height:280
	(
		DropDownList 	ddl_callbacks 	"Callback Events:" across:2 width:200
		Button 		btn_addevent 	"+" offset:[50,18]
		ListBox 		lb_output 		"Out put:" height:15
		
		-- 添加新回调事件
		on btn_addevent pressed do
		(
			r = yviAddCallbackEvent();
		)
		
		-- 从列表中选择一个回调事件并激活
		-- 同时注销之前的回调事件
		-- 将当前激活的回调保存，供以后注销。		
		on ddl_callbacks selected index do
		(
			if( currentEvent != "")do
				yviUnRegisterEvent currentEvent;
			
			event_type = ddl_callbacks.items[index];
			yviRegisterEvent event_type;
			currentEvent = event_type;
		)
		
		-- 打开窗口并初始化界面数据
		on callbackEventsTester open do
		(
			yviInitializ();
		)
		
		-- 关闭窗口注销所有回调事件
		on callbackEventsTester close do
		(
			for n in eventsArr do
				yviUnRegisterEvent n;
		)
		
	)createDialog callbackEventsTester

)
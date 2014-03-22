/*
DialogMonitorOPS：
	DialogMonitorOPS核心接口提供了一个回调系统，以监察3ds Max的用户界面,
	在结合Interface: UIAccessor  ，它可以被用于收集数据和对话框以及弹出菜单的交互
	例如，自动运行的任务 或 3ds Max用户界面中当前激活某个动作执行的数据，写入数据存盘或读取一个存盘文件数据时，
	在弹出对的话框，自动按下默认（保存，打开，确定，取消等）或指定的按钮。
	另：在Windows中，所有交互型的用户界面（控件），都是称为窗口。
	例如:应用程序的主框架窗口、对话框、编辑框、按钮、标签、单选框、复选框、图标、滚动条、输出设备、文件等都一律被称为是窗口。是窗口就会有句柄。
	句柄，是标志应用程序中的不同对象和同类对象中的不同的实例，这些实例就是一系列你所看到的“窗口”。
	
	DialogMonitorOPS 在 3ds Max 9 以上版本可用。
	
UIAccessor：
	UIAccessor Core Interface 所提供的函数，与3ds Max用户界面弹出的对话框或菜单进行交互和收集数据，
	UIAccessor提供的函数接口配合DialogMonitorOPS提供的回调函数使用，以便执行自动UI处理操作。
	
	UIAccessor 在 3ds Max 9 以上版本可用。

*/

DialogMonitorOPS.unRegisterNotification id:#fxxkTheScript  /*注销之前可能未完成注销的这个ID回调函数*/

-- 回调函数入口
function Yvi_dlgMonitorNotification = 
(
	/*句柄可以被用来作为大多用户接口（Interface: UIAccessor）的参数，
	利用 Interface: UIAccessor 方法可以使用该句柄作为参数，来收集有关当前对话框的详细信息并操作*/
	WindowHandle = DialogMonitorOPS.GetWindowHandle()/*返回当前对话框的窗口句柄*/
	WindowName = UIAccessor.GetWindowText WindowHandle/*返回给定窗口句柄的窗口标题*/
	if (WindowName != undefined) then
	(
		-- 找到这个包含有“Save UVW”或 “保存 UVW”的保存文件对话框的窗口
	    if ((findString WindowName "Save UVW") != undefined or (findString WindowName "保存 UVW")!= undefined) then
		 (
			-- 通过找到的”保存uvw文件“对话框窗口句柄，得到其所有子窗口，目的是找到输入文件名这个子窗口，用于输入文件名。
			cWindows = UIAccessor.GetChildWindows WindowHandle/* 通过给定的窗口句柄得到其所有子窗口的句柄，返回这些子窗口句柄的数组*/
			 --定义一个句柄输入模块，初始化为指向空
			hEntryField = 0 
			-- 在得到的所有子窗口数组中遍历，找出输入文件名的窗口即Combobox窗口，有可能有些是EditControl
			for h = 1 to cWindows.count do
			(
				if (cWindows[h] != 0) then
				(
					-- 这里得到ComboBox下拉框类型窗口
					if ((UIAccessor.GetWindowClassName cWindows[h]) == "ComboBoxEx32") then/*返回给定窗口句柄的窗口类名*/
					-- 找到这个保存文件对话框输入文件名的ComboBox句柄输入模块
					hEntryField = cWindows[h]
				)
			)
			-- 如果找到了输入文件名的ComboBox窗口
			if (hEntryField != 0) then
			(
				-- 在文件保存对话框中的ComboBox窗口中输入文本，即输入保存完整路径的文件
				UIAccessor.SetWindowText hEntryField "C:\\Tmp\\tmp.uvw" /*通过给定的句柄，设置这个窗口句柄的文本*/
				-- 输入完文本后点击保存按钮，注意：中文中写法是这样的。下面注释中是英文的写法。其它写法错误
				UIAccessor.PressButtonByName WindowHandle "保存(&S)"/*通过给定的父窗口句柄，按下参数二给定的子窗口名称的按钮*/
				--UIAccessor.PressButtonByName WindowHandle "&Save" -- 英文写法
				--UIAccessor.PressButtonByName WindowHandle "&保存" -- 错误写法
				--UIAccessor.PressButtonByName WindowHandle "保存" -- 错误写法
			)
		)
	)
	true --结果返回true
)

DialogMonitorOPS.RegisterNotification Yvi_dlgMonitorNotification id:#fxxkTheScript/*通过用户定义的ID来注册一个通知回调函数，这个回调函数在上面已经定义好*/
DialogMonitorOPS.Enabled = true /*设置对话框监视器启用状态*/

-- 必须先选中场景中一个物体对象，否则保存不能保存
modPanel.addModToSelection (Unwrap_UVW ()) ui:on /*将3ds max命令界面切换到“UVW展开”面板中*/
$.modifiers[#Unwrap_UVW].unwrap.save()/*将选择的场景对象拥有的 uvw 保存，等同于直接鼠标单击“UVW展开”命令界面下的"保存..."按钮*/

DialogMonitorOPS.Enabled = false /*设置对话框监视器禁用状态*/
DialogMonitorOPS.unRegisterNotification id:#fxxkTheScript /*注销这个回调函数*/
	

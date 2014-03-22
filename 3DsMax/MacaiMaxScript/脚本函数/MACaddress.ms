
--使用MD5加密法
--基本方式为，求余、取余、调整长度、与链接变量进行循环运算。得出结果
--  用 XOR, AND, OR , NOT 。
/*
  .[static]<System.Security.Cryptography.HashAlgorithm>Create()
  .[static]<System.Security.Cryptography.MD5>Create()
  .[static]<System.Security.Cryptography.HashAlgorithm>Create <System.String>hashName
  .[static]<System.Security.Cryptography.MD5>Create <System.String>algName
  .[static]<System.Boolean>Equals <System.Object>objA <System.Object>objB
  .[static]<System.Boolean>ReferenceEquals <System.Object>objA <System.Object>objB
*/



	-- 返回多个以太网物理网卡地址,不包含无线网卡
	function Yvi_getNetcarPhysicalAddress =
	(
		NWI = dotnetClass "System.Net.NetworkInformation.NetworkInterface"

		if (NWI.GetIsNetworkAvailable()) then
			the_Mac_array = NWI.GetAllNetworkInterfaces() -- this is an array of all the Networks
			
		the_PhysicalAddress_Array = #()
		for net in the_Mac_array where(net.NetworkInterfaceType.toString() == "Ethernet" ) do --"Ethernet"
		(
			--print (net.NetworkInterfaceType.toString())
			append the_PhysicalAddress_Array ((net.GetPhysicalAddress()).toString())
		)
		
		return the_PhysicalAddress_Array
	)
-------------------------------------------------
	global functions
	struct temp
	(
		val = 0 --不要更改这个
		,
		gIs = fn gIs Category Key File:"Settings" =  
		(
			local type, val, key1
 
			registry.createKey HKEY_CURRENT_USER ("Software\\STSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
			registry.queryValue key1 (Key as string) type:&type value:&val
			val
		)--END gIs FN
		,
		sIs = fn sIs Category Key Val File:"Settings" del:false = 
		(
			try
			(
				local newKeyCreated , key1
				registry.createKey HKEY_CURRENT_USER ("Software\\STSettings\\"+File as string + "\\" + Category as string) accessRights:#all newKeyCreated:&newKeyCreated key:&key1
				if not del then
				(
					registry.setvalue key1 (Key as string) #REG_SZ (Val as string)
				)
				else
				(
					registry.deleteKey key1
				)
			)
			catch
			(	
				print (registry.getLastError())	
			)
		)--END sIs FN
		,
		saveMCSettings = fn saveMCSettings MC ilimited =
		(
			local items = MC.controls
			for i in items do
			(
				--print (classOf i)  --uncommend to find out the classes of the UI items	不推荐找出UI项的类型
				case classOf i of
				(
					SpinnerControl : (functions.sIs MC.name i.name i.value)
					ColorPickerControl : (functions.sIs MC.name i.name i.color)
					CheckBoxControl : (functions.sIs MC.name i.name i.checked)
					CheckButtonControl : (functions.sIs MC.name i.name i.checked)
					RadioControl : (functions.sIs MC.name i.name i.state)
					ListBoxControl : (with printAllElements true functions.sIs MC.name i.name i.items)
					EdittextControl : (functions.sIs MC.name i.name (i.text = ilimited as string))
 
				)--END classes cases
			)--END items loop
		)--END saveMCSettings FN
		,
		getMCSettings = fn getMCSettings MC =
		(
			local items = MC.controls
			for i in items do
			(
				case classOf i of
				(
					SpinnerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer do i.value = val
					)
					ColorPickerControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == color do i.color = val
					)
					CheckBoxControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)
					CheckButtonControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == BooleanClass do i.checked = val
					)					
					RadioControl : 
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == float OR classOf val == integer  do i.state = val
					)					
					ListBoxControl :
					(
						local val = functions.gIs MC.name i.name 
						if val != undefined AND classOf val == String do val = execute val 
						if classOf val == array do i.items = val
					)
					EdittextControl :
					(
						local val = functions.gIs MC.name i.name ; if val != undefined AND classOf val == String do val = execute val
						if classOf val == integer  And (i.name == "edt_limited")do 
						(
						)
					)
				)--END classes cases
			)--END items loop
		)--END getROSettings FN
	)
	functions = temp val:1

	
	

-- 回调函数入口
function Yvi_dlgMonitorNotification = 
(
	--WindowHandle = DialogMonitorOPS.GetWindowHandle()/*返回当前对话框的窗口句柄*/
	--WindowName = UIAccessor.GetWindowText WindowHandle/*返回给定窗口句柄的窗口标题*/
	hwndArr = UIAccessor.GetPopupDialogs()

	for WindowHandle in hwndArr do
	(
		if WindowHandle == undefined then (return 0)
		WindowName = UIAccessor.GetWindowText WindowHandle

	    if ((findString WindowName "输入密码") != undefined) then
		(
			-- 通过找到的”保存uvw文件“对话框窗口句柄，得到其所有子窗口，目的是找到输入文件名这个子窗口，用输入文件名。
			chWindows = UIAccessor.GetChildWindows WindowHandle
			 --定义一个句柄输入模块，初始化为指向空
			hEntryField = 0 
			for h = 1 to chWindows.count do
			(
				if (chWindows[h] != 0) then
				(
					if ((UIAccessor.GetWindowClassName chWindows[h]) == "Edit") then/*返回给定窗口句柄的窗口类名*/
					(
						hEntryField = chWindows[h]
					)
				)
			)
			if (hEntryField != 0) then
			(
				UIAccessor.SetWindowText hEntryField "1234" /*通过给定的句柄，设置这个窗口句柄的文本*/
				UIAccessor.PressButtonByName WindowHandle "OK"
			)
			break
		)
	)
	true --结果返回true
)

DialogMonitorOPS.unRegisterNotification id:#fxxkTheScript  /*注销之前可能未完成注销的这个ID回调函数*/

DialogMonitorOPS.RegisterNotification Yvi_dlgMonitorNotification id:#fxxkTheScript/*通过用户定义的ID来注册一个通知回调函数，这个回调函数在上面已经定义好*/
DialogMonitorOPS.Enabled = true /*设置对话框监视器启用状态*/

--监视以下操作
Yvi_dlgMonitorNotification()

DialogMonitorOPS.Enabled = false /*设置对话框监视器禁用状态*/
DialogMonitorOPS.unRegisterNotification id:#fxxkTheScript /*注销这个回调函数*/
-- 代码结束



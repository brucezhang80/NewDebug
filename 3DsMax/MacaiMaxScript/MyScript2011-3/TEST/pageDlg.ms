
/*****************************************************************************
功能：测试如何向父窗口中添加子窗口（子卷轴栏窗口），相当于tabControl分页。
作者：麻菜
日期：2012-9-27
联系：http://www.newdebug.com
函数：setDlgAChbtnChecked , showSubDlg
全局变量：page0, page1,page2, page3,子窗口页对象名 mainDlg 父对话框对象名 ，theFunStruct 函数结构体对象
******************************************************************************/
global page0, page1,page2, page3, mainDlg, theFunStruct; 			/*定义全局对话框变量*/
	
struct FuncStruct
(
	/* 
	参数
		DlgId 父窗口对象ID名
		CtrlID 该父窗口对象上的所有控件对象ID名
	*/
	function setDlgAChbtnChecked parentDlgID CtrlID = 
	(
		if (parentDlgID == undefined or CtrlID == undefined ) do
		(
			return false;
		)
		for item in parentDlgID.controls do
		(
			case classOf item of
			(
			CheckButtonControl:
				(
					if ( item == CtrlID ) then
					(
						item.checked = true;
					)
					else
					(
						item.checked = false;
					)
				)
			)
		)
		
		return true;
	),
	
	/*
	参数
		parentDlg 父窗口为subRollout,  
		newSubDlg 新的子窗口为rollout 
	*/
	Function showSubDlg parentDlg newSubDlg =			
	(
		if (parentDlg == undefined or newSubDlg == undefined ) do
		(
			return false;
		)
		if( parentDlg.rollouts != newSubDlg )then 				/*当前子对话框不是新添加的子对话框*/
		(														/*用新子对话框替代当前显示的子对话框*/
			removeSubRollout parentDlg parentDlg.rollouts[1];	/*删除当前子对话框*/
			addSubRollout parentDlg newSubDlg;					/*添加新的子对话框*/
		)
		
		return true;
	)
)

/*
	声明四个rollout子页面
*/
(
	rollout page0 "page 0"					/*定义第一个子对话框*/
	(
		button btn_rl00 "btn00" across:2
		button btn_rl01 "btn01"
		button btn_rl02 "btn02" across:2
		button btn_rl03 "btn03"
	)

	rollout page1 "page 1"					/*定义第二个子对话框*/
	(
		checkBox chb_00 "check00" across:2
		checkBox chb_01 "check01"
		checkBox chb_02 "check02" across:2
		checkBox chb_03 "check03"
	)

	rollout page2 "page 2"					/*定义第二个子对话框*/
	(
		spinner spn_00 "spinner00" across:2
		spinner spn_01 "spinner01"
		spinner spn_02 "spinner02" across:2
		spinner spn_03 "spinner03"
	)
	
	rollout page3 "page 3"					/*定义第二个子对话框*/
	(
		Slider sld_00 "slider00" across:2
		Slider sld_01 "slider01"
		Slider sld_02 "slider02" across:2
		Slider sld_03 "slider03"
	)
)


-- 实例化函数结构对象
theFunStruct = FuncStruct();

-- 主窗口
--////////////////////////////////////////////////////////////
try(DestroyDialog mainDlg)catch()
rollout mainDlg "PageDlg" width:256 height:256 		/*定义父对话框*/
(
	checkbutton chbtn_000 "page0" across:4 checked:true width:56 align:#left
	checkbutton chbtn_001 "page1" width:56
	checkbutton chbtn_002 "page2" width:56
	checkbutton chbtn_003 "page3" width:56

	subrollout subDlg "subDlg"  height:128 across:1 /*在父对话框上定义一个放置子对话框的部件*/
	
	edittext edt_text "" height:56 text:"Hello,Maxscript!"
	
-- 主对话框上的按钮事件处理
	
	on chbtn_000 changed state do
	(
		theFunStruct.setDlgAChbtnChecked mainDlg chbtn_000; /*保持当前的checkbutton被按下状态, 其它的checkbutton被弹起状态*/
		if( page0 != undefined and state )do				/*如果当前的子对话框非空*/
		(
			if ( not theFunStruct.showSubDlg subDlg page0 ) then
			(
				edt_text.text = "Show the page failed!";
				return 0;
			)
			edt_text.text = "Current page is : " + page0.name;
		)
	)
	
	on chbtn_001 changed state do				/*同上*/
	(
		theFunStruct.setDlgAChbtnChecked mainDlg chbtn_001;
		
		if( page1 != undefined and state )do
		(
			if ( not theFunStruct.showSubDlg subDlg page1 ) then
			(
				edt_text.text = "Show the page failed!";
				return 0;
			)
			edt_text.text = "Current page is : " + page1.name;
		)
	)
	
	on chbtn_002 changed state do				/*同上*/
	(
		theFunStruct.setDlgAChbtnChecked mainDlg chbtn_002;
		
		if( page2 != undefined and state )do
		(
			if ( not theFunStruct.showSubDlg subDlg page2 ) then
			(
				edt_text.text = "Show the page failed!";
				return 0;
			)
			edt_text.text = "Current page is : " + page2.name;
		)
	)
	
	on chbtn_003 changed state do				/*同上*/
	(
		theFunStruct.setDlgAChbtnChecked mainDlg chbtn_003;
		
		if( page3 != undefined and state )do
		(
			if ( not theFunStruct.showSubDlg subDlg page3 ) then
			(
				edt_text.text = "Show the page failed!";
				return 0;
			)
			edt_text.text = "Current page is : " + page3.name;
		)
	)
)
createdialog mainDlg;							/*显示父对话框*/
addSubRollout mainDlg.subDlg page0;				/*默认当前显示的子对话框页面为首页page0*/

-- 结束

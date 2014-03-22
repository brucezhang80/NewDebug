
/*****************************************************************************
���ܣ���������򸸴����������Ӵ��ڣ��Ӿ��������ڣ����൱��tabControl��ҳ��
���ߣ����
���ڣ�2012-9-27
��ϵ��http://www.newdebug.com
������setDlgAChbtnChecked , showSubDlg
ȫ�ֱ�����page0, page1,page2, page3,�Ӵ���ҳ������ mainDlg ���Ի�������� ��theFunStruct �����ṹ�����
******************************************************************************/
global page0, page1,page2, page3, mainDlg, theFunStruct; 			/*����ȫ�ֶԻ������*/
	
struct FuncStruct
(
	/* 
	����
		DlgId �����ڶ���ID��
		CtrlID �ø����ڶ����ϵ����пؼ�����ID��
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
	����
		parentDlg ������ΪsubRollout,  
		newSubDlg �µ��Ӵ���Ϊrollout 
	*/
	Function showSubDlg parentDlg newSubDlg =			
	(
		if (parentDlg == undefined or newSubDlg == undefined ) do
		(
			return false;
		)
		if( parentDlg.rollouts != newSubDlg )then 				/*��ǰ�ӶԻ����������ӵ��ӶԻ���*/
		(														/*�����ӶԻ��������ǰ��ʾ���ӶԻ���*/
			removeSubRollout parentDlg parentDlg.rollouts[1];	/*ɾ����ǰ�ӶԻ���*/
			addSubRollout parentDlg newSubDlg;					/*�����µ��ӶԻ���*/
		)
		
		return true;
	)
)

/*
	�����ĸ�rollout��ҳ��
*/
(
	rollout page0 "page 0"					/*�����һ���ӶԻ���*/
	(
		button btn_rl00 "btn00" across:2
		button btn_rl01 "btn01"
		button btn_rl02 "btn02" across:2
		button btn_rl03 "btn03"
	)

	rollout page1 "page 1"					/*����ڶ����ӶԻ���*/
	(
		checkBox chb_00 "check00" across:2
		checkBox chb_01 "check01"
		checkBox chb_02 "check02" across:2
		checkBox chb_03 "check03"
	)

	rollout page2 "page 2"					/*����ڶ����ӶԻ���*/
	(
		spinner spn_00 "spinner00" across:2
		spinner spn_01 "spinner01"
		spinner spn_02 "spinner02" across:2
		spinner spn_03 "spinner03"
	)
	
	rollout page3 "page 3"					/*����ڶ����ӶԻ���*/
	(
		Slider sld_00 "slider00" across:2
		Slider sld_01 "slider01"
		Slider sld_02 "slider02" across:2
		Slider sld_03 "slider03"
	)
)


-- ʵ���������ṹ����
theFunStruct = FuncStruct();

-- ������
--////////////////////////////////////////////////////////////
try(DestroyDialog mainDlg)catch()
rollout mainDlg "PageDlg" width:256 height:256 		/*���常�Ի���*/
(
	checkbutton chbtn_000 "page0" across:4 checked:true width:56 align:#left
	checkbutton chbtn_001 "page1" width:56
	checkbutton chbtn_002 "page2" width:56
	checkbutton chbtn_003 "page3" width:56

	subrollout subDlg "subDlg"  height:128 across:1 /*�ڸ��Ի����϶���һ�������ӶԻ���Ĳ���*/
	
	edittext edt_text "" height:56 text:"Hello,Maxscript!"
	
-- ���Ի����ϵİ�ť�¼�����
	
	on chbtn_000 changed state do
	(
		theFunStruct.setDlgAChbtnChecked mainDlg chbtn_000; /*���ֵ�ǰ��checkbutton������״̬, ������checkbutton������״̬*/
		if( page0 != undefined and state )do				/*�����ǰ���ӶԻ���ǿ�*/
		(
			if ( not theFunStruct.showSubDlg subDlg page0 ) then
			(
				edt_text.text = "Show the page failed!";
				return 0;
			)
			edt_text.text = "Current page is : " + page0.name;
		)
	)
	
	on chbtn_001 changed state do				/*ͬ��*/
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
	
	on chbtn_002 changed state do				/*ͬ��*/
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
	
	on chbtn_003 changed state do				/*ͬ��*/
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
createdialog mainDlg;							/*��ʾ���Ի���*/
addSubRollout mainDlg.subDlg page0;				/*Ĭ�ϵ�ǰ��ʾ���ӶԻ���ҳ��Ϊ��ҳpage0*/

-- ����
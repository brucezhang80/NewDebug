rollout test_buttons "Testing Buttons" 
(

	--button theButton images:#("Render_24i.bmp", "Render_24a.bmp", 1, 1, 1, 1, 1) border:false
	button theButton0 width:32 height:32 across:4
	button theButton1 width:32 height:32 
	button theButton2 width:32 height:32
	button theButton3 width:32 height:32
	button theButton4 width:32 height:32 across:4
	button theButton5 width:32 height:32 
	button theButton6 width:32 height:32 
	button theButton7 width:32 height:32

	checkbox chk_toggleEnabled "�л���ť����״̬" checked:true
	
	LOCAL theButtons = 
			#(
				theButton0, 
				theButton1, 
				theButton2,
				theButton3,
				theButton4,
				theButton5,
				theButton6, 
				theButton7
			)
			
	FUNCTION Yvi_fnUpdateButtons =
	(
		FOR i = 1 TO theButtons.count DO
		(
			LOCAL theIndex = (i*2)-1
			
			IF (i > 6 )THEN
			(
				/*Render_24i.bmpλͼ��13ֻ��ͼ��,Ϊ6����ťʹ��,
				���ҵ�13����ͼ���ǹ���6��ťʹ�õ�,
				���Բ��ṩ����7����ť�Ժ��ʹ��*/
				RETURN 0 
			)
			
			theButtons[i].images = 
			#(
				"Render_24i.bmp", 
				"Render_24a.bmp", 
				13, theIndex, theIndex+1, theIndex+1, theIndex+4, true, true
			)

		) 
	)
	ON test_buttons open DO Yvi_fnUpdateButtons()

	ON chk_toggleEnabled changed state DO
		FOR i IN theButtons DO i.enabled = state
	
	
	ON theButton pressed DO 	
		messagebox "Remember: Never press unknown buttons!"
)
createDialog test_buttons 230 110

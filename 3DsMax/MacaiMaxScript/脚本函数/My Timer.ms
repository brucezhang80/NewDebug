
/*MyTimer 返回一个时间数*/
fn MyDownTimer numbers =
(
	valDown = (numbers as integer) - 1
	--if(valDown > 0)then
	--(
		return valDown
	--)
	--else return 0
)

Global DesTroy
(
	rollout Destroy "Destroy" width:200 height:30
	(
		timer clock2 "Destroy" interval:1000
		label lb_txt1 "3秒后摧毁："pos:[40,2] --width:100
		label lb_txt2 "3" pos:[120,2] color:red enabel:false
		progressBar pgb_Progress2 "进度条" value:100 pos:[9,20] width:180 height:16	color:red
		on clock2 tick do
		(
			atime=MyDownTimer lb_txt2.caption
			
			for i = 1 to 3  do
			pgb_Progress2.value = atime*30
			
			if(atime >= 0)then
			(
				lb_txt2.caption = atime as string
			)
			else 
			(
				destroy_pos = GetDialogPos Destroy
				destroyDialog Destroy
				createDialog MyTime pos:destroy_pos-- 200 30
			)
		)
	)
)
Global MyTimer
(
	rollout MyTime "My Timer" width:200 height:30
	(

		timer clock "My Timer" interval:1000
		label lb_txt1 "正在启动脚本："pos:[30,2] --width:100
		label lb_text "10" pos:[120,2]
		progressBar pgb_Progress "进度条" pos:[9,20] width:180 height:16	color:green value:0
		on MyTime open do 
		(
			
		)

		on clock tick do
		(
			myclock = MyDownTimer lb_text.text
			print ("time:"+myclock as string)
			for i = 1 to 10 do
				pgb_Progress.value = 10*i - myclock*10
			
			if(myclock > 0)then
			(
				lb_text.text = myclock as string
			)
			else
			(
				mytime_pos = GetDialogPos MyTime
				destroyDialog MyTime
				createDialog Destroy pos:mytime_pos-- 200 30
			)
		)

	)
	createDialog MyTime 200 30
)
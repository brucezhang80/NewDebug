-- 1.	�������� == ��ͼ��(�޺�׺)
-- 2.	��ά���ʵ��Ӳ��ʵ�������ʾ:
-- 3.	�Ż�����ʱ���ֶ���ѡ�� (X)
-- 4.	һ������һ����ά,��ά���ʲ�Ƕ�׶�ά����.����׼����

--------------------------------------------------------------------------------------------------------
---------��ͼ������
--������ռ���ά�Ӳ���
function Fn_collectSubMultiMats arr_mats mat = 
(
	if (classof mat  == Multimaterial) then 
	(
		for i = 1 to mat.numsubs do 
		(
			Fn_collectSubMultiMats arr_mats mat[i]
		)
	)
	--�ռ���ά�Ӳ����еĲ���
	else if (classof mat == MultiSubMaterial) then
	(
		format "���ִ��ڶ�ά�Ӳ���: %"  mat.name
		for j = 1 to mat.numsubs do
		(
			Fn_collectSubMultiMats arr_mats mat[i]
		)
		--�ռ��������ά�Ӳ��ʺ�,ɾ��֮
		--�����ά�Ӳ����ڵı�׼���ʽ��ŵ���һ����ά������
		mat = undefined
	)
	else if(classof mat == Standard)then
	(
		append arr_mats mat
	)
)
	
--�����ռ������е������Ӳ���,������ά�Ӳ����еĲ���
function Fn_collectAllSubMats = 
(
	allSubMats=#()
	for mat in scenematerials do 
	(
	   Fn_collectSubMultiMats allsubMats mat
	)
	
	return allSubMats
)

-- ��������ͼ
function Fn_handleAllSubMats allsubmats =
(
	oldfilenames = #()
	for mat in allsubmats do 
	(
		if mat.diffuseMap == undefined then 
		(
			messagebox ("�����ˣ� "+ mat.name as string +" �������û�� Diffuesmap,����!")
			continue
		)
		filepathname = mat.diffuseMap.fileName
		--��ȡ�ļ����֣�����׺
		filename = filenameFromPath filepathname
		-- �����ͼ·��
		filepath = getFilenamePath filepathname
		-- ��ȡ�ļ���׺
		ext = getfilenametype filename
		
		-- ���������������Ƿ��Ѿ���������ļ���
		index = findItem oldfilenames filename
		if (index == 0) then 
		(
			append oldfilenames filename
			-- �����޸���ͼ�����֣������ļ����֣���Ҫ��׺
			mat.diffusemap.name = getFileNameFile filename
			mat.name  = mat.diffusemap.name
		)
		try
		(
			if(mat.opacityMap != undefined)then
			(
				mat.opacityMap.filename= mat.diffusemap.filename
				mat.opacityMap.name = mat.diffusemap.name
			)
		)catch(format "non-OpacityMap...")
	)
)


try(DestroyDialog rl_No1MatName)catch()
Rollout rl_No1MatName "N.o.1_�Ӳ����޸�"
(
	Button btn_MatRename "�޸��Ӳ����Ƕ�ά��"
	
	on btn_MatRename pressed do
	(
		local allsubMats = #()
		allsubMats = Fn_collectAllSubMats()
		print allsubMats
		
		Fn_handleAllSubMats allsubMats
	)
	
)
CreateDialog rl_No1MatName
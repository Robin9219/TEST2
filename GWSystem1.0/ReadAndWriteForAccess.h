#pragma once
#include "ADOAccess.h"
#include "GWSystem1.0.h"
#include<vector>

using std::vector;



class CReadAndWriteForAccess
{
public:
	CReadAndWriteForAccess();
	~CReadAndWriteForAccess();

public:
	ADOAccess m_Conn;
	_bstr_t sql;
	_variant_t var;


public:

	bool WriteToSlideSet(Table_SlideSet *slideset);

	vector<Table_SlideSet> ReadFromSlideSet();
	// ��ѯʱ����ڵ�ͼƬ
	vector<Table_SlideSet> QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySlidesetMN();

	vector<Table_SlideSet> QuerySlidesetCHRO();

	//���ѷ����Ľ�����������ʷ��Ϣ
	vector<ChroAnalysed>ArrangeAnalysedCHRO(COleDateTime begintime, COleDateTime endtime);

	//���ѷ����Ľ�����������ʷ��Ϣ
	vector<MNAnalysed>ArrangeAnalysedMN(COleDateTime begintime, COleDateTime endtime);


	// ���ݿ��ж�ȡһ�����˵ķ������
	//��ȡһ�����˱��ε�Ⱦɫ��������  �������ɸ��˱�
	CHRO_HandleResult ReadOneResultFromAccess(CString patientname);

	// ��һ��ͼƬ�ķ�������浽���ݿ�
	void SaveOnePicResult(CHRO_HandleResultOnePic resultstruct);
	// һ�����˵ķ���������浽���ݿ�
	bool SaveToAccess(CHRO_HandleResult * pb);
	//��ȡѡ�еĲ��˵ķ������ ���������ܱ�
	vector<CHRO_HandleResult> ReadAllResultFromAccess(vector<CString>patientname);

	//��ȡһ�����˱��ε�΢�˷������  �������ɸ��˱�
	MN_HandleResult ReadOneMNResultFromAccess(CString patientname);

	// ���ݿ��ж�ȡһ�����˵�ϵͳ�������
	MN_HandleResult ReadOneMNSystemResultFromAccess(CString patientname);


	// ��һ��ͼƬ�ķ�������浽���ݿ�
	void SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct);

	// һ�����˵�΢�˷���������浽���ݿ�
	bool SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult);

	// ���ݿ���������ǰ���˵Ĵ�����
	vector<MN_HandleResultOnePic> FindCurrentPatientResult(CString patientname);

	//��ȡѡ�еĲ��˵ķ������ ���������ܱ�
	vector<MN_HandleResult> ReadAllMNResultFromAccess(vector<CString>patientname);

	// һ�����˵�΢�˷���������浽���ݿ�--����ʱ
	bool SaveToAccessMNAnalysis(MN_HandleResult * pb);

	//ͳ��˫��ϸ�����������Ƿ�ﵽ2000��
	int CountDoubleCell(CString patientname);

	//ͳ��Ⱦɫ����������Ƿ�ﵽ200��
	int CountChromosome(CString patientname);

	//�����ݿ���ͳ��һ�Ŵ�ͼ�ķ������
	bool CountResultFromBigImg(CString patientname);

	//������һ�����ˣ�����Ƭ���ñ�����Ϊ�ѷ���������������ʱ��
	bool SetFinishedCHRO(CString patientname, COleDateTime grabtime);

	//������һ�����ˣ�����Ƭ���ñ�����Ϊ�ѷ���������������ʱ��
	bool SetFinishedMN(CString patientname, COleDateTime grabtime);

	//��ѯ���ݿ��пɴ�ӡ��΢�˱���
	vector<CString>ArrangeAllowPrintMN();

	//��ѯ���ݿ��пɴ�ӡ��Ⱦɫ�屨��
	vector<CString>ArrangeAllowPrintCHRO();

	//��Ⱦɫ��Сͼ���ͳ�Ƶõ����˽�������������ݿ⣨���Σ�
	bool CountPatientResultCHRO(CString patientname, COleDateTime grabtime);

	//��ͳ�Ƴ��Ľ��д�뱾�η�������
	bool WriteIntoThisTurn(CHRO_HandleResult* OnePatient);

	//����������΢�����������ݿ���
	bool WritetheMNIntoAccess(MN_NumCheck MNCheck);

};



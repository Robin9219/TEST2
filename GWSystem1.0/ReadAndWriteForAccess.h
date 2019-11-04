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
	// 查询时间段内的图片
	vector<Table_SlideSet> QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySlidesetMN();

	vector<Table_SlideSet> QuerySlidesetCHRO();

	//从已分析的结果中整理出历史信息
	vector<ChroAnalysed>ArrangeAnalysedCHRO(COleDateTime begintime, COleDateTime endtime);

	//从已分析的结果中整理出历史信息
	vector<MNAnalysed>ArrangeAnalysedMN(COleDateTime begintime, COleDateTime endtime);


	// 数据库中读取一个病人的分析结果
	//读取一个病人本次的染色体分析结果  用于生成个人表
	CHRO_HandleResult ReadOneResultFromAccess(CString patientname);

	// 将一张图片的分析结果存到数据库
	void SaveOnePicResult(CHRO_HandleResultOnePic resultstruct);
	// 一个病人的分析结果保存到数据库
	bool SaveToAccess(CHRO_HandleResult * pb);
	//读取选中的病人的分析结果 用于生成总表
	vector<CHRO_HandleResult> ReadAllResultFromAccess(vector<CString>patientname);

	//读取一个病人本次的微核分析结果  用于生成个人表
	MN_HandleResult ReadOneMNResultFromAccess(CString patientname);

	// 数据库中读取一个病人的系统分析结果
	MN_HandleResult ReadOneMNSystemResultFromAccess(CString patientname);


	// 将一张图片的分析结果存到数据库
	void SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct);

	// 一个病人的微核分析结果保存到数据库
	bool SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult);

	// 数据库中搜索当前病人的处理结果
	vector<MN_HandleResultOnePic> FindCurrentPatientResult(CString patientname);

	//读取选中的病人的分析结果 用于生成总表
	vector<MN_HandleResult> ReadAllMNResultFromAccess(vector<CString>patientname);

	// 一个病人的微核分析结果保存到数据库--分析时
	bool SaveToAccessMNAnalysis(MN_HandleResult * pb);

	//统计双核细胞分析数量是否达到2000张
	int CountDoubleCell(CString patientname);

	//统计染色体分析数量是否达到200张
	int CountChromosome(CString patientname);

	//从数据库中统计一张大图的分析结果
	bool CountResultFromBigImg(CString patientname);

	//分析完一个病人，将拨片设置表设置为已分析，并给出分析时间
	bool SetFinishedCHRO(CString patientname, COleDateTime grabtime);

	//分析完一个病人，将拨片设置表设置为已分析，并给出分析时间
	bool SetFinishedMN(CString patientname, COleDateTime grabtime);

	//查询数据库中可打印的微核报告
	vector<CString>ArrangeAllowPrintMN();

	//查询数据库中可打印的染色体报告
	vector<CString>ArrangeAllowPrintCHRO();

	//由染色体小图结果统计得到病人结果，并存入数据库（本次）
	bool CountPatientResultCHRO(CString patientname, COleDateTime grabtime);

	//将统计出的结果写入本次分析表中
	bool WriteIntoThisTurn(CHRO_HandleResult* OnePatient);

	//将分析出的微核数存入数据库中
	bool WritetheMNIntoAccess(MN_NumCheck MNCheck);

};



#include "stdafx.h"
#include "ReadAndWriteForAccess.h"
#include<iostream>

CReadAndWriteForAccess::CReadAndWriteForAccess()
{
	m_Conn.OnInitADOAccess();
}


CReadAndWriteForAccess::~CReadAndWriteForAccess()
{
}


bool CReadAndWriteForAccess::WriteToSlideSet(Table_SlideSet * slideset)
{
	sql = _T("select  * from 玻片设置表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(_T("无法找到玻片设置表！"));
	}
	try
	{
		slideset->time = COleDateTime::GetCurrentTime();

		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("已分析图片数"), _variant_t(slideset->ImgSolvedNum));
		m_Conn.m_pRecordset->PutCollect(_T("总图片数"), _variant_t(slideset->AllImgNum));
		m_Conn.m_pRecordset->PutCollect(_T("存图时间"), _variant_t(slideset->time));
		m_Conn.m_pRecordset->PutCollect(_T("AnalysedOrNot"), _variant_t(slideset->HaveSolved));
		m_Conn.m_pRecordset->PutCollect(_T("路径"), _variant_t(slideset->path));
		m_Conn.m_pRecordset->PutCollect(_T("病人名称"), _variant_t(slideset->name));
		m_Conn.m_pRecordset->PutCollect(_T("模式"), _variant_t(slideset->mode));
		m_Conn.m_pRecordset->PutCollect(_T("扫描范围"), _variant_t(slideset->range));
		m_Conn.m_pRecordset->PutCollect(_T("结果路径"), _variant_t(slideset->resultpath));


		m_Conn.m_pRecordset->Update();

		return false;
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("玻片设置表写入错误！"));
	}

}


vector<Table_SlideSet> CReadAndWriteForAccess::ReadFromSlideSet()
{
	Table_SlideSet slideset;
	vector<Table_SlideSet>SlideSetVector;
	sql = _T("select * from 玻片设置表");
	//sql = _T("select  * from 玻片设置表 where format(存图时间, 'yyyy-mm-dd') between format(#2018 - 10 - 16#, 'yyyy-mm-dd') and format(#2019 - 6 - 6#, 'yyyy-mm-dd')");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("路径"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			slideset.name = str;			
			str = m_Conn.m_pRecordset->GetCollect(_T("结果路径"));
			slideset.resultpath = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("模式"));
			slideset.mode = str;			
			str = m_Conn.m_pRecordset->GetCollect(_T("扫描范围"));
			slideset.range = str;			
			BOOL str1 = m_Conn.m_pRecordset->GetCollect(_T("AnalysedOrNot"));
			slideset.HaveSolved = str1;
			str = m_Conn.m_pRecordset->GetCollect(_T("已分析图片数"));
			slideset.ImgSolvedNum = str;		
			str = m_Conn.m_pRecordset->GetCollect(_T("总图片数"));
			slideset.AllImgNum = str;			
			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}


// 查询时间段内的图片-微核模式
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime)
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString mystr = _T("微核");
	sql = _T("select  * from 玻片设置表 where format(存图时间, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')  and 玻片设置表.[模式] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str;
			BOOL str1 = m_Conn.m_pRecordset->GetCollect(_T("AnalysedOrNot"));
			std::cout << "str1" << str1 <<std::endl;
			if (!str1)
			{
				str = m_Conn.m_pRecordset->GetCollect(_T("路径"));
				slideset.path = str;
				str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
				slideset.name = str;
				str = m_Conn.m_pRecordset->GetCollect(_T("结果路径"));
				slideset.resultpath = str;
				COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));
				slideset.time = str2;
				SlideSetVector.push_back(slideset);

			}

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}

//查询玻片设置表
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySlidesetMN()
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString mystr = _T("微核");
	sql = _T("select  * from 玻片设置表 where 玻片设置表.[模式] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("路径"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			slideset.name = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("结果路径"));
			slideset.resultpath = str;
			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;

}

//查询玻片设置表染色体模式
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySlidesetCHRO()
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString mystr = _T("染色体");
	sql = _T("select  * from 玻片设置表 where 玻片设置表.[模式] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("路径"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			slideset.name = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("结果路径"));
			slideset.resultpath = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));

			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}

// 查询时间段内的图片-染色体模式
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime)
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString mystr = _T("染色体");
	sql = _T("select  * from 玻片设置表 where format(存图时间, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')  and 玻片设置表.[模式] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			//如果曾被分析过，则不统计
			CString str;
			BOOL str1 = m_Conn.m_pRecordset->GetCollect(_T("AnalysedOrNot"));
			if (!str1)
			{
				str = m_Conn.m_pRecordset->GetCollect(_T("路径"));
				slideset.path = str;
				str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
				slideset.name = str;
				str = m_Conn.m_pRecordset->GetCollect(_T("结果路径"));
				slideset.resultpath = str;

				COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("存图时间"));
				slideset.time = str2;
				SlideSetVector.push_back(slideset);
			}


			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}


//整理历史分析完的染色体图片
vector<ChroAnalysed> CReadAndWriteForAccess::ArrangeAnalysedCHRO(COleDateTime begintime, COleDateTime endtime)
{
	vector<ChroAnalysed>ChroAnalysedResult;
	ChroAnalysed  OnePatientResult;
	CHRO_HandleResultOnePic onepic;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString mystr = _T("染色体");
	sql = _T("select  * from 染色体图像分析结果数据表 where format(grabtime, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str;
			//获取每一行的信息
			onepic.picpath = m_Conn.m_pRecordset->GetCollect(_T("照片路径"));

			onepic.dic = m_Conn.m_pRecordset->GetCollect(_T("dic"));
			onepic.round = m_Conn.m_pRecordset->GetCollect(_T("系统判定的染色体环数"));
			onepic.ace = m_Conn.m_pRecordset->GetCollect(_T("系统判定的无着丝体数"));
			onepic.t = m_Conn.m_pRecordset->GetCollect(_T("系统判定的相互易位数"));
			onepic.inv = m_Conn.m_pRecordset->GetCollect(_T("系统判定的倒位体个数"));
			onepic.tri = m_Conn.m_pRecordset->GetCollect(_T("tri"));
			onepic.ten = m_Conn.m_pRecordset->GetCollect(_T("ten"));
			onepic.del = m_Conn.m_pRecordset->GetCollect(_T("del"));
			onepic.ctg = m_Conn.m_pRecordset->GetCollect(_T("ctg"));
			onepic.csg = m_Conn.m_pRecordset->GetCollect(_T("csg"));
			onepic.ctb = m_Conn.m_pRecordset->GetCollect(_T("ctb"));
			onepic.cte = m_Conn.m_pRecordset->GetCollect(_T("cte"));

			str = m_Conn.m_pRecordset->GetCollect(_T("CheckPath"));
			onepic.CheckPath = str;

			var = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num"));
			if (var.vt != VT_NULL)
				onepic.chromosome_num = _ttoi((_bstr_t)var);
			else
				onepic.chromosome_num = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
			if (var.vt != VT_NULL)
				onepic.QuaOrNot = _ttoi((_bstr_t)var);
			else
				onepic.QuaOrNot = -1;

			//onepic.chromosome_num = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num"));
			//onepic.QuaOrNot = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));


			var = m_Conn.m_pRecordset->GetCollect(_T("dic_check"));
			if (var.vt != VT_NULL)
				onepic.dic_rg = _ttoi((_bstr_t)var);
			else
				onepic.dic_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
			if (var.vt != VT_NULL)
				onepic.round_rg = _ttoi((_bstr_t)var);
			else
				onepic.round_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
			if (var.vt != VT_NULL)
				onepic.ace_rg = _ttoi((_bstr_t)var);
			else
				onepic.ace_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
			if (var.vt != VT_NULL)
				onepic.t_rg = _ttoi((_bstr_t)var);
			else
				onepic.t_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
			if (var.vt != VT_NULL)
				onepic.inv_rg = _ttoi((_bstr_t)var);
			else
				onepic.inv_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
			if (var.vt != VT_NULL)
				onepic.tri_rg = _ttoi((_bstr_t)var);
			else
				onepic.tri_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
			if (var.vt != VT_NULL)
				onepic.ten_rg = _ttoi((_bstr_t)var);
			else
				onepic.ten_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
			if (var.vt != VT_NULL)
				onepic.del_rg = _ttoi((_bstr_t)var);
			else
				onepic.del_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
			if (var.vt != VT_NULL)
				onepic.ctg_rg = _ttoi((_bstr_t)var);
			else
				onepic.ctg_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
			if (var.vt != VT_NULL)
				onepic.csg_rg = _ttoi((_bstr_t)var);
			else
				onepic.csg_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
			if (var.vt != VT_NULL)
				onepic.ctb_rg = _ttoi((_bstr_t)var);
			else
				onepic.ctb_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
			if (var.vt != VT_NULL)
				onepic.cte_rg = _ttoi((_bstr_t)var);
			else
				onepic.cte_rg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num_check"));
			if (var.vt != VT_NULL)
				onepic.chromosome_num_check = _ttoi((_bstr_t)var);
			else
				onepic.chromosome_num_check = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("grabtime"));
			if (var.vt != VT_NULL)
				onepic.GrabTime = var;

			//读取各个小染色体信息
			CString OneRST;
			vector<string>().swap(onepic.LastResult);
			for (size_t i = 0; i < Max_splitImgNum; i++)
			{
				CString number;
				number.Format(_T("%d"), i);
				CString rst = _T("rst") + number;

				OneRST = m_Conn.m_pRecordset->GetCollect(_variant_t(rst));
				string OneRSTstr = CStringtoString(OneRST);
				onepic.LastResult.push_back(OneRSTstr);
			}


			str = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			bool havepatientornot = FALSE;
			//1.遍历是否已经有该病人的信息
			for (size_t i = 0; i < ChroAnalysedResult.size(); i++)
			{
				if (ChroAnalysedResult[i].patientname == str)
				{
					havepatientornot = TRUE;
					ChroAnalysedResult[i].ChromosomeResult.push_back(onepic);
				}
			}
			if (!havepatientornot)//如果没有该病人
			{
				ChroAnalysed patient;
				patient.patientname = str;
				patient.ChromosomeResult.push_back(onepic);

				ChroAnalysedResult.push_back(patient);
			}




			m_Conn.m_pRecordset->MoveNext();
		}


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return ChroAnalysedResult;
}


//整理历史分析完的染色体图片
vector<MNAnalysed> CReadAndWriteForAccess::ArrangeAnalysedMN(COleDateTime begintime, COleDateTime endtime)
{
	vector<MNAnalysed>MNAnalysedResult;
	MNAnalysed  OnePatientResult;
	MN_HandleResultOnePic OneCellResult;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-dd模式
	
	sql = _T("select  * from 微核图像分析结果数据表 where format(grabtime, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString mystr;
			//获取每一行的信息

			mystr = m_Conn.m_pRecordset->GetCollect(_T("照片路径"));
			OneCellResult.picpath = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("细胞核数_校正"));
			OneCellResult.NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("微核数_校正"));
			OneCellResult.NumMicro = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("校正了NumCells"));
			OneCellResult.CheckOrNot_NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("校正了NumMicro"));
			OneCellResult.CheckOrNot_NumMicro = _ttoi(mystr);

			var = m_Conn.m_pRecordset->GetCollect(_T("grabtime"));
			if (var.vt != VT_NULL)
				OneCellResult.GrabTime = var;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			bool havepatientornot = FALSE;
			//1.遍历是否已经有该病人的信息
			for (size_t i = 0; i < MNAnalysedResult.size(); i++)
			{
				if (MNAnalysedResult[i].patientname == mystr)
				{
					havepatientornot = TRUE;
					MNAnalysedResult[i].MNResult.push_back(OneCellResult);
				}
			}
			if (!havepatientornot)//如果没有该病人
			{
				MNAnalysed patient;
				patient.patientname = mystr;
				patient.MNResult.push_back(OneCellResult);

				MNAnalysedResult.push_back(patient);
			}

			m_Conn.m_pRecordset->MoveNext();
		}


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return MNAnalysedResult;
}


//读取一个病人本次的染色体分析结果  用于生成个人表
// 数据库中读取一个病人的染色体分析结果
CHRO_HandleResult CReadAndWriteForAccess::ReadOneResultFromAccess(CString patientname)
{
	CHRO_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 染色体分析结果数据表（本次）\
			 			 		 where 染色体分析结果数据表（本次）.[所属病人或代号]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
	pA.patientname = str;

	//1
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的分析细胞总数"));
	pA.allcell = _ttoi(str);

	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的双着丝粒体数"));
	pA.dic = _ttoi(str);

	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的着丝粒环数"));
	pA.round = _ttoi(str);

	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的无着丝粒体数"));
	pA.ace = _ttoi(str);

	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的相互易位数"));
	pA.t = _ttoi(str);

	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的倒位数"));
	pA.inv = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的畸变率"));
	pA.Y = _ttof(str);

	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的正常细胞数"));
	pA.normalcell = _ttoi(str);

	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("人工校正的不正常细胞数"));
	pA.abnormal = _ttoi(str);

	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
	pA.tri = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
	pA.ten = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
	pA.del = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
	pA.ctg = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
	pA.csg = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
	pA.ctb = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
	pA.cte = _ttoi(str);

	return pA;
}

// 一张图片染色体分析结果保存到数据库
void CReadAndWriteForAccess::SaveOnePicResult(CHRO_HandleResultOnePic resultstruct)
{
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from 染色体图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(resultstruct.patientname));
		m_Conn.m_pRecordset->PutCollect(_T("照片路径"), _variant_t(resultstruct.picpath));
		m_Conn.m_pRecordset->PutCollect(_T("正常细胞数"), _variant_t(resultstruct.normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("不正常细胞数"), _variant_t(resultstruct.abnormal));
		m_Conn.m_pRecordset->PutCollect(_T("总细胞数"), _variant_t(resultstruct.allcell));
		m_Conn.m_pRecordset->PutCollect(_T("dic"), _variant_t(resultstruct.dic));
		m_Conn.m_pRecordset->PutCollect(_T("系统判定的染色体环数"), _variant_t(resultstruct.round));
		m_Conn.m_pRecordset->PutCollect(_T("系统判定的无着丝体数"), _variant_t(resultstruct.ace));
		m_Conn.m_pRecordset->PutCollect(_T("系统判定的相互易位数"), _variant_t(resultstruct.t));
		m_Conn.m_pRecordset->PutCollect(_T("系统判定的倒位体个数"), _variant_t(resultstruct.inv));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(resultstruct.tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(resultstruct.ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(resultstruct.del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(resultstruct.ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(resultstruct.csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(resultstruct.ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(resultstruct.cte));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(resultstruct.tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(resultstruct.ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(resultstruct.del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(resultstruct.ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(resultstruct.csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(resultstruct.ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(resultstruct.cte));



		m_Conn.m_pRecordset->PutCollect(_T("dic_check"), _variant_t(resultstruct.dic));
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的着丝粒环数"), _variant_t(resultstruct.round));
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的无着丝体数"), _variant_t(resultstruct.ace));
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的相互易位数"), _variant_t(resultstruct.t));
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的倒位体个数"), _variant_t(resultstruct.inv));

		m_Conn.m_pRecordset->PutCollect(_T("CheckOrNot"), _variant_t(resultstruct.CheckOrNot));
		m_Conn.m_pRecordset->PutCollect(_T("chromosome_num"), _variant_t(resultstruct.chromosome_num));
		m_Conn.m_pRecordset->PutCollect(_T("chromosome_num_check"), _variant_t(resultstruct.chromosome_num));
		m_Conn.m_pRecordset->PutCollect(_T("QuaOrNot"), _variant_t(resultstruct.QuaOrNot));
		m_Conn.m_pRecordset->PutCollect(_T("CheckPath"), _variant_t(resultstruct.CheckPath));

		m_Conn.m_pRecordset->PutCollect(_T("grabtime"), _variant_t(resultstruct.GrabTime));


		//保存各个小染色体信息
		vector<CString> RST;
		for (size_t i = 0; i < Max_splitImgNum; i++)
		{
			CString number;
			number.Format(_T("%d"), i);
			CString rst = _T("rst") + number;
			USES_CONVERSION; 
			CString str(resultstruct.LastResult[i].c_str());
			if (!resultstruct.LastResult[i].empty())
			{
				m_Conn.m_pRecordset->PutCollect(_variant_t(rst), _variant_t(str));
			}
		}
		


		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

// 一个病人染色体分析结果保存到数据库
bool CReadAndWriteForAccess::SaveToAccess(CHRO_HandleResult * pb)
{
	_bstr_t sql;
	_variant_t var;

	////填入历史数据
	//sql = _T("select  * from 染色体分析结果数据表（历史所有）");
	//try
	//{
	//	m_Conn.GetRecordSet(sql);
	//}
	//catch (_com_error *e)
	//{
	//	AfxMessageBox(e->ErrorMessage());
	//}
	//try
	//{
	//	//将对应的玻片编号和照片序数存入数组，用于报表生成
	//	m_Conn.m_pRecordset->AddNew();
	//	m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(pb->patientname));
	//	m_Conn.m_pRecordset->PutCollect(_T("分析细胞总数"), _variant_t(pb->allcell));
	//	m_Conn.m_pRecordset->PutCollect(_T("照片总数"), _variant_t(pb->picturesum));
	//	m_Conn.m_pRecordset->PutCollect(_T("双着丝粒体数"), _variant_t(pb->dic));
	//	m_Conn.m_pRecordset->PutCollect(_T("着丝粒环数"), _variant_t(pb->round));
	//	m_Conn.m_pRecordset->PutCollect(_T("无着丝粒体数"), _variant_t(pb->ace));
	//	m_Conn.m_pRecordset->PutCollect(_T("相互易位数"), _variant_t(pb->t));
	//	m_Conn.m_pRecordset->PutCollect(_T("倒位数"), _variant_t(pb->inv));
	//	m_Conn.m_pRecordset->PutCollect(_T("畸变率"), _variant_t(pb->Y));
	//	m_Conn.m_pRecordset->PutCollect(_T("图片处理文件夹"), _variant_t(pb->sourcefile));
	//	m_Conn.m_pRecordset->PutCollect(_T("结果存储文件夹"), _variant_t(pb->resultfile));

	//	m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(pb->tri));
	//	m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(pb->ten));
	//	m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(pb->del));
	//	m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(pb->ctg));
	//	m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(pb->csg));
	//	m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(pb->ctb));
	//	m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(pb->cte));


	//	m_Conn.m_pRecordset->Update();
	//	//m_Conn.ExitConnect();
	//}
	//catch (_com_error e)
	//{
	//	AfxMessageBox(e.Description());
	//}

	//填入本次数据
	sql = _T("select  * from 染色体分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(pb->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("分析细胞总数"), _variant_t(pb->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("照片总数"), _variant_t(pb->picturesum));
		m_Conn.m_pRecordset->PutCollect(_T("双着丝粒体数"), _variant_t(pb->dic));
		m_Conn.m_pRecordset->PutCollect(_T("着丝粒环数"), _variant_t(pb->round));
		m_Conn.m_pRecordset->PutCollect(_T("无着丝粒体数"), _variant_t(pb->ace));
		m_Conn.m_pRecordset->PutCollect(_T("相互易位数"), _variant_t(pb->t));
		m_Conn.m_pRecordset->PutCollect(_T("倒位数"), _variant_t(pb->inv));
		m_Conn.m_pRecordset->PutCollect(_T("畸变率"), _variant_t(pb->Y));
		m_Conn.m_pRecordset->PutCollect(_T("图片处理文件夹"), _variant_t(pb->sourcefile));
		m_Conn.m_pRecordset->PutCollect(_T("结果存储文件夹"), _variant_t(pb->resultfile));
		m_Conn.m_pRecordset->PutCollect(_T("正常细胞数"), _variant_t(pb->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("不正常细胞数"), _variant_t(pb->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("人工校正的分析细胞总数"), _variant_t(pb->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的双着丝粒体数"), _variant_t(pb->dic));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的着丝粒环数"), _variant_t(pb->round));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的无着丝粒体数"), _variant_t(pb->ace));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的相互易位数"), _variant_t(pb->t));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的倒位数"), _variant_t(pb->inv));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的畸变率"), _variant_t(pb->Y));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的正常细胞数"), _variant_t(pb->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的不正常细胞数"), _variant_t(pb->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(pb->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(pb->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(pb->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(pb->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(pb->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(pb->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(pb->cte));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(pb->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(pb->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(pb->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(pb->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(pb->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(pb->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(pb->cte));


		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;
}

//读取选中的所有病人的染色体分析结果 用于生成总表
vector<CHRO_HandleResult> CReadAndWriteForAccess::ReadAllResultFromAccess(vector<CString>patientname)
{
	vector<CHRO_HandleResult>AllChosePatient;
	CHRO_HandleResult OneChosePatient;
	_bstr_t sql;
	_variant_t var;
	CHRO_HandleResult amendpA;
	sql = _T("select * from 染色体分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			for (size_t i = 0; i < patientname.size(); i++)
			{
				if (mystr == patientname[i])
				{
					mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
					OneChosePatient.patientname = mystr;

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的分析细胞总数"));
					OneChosePatient.allcell = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的双着丝粒体数"));
					OneChosePatient.dic = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的着丝粒环数"));
					OneChosePatient.round = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的无着丝粒体数"));
					OneChosePatient.ace = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的相互易位数"));
					OneChosePatient.t = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的倒位数"));
					OneChosePatient.inv = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的畸变率"));
					OneChosePatient.Y = _ttof(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的正常细胞数"));
					OneChosePatient.normalcell = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校正的不正常细胞数"));
					OneChosePatient.abnormal = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
					OneChosePatient.tri = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
					OneChosePatient.ten = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
					OneChosePatient.del = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
					OneChosePatient.ctg = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
					OneChosePatient.csg = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
					OneChosePatient.ctb = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
					OneChosePatient.cte = _ttoi(mystr);

					AllChosePatient.push_back(OneChosePatient);
				}
			}
			m_Conn.m_pRecordset->MoveNext();
		}
		return AllChosePatient;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}


// 将一张图片的分析结果存到数据库
void CReadAndWriteForAccess::SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct)
{
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from 微核图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("病人名称"), _variant_t(resultstruct->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("照片路径"), _variant_t(resultstruct->picpath));
		m_Conn.m_pRecordset->PutCollect(_T("细胞核数"), _variant_t(resultstruct->NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("微核数"), _variant_t(resultstruct->NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("细胞核数_校正"), _variant_t(resultstruct->NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("微核数_校正"), _variant_t(resultstruct->NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("校正了NumCells"), _variant_t(resultstruct->CheckOrNot_NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("校正了NumMicro"), _variant_t(resultstruct->CheckOrNot_NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("grabtime"), _variant_t(resultstruct->GrabTime));

		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


}

// 一个病人的微核分析结果保存到数据库--校正时
bool CReadAndWriteForAccess::SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult)
{
	_bstr_t sql;
	_variant_t var;

	//填入历史数据
	sql = _T("select * from 微核分析结果数据表（历史所有）\
			 			 			 		 where 微核分析结果数据表（历史所有）.[所属病人或代号]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成

			//m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(patientname));
			////m_Conn.m_pRecordset->PutCollect(_T("分析细胞总数"), _variant_t(pb->sumcell));
			//m_Conn.m_pRecordset->PutCollect(_T("总微核数_校正"), _variant_t(pb->sumwh));
			m_Conn.m_pRecordset->PutCollect(_T("单核细胞数_校正"), _variant_t(pb->singlecell));
			m_Conn.m_pRecordset->PutCollect(_T("双核细胞数_校正"), _variant_t(pb->doublecell));
			m_Conn.m_pRecordset->PutCollect(_T("多核细胞数_校正"), _variant_t(pb->multiplecell));
			m_Conn.m_pRecordset->PutCollect(_T("单核MN_校正"), _variant_t(pb->singleMN));
			m_Conn.m_pRecordset->PutCollect(_T("单核MNC_校正"), _variant_t(pb->SingleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("单核微核细胞率_校正"), _variant_t(pb->SingleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("单核微核率_校正"), _variant_t(pb->SingleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核MN_校正"), _variant_t(pb->DoubleMN));
			m_Conn.m_pRecordset->PutCollect(_T("双核微核细胞率_校正"), _variant_t(pb->DoubleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核微核率_校正"), _variant_t(pb->DoubleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核MNC_校正"), _variant_t(pb->DoubleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("多核MNC_校正"), _variant_t(pb->multiplecell_wh));
			m_Conn.m_pRecordset->PutCollect(_T("多核MN_校正"), _variant_t(pb->MultiMN));
			//m_Conn.m_pRecordset->PutCollect(_T("多核微核细胞率"), _variant_t(pb->whcellrate));
			//m_Conn.m_pRecordset->PutCollect(_T("多核微核率"), _variant_t(pb->whcellrate));
			m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[6]
				+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

			m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[6] +
				pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

			m_Conn.m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
	}

	//填入本次数据
	sql = _T("select * from 微核分析结果数据表（本次）\
			 			 			 			 		 where 微核分析结果数据表（本次）.[所属病人或代号]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
			//m_Conn.m_pRecordset->PutCollect(_T("总微核数_校正"), _variant_t(pb->sumwh));
			m_Conn.m_pRecordset->PutCollect(_T("单核细胞数_校正"), _variant_t(pb->singlecell));
			m_Conn.m_pRecordset->PutCollect(_T("双核细胞数_校正"), _variant_t(pb->doublecell));
			m_Conn.m_pRecordset->PutCollect(_T("多核细胞数_校正"), _variant_t(pb->multiplecell));
			m_Conn.m_pRecordset->PutCollect(_T("单核MN_校正"), _variant_t(pb->singleMN));
			m_Conn.m_pRecordset->PutCollect(_T("单核MNC_校正"), _variant_t(pb->SingleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("单核微核细胞率_校正"), _variant_t(pb->SingleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("单核微核率_校正"), _variant_t(pb->SingleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核MN_校正"), _variant_t(pb->DoubleMN));
			m_Conn.m_pRecordset->PutCollect(_T("双核微核细胞率_校正"), _variant_t(pb->DoubleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核微核率_校正"), _variant_t(pb->DoubleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("双核MNC_校正"), _variant_t(pb->DoubleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("多核MNC_校正"), _variant_t(pb->multiplecell_wh));
			m_Conn.m_pRecordset->PutCollect(_T("多核MN_校正"), _variant_t(pb->MultiMN));
			//m_Conn.m_pRecordset->PutCollect(_T("多核微核细胞率"), _variant_t(pb->whcellrate));
			//m_Conn.m_pRecordset->PutCollect(_T("多核微核率"), _variant_t(pb->whcellrate));
			m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[6]
				+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

			m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

			m_Conn.m_pRecordset->Update();


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	sql = _T("select * from 微核图像分析结果数据表\
			 			 			 			 where 微核图像分析结果数据表.[照片路径]='" + onepicresult.picpath + "' ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		m_Conn.m_pRecordset->PutCollect(_T("细胞核数_校正"), _variant_t(onepicresult.NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("微核数_校正"), _variant_t(onepicresult.NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("校正了NumCells"), _variant_t(onepicresult.CheckOrNot_NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("校正了NumMicro"), _variant_t(onepicresult.CheckOrNot_NumMicro));

		m_Conn.m_pRecordset->Update();


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


	return false;
}

// 数据库中读取一个病人的人工校正结果
MN_HandleResult CReadAndWriteForAccess::ReadOneMNResultFromAccess(CString patientname)
{
	MN_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 微核分析结果数据表（本次）\
			 			 			 		 where 微核分析结果数据表（本次）.[所属病人或代号]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
	pA.patientname = str;


	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("单核细胞数_校正"));
	pA.singlecell = _ttoi(str);


	//3
	str = m_Conn.m_pRecordset->GetCollect(_T("双核细胞数_校正"));
	pA.doublecell = _ttoi(str);


	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("多核细胞数_校正"));
	pA.multiplecell = _ttoi(str);


	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("单核MN_校正"));
	pA.SingleMN_Num = _ttoi(str);


	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("单核微核率_校正"));
	pA.SingleMN_Rate = _ttof(str);


	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("单核MNC_校正"));
	pA.SingleMNC_Num = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("单核微核细胞率_校正"));
	pA.SingleMNC_Rate = _ttof(str);


	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("双核MNC_校正"));
	pA.DoubleMNC_Num = _ttoi(str);


	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("双核微核细胞率_校正"));
	pA.DoubleMNC_Rate = _ttof(str);


	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("双核MN_校正"));
	pA.DoubleMN_Num = _ttoi(str);


	//12
	str = m_Conn.m_pRecordset->GetCollect(_T("双核微核率_校正"));
	pA.DoubleMN_Rate = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("多核MN_校正"));
	pA.MultiMN = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("多核MNC_校正"));
	pA.multiplecell_wh = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("1MN in doublecell_校正"));
	pA.DoubleCellsWithMN[1] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("2MN in doublecell_校正"));
	pA.DoubleCellsWithMN[2] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("3MN in doublecell_校正"));
	pA.DoubleCellsWithMN[3] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("4MN in doublecell_校正"));
	pA.DoubleCellsWithMN[4] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("5MN in doublecell_校正"));
	pA.DoubleCellsWithMN[5] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T(">5MN in doublecell_校正"));
	pA.DoubleCellsWithMN[6] = _ttof(str);

	return pA;
}

// 数据库中读取一个病人的系统分析结果
MN_HandleResult CReadAndWriteForAccess::ReadOneMNSystemResultFromAccess(CString patientname)
{
	MN_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 微核分析结果数据表（本次）\
			 			 			 			 		 where 微核分析结果数据表（本次）.[所属病人或代号]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
	pA.patientname = str;


	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("单核细胞数"));
	pA.singlecell = _ttoi(str);


	//3
	str = m_Conn.m_pRecordset->GetCollect(_T("双核细胞数"));
	pA.doublecell = _ttoi(str);


	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("多核细胞数"));
	pA.multiplecell = _ttoi(str);


	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("单核MN"));
	pA.SingleMN_Num = _ttoi(str);


	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("单核微核率"));
	pA.SingleMN_Rate = _ttof(str);


	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("单核MNC"));
	pA.SingleMNC_Num = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("单核微核细胞率"));
	pA.SingleMNC_Rate = _ttof(str);


	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("双核MNC"));
	pA.DoubleMNC_Num = _ttoi(str);


	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("双核微核细胞率"));
	pA.DoubleMNC_Rate = _ttof(str);


	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("双核MN"));
	pA.DoubleMN_Num = _ttof(str);


	//12
	str = m_Conn.m_pRecordset->GetCollect(_T("双核微核率"));
	pA.DoubleMN_Rate = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("多核MN"));
	pA.MultiMN = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("多核MNC"));
	pA.multiplecell_wh = _ttof(str);


	return pA;
}

// 数据库中搜索当前病人的处理结果
vector<MN_HandleResultOnePic> CReadAndWriteForAccess::FindCurrentPatientResult(CString patientname)
{
	vector<MN_HandleResultOnePic>CurrentPatientResult;
	MN_HandleResultOnePic OneCellResult;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 微核图像分析结果数据表\
			 			 	   where 微核图像分析结果数据表.[病人名称]='" + patientname + "' ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{

			mystr = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			OneCellResult.patientname = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("照片路径"));
			OneCellResult.picpath = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("细胞核数_校正"));
			OneCellResult.NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("微核数_校正"));
			OneCellResult.NumMicro = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("校正了NumCells"));
			OneCellResult.CheckOrNot_NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("校正了NumMicro"));
			OneCellResult.CheckOrNot_NumMicro = _ttoi(mystr);

			CurrentPatientResult.push_back(OneCellResult);
			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return CurrentPatientResult;
}


//读取选中的病人的微核分析结果 用于生成总表
vector<MN_HandleResult> CReadAndWriteForAccess::ReadAllMNResultFromAccess(vector<CString>patientname)
{
	vector<MN_HandleResult>AllChosePatient;
	MN_HandleResult OneChosePatient;
	_bstr_t sql;
	_variant_t var;
	MN_HandleResult amendpA;
	sql = _T("select * from 微核分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			OneChosePatient.patientname = mystr;

			//看此病人是否在被添加的行列里
			for (size_t i = 0; i < patientname.size(); i++)
			{
				if (mystr == patientname[i])
				{
					mystr = m_Conn.m_pRecordset->GetCollect(_T("单核细胞数_校正"));
					OneChosePatient.singlecell = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("双核细胞数_校正"));
					OneChosePatient.doublecell = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("单核MN_校正"));
					OneChosePatient.singleMN = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("单核MNC_校正"));
					OneChosePatient.SingleMNC_Num = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("单核微核细胞率_校正"));
					OneChosePatient.SingleMNC_Rate = _ttof(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("单核微核率_校正"));
					OneChosePatient.SingleMN_Rate = _ttof(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("双核MN_校正"));
					OneChosePatient.DoubleMN = _ttoi(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("双核微核细胞率_校正"));
					OneChosePatient.DoubleMNC_Rate = _ttof(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("双核微核率_校正"));
					OneChosePatient.DoubleMN_Rate = _ttof(mystr);

					mystr = m_Conn.m_pRecordset->GetCollect(_T("双核MNC_校正"));
					OneChosePatient.DoubleMNC_Num = _ttoi(mystr);

					AllChosePatient.push_back(OneChosePatient);

				}
			}



			m_Conn.m_pRecordset->MoveNext();
		}
		return AllChosePatient;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}


// 一个病人的微核分析结果保存到数据库--分析时
bool CReadAndWriteForAccess::SaveToAccessMNAnalysis(MN_HandleResult * pb)
{
	_bstr_t sql;
	_variant_t var;



	//填入本次数据
	sql = _T("select  * from 微核分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(pb->patientname));
		//m_Conn.m_pRecordset->PutCollect(_T("分析细胞总数"), _variant_t(pb->sumcell));
		//m_Conn.m_pRecordset->PutCollect(_T("总微核数"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("单核细胞数"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("双核细胞数"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("多核细胞数"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("单核MN"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("单核MNC"), _variant_t(pb->singlecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("单核微核细胞率"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("单核微核率"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核MN"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("双核微核细胞率"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核微核率"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核MNC"), _variant_t(pb->doublecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("多核MNC"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("多核MN"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("多核微核细胞率"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("多核微核率"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));


		//m_Conn.m_pRecordset->PutCollect(_T("总微核数_校正"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("单核细胞数_校正"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("双核细胞数_校正"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("多核细胞数_校正"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("单核MN_校正"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("单核MNC_校正"), _variant_t(pb->singlecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("单核微核细胞率_校正"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("单核微核率_校正"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核MN_校正"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("双核微核细胞率_校正"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核微核率_校正"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("双核MNC_校正"), _variant_t(pb->doublecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("多核MNC_校正"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("多核MN_校正"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("多核微核细胞率"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("多核微核率"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_校正"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_校正"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;
}

////分析完毕，将玻片信息置为已分析
//bool CReadAndWriteForAccess::WriteToSlideSet(Table_SlideSet slideset)
//{
//	sql = _T("select  * from 玻片设置表");
//	try
//	{
//		m_Conn.GetRecordSet(sql);
//	}
//	catch (_com_error *e)
//	{
//		AfxMessageBox(_T("无法找到玻片设置表！"));
//	}
//	try
//	{
//		slideset.time = COleDateTime::GetCurrentTime();
//
//		m_Conn.m_pRecordset->PutCollect(_T("AnalysedOrNot"), _variant_t(slideset.HaveSolved));
//
//
//
//		m_Conn.m_pRecordset->Update();
//
//		return false;
//	}
//	catch (_com_error e)
//	{
//		AfxMessageBox(_T("玻片设置表写入错误！"));
//	}
//
//}

//统计双核细胞分析数量是否达到2000张
int CReadAndWriteForAccess::CountDoubleCell(CString patientname)
{
	_bstr_t sql;
	_variant_t var;
	int imgnum = 0;
	sql = _T("select * from 微核图像分析结果数据表\
			 			 			 			 			 	 where 微核图像分析结果数据表.[病人名称]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			imgnum++;		
			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
	}
	return imgnum;
}

//统计染色体分析数量是否达到200张
int CReadAndWriteForAccess::CountChromosome(CString patientname)
{
	_bstr_t sql;
	_variant_t var;
	int imgnum = 0;
	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 			  where 染色体图像分析结果数据表.[所属病人或代号]='" + patientname + "' ");
	
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		m_Conn.m_pRecordset->MoveFirst();

		while (!m_Conn.m_pRecordset->adoEOF)
		{
			imgnum++;
			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		/*AfxMessageBox(e.Description());*/
	}
	return imgnum;
}

//从数据库中统计一个病人每张大图的分析结果
bool CReadAndWriteForAccess::CountResultFromBigImg(CString patientname)
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 			 			  where 染色体图像分析结果数据表.[所属病人或代号]='" + patientname + "' ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{

		m_Conn.m_pRecordset->MoveFirst();

		while (!m_Conn.m_pRecordset->adoEOF)
		{
			//统计每一行中的双着畸变数量
			CString mystr;
			string str;
			int AbNum = 0;
			for (size_t i = 0; i < Max_splitImgNum; i++)
			{
				CString number;
				number.Format(_T("%d"), i);
				CString rst = _T("rst") +number;
				mystr = m_Conn.m_pRecordset->GetCollect((_variant_t)rst);
				str = CStringtoString(mystr);

				//发现#，表示搜索结束
				if (str.find("#") !=string::npos) break;
				int n = str.find("@");
				str = str.substr(n +1, string::npos);
				std::cout << "str: " << str << std::endl;
				if (str == "0")//"0"是双着
				{
					AbNum++;
				}
			}
			std::cout << AbNum << "\n";
			//将统计出的畸变数存入数据库
			m_Conn.m_pRecordset->PutCollect(_T("dic"), _variant_t(AbNum));
			m_Conn.m_pRecordset->PutCollect(_T("dic_check"), _variant_t(AbNum));


			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}


//分析完一个病人，将拨片设置表设置为已分析，并给出分析时间
bool CReadAndWriteForAccess::SetFinishedCHRO(CString patientname, COleDateTime grabtime)
{
	COleDateTime time;
	time = COleDateTime::GetCurrentTime();
	CString strDate = grabtime.Format(_T("%Y-%m-%d %H:%M:%S"));//yyyy-mm-dd模式
	CString mystr = _T("染色体");
	sql = _T("select  * from 玻片设置表 where format(存图时间, 'yyyy-mm-dd HH:mm:ss') = format(#" + strDate + "#, 'yyyy-mm-dd HH:mm:ss')  and 玻片设置表.[模式] = '" + mystr + "' and 玻片设置表.[病人名称]= '" + patientname + "'");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_Conn.m_pRecordset->PutCollect(_T("AnalysedOrNot"), _variant_t(TRUE));
			m_Conn.m_pRecordset->PutCollect(_T("分析时间"), _variant_t(time));

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


}

//分析完一个病人，将拨片设置表设置为已分析，并给出分析时间
bool CReadAndWriteForAccess::SetFinishedMN(CString patientname, COleDateTime grabtime)
{
	COleDateTime time;
	time = COleDateTime::GetCurrentTime();
	CString strDate = grabtime.Format(_T("%Y-%m-%d %H:%M:%S"));//yyyy-mm-dd模式
	CString mystr = _T("微核");
	sql = _T("select  * from 玻片设置表 where format(存图时间, 'yyyy-mm-dd HH:mm:ss') = format(#" + strDate + "#, 'yyyy-mm-dd HH:mm:ss')  and 玻片设置表.[模式] = '" + mystr + "' and 玻片设置表.[病人名称]= '" + patientname + "'");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_Conn.m_pRecordset->PutCollect(_T("AnalysedOrNot"), _variant_t(TRUE));
			m_Conn.m_pRecordset->PutCollect(_T("分析时间"), _variant_t(time));

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


}

//查询数据库中可打印的微核报告
vector<CString>CReadAndWriteForAccess::ArrangeAllowPrintMN()
{
	vector<CString>PatientPrintMN;
	CString mystr;
	sql = _T("select * from 微核分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();

		while (!m_Conn.m_pRecordset->adoEOF)
		{
			mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			PatientPrintMN.push_back(mystr);

			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return PatientPrintMN;
}


//查询数据库中可打印的染色体报告
vector<CString>CReadAndWriteForAccess::ArrangeAllowPrintCHRO()
{
	vector<CString>PatientPrintCHRO;
	CString mystr;
	sql = _T("select * from 染色体分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();

		while (!m_Conn.m_pRecordset->adoEOF)
		{
			mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			PatientPrintCHRO.push_back(mystr);

			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return PatientPrintCHRO;
}
//由染色体小图结果统计得到病人结果，并存入数据库（本次）
bool CReadAndWriteForAccess::CountPatientResultCHRO(CString patientname, COleDateTime grabtime)
{
	CHRO_HandleResult* OnePatient = new CHRO_HandleResult;


	_bstr_t sql;
	_variant_t var;
	CString mystr;
	CString strDate = grabtime.Format(_T("%Y-%m-%d %H:%M:%S"));//yyyy-mm-dd模式

	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 			 where 染色体图像分析结果数据表.[所属病人或代号]='" + patientname + "' and format(grabtime, 'yyyy-mm-dd HH:mm:ss') = format(#" + strDate + "#, 'yyyy-mm-dd HH:mm:ss') ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			//CHRO_HandleResult OneCell;
			int DistortionOneCell = 0;
			var = m_Conn.m_pRecordset->GetCollect(_T("dic_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->dic += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->dic = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
			if (var.vt != VT_NULL)
			{
				OnePatient->round += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->round = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
			if (var.vt != VT_NULL)
			{
				OnePatient->ace += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->ace = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
			if (var.vt != VT_NULL)
			{
				OnePatient->t += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->t = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
			if (var.vt != VT_NULL)
			{
				OnePatient->inv += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->inv = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->tri += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->tri = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->ten += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->ten = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->del += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->del = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->ctg += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->ctg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->csg += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->csg = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->ctb += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->ctb = 0;

			var = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
			if (var.vt != VT_NULL)
			{
				OnePatient->cte += _ttoi((_bstr_t)var);
				DistortionOneCell += _ttoi((_bstr_t)var);
			}
			else
				OnePatient->cte = 0;

			if (DistortionOneCell > 0)
				OnePatient->abnormal++;
			OnePatient->allcell++;




			m_Conn.m_pRecordset->MoveNext();
		}

		OnePatient->DistortionNum = OnePatient->dic + OnePatient->round + OnePatient->ace + OnePatient->t + OnePatient->inv
			+ OnePatient->csg + OnePatient->ctb + OnePatient->cte + OnePatient->ctg + OnePatient->tri + OnePatient->del;

		//计算畸变率和畸变细胞率
		float Distort_Rate = 0;
		float DistortCell_Rate = 0;

		OnePatient->Y = (float)OnePatient->DistortionNum / OnePatient->allcell;
		OnePatient->DistortionCellRate = (float)OnePatient->abnormal / OnePatient->allcell;
		OnePatient->patientname = patientname;
		//将统计出的结果写入本次分析表中
		WriteIntoThisTurn(OnePatient);



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;

}

bool CReadAndWriteForAccess::WriteIntoThisTurn(CHRO_HandleResult* OnePatient)
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("select  * from 染色体分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}



	//将对应的玻片编号和照片序数存入数组，用于报表生成
	try
	{
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("所属病人或代号"), _variant_t(OnePatient->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("分析细胞总数"), _variant_t(OnePatient->allcell));

		m_Conn.m_pRecordset->PutCollect(_T("双着丝粒体数"), _variant_t(OnePatient->dic));
		m_Conn.m_pRecordset->PutCollect(_T("着丝粒环数"), _variant_t(OnePatient->round));
		m_Conn.m_pRecordset->PutCollect(_T("无着丝粒体数"), _variant_t(OnePatient->ace));
		m_Conn.m_pRecordset->PutCollect(_T("相互易位数"), _variant_t(OnePatient->t));
		m_Conn.m_pRecordset->PutCollect(_T("倒位数"), _variant_t(OnePatient->inv));
		m_Conn.m_pRecordset->PutCollect(_T("畸变率"), _variant_t(OnePatient->Y));
		m_Conn.m_pRecordset->PutCollect(_T("图片处理文件夹"), _variant_t(OnePatient->sourcefile));
		m_Conn.m_pRecordset->PutCollect(_T("结果存储文件夹"), _variant_t(OnePatient->resultfile));
		m_Conn.m_pRecordset->PutCollect(_T("正常细胞数"), _variant_t(OnePatient->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("不正常细胞数"), _variant_t(OnePatient->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("人工校正的分析细胞总数"), _variant_t(OnePatient->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的双着丝粒体数"), _variant_t(OnePatient->dic));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的着丝粒环数"), _variant_t(OnePatient->round));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的无着丝粒体数"), _variant_t(OnePatient->ace));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的相互易位数"), _variant_t(OnePatient->t));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的倒位数"), _variant_t(OnePatient->inv));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的畸变率"), _variant_t(OnePatient->Y));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的正常细胞数"), _variant_t(OnePatient->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的不正常细胞数"), _variant_t(OnePatient->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("distortedcellrate"), _variant_t(OnePatient->DistortionCellRate));
		m_Conn.m_pRecordset->PutCollect(_T("distortedcellrate_check"), _variant_t(OnePatient->DistortionCellRate));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(OnePatient->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(OnePatient->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(OnePatient->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(OnePatient->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(OnePatient->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(OnePatient->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(OnePatient->cte));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(OnePatient->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(OnePatient->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(OnePatient->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(OnePatient->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(OnePatient->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(OnePatient->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(OnePatient->cte));

		m_Conn.m_pRecordset->Update();
	}

	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

}

//将分析出的微核数存入数据库中
bool CReadAndWriteForAccess::WritetheMNIntoAccess(MN_NumCheck MNCheck)
{
	_bstr_t sql;
	_variant_t var;
	CString CellNum = _T("2");
	CString mystr;
	string mystring;
	//sql = _T("select  * from 微核图像分析结果数据表");
	sql = _T("select * from 微核图像分析结果数据表\
			 			 			 		 where 微核图像分析结果数据表.[细胞核数]='" + CellNum + "'  ");

	try
	{

		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}



	try
	{
		for (size_t i = 0; i < MNCheck.ArraySize; i++)
		{
			m_Conn.m_pRecordset->MoveFirst();

			while (!m_Conn.m_pRecordset->adoEOF)
			{
				mystr = m_Conn.m_pRecordset->GetCollect(_T("照片路径"));
				mystring = CStringtoString(mystr);

				if (mystring == MNCheck.LittleImgPath[i])
				{
					//std::cout << "MNCheck.LittleImgPath[i]" << MNCheck.LittleImgPath[i] << std::endl;
					//CString str_MNNum = MNCheck.MNNum[i].c_str();
					USES_CONVERSION; CString str_MNNum(MNCheck.MNNum[i].c_str());
					//std::cout << "str_MNNum" << str_MNNum << std::endl;
					//std::cout << "MNCheck.MNNum[i]" << MNCheck.MNNum[i] << std::endl;

					m_Conn.m_pRecordset->PutCollect(_T("微核数"), _variant_t(str_MNNum));
					break;
				}
				
				m_Conn.m_pRecordset->MoveNext();
			}

		}
		//重要
		m_Conn.m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

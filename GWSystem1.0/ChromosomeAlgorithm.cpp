#include "stdafx.h"
#include"Python.h"
#include "ChromosomeAlgorithm.h"
#include"ReadAndWriteForAccess.h"
//染色体识别
#include"染色体算法文件\Int_to_String.h"
#include"染色体算法文件\BoolSort.h"
#include"染色体算法文件\MyFunction.h"
#include"染色体算法文件\ClassClusterInfo.h"
#include"染色体算法文件\InterestRegion.h"
#include"染色体算法文件\CutFirst.h"
#include"染色体算法文件\DrawClassChromInfoVector.h"
#include"染色体算法文件\ClassifyChromo.h"
#include"染色体算法文件\CutSecond.h"
#include"染色体算法文件\res.h"
//识别着丝粒点位置
#include"着丝粒点识别\LocateCentromere.h"
//去杂质
#include"染色体算法文件\HSVRemoveImpurity.h"
//检验用
#include"染色体算法文件\DrawClassChromInfoVector.h"
//排序
#include"染色体算法文件\Count.h"

//CNN
#include"染色体算法文件\normalizeImgSizeCNN.h"
#include"染色体算法文件\CNNpredict.h"

#include <io.h>
#include "direct.h"

#include<iostream>
#include<fstream>
#include<string>

#define PI 3.1415926

////CNN识别图像尺寸
int CNN_IMG_SIZE = 40;



vector<long>time1;
CReadAndWriteForAccess ReadCHRO;

inline string cutsplitImgName(string s);
inline int cutsplitImgOrder(string s);
bool split_cmp(string s1, string s2);
inline string cutName(string s);
bool imgName_cmp(string s1, string s2);
//string result[40000];
//string result_name[20000];
//string result_num[20000];

using namespace std;

CChromosomeAlgorithm::CChromosomeAlgorithm()
{
}


CChromosomeAlgorithm::~CChromosomeAlgorithm()
{
	
}


// //染色体分析主函数
CHRO_HandleResult* CChromosomeAlgorithm::DicMain(vector<string>ImgWaitingForAna, string writename, CString patientname,
	CHRO_HandleResult *pB, COleDateTime GrabTime)
{

	FILE *fp = NULL;
	fp = fopen("C:\\Users\\xibao\\Desktop\\rst.txt", "w");
	for (size_t i = 0; i < ImgWaitingForAna.size(); i++)
	{
		string enter = "\n";
		string oneImgpath = ImgWaitingForAna[i] + enter;
		fprintf(fp, oneImgpath.c_str());
		cout << "read1name1:" << ImgWaitingForAna[i] << endl;
		//转换后的文件格式
		//int pos0_3 = ImgWaitingForAna[i].rfind("_"); //, s_file_path.length()
		//string fn1_2 = ImgWaitingForAna[i].substr(pos0_3 + 1, ImgWaitingForAna[i].length());//读取路径最后的文件名,仅保留数字名字，不要中文
		//string filename = "t" + fn1_2 + ".txt";
		//ImgFormatConvert.push_back(filename);
	}
	fclose(fp);

	//删除已有分析结果的txt，避免下次认定错误
	string wholefilename = "E:\\mtf\\rstResult.txt";

	WCHAR   wstr[MAX_PATH];
	char sucTempFilePath[MAX_PATH + 1];
	sprintf_s(sucTempFilePath, "%s", wholefilename.c_str());
	MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));
	DeleteFile(wstr);

	//调用分析软件.exe
	//HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "‪E:\\GWsystem\\111vector_predict\\x64\\Release", NULL, NULL, SW_SHOW);
	HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "E:\\GWsystem\\ReleaseMTF1104\\mtf20191009.exe", NULL, NULL, SW_SHOW);
	//if ((int)hNewExe == ERROR_FILE_NOT_FOUND)
	//{
	//	std::cout << "指定工作目录：文件找不到" << std::endl;
	//}

	while ((int)hNewExe == ERROR_FILE_NOT_FOUND)
	{
		hNewExe = ShellExecuteA(NULL, "open", "E:\\GWsystem\\ReleaseMTF1104\\mtf20191009.exe", NULL, NULL, SW_SHOW);
	}

	//从out.txt中读取处理结果
	bool nothing = true;
	fstream file;
	int ch;


	while (nothing)
	{
		
		int nFileExit = _access(wholefilename.c_str(), 0);
		if (nFileExit == 0)
		{
			//存在
			nothing = false;
			////解析txt里的结果
			//保存结果路径
			string savetxtpath = "E:\\mtf\\rstResult.txt";
			ifstream txtfin;
			txtfin.open(savetxtpath, ios::in); // 打开模式可省略 ()
			assert(txtfin.is_open());   //若失败,则输出错误消息,并终止程序运行 
			string lineStr;
			vector<string> linestring;

			while (getline(txtfin, lineStr))
			{
				linestring.push_back(lineStr);
			}
			txtfin.close();
			//如果txt为空
			if (linestring.size() < 1) return pB;
			if (linestring.size() % 58 != 0)
			{
				cout << "E:\\mtf\\rstResult.txt文件中有错误!\n";
				return pB;
			}
			int ImgAllNum = linestring.size() / 58;
			vector<string> ImgPath(ImgAllNum);
			vector<string> ImgChromNum(ImgAllNum);
			vector<vector<string>> splitImgLocResult(ImgAllNum, vector<string>(56));
			
			for (int i = 0; i < linestring.size(); i++)
			{
				int ImgNum = i / 58;
				int InfoNum = i % 58;
				if (InfoNum == 0) ImgPath[ImgNum] = linestring[i];
				else if (InfoNum == 1) ImgChromNum[ImgNum] = linestring[i];
				else splitImgLocResult[ImgNum][InfoNum - 2] = linestring[i];
			}


			//将解析结果存入数据库
			for (size_t j = 0; j < ImgPath.size(); j++)
			{
				OnePicResult.patientname = patientname;
				USES_CONVERSION; CString s(ImgPath[j].c_str());
				OnePicResult.picpath =s;
				OnePicResult.LastResult = splitImgLocResult[j];
				OnePicResult.chromosome_num = stoi(ImgChromNum[j]);
				OnePicResult.GrabTime = GrabTime;
				//cout << "OnePicResult.chromosome_num" << OnePicResult.chromosome_num;
				ReadCHRO.SaveOnePicResult(OnePicResult);
				//统计总的双着数：
				ReadCHRO.CountResultFromBigImg(patientname);
			}

		}
		else
		{
			//不存在
			nothing = true;
		}


	}

	return pB;
}


//小图名字中的和大图名字中重合的部分
inline string cutsplitImgName(string s)
{
	int n = s.find("_");
	return s.substr(0, n);
}
//小图名字中分割出标注序列的数字
inline int cutsplitImgOrder(string s)
{
	int n = s.find("_");
	return std::stoi(s.substr(n + 1, string::npos));
}

bool split_cmp(string s1, string s2)
{
	string s1b = cutsplitImgName(s1);
	string s2b = cutsplitImgName(s2);
	if (s1b == s2b)
	{
		return cutsplitImgOrder(s1) < cutsplitImgOrder(s2);
	}
	return s1b < s2b;
}

// 截取大图地址中小图名字
//后续需要根据具体命名规则进行修改
//目前是截取大图全名中的数字部分
inline string cutName(string s)
{
	int n = s.find("染色体");
	int ne = s.find(".");
	return s.substr(n + 1 + 2 * 3, ne - (n + 1 + 2 * 3));
}

bool imgName_cmp(string s1, string s2)
{
	s1 = cutName(s1);
	s2 = cutName(s2);
	return s1 < s2;
}
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
	CHRO_HandleResult *pB)
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
	string wholefilename = "C:\\Users\\xibao\\Desktop\\rstResult.txt";

	WCHAR   wstr[MAX_PATH];
	char sucTempFilePath[MAX_PATH + 1];
	sprintf_s(sucTempFilePath, "%s", wholefilename.c_str());
	MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));
	DeleteFile(wstr);

	//调用分析软件.exe
	//HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "‪E:\\GWsystem\\111vector_predict\\x64\\Release", NULL, NULL, SW_SHOW);
	HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "E:\\GWsystem\\ReleaseMTF\\mtf20191009.exe", NULL, NULL, SW_SHOW);
	if ((int)hNewExe == ERROR_FILE_NOT_FOUND)
	{
		std::cout << "指定工作目录：文件找不到" << std::endl;
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
			//解析txt里的结果
			int64 start = 0, end = 0;
			start = cv::getTickCount();
			//保存结果路径
			string savetxtpath = "C:\\Users\\xibao\\Desktop\\rstResult.txt";
			ifstream txtfin;
			txtfin.open(savetxtpath, ios::in); // 打开模式可省略 ()
			assert(txtfin.is_open());   //若失败,则输出错误消息,并终止程序运行 
			string lineStr;
			vector<string> linestring;

			while (getline(txtfin, lineStr))
			{
				linestring.push_back(lineStr);
			}

			//如果txt为空
			if (linestring.size() < 1) return pB;

			int lineNum = linestring.size();
			string splitImgResultString = linestring[lineNum - 1];
			linestring.pop_back();
			string splitImgNameString = linestring[lineNum - 2];
			linestring.pop_back();

			//如果txt中没有识别的最终结果
			if (splitImgNameString.find("#") != string::npos) return pB;

			//更新lineNum
			lineNum -= 2;
			//cout << "a\n";
			//************************************************************************************************
			//解析splitImgResultString 和 splitImgNameString
			//************************************************************************************************
			vector<string> splitImgResult;
			vector<string> splitImgName;
			int splitImgResultString_n = splitImgResultString.find(",");
			while (splitImgResultString_n != string::npos)
			{
				//cout << splitImgResultString.substr(0, splitImgResultString_n) << "\n";
				splitImgResult.push_back(splitImgResultString.substr(0, splitImgResultString_n));
				splitImgResultString = splitImgResultString.substr(splitImgResultString_n + 1, string::npos);
				splitImgResultString_n = splitImgResultString.find(",");
			}
			int splitImgNameString_n = splitImgNameString.find(",");
			while (splitImgNameString_n != string::npos)
			{
				string tmpstring = splitImgNameString.substr(0, splitImgNameString_n);

				int tmp_b = tmpstring.find("\\");
				int tmp_e = tmpstring.find(".");
				tmpstring = tmpstring.substr(tmp_b + 1, tmp_e - tmp_b - 1);
				//cout << tmpstring << "\n";
				splitImgName.push_back(tmpstring);

				splitImgNameString = splitImgNameString.substr(splitImgNameString_n + 1, string::npos);
				splitImgNameString_n = splitImgNameString.find(",");
			}

			//************************************************************************************************
			//mtf20191011 目前小图名字与大图对应匹配的算法相当暴力，后续需要改进算法提高效率
			//************************************************************************************************
			//cout << splitImgResult.size() << "\t" << splitImgName.size() << "\t";
			assert(splitImgResult.size() == splitImgResult.size());
			for (int i = 0; i < splitImgResult.size(); i++)
			{
				splitImgResult[i] = splitImgName[i] + "," + splitImgResult[i];
				//cout << splitImgResult[i] << "\n";
			}
			splitImgName.clear();

			sort(splitImgResult.begin(), splitImgResult.end(), split_cmp);
			for (int i = 0; i < splitImgResult.size(); i++)
			{
				cout << splitImgResult[i] << "\n";
			}
			sort(linestring.begin(), linestring.end() - 2, imgName_cmp);
			for (int i = 0; i < lineNum; i++)
			{
				cout << linestring[i] << "\n";
			}

			//************************************************************************************************

			//************************************************************************************************
			//用于指示splitImgResult当前位置
			
			int sIRp = 0;
			vector<vector<string>>	splitImgLocResult(lineNum, vector<string>(Max_splitImgNum));//存每张大图中小图的详细信息
			vector<string> ImgChromNum(lineNum);//存每张大图的染色体数
			vector<string> ImgPath(lineNum);//存每张大图的路径
			for (int i = 0; i < lineNum; i++)
			{
				//if (i > 10) break;
				string tmpLine = linestring[i];
				int tmpLine_n = tmpLine.find("#");
				string tmpimgPath = tmpLine.substr(0, tmpLine_n);
				ImgPath[i] = tmpimgPath;
				//输出大图路径名
				cout << tmpimgPath << "\n";

				string subtmpLine = tmpLine.substr(tmpLine_n + 1, string::npos);

				//解析染色体条数
				int tmpChromNum_n = subtmpLine.find("#");
				string tmpChromNum = subtmpLine.substr(0, tmpChromNum_n);
				ImgChromNum[i] = tmpChromNum;
				cout << tmpChromNum << "\t";

				subtmpLine = subtmpLine.substr(tmpChromNum_n + 1, string::npos);


				int subtmpLine_n = subtmpLine.find("#");
				
				int sILR = 0;

				while (subtmpLine_n != string::npos)
				{
					string tmpsplitImgLoc = subtmpLine.substr(0, subtmpLine_n);
					subtmpLine = subtmpLine.substr(subtmpLine_n + 1, string::npos);
					subtmpLine_n = subtmpLine.find("#");
					//将识别结果与小图对应坐标写在一起，两者应该一一对应，故数据长度必定一样
					if (sIRp >= splitImgResult.size()) break;
					string sIRstring = splitImgResult[sIRp];
					cout << cutName(tmpimgPath) << "\t" << cutsplitImgName(sIRstring) << "\n";

					if (cutName(tmpimgPath) == cutsplitImgName(sIRstring))
					{
						int n = sIRstring.find(",");
						sIRstring = sIRstring.substr(n + 1, string::npos);
						tmpsplitImgLoc = tmpsplitImgLoc + "@" +  sIRstring;
						//splitImgLocResult[i].push_back(tmpsplitImgLoc);
						splitImgLocResult[i][sILR] = tmpsplitImgLoc;
						sILR++;
						sIRp++;
					}
					else
					{
						cout << "有遗漏\n";
					}
					
					//splitImgLoc[i].push_back(tmpsplitImgLoc);
					//输出小图相关信息
					cout << tmpsplitImgLoc << "\n";
				}
				//补齐
				while (sILR < Max_splitImgNum)
				{
					splitImgLocResult[i][sILR] = "#,#,#,#,#";
					sILR++;
				}
			}
			end = cv::getTickCount();
			cout << "解析需要时间: " << (end - start) / cv::getTickFrequency() << " s" << endl;


			//将解析结果存入数据库
			for (size_t j = 0; j < ImgPath.size(); j++)
			{
				OnePicResult.patientname = patientname;
				USES_CONVERSION; CString s(ImgPath[j].c_str());
				OnePicResult.picpath =s;
				OnePicResult.LastResult = splitImgLocResult[j];
				OnePicResult.chromosome_num = stoi(ImgChromNum[j]);
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
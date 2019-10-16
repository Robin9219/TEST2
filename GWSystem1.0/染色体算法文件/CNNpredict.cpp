// opencv_cpp2py.cpp : Defines the entry point for the console application.
//

/*
马腾飞
该例程将opencv-cpp Mat数据传输给python，再将python返回值传递给cpp；
传递值类型包括int list 以及 numpy.array等
可python返回array并利用opencv-cpp显示
python程序如下
def run(imgdata):
print(imgdata)
return imgdata

python py脚本文件间相互调用没有解决，目前我只能用cpp调用一个py文件中的方程，但是，方程之间可以相互调用
import numpy
import cv2

def hello(a):
print("hello",a)


def shit():
hello("shit")


def run(imgdata):
hello("world")
shit()
return imgdata
*/


#include<iostream>
#include<string>
#include"windows.h"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"


using namespace std;

//mtf20191010
PyObject* StringToPyByWin_chrom(std::string str);
int CNNpredict(string savefilepath)
{
	
	if (PyArray_API == NULL)
	{
		cout << "加载numpy\n";
		import_array();
	}
	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject *pArgs = NULL;
	PyObject *pReturn = NULL;
	//PyObject* pKeyWord = NULL;
	PyObject* pKeyWord1 = NULL;
	PyObject* pKeyWord2 = NULL;

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	cout << "111 Module[predict]：" << endl;
	pModule = PyImport_ImportModule("test1011");//调用test0820.py
	if (!pModule) 
	{
		std::cout << "can't find .py file" << std::endl;
		getchar();
		return -1;
	}
	else
	{
		cout << "open Module" << endl;
	}
	cout << pModule << endl;
	pFunc = PyObject_GetAttrString(pModule, "classifier_pridictor_keras");//调用classifier_pridictor_keras函数


	string fn1_2 = "0000000000mtf";

	string fileName0_1 = savefilepath;

	//pKeyWord = StringToPyByWin(fileName0_1);
	//pArgs = PyTuple_New(1);
	//PyTuple_SetItem(pArgs, 0, pKeyWord);
	pArgs = PyTuple_New(2);//传递两个参数
	pKeyWord1 = StringToPyByWin_chrom(fileName0_1);//给第一个赋值
	PyTuple_SetItem(pArgs, 0, pKeyWord1);
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", fileName0_2));fn1_2
	pKeyWord2 = StringToPyByWin_chrom(fn1_2);//给第二个赋值
	PyTuple_SetItem(pArgs, 1, pKeyWord2);

	pReturn = PyEval_CallObject(pFunc, pArgs);//调用函数

	Py_XDECREF(pModule);
	Py_XDECREF(pFunc);
	Py_XDECREF(pArgs);
	Py_XDECREF(pReturn);
	Py_XDECREF(pKeyWord1);
	Py_XDECREF(pKeyWord2);
	//Py_CLEAR(pModule);
	//Py_CLEAR(pFunc);
	//Py_CLEAR(pArgs);
	//Py_CLEAR(pReturn);
	//Py_CLEAR(pKeyWord1);
	//Py_CLEAR(pKeyWord2);


	return 0;
}
PyObject* StringToPyByWin_chrom(std::string str)
{
	int wlen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), int(str.size()), NULL, 0);
	wchar_t* wszString = new wchar_t[wlen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), int(str.size()), wszString, wlen);
	wszString[wlen] = '\0';
	PyObject* pobj = PyUnicode_FromUnicode((const Py_UNICODE*)wszString, wlen);
	delete wszString;
	return pobj;
}

//mtf前一版
////PyObject* init_numpy()
////{
////	import_array();
////}

//int CNNpredict(vector<cv::Mat> imgvec, vector<int>& ResultFlage)
//{
//	//
//	ResultFlage.clear();
//	//// 初始化Python
//	////在使用Python系统前，必须使用Py_Initialize对其
//	////进行初始化。它会载入Python的内建模块并添加系统路
//	////径到模块搜索路径中。这个函数没有返回值，检查系统
//	////是否初始化成功需要使用Py_IsInitialized。
//	//Py_Initialize();
//	//// 检查初始化是否成功
//	//if (!Py_IsInitialized())
//	//{
//	//	cout << "python初始化失败!\n";
//	//	return -1;
//	//}
//	////// 初始化线程支持	
//	////PyEval_InitThreads();
//	////// 启动子线程前执行，为了释放PyEval_InitThreads获得的全局锁，否则子线程可能无法获取到全局锁。
//	////PyEval_ReleaseThread(PyThreadState_Get());
//
//	//if (PyArray_API == NULL)
//	//{
//	//	import_array();
//	//}
//	////显示python版本信息
//	//cout << (Py_GetVersion()) << "\n";
//	///*PyObject* sys = PyImport_ImportModule("sys");*/
//	//PyRun_SimpleString("print('come in python')");
//	////PyRun_SimpleString("import sys"); // 执行 python 中的短语句  
//	////PyRun_SimpleString("sys.path.append('./')");
//	////PyRun_SimpleString("import cv2");
//	//
//
//	if (PyArray_API == NULL)
//	{
//		cout << "加载numpy\n";
//		import_array();
//	}
//	cout << "第一次需要显示tensorflow版本信息（未显示表明tf加载失败，加载成功后不再显示）：";
//	// load python script
//	PyObject* pName = PyUnicode_FromString("predictMTF");
//	PyObject* pModule = PyImport_Import(pName);
//	if (!pModule) {
//		std::cout << "can't find predict.py" << std::endl;
//		getchar();
//		return -1;
//	}
//	else
//	{
//		cout << "open Module" << endl;
//	}
//	PyObject* pDict = PyModule_GetDict(pModule);
//	if (!pDict)
//	{
//		return -1;
//	}
//	PyObject* pFunc = PyDict_GetItemString(pDict, "classifier_pridictor");
//	if (!pFunc || !PyCallable_Check(pFunc))
//	{
//		printf("can't find function [classifier_pridictor]");
//		getchar();
//		return -1;
//	}
//	else
//	{
//		cout << "open function [classifier_pridictor]" << endl;
//	}
//	
//
//	//cv::Mat img1 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/1.jpg");
//	//cv::Mat img2 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/2.jpg");
//	//cv::Mat img3 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/3.jpg");
//	//cv::Mat img4 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/4.jpg");
//	//cv::Mat img5 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/5.jpg");
//	//cv::Mat img6 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/6.jpg");
//	//cv::Mat img7 = cv::imread("E:/workspace/cpp_call_py/vector_predict/sample/7.jpg");
//
//	//vector<cv::Mat> imgvec;
//	//imgvec.push_back(img1);
//	//imgvec.push_back(img2);
//	//imgvec.push_back(img3);
//	//imgvec.push_back(img4);
//	//imgvec.push_back(img5);
//	//imgvec.push_back(img6);
//	//imgvec.push_back(img7);
//
//
//	
//
//	//cout << img.ptr<cv::Vec3b>(0)[1];
//	PyObject *ArgList = PyTuple_New(1);
//	auto sz = imgvec[0].size();
//	int x = sz.width;
//	int y = sz.height;
//	int z = imgvec[0].channels();
//	int Numimg = imgvec.size();
//	//cout << "vectorsize:" << Numimg << "\n";
//	uchar *CArrays = new uchar[Numimg*x*y*z];
//	int iChannels = imgvec[0].channels();
//	int iRows = imgvec[0].rows;
//	int iCols = imgvec[0].cols * iChannels;
//	//if (img.isContinuous())
//	//{
//	//	iCols *= iRows;
//	//	iRows = 1;
//	//	cout << "image is Continuous\n";
//	//}
//	uchar* p;
//	int id = -1;
//	//cout << iRows <<"\t"<< iCols<<"\n";
//	for (int nimg = 0;nimg < Numimg;nimg++)
//	{
//		for (int i = 0; i < iRows; i++)
//		{
//			// get the pointer to the ith row
//			p = imgvec[nimg].ptr<uchar>(i);
//			// operates on each pixel
//			for (int j = 0; j < iCols; j++)
//			{
//				CArrays[++id] = p[j];//连续空间
//			}
//		}
//	}
//	
//	npy_intp Dims[4] = { Numimg, y, x, z }; //注意这个维度数据！
//	PyObject *PyArray = PyArray_SimpleNewFromData(4, Dims, NPY_UBYTE, CArrays);
//	PyTuple_SetItem(ArgList, 0, PyArray);
//	//return int list
//	//PyObject *pRetVal = PyObject_CallObject(pFunc, ArgList);
//	//for (int i = 0; i < PyList_Size(pRetVal); i++)
//	//{
//	//	cout << PyLong_AsLong(PyList_GetItem(pRetVal, (Py_ssize_t)i)) << "\n";
//	//}
//	//return array to check image 将python中的图像回传给c++，并显示
//	//cout << "大傻逼\n";
//	PyObject  *pRetVal = PyObject_CallObject(pFunc, ArgList);
//	//cout << "大傻逼\n";
//	//cout << "\n\nchrom Number:" << PyList_Size(pRetVal) << "\n";
//	//cout << "predict result:\n";
//	//cout << (int)PyLong_AsLong(PyList_GetItem(pRetVal,0) )<< "\n";
//	for (int i = 0; i < PyList_Size(pRetVal); i++)
//	{
//		//cout << (int)PyLong_AsLong(PyList_GetItem(pRetVal, (Py_ssize_t)i)) << "\t";
//		ResultFlage.push_back((int)PyLong_AsLong(PyList_GetItem(pRetVal, (Py_ssize_t)i)));
//	}
//	//cout << ResultFlage.size() << "\n";
//	//for (int i = 0; i < PyList_Size(pRetVal); i++)
//	//{
//	//	cout << (int)ResultFlage[i] << "\n";
//	//}
//	
//	
//	cout << PyList_Size(pRetVal) << "\n";
//
//	////// 保证子线程调用都结束后
//	////PyGILState_Ensure();
//	//Py_Finalize();
//	//// 之后不能再调用任何python的API
//	cout << "end*****************************************************************************************\n";
//	//Py_DECREF(pName);
//	//Py_DECREF(pModule);
//	//Py_DECREF(pDict);
//	//Py_DECREF(pFunc);
//	//Py_DECREF(ArgList);
//	//Py_DECREF(PyArray);
//	//Py_DECREF(pRetVal);
//	//Py_CLEAR(pName);
//	//Py_CLEAR(pModule);
//	//Py_CLEAR(pDict);
//	//Py_CLEAR(pFunc);
//	//Py_CLEAR(ArgList);
//	//Py_CLEAR(PyArray);
//	//Py_CLEAR(pRetVal);
//	return 0;
//}
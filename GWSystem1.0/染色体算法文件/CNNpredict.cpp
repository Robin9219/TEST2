// opencv_cpp2py.cpp : Defines the entry point for the console application.
//

/*
���ڷ�
�����̽�opencv-cpp Mat���ݴ����python���ٽ�python����ֵ���ݸ�cpp��
����ֵ���Ͱ���int list �Լ� numpy.array��
��python����array������opencv-cpp��ʾ
python��������
def run(imgdata):
print(imgdata)
return imgdata

python py�ű��ļ����໥����û�н����Ŀǰ��ֻ����cpp����һ��py�ļ��еķ��̣����ǣ�����֮������໥����
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
//����OpenCVAPI
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
		cout << "����numpy\n";
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
	cout << "111 Module[predict]��" << endl;
	pModule = PyImport_ImportModule("test1011");//����test0820.py
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
	pFunc = PyObject_GetAttrString(pModule, "classifier_pridictor_keras");//����classifier_pridictor_keras����


	string fn1_2 = "0000000000mtf";

	string fileName0_1 = savefilepath;

	//pKeyWord = StringToPyByWin(fileName0_1);
	//pArgs = PyTuple_New(1);
	//PyTuple_SetItem(pArgs, 0, pKeyWord);
	pArgs = PyTuple_New(2);//������������
	pKeyWord1 = StringToPyByWin_chrom(fileName0_1);//����һ����ֵ
	PyTuple_SetItem(pArgs, 0, pKeyWord1);
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", fileName0_2));fn1_2
	pKeyWord2 = StringToPyByWin_chrom(fn1_2);//���ڶ�����ֵ
	PyTuple_SetItem(pArgs, 1, pKeyWord2);

	pReturn = PyEval_CallObject(pFunc, pArgs);//���ú���

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

//mtfǰһ��
////PyObject* init_numpy()
////{
////	import_array();
////}

//int CNNpredict(vector<cv::Mat> imgvec, vector<int>& ResultFlage)
//{
//	//
//	ResultFlage.clear();
//	//// ��ʼ��Python
//	////��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
//	////���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
//	////����ģ������·���С��������û�з���ֵ�����ϵͳ
//	////�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��
//	//Py_Initialize();
//	//// ����ʼ���Ƿ�ɹ�
//	//if (!Py_IsInitialized())
//	//{
//	//	cout << "python��ʼ��ʧ��!\n";
//	//	return -1;
//	//}
//	////// ��ʼ���߳�֧��	
//	////PyEval_InitThreads();
//	////// �������߳�ǰִ�У�Ϊ���ͷ�PyEval_InitThreads��õ�ȫ�������������߳̿����޷���ȡ��ȫ������
//	////PyEval_ReleaseThread(PyThreadState_Get());
//
//	//if (PyArray_API == NULL)
//	//{
//	//	import_array();
//	//}
//	////��ʾpython�汾��Ϣ
//	//cout << (Py_GetVersion()) << "\n";
//	///*PyObject* sys = PyImport_ImportModule("sys");*/
//	//PyRun_SimpleString("print('come in python')");
//	////PyRun_SimpleString("import sys"); // ִ�� python �еĶ����  
//	////PyRun_SimpleString("sys.path.append('./')");
//	////PyRun_SimpleString("import cv2");
//	//
//
//	if (PyArray_API == NULL)
//	{
//		cout << "����numpy\n";
//		import_array();
//	}
//	cout << "��һ����Ҫ��ʾtensorflow�汾��Ϣ��δ��ʾ����tf����ʧ�ܣ����سɹ�������ʾ����";
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
//				CArrays[++id] = p[j];//�����ռ�
//			}
//		}
//	}
//	
//	npy_intp Dims[4] = { Numimg, y, x, z }; //ע�����ά�����ݣ�
//	PyObject *PyArray = PyArray_SimpleNewFromData(4, Dims, NPY_UBYTE, CArrays);
//	PyTuple_SetItem(ArgList, 0, PyArray);
//	//return int list
//	//PyObject *pRetVal = PyObject_CallObject(pFunc, ArgList);
//	//for (int i = 0; i < PyList_Size(pRetVal); i++)
//	//{
//	//	cout << PyLong_AsLong(PyList_GetItem(pRetVal, (Py_ssize_t)i)) << "\n";
//	//}
//	//return array to check image ��python�е�ͼ��ش���c++������ʾ
//	//cout << "��ɵ��\n";
//	PyObject  *pRetVal = PyObject_CallObject(pFunc, ArgList);
//	//cout << "��ɵ��\n";
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
//	////// ��֤���̵߳��ö�������
//	////PyGILState_Ensure();
//	//Py_Finalize();
//	//// ֮�����ٵ����κ�python��API
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
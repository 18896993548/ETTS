#pragma once
/*********************************************************************
数据预处理过程：1.获取新闻时间表
				2.新闻时间表转为整型，便于后面处理
				3.计算每个时间点的新闻个数，视为每个时间点的重要性依据
				4.进行时间阶段划分，得到的是时间点
				5.根据划分好的时间阶段提取文本
**********************************************************************/
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;
struct Int_Date{
	int int_date;					//时间
	int id;							//表示第几个文档
};
//存储词语与句子的对应关系
struct Word_frequency{
	int count;							//词语出现的次数
	double frequency;					//词语出现的频率
};

class Pre_Processing{
public:
	//去除一些包含句子太少的文档
	void Remove_Useless_Doc( string old_dir, string new_dir );

	//根据给定目录路径，获取该路径下的新闻的时间，存放到news_date向量中
	void Get_news_date( string file_name, vector<string> &news_date );

	//将新闻的时间news_date转换为数值表示；得到对结果中包含：每一个时间点->文章个数
	void Trans_date_to_int_date( vector<string> news_date, vector<Int_Date> &int_date );

	//将用数值型表示的时间按照从小到大的顺序排序
	void Select_sort( vector<Int_Date> &int_date );

	//得出时间分布趋势（暂时不用）
	void Time_State(vector<Int_Date> int_date);

	//根据一连串整形时间进行阶段划分,返回一系列时间点，划分的可能性从高到低排列
	vector<int> state_division( vector<Int_Date> int_date );

	//根据已选择出的时间点进行划分,将新闻分为NUM个集合，结果存放在partition_dates中
	void divide_by_gradient( vector<int> gradient_point, vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM = 10 );

	//按照时间序将新闻分为N个集合（采用平均分配的方法）
	void Divide_C( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM = 10 );

	//按照时间划分文章集合（采用将相同时间里的文章放在一起的方法，选择同一时间文章最多的节点）
	/*int_date里面存放所有文档的编号以及对应的时间；结果partition_dates里存放分好后的文档集合以及对应的时间*/
	void Divide_C2( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates );

	//读取文件的函数，输入为文件的路径
	void read_file( string path, vector<string> &content);

	//获取时间T下的文档集合:path存放原始文档所在的目录名称；dates存放T时间下的文档号；contents里存放获得的文档内容，time存放内容对应的日期
	void read_file_T( string path, vector<Int_Date> dates, vector<string> &contents, vector<int> &time );
};
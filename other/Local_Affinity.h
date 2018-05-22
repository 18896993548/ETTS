#pragma once
#include "Pre_Processing.h"
#include <map>


class Local_Affinity{
public:
	//
	//将分好词的句子拆分成词
	void cut_word( vector<string> content, vector<vector<string>> &content_word );
	//计算词频tf
	void calculate_tf( vector<vector<string>> content_word, vector< map<string,Word_frequency> > &tf );
	//计算idf
	void calculate_idf( vector< map<string,Word_frequency> > tf, map<string,double> &idf );
	//计算Si-->Sj的转移概率
	void calculate_P( vector<vector<double>> &M );
	//计算Si与Sj的余弦相似度vector<vector<string>> content_word, vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M
	void calculate_cos( vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M );
	//计算余弦相似度
	//double calculate_PI( vector< map<string, Word_frequency> > tf, map<string,double> idf );
	double calculate_PI( string word, int ID, vector< map<string, Word_frequency> > tf, map<string,double> idf );
	//输出转移矩阵数据
	void print_M( vector<vector<double>> M );
	//计算矩阵M的转置
	void transposition( vector<vector<double>> M, vector<vector<double>> &T);
	//计算每个句子的可能性
	void calculate_probability( vector<vector<double>> T, vector<double> &probability );
	//两个矩阵相乘
	vector<double> mul_matrix( vector<vector<double>> x, vector<double> y);
	//两个矩阵相乘
	vector<double> mul_matrix( vector<double> x, vector<double> y);
	
};
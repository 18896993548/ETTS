#pragma once
#include "Pre_Processing.h"
#include <map>


class Local_Affinity{
public:
	//
	//���ֺôʵľ��Ӳ�ֳɴ�
	void cut_word( vector<string> content, vector<vector<string>> &content_word );
	//�����Ƶtf
	void calculate_tf( vector<vector<string>> content_word, vector< map<string,Word_frequency> > &tf );
	//����idf
	void calculate_idf( vector< map<string,Word_frequency> > tf, map<string,double> &idf );
	//����Si-->Sj��ת�Ƹ���
	void calculate_P( vector<vector<double>> &M );
	//����Si��Sj���������ƶ�vector<vector<string>> content_word, vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M
	void calculate_cos( vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M );
	//�����������ƶ�
	//double calculate_PI( vector< map<string, Word_frequency> > tf, map<string,double> idf );
	double calculate_PI( string word, int ID, vector< map<string, Word_frequency> > tf, map<string,double> idf );
	//���ת�ƾ�������
	void print_M( vector<vector<double>> M );
	//�������M��ת��
	void transposition( vector<vector<double>> M, vector<vector<double>> &T);
	//����ÿ�����ӵĿ�����
	void calculate_probability( vector<vector<double>> T, vector<double> &probability );
	//�����������
	vector<double> mul_matrix( vector<vector<double>> x, vector<double> y);
	//�����������
	vector<double> mul_matrix( vector<double> x, vector<double> y);
	
};
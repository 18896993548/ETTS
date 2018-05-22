#pragma once
/*********************************************************************
����Ԥ������̣�1.��ȡ����ʱ���
				2.����ʱ���תΪ���ͣ����ں��洦��
				3.����ÿ��ʱ�������Ÿ�������Ϊÿ��ʱ������Ҫ������
				4.����ʱ��׶λ��֣��õ�����ʱ���
				5.���ݻ��ֺõ�ʱ��׶���ȡ�ı�
**********************************************************************/
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;
struct Int_Date{
	int int_date;					//ʱ��
	int id;							//��ʾ�ڼ����ĵ�
};
//�洢��������ӵĶ�Ӧ��ϵ
struct Word_frequency{
	int count;							//������ֵĴ���
	double frequency;					//������ֵ�Ƶ��
};

class Pre_Processing{
public:
	//ȥ��һЩ��������̫�ٵ��ĵ�
	void Remove_Useless_Doc( string old_dir, string new_dir );

	//���ݸ���Ŀ¼·������ȡ��·���µ����ŵ�ʱ�䣬��ŵ�news_date������
	void Get_news_date( string file_name, vector<string> &news_date );

	//�����ŵ�ʱ��news_dateת��Ϊ��ֵ��ʾ���õ��Խ���а�����ÿһ��ʱ���->���¸���
	void Trans_date_to_int_date( vector<string> news_date, vector<Int_Date> &int_date );

	//������ֵ�ͱ�ʾ��ʱ�䰴�մ�С�����˳������
	void Select_sort( vector<Int_Date> &int_date );

	//�ó�ʱ��ֲ����ƣ���ʱ���ã�
	void Time_State(vector<Int_Date> int_date);

	//����һ��������ʱ����н׶λ���,����һϵ��ʱ��㣬���ֵĿ����ԴӸߵ�������
	vector<int> state_division( vector<Int_Date> int_date );

	//������ѡ�����ʱ�����л���,�����ŷ�ΪNUM�����ϣ���������partition_dates��
	void divide_by_gradient( vector<int> gradient_point, vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM = 10 );

	//����ʱ�������ŷ�ΪN�����ϣ�����ƽ������ķ�����
	void Divide_C( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM = 10 );

	//����ʱ�仮�����¼��ϣ����ý���ͬʱ��������·���һ��ķ�����ѡ��ͬһʱ���������Ľڵ㣩
	/*int_date�����������ĵ��ı���Լ���Ӧ��ʱ�䣻���partition_dates���ŷֺú���ĵ������Լ���Ӧ��ʱ��*/
	void Divide_C2( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates );

	//��ȡ�ļ��ĺ���������Ϊ�ļ���·��
	void read_file( string path, vector<string> &content);

	//��ȡʱ��T�µ��ĵ�����:path���ԭʼ�ĵ����ڵ�Ŀ¼���ƣ�dates���Tʱ���µ��ĵ��ţ�contents���Ż�õ��ĵ����ݣ�time������ݶ�Ӧ������
	void read_file_T( string path, vector<Int_Date> dates, vector<string> &contents, vector<int> &time );
};
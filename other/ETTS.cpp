#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <io.h>
#include "Pre_Processing.h"
#include "Global_Affinity.h"
#include "Local_Affinity.h"
#include <math.h>
#include <time.h>

using namespace std;

vector< map<string,Word_frequency> > tf;
map<string,double> idf;

//��ȡĳһ�ļ����������ĵ�������
void getFiles2(string path,vector<string> &files){											
	long hFile=0;
	struct _finddata_t fileinfo;
	string p;
	if((hFile=_findfirst(p.assign(path).append("\\*").c_str(),&fileinfo))!=-1){
		do{
			if((fileinfo.attrib & _A_SUBDIR)){
				if(strcmp(fileinfo.name,".")!=0&&strcmp(fileinfo.name,"..")!=0)
					getFiles2(p.assign(path).append("\\").append(fileinfo.name),files);
			}
			else{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		}while(_findnext(hFile,&fileinfo)==0);
		_findclose(hFile);
	}
}
//��ȡ�ļ��ĺ���������Ϊ�ļ���·��
void read_file( string path, vector<string> &content);
//�������Ҫ�ľ���
void printf_sentence( vector<double> y, vector<string> content, int rank_num = 3 );
//����Global
void Global( string path, vector<Int_Date> &partition_dates, vector<string> contents, vector<double> &probability );
//����Local
vector<string> Local( ofstream &out, string path, vector<Int_Date> partition_dates, vector<double> &probability );
//����һ�����¼��ϣ�ѡ������һƪ��������IDֵ
vector<int> Select_News( vector<Int_Date> partition_dates, int COUNT=1);
//����һ�����¼��ϣ�ѡ��ժҪ�����ʱ���
string Select_News_Date( vector<Int_Date> partition_dates );
//�����ӵĵ÷�ֵ���й�һ��
void Normalization( vector<double> &probability );
//����õ�ժҪ���ӵĵ÷�ֵд���ļ���
void write_file( string path, vector<double> probability );
//�ۺ�Global score��Local score�ó����ս��
vector<double> combinate_Global_Lobal( vector<double> probability_local, vector<double> probability_global, double local = 0.7, double global = 0.3 );
//����õ�ʱ����ժҪ����д�����ļ���,�Ѿ����������out
void Write_Summary( ofstream &out, vector<double> y, vector<string> content, int rank_num = 3 );
int main(){
	
	string path = "D:\\Innovation_plan\\Final";
	Pre_Processing pre_processing;
	vector<string> news_date;
	vector<Int_Date> int_date;
	pre_processing.Get_news_date( path, news_date );								//�õ����ŵ�ʱ��
	pre_processing.Trans_date_to_int_date( news_date, int_date );					//�����ŵ�ʱ�䣨string��תΪ����
	pre_processing.Select_sort( int_date );											//��ʱ���������
	//pre_processing.Time_State(int_date);
	vector<int> date_gradient = pre_processing.state_division( int_date );			//���ݶȽ��н׶λ���

	vector<vector<Int_Date>> partition_dates;
	pre_processing.divide_by_gradient( date_gradient, int_date,partition_dates, 10 );	//����ʱ��㻮���ĵ�����
	//pre_processing.Divide_C( int_date, partition_dates, 10 );					//���о���Ϊ���¼���
	vector<string> contents;
	/*****************************************************************************************/
	//����ժҪЧ��
	//vector<string> filePath;
	//getFiles2( "D:\\Innovation_plan\\Final", filePath );
	//ofstream test_summary;
	//test_summary.open("D:\\test_summary.txt");
	//for( int i = 0; i < 2; i++){
	//	read_file( filePath[i], contents );
	//	vector<double> probability;
	//	Local_Affinity local_affinity;
	//	vector<vector<string>> content_word;
	//	local_affinity.cut_word( contents, content_word );						//�õ�ÿһ������
	//	vector< map<string,Word_frequency> > tf;
	//	local_affinity.calculate_tf( content_word, tf );
	//	map<string,double> idf;
	//	local_affinity.calculate_idf( tf, idf );
	//	vector<vector<double>> M;
	//	local_affinity.calculate_cos( tf, idf, M );
	//	local_affinity.calculate_P( M );
	//	vector<vector<double>> T;
	//	local_affinity.transposition( M, T );									//��������ת��

	//	local_affinity.calculate_probability( T, probability );

	//	printf_sentence( probability, contents );
	//	test_summary<<filePath[i]<<endl;
	//	Write_Summary( test_summary, probability, contents, contents.size() -1 );
	//	contents.clear();
	//}
	//test_summary.close();
	/******************************************************************************************/

	ofstream out_summary;
	out_summary.open("D:\\ETTS_summary.txt");
	for( int i = 0; i < 10; i++ ){
		vector<string> contents_local;											//���local�е�ÿ������
		vector<double> probability_local;
		cout<<"Local"<<endl;
		contents_local = Local( out_summary, path, partition_dates[i], probability_local );		//����local��ֵ
		cout<<"Global"<<endl;
		vector<double> probability_global;
		//Global( path, partition_dates[i], contents_local, probability_global );		//����global��ֵ
		vector<double> result;
		//result = combinate_Global_Lobal( probability_local, probability_global );	//�ۺ�global��local��ֵ�ó����ս��

		//printf_sentence( result, contents_local );
		Write_Summary( out_summary, probability_local, contents_local, 3 );
	}
	out_summary.close();
	return 0;
}
//����Local,����һƪ���µľ���·��������ó�����rangking scores
vector<string> Local( ofstream &out, string path, vector<Int_Date> partition_dates, vector<double> &probability ){
/**/
	vector<string> files;
	getFiles2( path, files );												//��ȡĳһ�ļ����������ĵ�������
	vector<int> id = Select_News( partition_dates, 1 );						//ѡȡ���¼��ϣ�����һ�����е�һ�����¹���
	vector<string> contents;
	out<<Select_News_Date(partition_dates)<<endl;							//���ʱ���
	for( int i = 0; i < id.size(); i++ ){
		read_file( files[ id[i] ], contents );
	}
	Local_Affinity local_affinity;
	vector<vector<string>> content_word;
	local_affinity.cut_word( contents, content_word );						//�õ�ÿһ������
	vector< map<string,Word_frequency> > tf;
	local_affinity.calculate_tf( content_word, tf );
	map<string,double> idf;
	local_affinity.calculate_idf( tf, idf );
	cout<<"idf�������"<<endl;
	vector<vector<double>> M;
	size_t clock_begin, clock_end;
	clock_begin = clock();
	local_affinity.calculate_cos( tf, idf, M );
	clock_end = clock();
	cout<<clock_end-clock_begin<<"ms"<<endl;
	local_affinity.calculate_P( M );
	cout<<clock()-clock_end<<"ms"<<endl;
	cout<<"M�Ĵ�С"<<M.size()<<endl;
	vector<vector<double>> T;
	cout<<"before transposition"<<endl;
	local_affinity.transposition( M, T );									//��������ת��
	cout<<"transposition"<<endl;
	local_affinity.calculate_probability( T, probability );
	//printf_sentence( probability, contents );
	Normalization( probability );
	write_file( "D:\\Local_score.txt", probability );
	return contents;
}

//����һ�����¼��ϣ�ѡ�����еĶ�ƪ��Ĭ�Ϸ���һƪ����������IDֵ
vector<int> Select_News( vector<Int_Date> partition_dates, int COUNT ){
	int temporary[1000][2];
	int k = 0;
	for( int i = 0; i < partition_dates.size(); i++ ){
		if( i == 0 ){
			temporary[k][0] = partition_dates[i].int_date;
			temporary[k][1] = 1;
		}
		else{
			if( temporary[k][0] == partition_dates[i].int_date ){
				temporary[k][1]++;
			}
			else{
				k++;
				temporary[k][0] = partition_dates[i].int_date;
				temporary[k][1] = 1;
			}
		}
	}
	//����ѡ����ĵ�������ʱ��ڵ�
	for( int i = 0; i < k + 1; i++ ){
		int m = i;
		for( int j = i + 1; j <= k; j++){
			if(temporary[m][1] < temporary[j][1] )
				m = j;
		}
		if( m != i){
			int x,y;
			x = temporary[m][0];
			y = temporary[m][1];
			temporary[m][0] = temporary[i][0];
			temporary[m][1] = temporary[i][1];
			temporary[i][0] = x;
			temporary[i][1] = y;
		}
	}
	vector<int> ID;
	for ( int i = 0; i < partition_dates.size() && ID.size() < COUNT; i++ ){
		if( partition_dates[i].int_date == temporary[0][0] )
			ID.push_back( partition_dates[i].id);
	}
	return ID;
}
//����һ�����¼��ϣ�ѡ��ժҪ�����ʱ���
string Select_News_Date( vector<Int_Date> partition_dates ){
	int temporary[1000][2];
	int k = 0;
	for( int i = 0; i < partition_dates.size(); i++ ){
		if( i == 0 ){
			temporary[k][0] = partition_dates[i].int_date;
			temporary[k][1] = 1;
		}
		else{
			if( temporary[k][0] == partition_dates[i].int_date ){
				temporary[k][1]++;
			}
			else{
				k++;
				temporary[k][0] = partition_dates[i].int_date;
				temporary[k][1] = 1;
			}
		}
	}
	//����ѡ����ĵ�������ʱ��ڵ�
	for( int i = 0; i < k + 1; i++ ){
		int m = i;
		for( int j = i + 1; j <= k; j++){
			if(temporary[m][1] < temporary[j][1] )
				m = j;
		}
		if( m != i){
			int x,y;
			x = temporary[m][0];
			y = temporary[m][1];
			temporary[m][0] = temporary[i][0];
			temporary[m][1] = temporary[i][1];
			temporary[i][0] = x;
			temporary[i][1] = y;
		}
	}
	int date = temporary[0][0];
	string year,month, day;
	ostringstream os,os2,os3;
	int d = date % 31;
	if( d == 0 ){
		date -= 31;
		d = 31;
	}
	else{
		date -= d;
	}
	os<<d;
	day = os.str() + "��";

	int m = date % 372;
	if( m == 0){
		m = 12;
		date -= 372;
	}
	else{
		date = date - m;
		m = m / 31;
	}
	os2<<m;
	month = os2.str() + "��";

	date = date / 372;
	os3<<date;
	year = os3.str() + "��";
	return year+month+day;

}
//����Global
void Global( string path, vector<Int_Date> &partition_dates, vector<string> contents, vector<double> &probability ){
	Pre_Processing pre_processing;
	clock_t clockBegin,clockEnd;										//�Ժ���Ϊ��λ
	Global_Affinity global_affinity;
	tf.clear();
	idf.clear();
	vector< map<string,Word_frequency> > tf_local;
	map<string,double> idf_local;
	clockBegin = clock();
	vector<int> time;
	vector<string> contents_local = contents;							//�ȱ���local scores��ľ���
	pre_processing.read_file_T( path, partition_dates, contents, time );
	vector<vector<string>> content_word, content_word_local;
	global_affinity.cut_word( contents, content_word );
	global_affinity.cut_word( contents, content_word_local );

	global_affinity.calculate_tf( content_word, tf_local );
	global_affinity.calculate_tf( content_word, tf );

	global_affinity.calculate_idf( tf, idf_local );
	global_affinity.calculate_idf( tf, idf );

	vector<vector<double>> Local_M;
	global_affinity.calculate_cos( tf_local, tf, idf, Local_M, time );						//����ʱ�����
	clockEnd = clock();
	cout<<"�ķ�"<<clockEnd - clockBegin<<"ms"<<endl;
	global_affinity.calculate_P( Local_M );
	vector<vector<double>> T;
	global_affinity.transposition( Local_M, T );
	global_affinity.print_M( Local_M );
	global_affinity.print_M( T );

	global_affinity.calculate_probability( T, probability );
	cout<<"�ķ�"<<clock() - clockEnd<<"ms"<<endl;
	printf_sentence( probability, contents );
	Normalization( probability );
	write_file( "D:\\Global_score.txt", probability );
}
//�������Ҫ�ľ���
void printf_sentence( vector<double> y, vector<string> content, int rank_num ){
	vector<vector<double>> rank;
	for( int i = 0; i < y.size(); i++ ){
		vector<double> temp;
		temp.push_back(y[i]);
		temp.push_back( i );
		rank.push_back( temp );
	}
	int k;
	for( int i = 0; i < rank.size(); i++ ){
		k = i;
		for( int j = i; j < rank.size(); j++ ){
			if( rank[k][0] < rank[j][0] )
				k = j;
		}
		if( k != i ){
			vector<double> t;
			t = rank[i];
			rank[i] = rank[k];
			rank[k] = t;
		}
	}
	cout<<"�����"<<endl;
	cout<<content.size()<<endl;
	for( int i = 0; i < rank_num && i < content.size(); i++ ){
		//cout<<rank[i][0]<<endl;
		
		cout<<rank[i][1]<<'\t'<<content[int(rank[i][1])]<<endl;
	}
}
//��ȡ�ļ��ĺ���������Ϊ�ļ���·��
void read_file( string path, vector<string> &content){
	ifstream in;
	in.open( path );
	if( !in )
		cout<<"Open "<<path<<" error!"<<endl;
	string str;
	getline( in, str );
	getline( in, str );
	getline( in, str );
	getline( in, str );
	getline( in, str );
	while( getline( in, str) ){
		content.push_back( str );
	}
	in.close();
}
//����õ�ժҪ���ӵĵ÷�ֵд���ļ���
void write_file( string path, vector<double> y ){
	vector<vector<double>> rank;
	for( int i = 0; i < y.size(); i++ ){
		vector<double> temp;
		temp.push_back(y[i]);
		temp.push_back( i );
		rank.push_back( temp );
	}
	int k;
	for( int i = 0; i < rank.size(); i++ ){
		k = i;
		for( int j = i; j < rank.size(); j++ ){
			if( rank[k][0] < rank[j][0] )
				k = j;
		}
		if( k != i ){
			vector<double> t;
			t = rank[i];
			rank[i] = rank[k];
			rank[k] = t;
		}
	}
	ofstream out;
	out.open( path );
	if( !out )
		cout<<"Open "<<path<<" error"<<endl;
	for( int i = 0; i < rank.size(); i++ ){
		out<<rank[i][0]<<endl;
	}
	out.close();
}
//�ۺ�Global score��Local score�ó����ս��
vector<double> combinate_Global_Lobal( vector<double> probability_local, vector<double> probability_global, double local, double global ){
	vector<double> result;
	for( int i = 0; i < probability_local.size(); i++ ){
		result.push_back( probability_local[i] * local + probability_global[i] * global );
	}
	return result;
}
//�����ӵĵ÷�ֵ���й�һ��
void Normalization( vector<double> &probability ){
	double sum = 0;
	for( int i = 0; i < probability.size(); i++ ){
		sum += probability[i];
	}
	for( int i = 0; i < probability.size(); i++ ){
		probability[i] /= sum;
	}
}

//����õ�ʱ����ժҪ����д�����ļ���,�Ѿ����������
void Write_Summary( ofstream &out, vector<double> y, vector<string> content, int rank_num ){
	vector<vector<double>> rank;
	for( int i = 0; i < y.size(); i++ ){
		vector<double> temp;
		temp.push_back(y[i]);
		temp.push_back( i );
		rank.push_back( temp );
	}
	int k;
	for( int i = 0; i < rank.size(); i++ ){
		k = i;
		for( int j = i; j < rank.size(); j++ ){
			if( rank[k][0] < rank[j][0] )
				k = j;
		}
		if( k != i ){
			vector<double> t;
			t = rank[i];
			rank[i] = rank[k];
			rank[k] = t;
		}
	}
	for( int i = 0; i < rank_num && i < content.size(); i++ ){
		//cout<<rank[i][0]<<endl;
		out<<content[int(rank[i][1])]<<endl;
	}
	out<<endl;
}
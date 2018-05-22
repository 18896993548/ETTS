#include "Local_Affinity.h"

//���ֺôʵľ��Ӳ�ֳɴ�
void Local_Affinity::cut_word( vector<string> content, vector<vector<string>> &content_word ){
	for( int i = 0; i < content.size(); i++){
		string str;
		vector<string> temp;
		for( int j = 0; j < content[i].size(); j++ ){
			if( content[i][j] != ' ' )
				str += content[i][j];
			else{
				temp.push_back( str );
				str.clear();
			}
		}
		content_word.push_back( temp );
	}
	/*for ( int i = 0; i < content_word.size(); i++){
		for( int j = 0; j < content_word[i].size(); j++){
			cout<<content_word[i][j]<<'\t';
		}
		cout<<endl;
	}*/
}

//�����Ƶtf��ÿһ�д��һ���д����tf
void Local_Affinity::calculate_tf( vector<vector<string>> content_word, vector< map<string,Word_frequency> > &tf ){
	map<string, Word_frequency>::iterator word_iter;
	for( int i = 0; i < content_word.size(); i++ ){								//��i������
		map<string,Word_frequency> blank;
		tf.push_back( blank );
		//int num_word = 0;														//����ÿһ�京�еĴ������
		for( int j = 0; j < content_word[i].size(); j++ ){						//��i�����ӵĵ�j����
			word_iter = tf[i].find( content_word[i][j] );
			if( word_iter != tf[i].end() ){										//���ô��Ѿ�����ʱ
				word_iter->second.count += 1;
			}
			else{																//����ôʲ����ڣ�����ӽ�ȥ
				Word_frequency temp;
				temp.count = 1;
				tf[i].insert( pair<string,Word_frequency>(content_word[i][j], temp) );
			}
		}
		map<string, Word_frequency>::iterator iter;
		for ( iter = tf[i].begin(); iter != tf[i].end(); iter++ ){						//�õ�tf
			iter->second.frequency = iter->second.count * 1.0 / content_word[i].size();
		}
	}
	//*********************************����***********************************************
	/*for( int i = 0; i < tf.size(); i++ ){
		for( word_iter = tf[i].begin(); word_iter != tf[i].end(); word_iter++ ){
			cout<<word_iter->second.frequency<<'\t';
		}
	}*/
}

//����idf
void Local_Affinity::calculate_idf( vector< map<string,Word_frequency> > tf, map<string,double> &idf ){
	map<string, Word_frequency>::iterator iter_tf;
	map<string, double>::iterator iter_idf;
	for( int i = 0; i < tf.size(); i++ ){
		for( iter_tf = tf[i].begin(); iter_tf != tf[i].end(); iter_tf++ ){
			iter_idf = idf.find( iter_tf->first );
			if( iter_idf != idf.end() ){
				iter_idf->second += 1;
			}
			else{
				idf.insert( pair<string,double>(iter_tf->first, 1.0) );
			}
		}
	}
	//*********************************����***********************************************
	/*for( iter_idf = idf.begin(); iter_idf != idf.end(); iter_idf++ ){
		cout<<iter_idf->first<<' '<<iter_idf->second<<'\t';
	}*/
}

//�����������ƶ�
double Local_Affinity::calculate_PI( string word, int ID, vector< map<string, Word_frequency> > tf, map<string,double> idf ){
	double sum = 0;
	double key = 0;
	map<string, Word_frequency>::iterator iter_tf;
	map<string, double>::iterator iter_idf;
	for( int i = 0; i < tf.size(); i++ ){
		iter_tf = tf[i].find( word );
		double tf_temp = 0;
		double n = 0;
		if( iter_tf != tf[i].end() ){
			tf_temp = iter_tf->second.frequency;
		}
		iter_idf = idf.find( word );
		if( iter_idf != idf.end() ){
			n = iter_idf->second;
		}
		double t = tf_temp * ( 1 + log( tf.size() / n ) / log( 2.0 ) );
		if( i == ID )
			key = t;
		t *= t;
		sum += t;
	}
	return ( key / sqrt( sum ) );
}
//����Si��Sj���������ƶ�
void Local_Affinity::calculate_cos( vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M ){
	vector<double> temp;
	for( int i = 0; i < tf.size(); i++ ){
		for( int j = 0; j < tf.size(); j++ ){
			double sum = 0;
			if( i == j ){															//ͬһ�����ӣ�ת�Ƹ���Ϊ0
				temp.push_back(0);
				continue;
			}
			//Ѱ����ͬ�Ĵ���
			map<string, Word_frequency>::iterator iter_tfi;
			map<string, Word_frequency>::iterator iter_tfj;
			for( iter_tfi = tf[i].begin(); iter_tfi != tf[i].end(); iter_tfi++ ){
				iter_tfj = tf[j].find( iter_tfi ->first );
				if( iter_tfj != tf[j].end() ){											//�ҵ���ͬ�Ĵ���
					sum = sum + calculate_PI( iter_tfi->first, i, tf, idf ) * calculate_PI( iter_tfi->first, j, tf, idf );
				}
			}
			temp.push_back( sum );
		}
		M.push_back( temp );
		temp.clear();
	}
	//*********************************����***********************************************
	/*for( int i = 0; i < M.size(); i++ ){
		for( int j = 0; j < M[i].size(); j++)
			cout<<M[i][j]<<'\t';
		cout<<endl;
	}*/
}

//����Si-->Sj��ת�Ƹ���
void Local_Affinity::calculate_P( vector<vector<double>> &M ){
	for( int i = 0; i < M.size(); i++ ){
		double sum_row = 0;
		for( int j = 0 ; j < M[i].size(); j++ ){
			sum_row += M[i][j];
		}
		for( int j = 0 ; j < M[i].size(); j++ ){
			//cout<<sum_row<<endl;
			if(sum_row != 0)
				M[i][j] /= sum_row;
		}
	}
}

//���ת�ƾ�������
void Local_Affinity::print_M( vector<vector<double>> M ){
	ofstream out;
	out.open("D:\\Trans_M.txt");
	if( !out )
		cout<<"Open file error!"<<endl;
	for( int i = 0; i < M.size(); i++ ){
		for( int j = 0; j < M[i].size(); j++ ){
			out<<M[i][j]<<'\t';
		}
		out<<endl;
	}
	out.close();
}

//�������M��ת��
void Local_Affinity::transposition( vector<vector<double>> M, vector<vector<double>> &T ){
	int len = M.size();
	for( int i = 0; i < len; i++ ){
		vector<double> temp;
		for( int j = 0; j < len; j++ ){
			temp.push_back( M[j][i] );
		}
		T.push_back( temp );
	}
}

//����ÿ�����ӵĿ�����
void Local_Affinity::calculate_probability( vector<vector<double>> T, vector<double> &probability ){
	int len = T.size();
	vector<double> e;
	for( int i = 0; i < len; i++ ){
		e.push_back( 1 );
		probability.push_back( 1.0 / len );
	}
	vector<double> probability_front;
	vector<double> T_front;
	double cost = 1;
	while( cost > 0.01 ){
		probability_front = probability;
		cost = 0;
		probability = mul_matrix( T, probability );
		double sub;
		for( int i = 0; i < len; i++ ){
			probability[i] = probability[i] * 0.85 + 0.15 / len;
			sub = fabs( probability_front[i] - probability[i] );
			if( cost < sub )
				cost = sub;
		}
		//�ı�T��ֵ
		for( int i = 0; i < len; i++){
			for( int j = 0; j < len; j++){
				T[i][j] = 0.85 * T[i][j] * probability_front[i] + 0.15 / len;
			}
		}
		//��һ��
		for( int i = 0; i < len; i++){
			double sum = 0;
			for( int j = 0; j < len; j++){
				sum += T[j][i];
			}
			for( int k = 0; k < len; k++ ){
				T[k][i] = T[k][i] / sum;
			}
		}
	}
	//***********************************����***********************
	/*for( int i = 0; i < probability.size(); i++ ){
		cout<<probability[i]<<'\t'<<i<<endl;
	}*/
}

//�����������
vector<double> Local_Affinity::mul_matrix( vector<vector<double>> x, vector<double> y){
	int len = x.size();
	vector<double> result;
	for( int i = 0; i < x.size(); i++ ){
		double sum = 0;
		for( int j = 0; j < x[i].size(); j++ ){
			sum = sum + x[i][j] * y[j];
		}
		result.push_back( sum );
	}
	return result;
}

//�����������
vector<double> Local_Affinity::mul_matrix( vector<double> x, vector<double> y){
	int len = x.size();
	vector<double> result;
	for( int i = 0; i < x.size(); i++ ){
		double sum = 0;
		result.push_back( sum );
	}
	return result;
}



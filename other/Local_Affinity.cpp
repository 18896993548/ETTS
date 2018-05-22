#include "Local_Affinity.h"

//将分好词的句子拆分成词
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

//计算词频tf，每一行存放一行中词语的tf
void Local_Affinity::calculate_tf( vector<vector<string>> content_word, vector< map<string,Word_frequency> > &tf ){
	map<string, Word_frequency>::iterator word_iter;
	for( int i = 0; i < content_word.size(); i++ ){								//第i个句子
		map<string,Word_frequency> blank;
		tf.push_back( blank );
		//int num_word = 0;														//计算每一句含有的词语个数
		for( int j = 0; j < content_word[i].size(); j++ ){						//第i个句子的第j个词
			word_iter = tf[i].find( content_word[i][j] );
			if( word_iter != tf[i].end() ){										//当该词已经存在时
				word_iter->second.count += 1;
			}
			else{																//如果该词不存在，则添加进去
				Word_frequency temp;
				temp.count = 1;
				tf[i].insert( pair<string,Word_frequency>(content_word[i][j], temp) );
			}
		}
		map<string, Word_frequency>::iterator iter;
		for ( iter = tf[i].begin(); iter != tf[i].end(); iter++ ){						//得到tf
			iter->second.frequency = iter->second.count * 1.0 / content_word[i].size();
		}
	}
	//*********************************测试***********************************************
	/*for( int i = 0; i < tf.size(); i++ ){
		for( word_iter = tf[i].begin(); word_iter != tf[i].end(); word_iter++ ){
			cout<<word_iter->second.frequency<<'\t';
		}
	}*/
}

//计算idf
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
	//*********************************测试***********************************************
	/*for( iter_idf = idf.begin(); iter_idf != idf.end(); iter_idf++ ){
		cout<<iter_idf->first<<' '<<iter_idf->second<<'\t';
	}*/
}

//计算余弦相似度
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
//计算Si与Sj的余弦相似度
void Local_Affinity::calculate_cos( vector< map<string, Word_frequency> > tf, map<string,double> idf, vector<vector<double>> &M ){
	vector<double> temp;
	for( int i = 0; i < tf.size(); i++ ){
		for( int j = 0; j < tf.size(); j++ ){
			double sum = 0;
			if( i == j ){															//同一个句子，转移概率为0
				temp.push_back(0);
				continue;
			}
			//寻找相同的词语
			map<string, Word_frequency>::iterator iter_tfi;
			map<string, Word_frequency>::iterator iter_tfj;
			for( iter_tfi = tf[i].begin(); iter_tfi != tf[i].end(); iter_tfi++ ){
				iter_tfj = tf[j].find( iter_tfi ->first );
				if( iter_tfj != tf[j].end() ){											//找到相同的词语
					sum = sum + calculate_PI( iter_tfi->first, i, tf, idf ) * calculate_PI( iter_tfi->first, j, tf, idf );
				}
			}
			temp.push_back( sum );
		}
		M.push_back( temp );
		temp.clear();
	}
	//*********************************测试***********************************************
	/*for( int i = 0; i < M.size(); i++ ){
		for( int j = 0; j < M[i].size(); j++)
			cout<<M[i][j]<<'\t';
		cout<<endl;
	}*/
}

//计算Si-->Sj的转移概率
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

//输出转移矩阵数据
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

//计算矩阵M的转置
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

//计算每个句子的可能性
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
		//改变T的值
		for( int i = 0; i < len; i++){
			for( int j = 0; j < len; j++){
				T[i][j] = 0.85 * T[i][j] * probability_front[i] + 0.15 / len;
			}
		}
		//归一化
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
	//***********************************测试***********************
	/*for( int i = 0; i < probability.size(); i++ ){
		cout<<probability[i]<<'\t'<<i<<endl;
	}*/
}

//两个矩阵相乘
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

//两个矩阵相乘
vector<double> Local_Affinity::mul_matrix( vector<double> x, vector<double> y){
	int len = x.size();
	vector<double> result;
	for( int i = 0; i < x.size(); i++ ){
		double sum = 0;
		result.push_back( sum );
	}
	return result;
}



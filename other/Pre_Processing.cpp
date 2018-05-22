#include"Pre_Processing.h"
#include<io.h>


//��ȡĳһ�ļ����������ĵ�������
void getFiles(string path,vector<string> &files){											
	long hFile=0;
	struct _finddata_t fileinfo;
	string p;
	if((hFile=_findfirst(p.assign(path).append("\\*").c_str(),&fileinfo))!=-1){
		do{
			if((fileinfo.attrib & _A_SUBDIR)){
				if(strcmp(fileinfo.name,".")!=0&&strcmp(fileinfo.name,"..")!=0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name),files);
			}
			else{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		}while(_findnext(hFile,&fileinfo)==0);
		_findclose(hFile);
	}
}

void Quick_Sort( vector<int> &array, int begin, int end ){
	//��׼�㷨˼�뼰ʵ��
	int i = begin, j = end, x = array[begin];
	while( i < j){
	while( i < j && array[j] > x )
	j--;
	if( i < j )
	array[i++] = array[j];
	while( i < j && array[i] < x )
	i++;
	if( i < j )
	array[j--] = array[i];
	}
	array[i] = x;
	if( begin < i )
	Quick_Sort( array, begin, i - 1 );
	if( end > j )
	Quick_Sort( array, i + 1, end );

	//partition�ǵݹ�ʵ�֣��ٷ���
	//if(begin >= end) return;  
	//int i = begin, j = end, x = array[(i + j)>>1],tmp =0;//�����׼ֵѡ���м��ֵ  
	//while(i <= j){//ȡ�Ⱥţ�ȷ���ֳ��������ཻ����  
	//	while( array[i] < x) i++;  
	//	while(array[j] > x) j--;  
	//	if(i <= j ){  
	//		tmp = array[i];  
	//		array[i] = array[j];  
	//		array[j] = tmp;  
	//		i++;  
	//		j--;  
	//	}       
	//}  
	//Quick_Sort(array,begin,j);  
	//Quick_Sort(array,i,end);  
}

//ȥ��һЩ��������̫�ٵ��ĵ�
void Pre_Processing::Remove_Useless_Doc( string old_dir, string new_dir ){
	vector<string> files;
	getFiles( old_dir, files );
	vector<string> temp;
	vector<int> rows;
	string str;
	for( int i = 0; i < files.size(); i++){
		ifstream in;
		ofstream out;
		in.open(files[i]);
		size_t pos = files[i].find('d');
		files[i].replace( pos, 16, "Final" );
		while(getline(in, str)){
			temp.push_back(str);
		}
		if(temp.size() > 10 ){
			out.open(files[i]);
			for( int i = 0; i < temp.size(); i++){
				out<<temp[i]<<endl;
			}
			out.close();
		}
		rows.push_back(temp.size());
		temp.clear();
		in.close();
	}
	/*Quick_Sort(rows, 0, rows.size()-1);
	for( int i = 0; i < rows.size(); i++ ){
		cout<<rows[i]<<endl;
	}*/
}

void Pre_Processing::Get_news_date( string file_name, vector<string> &news_date ){
	//��ø�·���������ĵ��ľ���·��
	vector<string> files;
	getFiles( file_name, files );
	for( int i = 0; i < files.size(); i++ ){
		ifstream in;
		in.open( files[i] );
		if( !in )
			cout<<"Open "<<files[i]<<" error!"<<endl;
		string str;
		getline( in, str );				//����url
		getline( in, str );				//����title_en
		getline( in, str );				//����title_cn
		getline( in, str );			//�õ�date
		size_t position;
		position = str.find( ' ' );			//ȷ����һ���ո��λ��
		str = str.substr( position + 1 );
		news_date.push_back( str );
		in.close();
	}
	//********************************����************************************
	//for( int i = 0; i < news_date.size(); i++ ){
	//	//if(news_date[i].size()<8)
	//	cout << news_date[i]<<"\t"<<files[i]<<endl;
	//}
}

//�����ŵ�ʱ��news_dateת��Ϊ��ֵ��ʾ
void Pre_Processing::Trans_date_to_int_date( vector<string> news_date, vector<Int_Date> &int_date ){
	for( int i = 0; i < news_date.size(); i++ ){
		int date = 0;
		size_t pos;
		pos = news_date[i].find( ' ' );										//�õ���
		date = atoi( news_date[i].substr(0,pos).c_str() ) * 372;
		//cout<<date<<'\t';
		news_date[i].erase( 0, pos+1 );
		pos = news_date[i].find( ' ' );										//�õ���
		date += atoi( news_date[i].substr(0,pos).c_str() ) * 31;
		news_date[i].erase( 0, pos+1 );
		pos = news_date[i].find( ' ' );										//�õ���
		date += atoi( news_date[i].substr(0,pos).c_str() );
		news_date[i].erase( 0, pos+1 );
		Int_Date temp;
		temp.int_date = date;
		temp.id = i;
		int_date.push_back( temp );
		//cout<<date<<endl;
	}
}

//�ó�ʱ��ֲ�����
void Pre_Processing::Time_State(vector<Int_Date> int_date){
	int temporary[1000][2];
	int k = 0;
	for( int i = 0; i < int_date.size(); i++ ){
		if( i == 0 ){
			temporary[0][0] = int_date[i].int_date;
			temporary[0][1] = 1;
		}
		else{
			if( temporary[k][0] == int_date[i].int_date )
				temporary[k][1] += 1;
			else{
				temporary[++k][0] = int_date[i].int_date;
				temporary[k][1] = 1;
			}
		}
	}
	//����ѡ����ĵ�������ʱ��ڵ�
	/*for( int i = 0; i < k + 1; i++ ){
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
	}*/
	/*for( int i = 0; i < k + 1; i++ ){
		cout<<temporary[i][0]<<"\t"<<temporary[i][1]<<endl;
	}*/
}

//����һ��������ʱ����н׶λ���
vector<int> Pre_Processing::state_division( vector<Int_Date> int_date ){
	//����ʱ���������������ͳ��
	int temporary[1000][2];
	int k = 0;
	for( int i = 0; i < int_date.size(); i++ ){
		if( i == 0 ){
			temporary[0][0] = int_date[i].int_date;
			temporary[0][1] = 1;
		}
		else{
			if( temporary[k][0] == int_date[i].int_date )
				temporary[k][1] += 1;
			else{
				temporary[++k][0] = int_date[i].int_date;
				temporary[k][1] = 1;
			}
		}
	}
	vector<double> gradient;									//�¼���չ�ı仯��
	vector<int> date;										//�¼��仯����Ӧ��ʱ��
	for( int i = 0; i < k + 1; i++ ){
		if( i == 0 ){										//�¼���һ�γ���ʱ
			gradient.push_back( (temporary[i][1] - 0) * 1.0 );
			date.push_back( temporary[i][0] );
		}
		else{
			if( temporary[i][1] - temporary[i-1][1] > 0 ){									//ֻ��¼�������ĵ�
				gradient.push_back( (temporary[i][1] - temporary[i-1][1]) * 1.0* ( temporary[i][0] - date[date.size()-1] ) / 10 );
				date.push_back( temporary[i][0] );
			}
		}
	}
	for( int i = 0; i < gradient.size(); i++ ){
		int k = i;
		for( int j = i; j < gradient.size(); j++ ){
			if( gradient[k] < gradient[j] )
				k = j;
		}
		if( k != i ){
			double temp = gradient[i];
			gradient[i] = gradient[k];
			gradient[k] = temp;
			int temp2 = date[i];
			date[i] = date[k];
			date[k] = temp2;
		}
	}
	//int max_date = date[0], min_date = date[0];												//�������ʱ��ֵ����Сʱ��ֵ
	//for( int i = 0; i < date.size(); i++ ){
	//	if( date[i] > max_date )
	//		max_date = date[i];
	//	if( date[i] < min_date)
	//		min_date = date[i];
	//}
	cout<<endl;
	for( int i = 0; i < gradient.size(); i++ ){
		cout<<gradient[i]<<'\t'<<date[i]<<endl;
	}
	return date;
}

//������ֵ�ͱ�ʾ��ʱ�䰴�մ�С�����˳������
void Pre_Processing::Select_sort( vector<Int_Date> &int_date ){
	for( int i = 0; i < int_date.size(); i++ ){
		int k = i;
		for( int j = i + 1; j < int_date.size(); j++ ){
			if( int_date[k].int_date > int_date[j].int_date )
				k = j;
		}
		if( k != i ){
			Int_Date temp;
			temp = int_date[i];
			int_date[i] = int_date[k];
			int_date[k] = temp;
		}
	}
	//********************************����************************************
	//for( int i = 0; i < int_date.size(); i++ ){
	//	//if(news_date[i].size()<8)
	//	cout << int_date[i].int_date<<endl;
	//}
}

//����ʱ�������ŷ�ΪNUM������
void Pre_Processing::Divide_C( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM ){
	int count = int_date.size() / NUM;
	for( int i = 0; i < NUM; i++ ){
		vector<Int_Date> temp;
		for( int j = 0; j < count; j++ ){
			temp.push_back( int_date[ i * count + j ] );
		}
		partition_dates.push_back( temp );
	}
}
//������ѡ�����ʱ�����л���,�����ŷ�ΪNUM�����ϣ���������partition_dates��
void Pre_Processing::divide_by_gradient( vector<int> gradient_point, vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates, int NUM ){
	vector<int> date;
	for( int i = 0; i < NUM; i++ ){
		date.push_back( gradient_point[i] );
	}
	for( int i = 0; i < date.size(); i++ ){								//��������
		int k = i;
		for( int j = i + 1; j < date.size(); j++ ){
			if( date[k] > date[j])
				k = j;
		}
		if( k != i){
			int temp = date[i];
			date[i] = date[k];
			date[k] = temp;
		}
	}
	int k = 1;
	vector<Int_Date> temp;
	for( int i = 0; i < int_date.size(); i++ ){
		if( k == NUM ){
			temp.push_back( int_date[i] );
		}
		else{
			if( int_date[i].int_date < (date[k] +date[k-1] )/2.0 ){					//���м�ֵΪ����зֶ�
				temp.push_back( int_date[ i ] );
			}
			else{
				partition_dates.push_back(temp);
				temp.clear();
				k++;
			}
		}
	}
	partition_dates.push_back(temp);
	for( int i = 0; i < NUM; i++ ){
		cout<<partition_dates[i].size()<<endl;
	}
}

//����ʱ�仮�����¼��ϣ����ý���ͬʱ��������·���һ��ķ�����ѡ��ͬһʱ���������Ľڵ㣩
	/*int_date�����������ĵ��ı���Լ���Ӧ��ʱ�䣻���partition_dates���ŷֺú���ĵ������Լ���Ӧ��ʱ��*/
void Pre_Processing::Divide_C2( vector<Int_Date> int_date, vector<vector<Int_Date>> &partition_dates ){
	int temporary[10000][2];
	int k = 0;
	for( int i = 0; i < int_date.size(); i++ ){
		if( i == 0 ){
			temporary[k][0] = int_date[i].int_date;
			temporary[k][1] = 1;
		}
		else{
			if( temporary[k][0] == int_date[i].int_date ){
				temporary[k][1]++;
			}
			else{
				k++;
				temporary[k][0] = int_date[i].int_date;
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
	/***************************************����*************************/
	for( int i = 0; i <= k; i++ ){
		cout<<temporary[i][1]<<'\t'<<temporary[i][0]<<endl;
	}
}
//��ȡ�ļ��ĺ���������Ϊ�ļ���·��
void Pre_Processing::read_file( string path, vector<string> &content){
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

//��ȡʱ��T�µ��ĵ�����:dates���Tʱ���µ��ĵ��ţ�contents���Ż�õ��ĵ�����
void Pre_Processing::read_file_T( string path, vector<Int_Date> dates, vector<string> &contents, vector<int> &time ){
	contents.clear();
	vector<string> files;
	getFiles( path, files );
	for( int i = 0; i < dates.size(); i++ ){
		read_file( files[dates[i].id], contents );
		time.push_back( dates[i].int_date );
	}
	//*************************����*******************************
	/*for( int i = 0; i < contents.size(); i++ ){
		cout<<contents[i]<<endl;
	}*/
}
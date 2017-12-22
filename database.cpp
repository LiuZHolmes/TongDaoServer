#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <Windows.h>
#include <time.h>
using namespace std;
class EigenValue
{
public:
	friend double operator * (EigenValue a, EigenValue b);
	int count[12];
	double norm[12];
	int size;
	double length;
	// 剧情，爱情，喜剧，科幻，动作，悬疑，犯罪，恐怖，青春，励志，战争，文艺

	//构造函数
	EigenValue()
	{
		size = 12;
		for (int i = 0; i < size; i++)
		{
			count[i] = 0;
		}
	}

	//计算向量长度
	void CalculateLength()
	{
		int sum = 0;
		for (int i = 0; i < size; i++)
		{
			//sum += pow(count[i], 2);
			sum += count[i];
		}
		//length = sqrt(sum);
		length = sum;
	}

	//计算坐标值
	void Update(string s)
	{
		if (s == "剧情")
		{
			count[0]++;
		}
		else if (s == "爱情")
		{
			count[1]++;
		}
		else if (s == "喜剧")
		{
			count[2]++;
		}
		else if (s == "科幻")
		{
			count[3]++;
		}
		else if (s == "动作")
		{
			count[4]++;
		}
		else if (s == "悬疑")
		{
			count[5]++;
		}
		else if (s == "犯罪")
		{
			count[6]++;
		}
		else if (s == "恐怖")
		{
			count[7]++;
		}
		else if (s == "青春")
		{
			count[8]++;
		}
		else if (s == "励志")
		{
			count[9]++;
		}
		else if (s == "战争")
		{
			count[10]++;
		}
		else if (s == "文艺")
		{
			count[11]++;
		}
		CalculateLength();
	}

	//向量单位化
	void normalize()
	{
		for (int i = 0; i < size; i++)
		{
			norm[i] = count[i] * 1.0 / length;
		}
	}
};

class Relation {
public:

	double distance[5];
	int position[5];
	string name[5];
	Relation() {
		for (int i = 0; i < 5; i++) {
			distance[i] = 100;
			position[i] = 0;
		}
	}

	double maxR() {
		double max = 0;
		for (int i = 0; i < 5; i++) {
			if (distance[i] > max) {
				max = distance[i];
			}
		}
		return max;
	}

	double minR() {
		double min = 0;
		for (int i = 0; i < 5; i++) {
			if (distance[i] < min) {
				min = distance[i];
			}
		}
		return min;
	}

	double min_position() {
		double min = 0;
		double position = 0;
		for (int i = 0; i < 5; i++) {
			if (distance[i] < min) {
				min = distance[i];
				position = i;
			}
		}
		return position;
	}
} relation[100];

//向量点乘
double operator * (EigenValue a, EigenValue b)
{
	a.normalize();
	b.normalize();
	double sum = 0;
	for (int i = 0; i < a.size; i++)
	{
		sum += (a.norm[i] * b.norm[i]);
	}
	return sum;
}

//用户
class USER
{
public:
	USER(string id)
	{
		user_id = id;
		eigenvalue = EigenValue();
	}
	string user_id;
	EigenValue eigenvalue;
	void detail()
	{
		cout << "USER >> I am " << user_id << ", my eigenvalue is:" << endl;
		cout << "USER >> (";
		for (int i = 0; i < eigenvalue.size - 1; i++)
		{
			cout << eigenvalue.count[i] << ",";
		}
		cout << eigenvalue.count[eigenvalue.size - 1] << ")" << endl;
	}
};

//
class DataBaseConfig
{
public:
	//database configuartion
	DataBaseConfig(string table_name, string user = "qyr", string passwd = "123456", string ip = "123.207.26.102", string dababase_name = "tongdao")
	{
		dbuser = user;
		dbpasswd = passwd;
		dbip = ip;
		dbname = dababase_name;
		tablename = table_name;
		con = mysql_init((MYSQL*)0);
		mysql_set_character_set(con, "gbk");
	}

	void SetFindQuery(string select, string where = "")
	{
		if (where == "")
		{
			query = "SELECT " + select + " FROM " + tablename;
		}
		else
		{
			query = "SELECT " + select + " FROM " + tablename + " WHERE " + where;
		}
	}

	void SetDeleteQuery(string where = "")
	{
		query = "DELETE FROM " + tablename + " WHERE " + where;
	}

	void SetInsertQuery(string value = "")
	{
		query = "INSERT INTO " + tablename + " VALUES(" + value + ")";
	}

	string display()
	{
		string s;
		s += "The info of this SQL config:\n";
		s += "user:" + dbuser + "\n";
		s += "passwd:" + dbpasswd + "\n";
		s += "ip:" + dbip + "\n";
		s += "name:" + dbname + "\n";
		s += "tablename:" + tablename + "\n";
		return s;
	}

	void connect()
	{
		if (con != NULL && mysql_real_connect(con, dbip.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
		{
			if (!mysql_select_db(con, dbname.c_str()))
			{
				cout << "SQL >> Select the database successfully !" << endl;
				con->reconnect = 1;

				// sprintf(tmp, "select id from user", tablename, id, password);
				rt = mysql_real_query(con, query.c_str(), strlen(query.c_str()));
				if (rt)
				{
					cout << "SQL >> query(" << query << ") failed!" << endl;
					printf("SQL >>	Error making query: %s !!!\n", mysql_error(con));
				}
				else
				{
					cout << "SQL >> query(" << query << ") succeed!" << endl;
				}
			}
		}
		else
		{
			MessageBoxA(NULL, display().c_str(), "Error", NULL);
		}
		res = mysql_store_result(con);
	}

	~DataBaseConfig()
	{
		cout << "SQL >> mysql_free_result..." << endl;
		mysql_free_result(res);
		mysql_close(con);
	}
	string dbuser;
	string dbpasswd;
	string dbip;
	string dbname;
	string tablename;
	string query;
	MYSQL * con; //= mysql_init((MYSQL*) 0); 
	MYSQL_RES *res;
	MYSQL_ROW row;
	int rt;//return value
};


void ShowUserList(vector<USER> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		v[i].detail();
	}
	return;
}

int main()
{
	/*while(1)*/
	{
		int time = clock();
		vector<USER> user_list;

		// 1. Put User List to vector <user_list>
		DataBaseConfig FindUser("user");
		//FindUser.display();
		FindUser.SetFindQuery("id");
		FindUser.connect();
		while (FindUser.row = mysql_fetch_row(FindUser.res))
		{
			/**
			* MYSQL_ROW STDCALL mysql_fetch_row(MYSQL_RES *result);
			* 检索行
			*/
			for (int t = 0; t < mysql_num_fields(FindUser.res); t++)
			{
				user_list.push_back(USER(FindUser.row[t]));
			}
		}
		ShowUserList(user_list);
		// User fetched!

		// 2. For every user, find their movies and count the tags, finally calculate their Eigenvalues
		for (int i = 0; i < user_list.size(); i++)
		{
			DataBaseConfig FindMovies("user_movie");
			//FindMovies.display();
			FindMovies.SetFindQuery("movie_name", "user_id = \"" + user_list[i].user_id + "\"");
			FindMovies.connect();
			while (FindMovies.row = mysql_fetch_row(FindMovies.res))
			{
				for (int t = 0; t < mysql_num_fields(FindMovies.res); t++)
				{
					string movie = FindMovies.row[t];
					DataBaseConfig FindTags("movie");
					//FindTags.display();
					FindTags.SetFindQuery("tag1,tag2,tag3", "movie_name = \"" + movie + "\"");
					FindTags.connect();
					while (FindTags.row = mysql_fetch_row(FindTags.res))
					{
						for (int j = 0; j < mysql_num_fields(FindTags.res); j++)
						{
							user_list[i].eigenvalue.Update(FindTags.row[j]);
						}
					}
				}
			}
		}
		ShowUserList(user_list);
		// Eigenvalues calculated!

		// 3. For every two users, calculate the dot product of their eigenvalues, after that, update their relation and put it to database

		for (int i = 0; i < user_list.size(); i++)
		{
			DataBaseConfig DeleteRelationA("relation");
			DeleteRelationA.SetDeleteQuery("first = '" + user_list[i].user_id + "' and rank = 0");
			DeleteRelationA.connect();
			DataBaseConfig DeleteRelationB("relation");
			DeleteRelationB.SetDeleteQuery("second = '" + user_list[i].user_id + "' and rank = 0");
			DeleteRelationB.connect();
			for (int j = 0; j < user_list.size(); j++)
			{
				if (j != i) {
					double DotProduct = user_list[j].eigenvalue*user_list[j].eigenvalue;
					// Update their relation and put it to database
					if (DotProduct > relation[i].minR()) {
						int position = relation[i].min_position();
						relation[i].distance[position] = DotProduct;
						relation[i].position[position] = j;
						relation[i].name[position] = user_list[j].user_id;
					}
					if (DotProduct > relation[j].minR()) {
						int position = relation[j].min_position();
						relation[j].distance[position] = DotProduct;
						relation[j].position[position] = i;
						relation[j].name[position] = user_list[i].user_id;
					}
				}
			}
			for (int j = 0; j < 5; j++)
			{
				DataBaseConfig UpdateRelation("relation");
				UpdateRelation.SetInsertQuery("'" + user_list[i].user_id + "','" + relation[i].name[j] + "',0" );
				UpdateRelation.connect();
			}
			if (relation[i].maxR() > 1 / 5)
			{
				for (int j = 0; j < 5; j++)
				{
					if (relation[i].distance[j] > 1 / 5)
					{
						DataBaseConfig UpdateRelation("relation");
						UpdateRelation.SetInsertQuery("'" + user_list[i].user_id + "','" + relation[i].name[j] + "',1");
						UpdateRelation.connect();
					}
				}
			}
		}
		int time1 = clock() - time;
		//Sleep(86400000 - time1);
	}

	system("pause");
	return 0;
}
/*
Todo:1. 完成根据特征值决定关系，并更新到数据库的部分
2. 每天要自动执行
3. 优化（保留结果？）
*/

#include <iostream>
#include <string.h>
#include <dirent.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

/*
	@Author：codekiang
	@Time：2020.11.20
	实现了 ls、pwd、cd、mkdir、rmdir 指令
*/

using namespace std;

void print(string path);
void mylist();
void mypwd();
void mycd(string s);
void mymkdir(string s);
void myrmdir(char* p, int PIndex, int iteration);

// 标记：是否属于命令
int flag;

int main(){
	while(1){
		string str;
		flag = 0;
		char t[9999];
		// 仿shell输出
		print(getcwd(t, 1024));

		getline(cin, str);

		if(str == "myexit") exit(0);
		else if(str == "mylist") mylist();
		else if(str == "mypwd") mypwd();
		else if(str.find("mycd") == 0) mycd(str);
		else if(str.find("mymkdir") == 0) mymkdir(str.erase(0,7));
		else if(str.find("myrmdir") == 0) {
			char p[999];
			// PIndex用来判断是否存在 -p
			int len, PIndex, index;
			string s = str.erase(0,7);
			index = s.find_last_of(' ');
			PIndex = s.find(" -p ");
			strncpy(p, s.substr(index+1).c_str(), 999);
			myrmdir(p, PIndex, 0);
		}

		if(!flag) cout<<"ERROR: no "<<str<<" command!"<<endl;
	}
	return 0;
}

/*
	@Param string path : 当前路径
	仿shell输出
*/
void print(string path){
	cout<<"user@ubuntu:"<<path<<"$ ";
}

/*
	输出当前文件夹的所有文件
*/
void mylist(){
	DIR *dp = NULL;
	struct dirent *dt = NULL;
	// 打开当前目录的文件夹
	dp = opendir("./");
	while(1){
		dt = readdir(dp);

		if(dt == NULL) break;
		if(strcmp(dt->d_name, "..") != 0 &&  strcmp(dt->d_name, ".") != 0){
			cout<<dt->d_name<<endl;
		}
	}
	closedir(dp);
	flag = 1;
}

/*
	获取当前路径并输出
*/
void mypwd(){
	char buffer[1024];
	getcwd(buffer, 1024);
	cout<<buffer<<endl;
	flag = 1;
}

/*
	@Param string s : 要改变的目录路径
	改变当前工作路径
*/
void mycd(string s){
	int spaceNum = 0, i;
	char buf[999], chPath[999];
	// 将字符串变成字符数组
	strcpy(buf, s.c_str());
	// 提取路径
	for(i=4; i<s.length(); i++){
		if(buf[i] != ' '){
			chPath[i-4-spaceNum] = buf[i];
		}
		else spaceNum++;
	}
	chPath[i-4-spaceNum] = '\0';

	// 改变路径
	if(chdir(chPath) == -1){
		cout<<"ERROR: PATH "<<chPath<<" no exit!"<<endl;
	}
	flag = 1;
}

/*
	@Param string s : 要创建的目录路径
	创建目录
*/
void mymkdir(string s){
	char p[999];
	int len, PIndex, index;
	index = s.find_last_of(' ');
	PIndex = s.find(" -p ");
	// 将字符数组变成字符串
	strncpy(p, s.substr(index+1).c_str(), 999);
	len = strlen(p);

	for(int i=0; i<len; i++){
		// 逐目录创建
		if(p[i] == '/'){
			p[i] = '\0';
			// 目录不存在 并且 存在 -p 就创建
			if(access(p, F_OK) != 0 && PIndex>=0 && PIndex<10){
				mkdir(p, 0777);
			}
			else{
				cout<< p <<" no exit! "<<endl;
				flag = 1;
				return;
			}
			p[i] = '/';
		}
	}
	// 创建最里面的文件夹
	if(len > 0 && access(p, F_OK) != 0){
		mkdir(p, 0777);
	}
	flag = 1;
}

/*
	@Param char* p : 文件路径或文件名
	@Param int PIndex : -p 的位置
	@Param int iteration : 该函数循环调用的次数，防止-p循环删除时输出 isn't empty!
*/
void myrmdir(char* p, int PIndex, int iteration){
	// 文件夹是否存在
	if(access(p, F_OK) == 0){
		struct dirent *dt;
		dt = readdir(opendir(p));
		// 一开始与".."相等表示文件夹为空
		if(strcmp(dt->d_name, "..")){
			if(iteration == 0) cout<< p <<" isn't empty! "<<endl;
		}
		else{
			rmdir(p);
			// 如果存在-p 则循环删除空目录
			if(PIndex<10 && PIndex>=0){
				int len = strlen(p);
				for(int i=len-1; i>=0; i--){
					if(p[i] == '/'){
						p[i] = '\0';
						myrmdir(p, PIndex, iteration+1);
					}
				}
			}
			
		}
	}
	else{
		cout<< p <<" no exit! "<<endl;
	}
	flag = 1;
}
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

// 标记：是否属于命令
int flag;

void print(string path);
void mylist();
void mypwd();
void mycd(string s);
void mymkdir(string s);
void myrmdir(string s);

int main(){
	string str;
	while(1){
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
		else if(str.find("myrmdir") == 0) myrmdir(str.erase(0,7));

		if(!flag) cout<<"ERROR: no "<<str<<" command!"<<endl;
	}
	return 0;
}

void print(string path){
	cout<<"user@ubuntu:"<<path<<"$ ";
}

void mylist(){
	DIR *dp = NULL;
	struct dirent *dt = NULL;
	// 打开当前目录的文件夹
	dp = opendir("./");
	while(1){
		dt = readdir(dp);

		if(dt == NULL) break;
		if(dt->d_name[0] != '.'){
			cout<<dt->d_name<<endl;
		}
	}

	closedir(dp);
	flag = 1;
}

void mypwd(){
	char buffer[1024];
	// 获取当前路径
	getcwd(buffer, 1024);
	cout<<buffer<<endl;
	flag = 1;
}

void mycd(string s){
	int spaceNum = 0;
	char buf[999], chPath[999];
	// 将字符串变成字符数组
	strcpy(buf, s.c_str());
	// 提取路径
	for(int i=5; i<s.length(); i++){
		if(buf[i] != ' '){
			chPath[i-5-spaceNum] = buf[i];
		}
		else spaceNum++;
	}
	// 改变路径
	if(chdir(chPath) == -1){
		cout<<"ERROR: PATH "<<chPath<<" no exit!"<<endl;
	}
	flag = 1;
}

void mymkdir(string s){
	char p[999];
	int len, PIndex, index;
	index = s.find_last_of(' ');
	PIndex = s.find(" -p ");

	strncpy(p, s.substr(index+1).c_str(), 999);
	len = strlen(p);

	for(int i=0; i<len; i++){
		if(p[i] == '/'){
			p[i] = '\0';
			if(access(p, 0) != 0 && PIndex>=0 && PIndex<10){
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
	if(len > 0 && access(p, 0) != 0){
		mkdir(p, 0777);
	}
	flag = 1;
}

void myrmdir(string s){
	char p[999];
	int len, PIndex, index;
	index = s.find_last_of(' ');
	PIndex = s.find(" -p ");

	strncpy(p, s.substr(index+1).c_str(), 999);
	len = strlen(p);

	struct _stat fileStat;

	if(access(p, 0) == 0 && stat(p, &fileStat) == 0 && fileStat.st_mode && _S_IFDIR){
		if(PIndex>=0 && PIndex<10){

		}
		else rmdir(p);
	}
	else{
		cout<< p <<" no exit! "<<endl;
		flag = 1;
		return;
	}
	for(int i=0; i<len; i++){
		if(p[i] == '/'){
			p[i] = '\0';
			if(access(p, 0) != 0 && PIndex>=0 && PIndex<10){
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
	flag = 1;
}
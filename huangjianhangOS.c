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
void myrmdir(char* p, int PIndex, int iteration);

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
	if(len > 0 && access(p, F_OK) != 0){
		mkdir(p, 0777);
	}
	flag = 1;
}

void myrmdir(char* p, int PIndex, int iteration){
	if(access(p, F_OK) == 0){
		struct dirent *dt;
		dt = readdir(opendir(p));
		// exist dir or file
		if(strcmp(dt->d_name, "..")){
			if(iteration == 0) cout<< p <<" isn't empty! "<<endl;
		}
		else{
			rmdir(p);
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
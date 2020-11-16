#include <iostream>
#include <string.h>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h>


using namespace std;

int flag = 0;

void print(string path);
void mylist();
void mypwd();
void mycd(string s);


int main(){
	string str;
	while(1){
		char t[1024];
		print(getcwd(t, 1024));

		getline(cin, str);

		if(str == "myexit") exit(0);
		if(str == "mylist") mylist();
		if(str == "mypwd") mypwd();
		if(str.find("mycd") == 0) mycd(str);
		if(!flag) cout<<"no "<<str<<" command!"<<endl;
	}

	return 0;
}

void print(string path){
	cout<<"user@ubuntu:"<<path<<"$ ";
}

void mylist(){
	DIR *dp = NULL;
	struct dirent *dt = NULL;

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
	getcwd(buffer, 1024);
	cout<<buffer<<endl;
	flag = 1;
}

void mycd(string s){
	char buf[999], chPath[999];
	strcpy(buf, s.c_str());
	for(int i=5; i<s.length(); i++){
		chPath[i-5] = buf[i];
	}
	chdir(chPath);
	flag = 1;
}

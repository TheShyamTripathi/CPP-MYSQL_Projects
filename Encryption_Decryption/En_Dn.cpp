#include<iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include<windows.h>

using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "your password";
const char* DB = "my_cpp";

class login{
	private:
		string userId, userPW;
	public: 
		login(): userId(""), userPW(""){}
	
	void setId(string id){
		userId = id;
	}
	
	void setPW(string pw){
		userPW = pw;
	}
	
	string getId() const{
	return userId;
	}
	
	string getPW() const{
	return userPW;
	}
};

char encryptch(char ch, int shift){
	if(isalpha(ch)){
		char base = isupper(ch) ? 'A' : 'a';
		char encrypted_ch = (ch - base +shift + 26) % 26 + base;
		return encrypted_ch;
	}
	if(isdigit(ch)){
		char encrypted_ch = (ch - '0' + shift + 10)% 10 + '0';
		return encrypted_ch;
	}
	
	return ch;
}

string encrypted(const string& password, int shift){
	string encrypted = "";
	for(int i=0; i<password.length(); i++){
		char ch = password[i];
		char encryptedChar = encryptch(ch, shift);
		encrypted += encryptedChar;
	}
	return encrypted;
} 

char decryptCh(char ch, int shift){
	if(isalpha(ch)){
		char base = isupper(ch) ? 'A' : 'a';
		char decrypted_ch = (ch-base-shift +26)% 26 + base;
		return decrypted_ch;
	}
	else if(isdigit(ch)){
		char decrypted_ch = (ch - '0'- shift + 10) % 10 + '0';
		return decrypted_ch;
	}
	else {
		return ch;
	}
}

string decrypt (const string& encrypted, int shift){
	string decrypted = "";
	for(size_t i = 0; i < encrypted.length(); i++){
		char ch = encrypted[i];
		char decryptedChar = decryptCh(ch, shift);
		decrypted += decryptedChar;
	}
	return decrypted;
}

string DBpw(MYSQL* conn, const string id){
	string encryptedPW;
	string get = "SELECT PW FROM password WHERE Id = '"+id+"' ";
	if(mysql_query(conn, get.c_str())){
		cout << "Error: " << mysql_error(conn) << endl;
	}
	else{
		MYSQL_RES* res;
		res = mysql_store_result(conn);
		if(res){
			MYSQL_ROW row = mysql_fetch_row(res);
			if(row){
				encryptedPW = row[0];
			}
		}
	}
	return encryptedPW;
}

int main(){
	login l;
	
	MYSQL* conn;
	conn = mysql_init(NULL);
	
	if(!mysql_real_connect(conn, HOST, USER, PW, DB, 3306 , NULL, 0)){
		cout << "Error: " << mysql_error(conn) << endl;
	}
	else{
		cout << "Logged In Database!" << endl;
	}
	Sleep(3000);
	int shift = 7;
	bool exit = false;
	while(!exit){
		system("cls");
	
	cout << "1. Signup." << endl;
	cout << "2. SignIn."<< endl;
	cout << "0. Exit."<< endl;
	cout << "Enter Your Choice: ";
	int val;
	cin >> val;
	
	if(val ==1){
		system("cls");
		string id, pw;
	cout << "Enter ID for Signup: ";
	cin >> id;
	l.setId(id);
	cout << "Enter A Srong Password: ";
	cin >> pw;
	l.setPW(pw);
	
	string encryptedPW = encrypted(l.getPW(), shift);
	
	string Sup = "INSERT INTO password (Id, PW) VALUES ('"+l.getId()+"', '"+encryptedPW+"')";
	if(mysql_query(conn, Sup.c_str())){
		cout << "Error: "<< mysql_error(conn) << endl;
	}
	else{
		cout << "Signup Successfully" << endl;
	}
	Sleep(3000);
	} // if 1
	
	else if(val == 2){
		system("cls");
		string id, pw;
		cout << "Enter ID: ";
		cin >> id;
		cout << "Enter Your password: ";
		cin >> pw;
		string getDB = DBpw(conn,id);
		
		if(!getDB.empty()){
			string decryptedPW = decrypt(getDB, shift);
			
			if(decryptedPW == pw){
				cout<< "welcome" << endl;
			}
			else{
				cout << "Incorrect Password. Try Again!"<< endl;
			}
		}
		else{
			cout << "User ID not found Try Again!";
		}
		Sleep(5000);
	} // if 2
	
	else if( val == 0){
		exit = true;
		cout << "Bye!" << endl;
	}
	else{
		cout << "Invalid Input" << endl;
	}
	
}
	return 0;
}

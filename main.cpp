#include <iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include<windows.h>
#include<sstream>

using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "your_password";
const char* DB = "my-cpp";

class University {
    private:
        int Id;
        string Name, Subject;
        float CGPA;
    public:
        // Constructor
        University() : Id(0), Name(""), Subject(""), CGPA(0.0) {}

        // Setter functions
        void setId(int id) {
            Id = id;
        }

        void setName(string name) {
            Name = name;
        }

        void setSubject(string subject) {
            Subject = subject;
        }

        void setGrade(float cgpa) {
            CGPA = cgpa;
        }

        // Getter functions
        int getId() {
            return Id;
        }

        string getName() {
            return Name;
        }

        string getSubject() {
            return Subject;
        }

        float getGrade() {
            return CGPA;
        }
};


void insert(MYSQL* conn, University u) {
    int id;
    string name, subject;
    float cgpa;
    
    cout << "Enter Id: ";
    cin >> id;
    u.setId(id);
    
    cout << "Enter Name: ";
    cin.ignore();  // Ignore the newline character left in the input buffer
    getline(cin, name);  // Use getline to capture the full name with spaces
    u.setName(name);
    
    cout << "Enter Subject: ";
    getline(cin, subject);  // Use getline for the subject as well
    u.setSubject(subject);
    
    cout << "Enter CGPA: ";
    cin >> cgpa;
    u.setGrade(cgpa);
    
    // Convert int and float to string
    int iId = u.getId();
    float fGrade = u.getGrade();
    
    stringstream ss;
    ss << iId;
    string sId = ss.str();
    
    stringstream as;
    as << fGrade;
    string sGrade = as.str();
    
    // Form the SQL query string
    string ins = "INSERT INTO student (Id, Name, Subject, CGPA) VALUES('" + sId + "','" + u.getName() + "','" + u.getSubject() + "','" + sGrade + "')";
    
    // Execute the query and check for errors
    if (mysql_query(conn, ins.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "Data Inserted Successfully." << endl;
    }
    
    Sleep(4000);  // Pause for 4 seconds (Windows-specific)
}

// Show Student data Function

showRecord(MYSQL* conn){
	string show= "SELECT * FROM student";
	
	if(mysql_query(conn,show.c_str())){
		cout<<"Error: "<<mysql_error(conn)<<endl;
	}
	else{
		MYSQL_RES* res;
		res = mysql_store_result(conn); // res = result 
		if(res){
			int num = mysql_num_fields(res);
			MYSQL_ROW row;
			int i;
			while(row= mysql_fetch_row(res)){
				
				for(i=0; i<num; i++){
					cout<<" "<<row[i];
				}
				cout<<endl;
			}
		}
	}
	
	Sleep(7000);
}

// Search data Function 

void search(MYSQL* conn){
	int id;
	cout<<"Enter Student ID: ";
	cin>>id;
	
	stringstream ss;
	ss<<id;
	string sId = ss.str();
	string show = "SELECT * FROM student WHERE Id = '"+sId+"' ";
	if(mysql_query(conn, show.c_str())){
		cout<<"Error: "<<mysql_error(conn)<<endl;
	}
	else{
		MYSQL_RES* res;
		res = mysql_store_result(conn);
		if(res){
			MYSQL_ROW row = mysql_fetch_row(res);
		if(row){
			int num = mysql_num_fields(res);
			for(int i=0; i<num; i++){
				cout<<" "<<row[i];
			}
		}
		mysql_free_result(res);
	}
}
 Sleep(5000);
}

// Data updation function 
update(MYSQL* conn, University u){
	int id;
	cout<<"Enter Id: ";
	cin>> id;
	
	int iId = u.getId();
	stringstream ss;
	ss<<id;
	string sId = ss.str();
	
	string subject;
	cout<<"Enter New Subjects: ";
	cin>> subject;
	u.setSubject(subject);
	
	string upd = "UPDATE student SET Subject = '"+u.getSubject()+"' WHERE Id ='"+sId+"'";
	if(mysql_query(conn,upd.c_str())){
cout<<"Error: "<<mysql_error(conn)<<endl;
}

else{
 cout<<"Data Updated Successfuly"<<endl;
}
Sleep(3000);
}

// Function to delete the data of student 
deleteS(MYSQL* conn){
	int id;
	cout <<"Enter student ID: ";
	cin>>id;
	
	stringstream ss;
	ss<<id;
	string sId = ss.str();
	
	string del = "DELETE FROM student WHERE Id = '"+sId+"'";
	 if(mysql_query(conn,del.c_str())){
cout<<"Error: "<<mysql_error(conn)<<endl;
}

else{
 cout<<"Data Delted Successfuly"<<endl;
}
Sleep(3000);
}


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	University u;
	MYSQL* conn;
	conn = mysql_init(NULL);
	
	if(!mysql_real_connect(conn,HOST,USER,PW,DB,3306,NULL,0))
{
	cout<<"Error: "<<mysql_error(conn)<<endl;
	}	
	else{
		cout<<"Logged in!"<<endl;
	}
	Sleep(3000);
	
	bool exit = false;
	while(!exit){
		system("cls");
		int val;
		cout<<"Welcome to University management System"<<endl;
		cout<<"******************************************"<<endl;
		cout<<"1. Insert Student Data"<<endl;
		cout<<"2. Show Student Data"<<endl;
		cout<<"3. Search Student Data"<<endl;
		cout<<"4. Update Student Data"<<endl;
		cout<<"5. Delete Student Data"<<endl;
		cout<<"0. Exit"<<endl;
		cout<<"Enter choice: ";
		cin>>val;
		
		cout<< endl;
		if(val==1){
			insert(conn,u);
		}
		else if(val==2){
			showRecord(conn);
		}
		else if(val==3){
			search(conn);
		}		
		else if(val==4){
			update(conn,u);
		}
		else if(val==5){
			deleteS(conn);
		}
		else if (val==0){
			exit = true;
			cout<<"Good Day!"<<endl;
			Sleep(3000);
		}
		else{
			cout<<"Invalid Input"<<endl;
			Sleep(2000);
		}

	}
	
	return 0;
}

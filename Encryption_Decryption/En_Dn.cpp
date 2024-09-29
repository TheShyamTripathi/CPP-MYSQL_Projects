#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <windows.h>

using namespace std;

// Constants for MySQL connection
const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "Shyamsql@123";
const char* DB = "my_cpp";

// Class to handle user login
class Login {
private:
    string userId;  // Stores the user ID
    string userPW;  // Stores the user password

public:
    // Default constructor initializes userId and userPW to empty strings
    Login() : userId(""), userPW("") {}

    // Setter function for userId
    void setId(string id) {
        userId = id;
    }

    // Setter function for userPW
    void setPW(string pw) {
        userPW = pw;
    }

    // Getter function for userId
    string getId() const {
        return userId;
    }

    // Getter function for userPW
    string getPW() const {
        return userPW;
    }
};

// Function to encrypt a single character with a shift
char encryptChar(char ch, int shift) {
    if (isalpha(ch)) {
        // Encrypt letters (A-Z or a-z)
        char base = isupper(ch) ? 'A' : 'a';
        char encrypted_ch = (ch - base + shift + 26) % 26 + base;
        return encrypted_ch;
    }
    if (isdigit(ch)) {
        // Encrypt digits (0-9)
        char encrypted_ch = (ch - '0' + shift + 10) % 10 + '0';
        return encrypted_ch;
    }
    
    // Return the character as-is if it's not a letter or digit
    return ch;
}

// Function to encrypt the entire password string
string encrypt(const string& password, int shift) {
    string encrypted = "";
    for (size_t i = 0; i < password.length(); i++) {
        char ch = password[i];
        char encryptedChar = encryptChar(ch, shift);
        encrypted += encryptedChar;
    }
    return encrypted;
}

// Function to decrypt a single character with a shift
char decryptChar(char ch, int shift) {
    if (isalpha(ch)) {
        // Decrypt letters (A-Z or a-z)
        char base = isupper(ch) ? 'A' : 'a';
        char decrypted_ch = (ch - base - shift + 26) % 26 + base;
        return decrypted_ch;
    } else if (isdigit(ch)) {
        // Decrypt digits (0-9)
        char decrypted_ch = (ch - '0' - shift + 10) % 10 + '0';
        return decrypted_ch;
    } else {
        // Return the character as-is if it's not a letter or digit
        return ch;
    }
}

// Function to decrypt the entire encrypted password string
string decrypt(const string& encrypted, int shift) {
    string decrypted = "";
    for (size_t i = 0; i < encrypted.length(); i++) {
        char ch = encrypted[i];
        char decryptedChar = decryptChar(ch, shift);
        decrypted += decryptedChar;
    }
    return decrypted;
}

// Function to retrieve the encrypted password from the database
string getDBPassword(MYSQL* conn, const string id) {
    string encryptedPW;
    string query = "SELECT PW FROM password WHERE Id = '" + id + "' ";
    
    // Execute the query
    if (mysql_query(conn, query.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        MYSQL_RES* res;
        res = mysql_store_result(conn); // Store the result of the query
        
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                encryptedPW = row[0]; // Get the encrypted password from the result
            }
        }
    }
    return encryptedPW;
}

int main() {
    Login l;
    
    // Initialize MySQL connection
    MYSQL* conn;
    conn = mysql_init(NULL);
    
    // Connect to the MySQL database
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "Logged In Database!" << endl;
    }
    
    Sleep(3000);  // Pause for 3 seconds (Windows-specific)
    int shift = 7;  // Shift value for encryption/decryption
    bool exit = false;  // Loop control variable
    
    // Main loop for handling user interactions
    while (!exit) {
        system("cls");  // Clear the console screen
        
        // Display menu options
        cout << "1. Signup." << endl;
        cout << "2. SignIn." << endl;
        cout << "0. Exit." << endl;
        cout << "Enter Your Choice: ";
        int val;
        cin >> val;
        
        if (val == 1) {
            // Signup process
            system("cls");
            string id, pw;
            cout << "Enter ID for Signup: ";
            cin >> id;
            l.setId(id);
            cout << "Enter A Strong Password: ";
            cin >> pw;
            l.setPW(pw);
            
            // Encrypt the password
            string encryptedPW = encrypt(l.getPW(), shift);
            
            // Insert the ID and encrypted password into the database
            string signupQuery = "INSERT INTO password (Id, PW) VALUES ('" + l.getId() + "', '" + encryptedPW + "')";
            if (mysql_query(conn, signupQuery.c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                cout << "Signup Successfully" << endl;
            }
            Sleep(3000);
        } 
        
        else if (val == 2) {
            // SignIn process
            system("cls");
            string id, pw;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Your Password: ";
            cin >> pw;
            
            // Get the encrypted password from the database
            string encryptedDBPassword = getDBPassword(conn, id);
            
            if (!encryptedDBPassword.empty()) {
                // Decrypt the password retrieved from the database
                string decryptedPW = decrypt(encryptedDBPassword, shift);
                
                // Compare the decrypted password with the entered password
                if (decryptedPW == pw) {
                    cout << "Welcome!" << endl;
                } else {
                    cout << "Incorrect Password. Try Again!" << endl;
                }
            } else {
                cout << "User ID not found. Try Again!" << endl;
            }
            Sleep(5000);
        } 
        
        else if (val == 0) {
            // Exit the program
            exit = true;
            cout << "Bye!" << endl;
        } 
        
        else {
            // Invalid input handling
            cout << "Invalid Input" << endl;
        }
    }
    
    return 0;
}


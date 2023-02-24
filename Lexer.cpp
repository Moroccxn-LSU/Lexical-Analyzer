// Adam Elkhanoufi
// 02/23/2023
// Lexical Analyzer for a simple programming language
// C++

#include <iostream>         // standard I/O functions
#include <fstream>          // file I/O functions
#include <sstream>          // string I/O functions
#include <string>           // more complex string methods
#include <vector>           // vector data structure and its methods
#include <unordered_map>    // unordered_map data structure and its methods
#include <algorithm>        // find() function
#include <cctype>           // isalpha(), isdigit(), and isalnum functions

using namespace std;

//Global flag to check if there is an invalid identifier name
bool ERROR_FLAG = false;

//Lexer class that initializes the Symbol-Token Reference Map, the vector of tokenized lexemes, and the class methods
class Lexer{
    public:
        unordered_map<string, string> tokenMap; //Symbol-Token Reference Map
        vector<string> tokens;                  //Vector of tokenized lexemes 

        void tokenize(string lexemes);          //tokenize function definition
        void readFile(string fileName);         //readFile function definition
        void printTokens();                     //printTokens function definition

        //Constructor that initializes the Symbol-Token Reference Map
        Lexer(){
            tokenMap["if"] = "IF";
            tokenMap["for"] = "FOR";
            tokenMap["while"] = "WHILE";
            tokenMap["procedure"] = "PROC";
            tokenMap["return"] = "RETURN";
            tokenMap["int"] = "INT";
            tokenMap["else"] = "ELSE";
            tokenMap["do"] = "DO";
            tokenMap["break"] = "BREAK";
            tokenMap["end"] = "END";
            tokenMap["="] = "ASSIGN";
            tokenMap["+"] = "ADD_OP";
            tokenMap["-"] = "SUB_OP";
            tokenMap["*"] = "MUL_OP";
            tokenMap["/"] = "DIV_OP";
            tokenMap["%"] = "MOD_OP";
            tokenMap[">"] = "GT";
            tokenMap["<"] = "LT";
            tokenMap[">="] = "GE";
            tokenMap["<="] = "LE";
            tokenMap["++"] = "INC";
            tokenMap["("] = "LP";
            tokenMap[")"] = "RP";
            tokenMap["{"] = "LB";
            tokenMap["}"] = "RB";
            tokenMap["|"] = "OR";
            tokenMap["&"] = "AND";
            tokenMap["=="] = "EE";
            tokenMap["!"] = "NEG";
            tokenMap[","] = "COMMA";
            tokenMap[";"] = "SEMI";
        }

        //Destructor that clears the token map and the tokens vector
       ~Lexer(){
            tokens.clear();
            tokenMap.clear();
        }
};

int main(){
    Lexer lexer;
    string fileName;
    cin >> fileName;
    lexer.readFile(fileName);
    lexer.printTokens();
    return 0;
}

//Tokenizes a continuous string of lexemes
void Lexer::tokenize(string lexemes) {
    //If the entire lexemes string happens to be a keyword, add it to the tokens vector and proceed to the next string of lexemes
    if (tokenMap.find(lexemes) != tokenMap.end()) {
        tokens.push_back(tokenMap[lexemes]);
    }else{ //Otherwise loop through the lexemes string character by character
        // Initialize variables to keep track of the current and next character, the current token, and the type of token
        string token = "";
        char curr, next;
        bool isNum = false;
        bool isIdent = false;
        int i = 0;

        while (i < lexemes.length()) {
            // Set the current and next character
            curr = lexemes[i];
            //Check if the next character exists, otherwise set it to null
            if (i + 1 < lexemes.length()){
                next = lexemes[i + 1];
            }else{
                next = '\0';
            }
            //Check if the current character is a letter
            if (isalpha(curr)){
                //Check if the current token is a number, if so then it is an invalid identifier name
                if (isNum){
                    ERROR_FLAG = true;  //Set the error flag to true to disrupt the program on the next getline() call
                    tokens.push_back("SYNTAX ERROR: INVALID IDENTIFIER NAME");
                    token = "";
                    break;
                }
                //Set the current token to the ident flag and add the current character to the token
                isIdent = true;
                isNum = false;
                token += curr;
            }else if (isdigit(curr)) {  //Check if the current character is a digit
                //If current token is an identifier then continue to add the current character to the token
                if (isIdent){
                    token += curr;
                }else{  //Otherwise it means we are starting a new token, so set the current token to the number flag
                    isNum = true;
                }
            }else if(!isalnum(curr)){   //Redundant check to see if current character is not alphanumeric
                //If the current token hasnt been caught by the keyword check, then it is either an identifier or a number
                //Check which one it is, add the appropriate token to the vector, and reset the flags
                if (isIdent && !isNum){
                    tokens.push_back("IDENT");
                    isIdent = false;
                }else if (isNum){
                    tokens.push_back("INT_CONST");
                     isNum = false;
                }
                token = "";
                token += curr;  //Add the current character to the token string

                //Check for the edge case of the double character tokens (ex: >=, ++, <=) by looking at the next character
                if (!isalnum(next)){
                    token += next;
                    //If the double char token exists add it to the vector and skip the next character since it was already added
                    if (tokenMap.find(token) != tokenMap.end()){
                        tokens.push_back(tokenMap[token]);
                        i++;
                    }else{  //Otherwise revert back to the original token 
                        token = "";
                        token += curr;
                        if (tokenMap.find(token) != tokenMap.end()){
                            tokens.push_back(tokenMap[token]);
                        }else{
                            tokens.push_back("ERROR UNKNOWN TOKEN");
                            ERROR_FLAG = true;
                            break;
                        }
                    }
                }else{  //Otherwise add the single non alphanumeric char to the token vector
                    if (tokenMap.find(token) != tokenMap.end()){
                        tokens.push_back(tokenMap[token]);
                    }else{
                        tokens.push_back("ERROR UNKNOWN TOKEN");
                        ERROR_FLAG = true;
                        break;
                    }
                }
                token = "";
            }
            //Check for keywords at each iteration to add them to the token vector and reset both flags
            if (token != "" && tokenMap.find(token) != tokenMap.end()) {
                tokens.push_back(tokenMap[token]);
                token = "";
                isIdent = false;
                isNum = false;
            }
            i++;
        }
        //Check for the edge case where a single character identifier also happens to be the entire lexemes string (ex: i)
        if (token != "" && !ERROR_FLAG){
            tokens.push_back("IDENT");
            token = "";
        }
    }
}

//Reads in a file to tokenize chunks of lexemes
void Lexer::readFile(string fileName){
    ifstream file(fileName);
    string line, lexemes; 
    
    if(file.is_open()){
        //Read each line of the file and store it into a string stream for the next while loop to use
        while (getline(file, line)){
            stringstream ss(line);
            //Read in a chunk of lexemes from the stringstream by using getline() and a space as a delimiter
            //Check if syntax error has been caught, if so then break out the loops to stop the program
            while(getline(ss, lexemes, ' ') && !ERROR_FLAG){
                //Remove all whitespaces
                lexemes.erase(remove_if(lexemes.begin(), lexemes.end(), ::isspace), lexemes.end());

                //Tokenize Lexemes if exists
                if (lexemes != "") {
                    tokenize(lexemes);
                }
            }
            //Clear stringstream for next line
            ss.clear();
        }
        file.close();
    }else { //Default error if file cannot be opened
        cout << "[Error] Unable to open file";
    }
}

//Prints the tokens in the vector to standard output
void Lexer::printTokens(){
    for (string token : tokens) {
        cout << token << endl;
    }
};

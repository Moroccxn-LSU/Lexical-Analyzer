// Adam Elkhanoufi
// 898674229

import java.util.Scanner;               // Scanner class for reading input
import java.util.ArrayList;             // ArrayList data structure and its methods
import java.util.HashMap;               // HashMap data structure and its methods
import java.io.File;                    // file I/O functions
import java.io.FileNotFoundException;   // try catch for file I/O

public class Lexer {
    private HashMap<String, String> tokenMap;   // HashMap to store the predefined symbol/token pairs
    private ArrayList<String> tokens;           // ArrayList to store the tokens
    boolean ERROR_FLAG = false;                 // Flag to check for syntax errors
    
    // Constructor
    public Lexer() {
        tokenMap = new HashMap<String, String>();   // Initialize tokenMap
        tokens = new ArrayList<String>();           // Initialize tokens list

        // Populate tokenMap with predefined symbols and their corresponding tokens
        tokenMap.put("if", "IF");
        tokenMap.put("for", "FOR");
        tokenMap.put("while", "WHILE");
        tokenMap.put("procedure", "PROC");
        tokenMap.put("return", "RETURN");
        tokenMap.put("int", "INT");
        tokenMap.put("else", "ELSE");
        tokenMap.put("do", "DO");
        tokenMap.put("break", "BREAK");
        tokenMap.put("end", "END");
        tokenMap.put("=", "ASSIGN");
        tokenMap.put("+", "ADD_OP");
        tokenMap.put("-", "SUB_OP");
        tokenMap.put("*", "MUL_OP");
        tokenMap.put("/", "DIV_OP");
        tokenMap.put("%", "MOD_OP");
        tokenMap.put(">", "GT");
        tokenMap.put("<", "LT");
        tokenMap.put(">=", "GE");
        tokenMap.put("<=", "LE");
        tokenMap.put("++", "INC");
        tokenMap.put("(", "LP");
        tokenMap.put(")", "RP");
        tokenMap.put("{", "LB");
        tokenMap.put("}", "RB");
        tokenMap.put("|", "OR");
        tokenMap.put("&", "AND");
        tokenMap.put("==", "EE");
        tokenMap.put("!", "NEG");
        tokenMap.put(",", "COMMA");
        tokenMap.put(";", "SEMI");
    }
    
    // Method to tokenize the given string of lexemes
    public void tokenize(String lexemes) {
        // If the entire lexemes string happens to be a keyword, add it to the tokens list and proceed to the next string of lexemes
        if (tokenMap.containsKey(lexemes)) {
            tokens.add(tokenMap.get(lexemes));
        } else { // Otherwise loop through the lexemes string character by character
            // Initialize variables to keep track of the current and next character, the current token, and the type of token
            String token = "";
            char curr, next;
            boolean isNum = false;
            boolean isIdent = false;
            int i = 0;

            while (i < lexemes.length()) {
                // Set the current and next character
                curr = lexemes.charAt(i);
                // Check if the next character exists, otherwise set it to null
                if (i + 1 < lexemes.length()) {
                    next = lexemes.charAt(i + 1);
                } else {
                    next = '\0';
                }
                // Check if the current character is a letter
                if (Character.isLetter(curr)) {
                    // Check if the current token is a number, if so then it is an invalid identifier name
                    if (isNum) {
                        ERROR_FLAG = true;  // Set the error flag to true to disrupt the program on the next getline() call
                        tokens.add("SYNTAX ERROR: INVALID IDENTIFIER NAME");
                        token = "";
                        break;
                    }
                    // Set the current token to the ident flag and add the current character to the token
                    isIdent = true;
                    isNum = false;
                    token += curr;
                } else if (Character.isDigit(curr)) {  // Check if the current character is a digit
                    // If current token is an identifier then continue to add the current character to the token
                    if (isIdent) {
                        token += curr;
                    } else {  // Otherwise it means we are starting a new token, so set the current token to the number flag
                        isNum = true;
                    }
                } else if (!Character.isLetterOrDigit(curr)) {   // Redundant check to see if current character is not alphanumeric
                    // If the current token hasnt been caught by the keyword check, then it is either an identifier or a number
                    // Check which one it is, add the appropriate token to the list, and reset the flags
                    if (isIdent && !isNum) {
                        tokens.add("IDENT");
                        isIdent = false;
                    } else if (isNum) {
                        tokens.add("INT_CONST");
                        isNum = false;
                    }
                    token = "";
                    token += curr;  // Add the current character to the token string

                    // Check for the edge case of the double character tokens (ex: >=, ++, <=) by looking at the next character
                    if (!Character.isLetterOrDigit(next)) {
                        token += next;
                        // If the double char token exists add it to the list and skip the next character since it was already added
                        if (tokenMap.containsKey(token)) {
                            tokens.add(tokenMap.get(token));
                            i++;
                        } else {  // Otherwise revert back to the original token 
                            token = "";
                            token += curr;
                            tokens.add(tokenMap.get(token));
                        }
                    } else {  // Otherwise add the single non alphanumeric char to the token list
                        tokens.add(tokenMap.get(String.valueOf(curr)));
                    }
                    token = "";
                }
                //Check for keywords at each iteration to add them to the token vector and reset both flags
                if (token != "" && tokenMap.containsKey(token)){
                    tokens.add(tokenMap.get(token));
                    token = "";
                    isIdent = false;
                    isNum = false;
                }
                i++;
            }
            //Check for the edge case where a single character identifier also happens to be the entire lexemes string (ex: i)
            if (token != ""){
                tokens.add("IDENT");
                token = "";
            }
        }
    }

    // Method to read and tokenize the contents of the given file
    public void readFile(String fileName) {
        try {
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {     //Read each line of the file
                String line = scanner.nextLine();
                String[] lexemes = line.split(" "); // Split the line into an array of lexemes using a space as a delimiter
                for (String lexeme : lexemes) {
                    lexeme = lexeme.trim(); // Remove any leading or trailing white space
                    if (!lexeme.isEmpty()) {
                        tokenize(lexeme);
                    }
                    // If the error flag is set, then break out of the loop
                    if (ERROR_FLAG) {break;}
                }
            }
            scanner.close();
        } catch (FileNotFoundException e) {
            System.err.println("[Error] Unable to open file");
        }
    }
    
    // Method to print the tokens list
    public void printTokens() {
        for (String token : tokens) {
            System.out.print(token + "\n");
        }
        System.out.println();
    }
    
    //Main method to run the program
    public static void main (String[] args) {
        Scanner scanner = new Scanner(System.in);
        String fileName = "";
        System.out.print("Enter the name of the file to be tokenized: ");
        fileName = scanner.nextLine();
        Lexer lexer = new Lexer();
        lexer.readFile(fileName);
        lexer.printTokens();
        lexer.tokenMap.clear();
        lexer.tokens.clear();
        scanner.close();
    }
}
Lexical Analyzer in C++ and Java
================================
### C++ Lexical Analyzer compilation commands
```bash
$ g++ -std=c++11 Lexer.cpp -o lex
$ ./lex
```

### Java Lexical Analyzer compilation commands
```bash
$ javac Lexer.java
$ java Lexer
```
*This was created as a Programming Languages project, where we were given source code in a simple programming language and predefined symbol/token pairs. I originally coded the Lexical Analyzer in C++ using a "build and check" algorithm, then I used OpenAI's chatGPT to convert the Lexer class and it's methods to Java piece by piece. It took many queries and a good bit of manual debugging/editing, but eventually the Java version worked as intended. If you want to create a more accurate Lexical Analyzer use separate data structures for handling identifiers, operators, and keywords. Add symbol/token pairs to tokenMap to add keywords and operators for a specific language.*

import bg.tu_varna.kst_sit.ci_ep.exceptions.LexicalException;
import bg.tu_varna.kst_sit.ci_ep.lexer.Lexer;
import bg.tu_varna.kst_sit.ci_ep.lexer.token.Token;
import bg.tu_varna.kst_sit.ci_ep.source.Source;
import bg.tu_varna.kst_sit.ci_ep.source.SourceImpl;
import bg.tu_varna.kst_sit.ci_ep.utils.CompilerTestHelper;
import token.TokenImpl;
import token.TokenType;

import java.io.IOException;

public class LexerImpl extends Lexer<TokenType> {

    private int line;
    private int position;

    public LexerImpl(Source source) {
        super(source);
    }

    @Override
    public Token<TokenType> nextToken() {
        currentChar = source.getCurrentChar();
        line = source.getLineNumber();
        position = source.getPosition() + 1;
        while (currentChar != Source.EOF) {
            switch (currentChar) {
                //space and tabs
                case ' ' : case '\t' : handleSpaceAndTabs(); continue;

                //2 character operators
                case '-' : return handleTwoCharOp('>', TokenType.MINUS, TokenType.ARROW);
                case '=' : return handleTwoCharOp('=', TokenType.BECOMES, TokenType.EQUALS);/* ToDo handle operators '=' (BECOMES) and '==' (EQUALS) */
                case '>' : return handleTwoCharOp('=', TokenType.GREATER, TokenType.GREATER_EQ);/* ToDo handle operators '>' (GREATER) and '>=' (GREATER_EQ) */
                case '<' : return handleTwoCharOp('=', TokenType.LESS, TokenType.LESS_EQ);/* ToDo handle operators '<' (LESS) and '<=' (LESS_EQ) */
                case '!' : return handleTwoCharOp('=', TokenType.NOT, TokenType.NOTEQUALS);/* ToDo handle operators '!' (NOT) and '!=' (NOTEQUALS) */
                case '&' : return handleTwoCharOp('&', TokenType.OTHER, TokenType.AND);/* ToDo handle operator '&&' (AND) or unknown symbol (OTHER)  */
                case '|' : return handleTwoCharOp('|', TokenType.OTHER, TokenType.OR);/* ToDo handle operator '|' (OR) or unknown symbol (OTHER) */
                case '/' : return handleSlash();
                case '\'': return handleCharLiteral();
                case '"' : return handleStringLiteral();

                //1 character operators
                case '+' : return retTokenAndAdvance(TokenType.PLUS);
                case '[' : return retTokenAndAdvance(TokenType.LSQUARE);/* ToDo handle operator '[' (LSQUARE) */
                case ']' : return retTokenAndAdvance(TokenType.RSQUARE);/* ToDo handle operator ']' (RSQUARE) */
                case '{' : return retTokenAndAdvance(TokenType.LBRACKET);/* ToDo handle operator '{' (LBRACKET) */
                case '}' : return retTokenAndAdvance(TokenType.RBRACKET);/* ToDo handle operator '}' (RBRACKET) */
                case '(' : return retTokenAndAdvance(TokenType.LPAREN);/* ToDo handle operator '(' (LPAREN) */
                case ')' : return retTokenAndAdvance(TokenType.RPAREN);/* ToDo handle operator ')' (RPAREN) */
                case ';' : return retTokenAndAdvance(TokenType.SEMICOLON);/* ToDo handle operator ';' (SEMICOLON) */
                case '*' : return retTokenAndAdvance(TokenType.MUL);/* ToDo handle operator '*' (MUL) */
                case '%' : return retTokenAndAdvance(TokenType.MOD);/* ToDo handle operator '%' (MOD) */
                case ',' : return retTokenAndAdvance(TokenType.COMMA);/* ToDo handle operator ',' (COMMA) */
                case '@' : return retTokenAndAdvance(TokenType.AT);/* ToDo handle operator '@' (AT) */

                default  :
                    if (isLetter(currentChar)) { return handleIdentifier(); }
                    if (isDigit(currentChar)) { return handleDigit(); }
                    return retTokenAndAdvance(TokenType.OTHER, currentChar + "");
            }
        }
        return null;
    }

    private Token<TokenType> retTokenAndAdvance(TokenType token) {
        source.next();
        return new TokenImpl(token, position, line);
    }

    private Token<TokenType> retTokenAndAdvance(TokenType token, String text) {
        source.next();
        return new TokenImpl(token, text, position, line);
    }

    private Token<TokenType> retToken(TokenType token) {
        return new TokenImpl(token, position, line);
    }

    private Token<TokenType> retToken(TokenType token, String text) {
        return new TokenImpl(token, text, position, line);
    }

    private void handleSpaceAndTabs() {
        while (currentChar == ' ' || currentChar == '\t') {
            currentChar = source.next();
        }
        line = source.getLineNumber();
        position = source.getPosition() + 1;
    }

    private Token<TokenType> handleTwoCharOp(char followingChar, TokenType firstMatchedToken, TokenType secondMatchedToken) {
        if (source.next() == followingChar) {
            return retTokenAndAdvance(secondMatchedToken);
        }
        return retToken(firstMatchedToken);
    }

    private Token<TokenType> handleSlash() {
        if (source.next() == '/') {
            int currentLineNum = source.getLineNumber();
            while (currentLineNum == source.getLineNumber()) {
                source.next();
            }
            return nextToken();
        }
        return retToken(TokenType.DIV);
    }

    private Token<TokenType> handleCharLiteral() {
        char ch = source.next();
        if (ch == '\'') { return retTokenAndAdvance(TokenType.OTHER); }
        if (ch == '\\') ch = handleSpecialChars();
        currentChar = source.next();
        if (currentChar == '\'') {
            return retTokenAndAdvance(TokenType.CHAR_LITERAL, "" + ch);
        }
        return retTokenAndAdvance(TokenType.OTHER);
    }

    private char handleSpecialChars() {
        switch (source.next()) {
            case 'n'    : return '\n';
            case 't'    : return '\t';
            case 'b'    : return '\b';
            case 'r'    : return '\r';
            case 'f'    : return '\f';
            case '\''   : return '\'';
            case '"'    : return '"';
            case '\\'   : return '\\';
            default     : throw new LexicalException("Incorrect char escape: " + currentChar, line, position);
        }
    }

    private Token<TokenType> handleStringLiteral() {
        StringBuilder sb = new StringBuilder();
        while((currentChar = source.next()) != Source.EOF && currentChar != '"') {
            if (currentChar == '\\') currentChar = handleSpecialChars();
            sb.append(currentChar);
        }
        //string is not closed
        if (currentChar == Source.EOF) {
            throw new LexicalException("String quote not closed!", line, position);
        }
        return retTokenAndAdvance(TokenType.STRING_LITERAL, sb.toString());
    }

    private Token<TokenType> handleIdentifier() {
        StringBuilder sb = new StringBuilder();
        sb.append(currentChar);
        currentChar = source.next();
        while(isLetter(currentChar) || isDigit(currentChar)) {
            sb.append(currentChar);
            currentChar = source.next();
        }
        String res = sb.toString();
        if (TokenType.isKeyword(res)) {
            //its a keyword so find it and return it
            return retToken(TokenType.valueOf(res.toUpperCase()));
        }
        return retToken(TokenType.IDENTIFIER, res);
    }

    private Token<TokenType> handleDigit() {
        StringBuilder sb = new StringBuilder();
        while (isDigit(currentChar)) {
            sb.append(currentChar);
            currentChar = source.next();
        }
        String digit = sb.toString();
        // check if it is an integer
        try {
            Integer.parseInt(digit);
        } catch (NumberFormatException e) {
            throw new LexicalException("Not a valid integer " + digit + "." , line, position, e);
        }
        return retToken(TokenType.NUMBER, digit);
    }

    private boolean isLetter(char ch) {
        /* The current character is written in ch */
        /* If ch is a character between 'a' and 'z' or between 'A' and 'Z', return true */
        if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') ) {
            return true; /* ToDo - REPLACE true */
        }else return false;
    }



    private boolean isDigit(char ch) {
        /* If ch is a character between '0' and '9', return true */
        if(ch>='0' && ch<='9') {
            return true; /* ToDo - REPLACE true */
        }else return false;
    }

    public static void main(String[] args) throws IOException {
        Lexer<TokenType> lexer = new LexerImpl(new SourceImpl("resources\\Fib.txt"));
        System.out.println(CompilerTestHelper.getTokensAsString(lexer));
    }
}

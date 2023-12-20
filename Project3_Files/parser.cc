
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "inputbuf.h"
#include "parser.h"


using namespace std;
string keyword1[] = { "int", "real", "bool", "true", "false", "if", "while", "switch", "case", "public", "private" };
struct scopeResolve {
 char* scope;
 scopeResolve* next;
};
int typecount = -1;

struct sTableEntry {
    std::string name;
    char* scope;
    int pubpriv;
    int type;
    int doneflag;
};

struct sTable {
sTableEntry* item;
sTable *prev;
sTable *next;
};

sTable* symbolTable;
char* currentScope;
char* lResolve;
char* rResolve;
scopeResolve* scopeTable;
int currentPrivPub = 0;

int typeupdation(int currentType, int updatedtype)
{
    sTable* temp = symbolTable;

    while(temp->next != NULL)
    {
        if(temp->item->type == currentType)
        {
            temp->item->type = updatedtype;
        }
        temp = temp->next;
    }
    if(temp->item->type == currentType)
    {
        temp->item->type = updatedtype;
    }
    return updatedtype;
}

void addScope(void){
    if(scopeTable == NULL){
        scopeResolve* newScopeItem = (scopeResolve *) malloc(sizeof(scopeResolve));
        newScopeItem->scope = (char *)malloc(sizeof(currentScope));
        memcpy(newScopeItem->scope,currentScope,sizeof(currentScope));
        newScopeItem->next = NULL;
        scopeTable = newScopeItem;
    }else{
        scopeResolve* temp = scopeTable;
        while(temp->next != NULL){
            temp = temp->next;
        }   
        scopeResolve* newScopeItem = (scopeResolve *) malloc(sizeof(scopeResolve));
        newScopeItem->scope = (char *)malloc(sizeof(currentScope));
        memcpy(newScopeItem->scope,currentScope,sizeof(currentScope));
        newScopeItem->next = NULL;
        temp->next = newScopeItem;
    }
}

void deleteScope(void){
        scopeResolve* temp = scopeTable;
        if(temp != NULL){
            if(temp->next == NULL){
                temp = NULL;
            }else{
                while(temp->next->next != NULL){
                    temp = temp->next;
                }
                currentScope = (char *)malloc(sizeof(temp->scope));
                memcpy(currentScope,temp->scope,sizeof(temp->scope));
                temp->next = NULL;   
            }
        }
}

void addList(std::string lexeme, int type, int typecount){
   if(symbolTable == NULL){
      sTable* newEntry = (sTable *) malloc(sizeof(sTable));
      sTableEntry* newItem = (sTableEntry *)malloc(sizeof(sTableEntry));
      newItem->name = lexeme;
      newItem->scope = currentScope;
      newItem->doneflag = 0;
      newItem->pubpriv = currentPrivPub;
      newItem->type = type;


      newEntry->item = newItem;
      newEntry->next = NULL;
      newEntry->prev = NULL;
      symbolTable = newEntry;
      }else{
         sTable* temp = symbolTable;
         while(temp->next != NULL){
            temp = temp->next;
         }
         sTable* newEntry = (sTable *) malloc(sizeof(sTable));
         sTableEntry* newItem = (sTableEntry *)malloc(sizeof(sTableEntry));
         newItem->name = lexeme;
         newItem->scope = currentScope;
         newItem->pubpriv = currentPrivPub;
         newItem->type = type;
         
         newEntry->item = newItem;
         newEntry->next = NULL;
         newItem->doneflag = 0;
         newEntry->prev = temp;
         temp->next = newEntry;

   }
   
}

void printScope(void){
    scopeResolve* temp = scopeTable;

    cout << "\n Printing Scope Table \n";
    while(temp->next != NULL){
        cout << " Scope " << temp->scope << " -> ";
        temp = temp->next;
    }
    cout << " Scope " << temp->scope << " \n";
}


// void printList(void){
//     sTable* temp = symbolTable;

//     cout << "\n Printing Symbol Table \n";
//     while(temp->next != NULL){
//         cout << "\n Name: " << temp->item->name << " Scope: " << temp->item->scope << " Persmission: " << temp->item->pubpriv << " \n";
//         temp = temp->next;
//     }
//     cout << "\n Name: " << temp->item->name << " Scope: " << temp->item->scope << " Persmission: " << temp->item->pubpriv << " \n";
// }

void deleteList(void){
    sTable* temp = symbolTable;
    
    if(temp!= NULL){
        while(temp->next != NULL && strcmp(temp->item->scope,currentScope) != 0){
            temp = temp->next;
        }
        if(strcmp(temp->item->scope,currentScope) == 0){
            if(strcmp(currentScope,"::") != 0){
            temp = temp->prev;
            temp->next = NULL;
            }else{
                temp = NULL;
            }
        }
    }
}

void addType(int ttype) {
    
    sTable* temp = symbolTable;
    if (temp == NULL) {

    } else {
        while(temp->next != NULL && temp->item->type != 0){
            temp = temp->next;
        }
        while(temp->next != NULL) {
            temp->item->type = ttype;
            temp = temp->next;
        }
        temp->item->type=ttype;

    }
}


int searchList(std::string iD, int lR, int type){// add an argument to accept a type
    bool found = false;
    
    sTable* temp = symbolTable;
    if(temp == NULL){
        // if(lR == 0){
        //     lResolve = (char *) malloc(1);
        //     memcpy(lResolve, "?", 1);
        // }else{
        //     rResolve = (char *) malloc(1);
        //     memcpy(rResolve, "?",1);
        // }
    }else{
        int count = 0;
        while(temp->next != NULL){
            temp = temp->next;
            count++;
        }
        if(temp->item->name==iD){
                // if(strcmp(temp->item->scope, currentScope) == 0 ){
                found = true;
                // if(lR == 0){
                //     lResolve = (char *) malloc(sizeof(currentScope));
                //     memcpy(lResolve, currentScope, sizeof(currentScope));
                // }else{
                //     rResolve = (char *) malloc(sizeof(currentScope));
                //     memcpy(rResolve, currentScope, sizeof(currentScope));
                //     }
                // }else{
                //     found = false;
                // }
            }
        while(temp->item->name != iD || !found){
            if(temp->prev == NULL && temp->item->name != iD){
                // if(lR == 0){
                //     lResolve = (char *) malloc(1);
                //     memcpy(lResolve, "?", 1);
                // }else{
                //     rResolve = (char *) malloc(1);
                //     memcpy(rResolve, "?", 1);
                // }
                found = false;
                
                break;
            }else{
                found = true;
            }
            if(temp->item->name == iD){
                // if(strcmp(temp->item->scope, currentScope) == 0 ){
                found = true;
                // if(lR == 0){
                //     lResolve = (char *) malloc(sizeof(currentScope));
                //     memcpy(lResolve, currentScope, sizeof(currentScope));
                // }else{
                //     rResolve = (char *) malloc(sizeof(currentScope));
                //     memcpy(rResolve, currentScope, sizeof(currentScope));
                // }
                break;
                // }else{
                //     found = false;
                //     temp = temp->prev;
                // }
            }else{
                found = false;
                temp = temp->prev;
            }
        }
    }
    
    if (found){
        if(lR==1) {
            //temp->item->type = type;
            temp->item->type = typeupdation(temp->item->type,type);
        }
        //
        
        return temp->item->type;
    }

	else{

        // if(lR == 0){
        //     lResolve = (char *) malloc(sizeof(currentScope));
        //     memcpy(lResolve, currentScope, sizeof(currentScope));
        // }else{
        //     rResolve = (char *) malloc(sizeof(currentScope));
        //     memcpy(rResolve, currentScope, sizeof(currentScope));
        // }
        if(typecount==type){
            typecount--;
        }
        //cout << temp->item->name<< type<<"mat"<<endl;
		addList(iD, type,typecount);

        return type;

	}
    // if(type >= 0){
    //     temp = symbolTable;
    //     while(temp != NULL){
    //         if((strcmp(temp->item->name, iD) == 0) && (temp->item->type == -1)){ 
    //             temp->item->type = type;
    //         }
    //         temp = temp->next;
    //     }
    // }
    
}


void syntax_error(){
	cout << "Syntax Error\n";
	exit(1);
}


int Parser::parse_program(){
	token = lexer.GetToken();
    
	if(token.token_type == ID){
		lexer.UngetToken(token);
		parse_globalVars();
		parse_body();
	}
	else if(token.token_type == LBRACE){
		lexer.UngetToken(token);
		parse_body();
	}
	else{
		syntax_error();
        //cout << "program"<<endl;
	}

	return 0;
}

int Parser::parse_globalVars(){
	parse_vardecllist();

	return 0;
}


int Parser::parse_vardecllist(){
	token = lexer.GetToken();
    
	while(token.token_type == ID){
		lexer.UngetToken(token);
		parse_vardecl();
		token = lexer.GetToken();
	}
	lexer.UngetToken(token);

	return 0;
}


int Parser::parse_vardecl(){
	// cout << "Entered Parse Variable Declaration" << endl;
	token = lexer.GetToken();
	if(token.token_type != ID){
        //cout << "vardeclid"<<endl;
		syntax_error();
	}
	lexer.UngetToken(token);
    
	parse_varlist();
    
	token = lexer.GetToken();
	if(token.token_type != COLON){
        //cout << "vardeclcolon"<<endl;

		syntax_error();
	}
	token = lexer.GetToken();
    
	if(token.token_type == INT || token.token_type == REAL || token.token_type == BOO){
		lexer.UngetToken(token);
		parse_typename();
		token = lexer.GetToken();
		if(token.token_type != SEMICOLON){
            //cout << "vardeclsemi"<<endl;
            
			syntax_error();
		}
	}
	else{
        //cout << "vardeclyes"<<endl;

		syntax_error();

	}

	return 0;
}


int Parser::parse_varlist(){
	// cout << "Entered Parse Variable List" << endl;
	token = lexer.GetToken();
    
	if(token.token_type != ID){
        //cout << "varlistid"<<endl;

		syntax_error();
	}
	else{
        addList(token.lexeme,0,typecount);
		Token t2 = lexer.GetToken();
		if(t2.token_type == COMMA){
			while(token.token_type == ID && t2.token_type == COMMA){
                //cout << token.lexeme<<"dec"<<endl;
				token = lexer.GetToken();
				if(token.token_type != ID){
                    //cout << "varlistid"<<endl;

					syntax_error();
				}
                addList(token.lexeme,0,typecount);
                
				t2 = lexer.GetToken();
			}
			lexer.UngetToken(t2);


		}
		else{
			lexer.UngetToken(t2);
		}
	}
    
	return 0;
}


int Parser::parse_typename(){
	#ifdef DEBUG
		cout << "Entered Parse Type Name" << endl;
	#endif
	token = lexer.GetToken();
   
	if(token.token_type == INT){
        addType(1);
	}
	else if(token.token_type == REAL){
        addType(2);
	}
	else if(token.token_type == BOO){
        addType(3);
	}
	else{
        //cout << "typename"<<endl;

		syntax_error();
	}
    
	return 0;
}


int Parser::parse_body(){
    // cout << "Entered Parse Body" << endl;
	token = lexer.GetToken();
	if(token.token_type == LBRACE){
        //cout << token.lexeme<<endl;
		parse_stmtlist();
		token = lexer.GetToken();
        
		if(token.token_type != RBRACE){
            //cout << "bodyrbrace"<<endl;
            
			syntax_error();
		}
        
	}
	else{
        //cout << "bodyelse"<<endl;

		syntax_error();
	}

	return 0;
}


int Parser::parse_stmtlist(){
	#ifdef DEBUG
		cout << "Entered Parse Statement List" << endl;
	#endif
	token = lexer.GetToken();
	if (token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH){
		while(token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH){
			lexer.UngetToken(token);
            
			parse_stmt();
			token = lexer.GetToken();
		}
		lexer.UngetToken(token);
	}
	else{
        //cout << "stmtelse"<<endl;

		syntax_error();
	}

	

	return 0;
}


int Parser::parse_stmt(){
	#ifdef DEBUG
		cout << "Entered Parse Statement" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type == ID){
		lexer.UngetToken(token);
		parse_assstmt();
	}
	else if(token.token_type == IF){
		lexer.UngetToken(token);
		parse_ifstmt();
	}
	else if(token.token_type == WHILE){
		lexer.UngetToken(token);
		parse_whilestmt();
	}
	else if(token.token_type == SWITCH){
		lexer.UngetToken(token);
		parse_switchstmt();
	}
	else{
        //cout << "stmtelse"<<endl;

		syntax_error();
	}

	return 0;
}

void printList(void)
{
    sTable* temp = symbolTable;
    int temp1;
    string stringout ="";

    while(temp->next != NULL)
    {
       if(temp->item->type <0 && temp->item->doneflag == 0)
        {          
            //cout << temp->item->name<< temp->item->type<<"fim"<<endl;
            temp1 = temp->item->type;
            stringout += temp->item->name;
            temp->item->doneflag = 1;
            while(temp->next != NULL)
            {
                //cout << temp->item->name<< temp->item->type<<"fim"<<endl;
                temp = temp->next;
                if(temp->item->type == temp1)
                {
                    stringout += ", " + temp->item->name;
                    temp->item->doneflag = 1;
                }
                else
                {

                }
            }
            stringout += ": ? #";
            cout << stringout <<endl;
            //temp->item->doneflag = 1;
            stringout = "";
            temp = symbolTable;
        }
        else if(temp->item->type >=0 && temp->item->doneflag == 0)
        {   //cout << temp->item->name<< temp->item->type<<"fim"<<endl;
            string lCase = keyword1[(temp->item->type)-1 ];
            int temp1 = temp->item->type;
            stringout = temp->item->name + ": " + lCase + " #";
            cout << stringout <<endl;
            stringout = "";
            temp->item->doneflag = 1;           

            while(temp->next != NULL  && temp->next->item->type == temp1)
            {
                //cout << temp->item->name<< temp->item->type<<"fim"<<endl;
                temp = temp->next;
                string lCase2 = keyword1[(temp->item->type)-1];
                stringout = temp->item->name + ": " + lCase2 + " #";
                cout << stringout <<endl;
                temp->item->doneflag = 1;
                stringout = "";
            }
        }
        else
        {
            temp = temp->next;
        }
    }
    if(temp->item->type <= 3 && temp->item->doneflag == 0)
    {        
        string lCase3 = keyword1[(temp->item->type)-1];
        stringout += temp->item->name + ": " + lCase3 + " #";
        cout << stringout <<endl;
        stringout = "";
    }
    else if (temp->item->type > 3 && temp->item->doneflag == 0)
    {
        stringout += temp->item->name + ":" + " ? " + "#";
        cout << stringout <<endl;
        stringout = "";
    }
    else
    {

    }
}

int Parser::parse_assstmt(){
	// cout << "Entered Parse Assignment Statement" << endl;
	token = lexer.GetToken();
    int lefttype=searchList(token.lexeme,0,typecount);
	if(token.token_type != ID){
        //cout << "asstid"<<endl;

		syntax_error();
	}
    // std::string var1= token.lexeme;
    
	token = lexer.GetToken();
    //cout << "asstsemi"<<token.lexeme<<endl;
	if(token.token_type != EQUAL){
       // cout << "asstequa"<<endl;

		syntax_error();
	}
    // token = lexer.GetToken();
    // std::string var2= token.lexeme;

    // lexer.UngetToken(token);
	int righttype = parse_expression();
    //cout << "left"<<lefttype<<righttype<<endl;
    if (lefttype!=righttype){
        if ((lefttype>0) && (righttype>0)){
            cout << "TYPE MISMATCH " << token.line_no << " C1" << endl;
            // cout << "yes"<<endl;
            exit(1);     
        }
        else if((lefttype>0) && (righttype<0)){
            righttype=typeupdation(righttype,lefttype);
        }
        else if((lefttype<0) && (righttype>0)){
           lefttype= typeupdation(lefttype,righttype);
        }    
        else if((lefttype<0) && (righttype<0)){
           lefttype= typeupdation(lefttype,righttype);
        }    

   
    }
    
	token = lexer.GetToken();
	if(token.token_type != SEMICOLON){
        // lexer.UngetToken(token);
        // cout << token.lexeme << endl;
        // lexer.UngetToken(token);
        // cout << token.lexeme << endl;
        // lexer.UngetToken(token);
        // cout << token.lexeme << endl;
        //cout << "asstsemi"<<token.lexeme<<endl;

		syntax_error();
	}

	return 0;
}


int Parser::parse_expression(){
	#ifdef DEBUG
		cout << "Entered Parse Expression" << endl;
	#endif
	token = lexer.GetToken();
	int tempI;
    //cout << token.lexeme<<endl;
	if(token.token_type == NOT){
		lexer.UngetToken(token);
		parse_unaryOperator();
        // token =lexer.GetToken();
        // std::string var=token.lexeme;
        // lexer.UngetToken(token);
		tempI=parse_expression();
        if((tempI != 3) && (tempI>=0))
        {
            cout << "TYPE MISMATCH " << token.line_no << " C3"<<endl;
            exit(1);
        }
        if (tempI<0){
            // tempI=searchList(var, 1, 3);
            tempI = typeupdation(tempI,3);
        }
        return tempI;

	}
	else if(token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT || token.token_type == DIV || token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL){
		lexer.UngetToken(token);
		// tempI = parse_primary();
		int tempI1, tempI2;
		tempI = parse_binaryOperator();
        
        // token= lexer.GetToken();
        // std::string var1= token.lexeme;
        // lexer.UngetToken(token);
		tempI1 = parse_expression();
        // token= lexer.GetToken();
        // std::string var2 = token.lexeme;
        // lexer.UngetToken(token);
		tempI2 = parse_expression();
        //cout << var1 << var2 << "check"<<endl;
        if((tempI1 != tempI2) && (tempI1>=0) && (tempI2>=0))
        {
            cout << "TYPE MISMATCH " << token.line_no << " C2"<<endl;
            exit(1);
        }
        else if ((tempI1>=0) && (tempI2<0)){
           tempI2=  typeupdation(tempI2,tempI1);
            
        }
        else if ((tempI1<0) && (tempI2>=0)){
           tempI1= typeupdation(tempI1,tempI2);
        }
        else if ((tempI1<0) && (tempI2<0)){
           tempI1= typeupdation(tempI1,tempI2);
        }        
        if(tempI == 15 || tempI == 16 || tempI == 17 || tempI == 18){
            
            return tempI1;
        }
        else{
            return tempI;
        }
        


	}
	else if(token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA){
		lexer.UngetToken(token);
		tempI = parse_primary();
        return tempI;
        
	}
	else{
       // cout << "parseexpelse"<<endl;

		syntax_error();
	}

	return 0;
}


int Parser::parse_unaryOperator(){
	#ifdef DEBUG
		cout << "Entered Parse Unary Operator" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type != NOT){
       // cout << "parsenot"<<endl;

		syntax_error();
	}

	return 0;
}



int Parser::parse_binaryOperator(){
    token = lexer.GetToken();
    if(token.token_type == PLUS  )
    {
        return(15);
    }
    else if(token.token_type == MINUS )
    {
        return(16);
    }
    else if(token.token_type == MULT)
    {

        return(17);
    }
    else if(token.token_type == DIV )
    {

        return(18);
    }
    else if(token.token_type == GREATER)
    {

        return(3);
    }
    else if(token.token_type == LESS  )
    {

        return(3);
    }
    else if(token.token_type == GTEQ )
    {

        return(3);
    }
    else if(token.token_type == LTEQ)
    {

        return(3);
    }
    else if(token.token_type == EQUAL )
    {

        return(3);
    }
    else if(token.token_type == NOTEQUAL)
    {

        return(3);
    }
    else
    {
        cout << "\n Syntax Error \n";
        //cout << "binary"<<endl;

        exit(1);
    }
}


int Parser::parse_primary(){
    token = lexer.GetToken();
    if(token.token_type == ID )
    {
        return(searchList(token.lexeme,0,typecount));
    }
    else if(token.token_type == NUM )
    {
        return(1);
    }
    else if(token.token_type == REALNUM)
    {
        return(2);
    }
    else if(token.token_type == TR )
    {

        return(3);
    }
    else if(token.token_type == FA)
    {
        return(3);
    }
    else
    {
        cout << "\n Syntax Error \n";
        //cout << "parseprim"<<endl;

        exit(1);
    }
}


int Parser::parse_ifstmt(){

	token = lexer.GetToken();
	if(token.token_type != IF){
        //cout << "if"<<endl;

		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != LPAREN){
        //cout << "iflpa"<<endl;

		syntax_error();
	}
    // token = lexer.GetToken();
    // std::string var = token.lexeme;
    // lexer.UngetToken(token);
	int tempI = parse_expression();
    if((tempI != 3) && (tempI>0))
    {
        cout<< "TYPE MISMATCH " << token.line_no << " C4" << endl;
        exit(1);
    }
    else if (tempI<0){
        //tempI = searchList(var,1,3);
       tempI= typeupdation(tempI,3);
    }
	token = lexer.GetToken();
	if(token.token_type != RPAREN){
        //cout << "ifrpa"<<endl;

		syntax_error();
	}
	parse_body();

	return 0;
}


int Parser::parse_whilestmt(){
	token = lexer.GetToken();
	if(token.token_type != WHILE){
        //cout << "while"<<endl;

		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != LPAREN){
        //cout << "whilelparen"<<endl;

		syntax_error();
	}
	int tempI = parse_expression();
    if(tempI != 3)
    {
        cout<< "TYPE MISMATCH " << token.line_no << " C4" << endl;
        exit(1);
    }
	token = lexer.GetToken();
	if(token.token_type != RPAREN){
        //cout << "whilerpar"<<endl;

		syntax_error();
	}
	parse_body();

	return 0;
}


int Parser::parse_switchstmt(){
    
	token = lexer.GetToken();
	if(token.token_type != SWITCH){
        //cout << "switch"<<endl;

		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != LPAREN){
        //cout << "switchlpar"<<endl;

		syntax_error();
	}
    // token = lexer.GetToken();
    // std::string var= token.lexeme;
    // lexer.UngetToken(token);
	int tempI=parse_expression();
    if(tempI>=0 && tempI != 1)
    {
        cout<< "TYPE MISMATCH " << token.line_no << " C5"<<endl;
        exit(1);
    }
    else if (tempI<0)
    {
        //tempI = searchList(var,1,1);
        tempI=typeupdation(tempI,1);
    }
	token = lexer.GetToken();
	if(token.token_type != RPAREN){
        //cout << "switchr"<<token.lexeme<<endl;
        
		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != LBRACE){
        //cout << "switchlbra"<<endl;

		syntax_error();
	}
	parse_caselist();
	token = lexer.GetToken();
	if(token.token_type != RBRACE){
        //cout << "switchrbra"<<endl;

		syntax_error();
	}

	return 0;
}

/*
 *
 */
int Parser::parse_caselist(){
	token = lexer.GetToken();
	if(token.token_type == CASE){
		while(token.token_type == CASE){
			lexer.UngetToken(token);
			parse_case();
			token = lexer.GetToken();
		}
		lexer.UngetToken(token);
	}
	else{
        //cout << "caselis"<<endl;

		syntax_error();
	}

	return 0;
}

int Parser::parse_case(){
	#ifdef DEBUG
		cout << "Entered Parse Case" << endl;
	#endif
	token = lexer.GetToken();
	if(token.token_type != CASE){
        //cout << "case"<<endl;

		syntax_error();
	}
	token = lexer.GetToken();
	if(token.token_type != NUM){
        //cout << "num"<<endl;

		syntax_error();
	}
	// Do something with this
	token = lexer.GetToken();
	if(token.token_type != COLON){
        //cout << "colon"<<endl;

		syntax_error();
	}
    
	parse_body();

	return 0;
}

int main(){

	int i;
    	Parser* parseProgram = new Parser();
    	i = parseProgram->parse_program();
        printList();
    	// cout << "\nEnd of Program" << endl;
        // cout << "symboltable"<<symbolTable->item->name;
	return 0;
}

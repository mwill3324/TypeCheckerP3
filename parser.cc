
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;
//////////////////struct array to hold decs & vars
//////////////////just my storage concept
string progdecl[255];

int numTypes = 0;
int numVars = 0;
int numIdT = 0;
int leftOp = -1;
int temptype = -1;
bool wasOp = false;
bool wasRelOp = false;
bool wasEq = false;
bool wasnew = false;
bool bodysec = false;

bool t13 = false;
int  t13line;



int asslhs = -1;
int assrhs = -1;
int assline=-1;


class TTokenList {
  public:
    void Print();
    std::string lexeme;  //always holds the character
    TokenType token_type;//should always be type
    bool tset;
    std::string type;    //Used to hold the TypeName if its not Traditional
    int line_no;
} typelist[255];

int numnames[255];

struct lines
{
    string names[255];
    TokenType linetype;
}line[255];
int linepos = 0;

class Idtypes {
    public:
    std::string lexeme;
    TokenType token_type;
}idtypelist[255];

class VTokenList {
  public:
    void Print();
    std::string lexeme;  //always holds the character
    TokenType token_type;//holds the type assigned
    bool tset;
    std::string type;     //Used to hold the TypeName if its not Traditional
    //std::string type;    // for holding type assigned if it is a type.
    int line_no;
} varlist[255];

int typenumber = -1;
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void Parser::syntax_error()
{
   cout << "Syntax Error\n";
    exit(1);
}
void ErrorCode(double code, string letter)
{
    for(int a = 0;a < numVars;a++)
        {
                //cout<<"vars:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<" "<<varlist[a].type<<endl;
        }
    for(int b = 0;b<numTypes;b++)
        {
                //cout<<"Types: "<<typelist[b].lexeme<<" "<<typelist[b].token_type<<endl;
        }
    for(int c = 0; c < numIdT; c++)
        {
                //cout<<"IDTypes: "<<idtypelist[c].lexeme<<" "<<idtypelist[c].token_type<<endl;
        }
    cout << "ERROR CODE "<<code<<" "<<letter<<endl;
    exit(1);
}
void TypeMismatch(int line, string num)
{
    for(int a = 0;a < numVars;a++)
        {
                //cout<<"vars:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<" "<<varlist[a].type<<endl;
        }
    for(int b = 0;b<numTypes;b++)
        {
                //cout<<"Types: "<<typelist[b].lexeme<<" "<<typelist[b].token_type<<" "<<varlist[b].type<<endl;
        }
    for(int c = 0; c < numIdT; c++)
        {
                //cout<<"IDTypes: "<<idtypelist[c].lexeme<<" "<<idtypelist[c].token_type<<endl;
        }
    cout << "TYPE MISMATCH "<<line<<" "<<num<<endl;
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    bool tfound = false;
    bool vfound = false;
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
    {
        syntax_error();
    }
    else
    {
////////////////////////

        if(wasnew == true){
                Token toke = t;

                bool newVar=false;
                if(toke.token_type == ID){ //this fills this line with the first id
                    line[linepos].names[numnames[linepos]] = toke.lexeme;
                    numnames[linepos]++;
                }
                bool found = false;
                //SET THE LINE TYPE IF FIRST ID HAS ONE
                for(int a = 0; a < numVars; a++){
                    if(toke.lexeme == varlist[a].lexeme){
                        //cout<<"list: "<<varlist[a].lexeme<<" "<<varlist[a].token_type<<endl;
                        if(varlist[a].token_type != 0)
                        {
                            temptype = varlist[a].token_type;
                        }
                        found = true;
                        newVar = false;

                        if((line[linepos].linetype == 0 || line[linepos].linetype == ID) && temptype!= -1){
                            line[linepos].linetype = varlist[a].token_type;
                        }

                    }
                }
                //TO ADD NEW VARIABLES...
                if(toke.token_type == ID && found == false){
                    newVar = true;
                    varlist[numVars].lexeme = toke.lexeme;//////ADDS LEX

                    //cout<<"NEW VAR:"<<varlist[numVars].lexeme<<endl;//<<","<<varlist[numVars].token_type<<endl;
                }
                /////////
                if(found == false){ //type still isnt established.
                    if(toke.token_type == NUM ||toke.token_type == REALNUM){ //if its not just an ID
                        if(toke.token_type == NUM){
                            temptype = INT;
                            //cout<<"LINE TYPE"<<endl;
                            line[linepos].linetype = INT;
                            //cout<<"type:"<<line[linepos].linetype<<endl;
                        }
                        else if(toke.token_type == REALNUM){
                            temptype = REAL; //set it to NUM or whatever it is
                            line[linepos].linetype = REAL;
                        }
                    }
                }

                //cout<< "L1:"<<leftOp<<" R:"<< temptype<<endl;

                if(newVar == true){
                        varlist[numVars].token_type = static_cast<TokenType>(temptype);
                        leftOp = temptype;// because its a new var, it sets the left op to the right type
                        newVar = false;
                        numVars++;
                    }
                if(wasEq == true) //CHECKS C1 ERRORS
                {
                    if(temptype != asslhs && temptype != -1 && temptype != ID && asslhs != -1 && asslhs != ID)
                    {
                        assrhs = temptype;
                    }
                }
                ///////////if it was a + or <> stuff
                if(wasOp == true)
                {
                        if(leftOp != temptype && (leftOp != ID && temptype != ID))
                        {
                           if(leftOp != -1 && temptype!= -1)// -1 is unkown
                            {
                                TypeMismatch(toke.line_no,"C2");  //2 points
                            }
                        }
                    wasOp = false;
                }

                /////////////////////////////////////
                if(toke.token_type == GREATER || toke.token_type== GTEQ ||toke.token_type== LESS ||toke.token_type== NOTEQUAL||toke.token_type== LTEQ)
                {
                    wasRelOp = true;
                }
                if(toke.token_type == EQUAL)
                {
                        wasEq = true;
                }
                if(toke.token_type == PLUS ||toke.token_type== MULT ||toke.token_type== DIV)
                {
                        wasOp = true;
                }
            leftOp = temptype;
        }
    wasnew = false;

    //////////////////////////////////


        if(expected_type == ID)
        {
            for(int a = 0; a < numVars; a++){
                if(t.lexeme == varlist[a].lexeme){
                        vfound = true;
                }
            }
            for(int a = 0; a < numTypes; a++){
                if(t.lexeme == typelist[a].lexeme){
                        tfound = true;
                }
            }
            ////////  1.2 gets 2 points
            for(int a = 0; a < numIdT; a++){
                //cout<<"IDT"<<endl;
                //cout<<t.lexeme<<" "<<idtypelist[a].lexeme<<endl;

                if(t.lexeme == idtypelist[a].lexeme){
                    if(bodysec == false && typenumber == 0)
                    {
                        ErrorCode(1.2,t.lexeme); //doesnt cause issues
                    }
                }
            }

            /////// 1.1 gets 2 points
            if(typenumber == 0)//a TYPE
            {
                if(tfound == false)
                {
                   typelist[numTypes].lexeme = t.lexeme;
                   numTypes++;
                }
                else if(bodysec == false && tfound == true)
                {
                    ErrorCode(1.1,t.lexeme); //doesnt cause issues..
                }
            }

            else if(typenumber == 1)//VAR
            {

                if(vfound == false)
                {
                    //cout<<"new type EXPECT: "<<t.lexeme<<endl;
                    varlist[numVars].lexeme = t.lexeme;
                    numVars++;
                }
                else if(bodysec == false)
                {
                    ErrorCode(2.1,t.lexeme); //doesnt cause issues Gets 3 points / 6
                }

            }

        }
    }
    ////////////////////////////////////////
    ///////////////////////////////////////////////
    return t;
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parse_program()
{
    //program -> decl body
    parse_decl();

    for(int a = 0;a<numTypes;a++)
    {
        //cout<<"type: "<<typelist[a].lexeme<<" "<<typelist[a].token_type<<endl;
    }
    bool changed = true;

    while(changed == true)
    {
        changed = false;
        for(int a = 0; a < numTypes;a++)
        {
            if(typelist[a].token_type == ID)
            {  //Find id
                for(int b = 0; b<numTypes;b++)
                {
                    //if they match, and aren't already the same, change it.
                    if(typelist[b].lexeme == typelist[a].type && typelist[b].token_type != typelist[a].token_type)
                    {
                        changed = true;
                        typelist[a].token_type = typelist[b].token_type;
                        //typelist[a].type = "";//;alsdfj;lasdf
                    }
                }
            }
        }
    }

    for(int a = 0;a<numTypes;a++)
    {
        //cout<<"type: "<<typelist[a].lexeme<<" "<<typelist[a].token_type<<endl;
    }

    //cout<<"------------"<<endl;
    for(int a = 0; a < numIdT; a++)
    {
       //cout<<"type list: "<<idtypelist[a].lexeme<<endl;
    }
     for(int b = 0;b<numVars;b++)
    {
       //cout<<"var: "<<varlist[b].lexeme <<endl;
    }

        //gets 2 points?
    for(int a = 0;a<numTypes;a++)
        {
            for(int b = 0;b<numVars;b++)
            {
                if(typelist[a].lexeme == varlist[b].lexeme)
                {
                        ErrorCode(1.3,typelist[a].lexeme); //no issues added points
                }
            }
        }

    for(int a = 0;a < numIdT;a++)
        {
            for(int b = 0;b < numVars;b++)
            {
                if(idtypelist[a].lexeme == varlist[b].lexeme)
                {
                        ErrorCode(1.3,idtypelist[a].lexeme); //no issues added points
                }
            }
        }
/////////////Parse Body/////////////////

    //cout<<"asdfasdf"<<endl;

    while(changed == true){
        changed = false;
        for(int a = numVars; a < numVars;a++){
            if(varlist[a].token_type == ID){  //Find id
                for(int b = numTypes;b<numTypes;b++){
                    //if they match, and aren't already the same, change it.
                    if(varlist[a].type == typelist[b].lexeme && varlist[a].token_type != typelist[b].token_type)
                    {
                        changed = true;
                        varlist[a].token_type = typelist[b].token_type;
                    }
                }
            }
        }
    }

    for(int a = 0; a < numVars ; a++){
        //cout<<"v:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<endl;
    }

/////////////
    for(int a = 0;a<255;a++)
    {
        numnames[a]=0;
    }

    bodysec =true;



    /////////////////////////////

    parse_body();



            /////2 total points/////
//////////////////////////////////////////////////
    for(int a = 0;a<numIdT;a++)
    {
        for(int b = 0;b<numVars;b++)
        {
            if(idtypelist[a].lexeme == varlist[b].lexeme)
            {
                    ErrorCode(1.4,idtypelist[a].lexeme); //no issues added points
            }
        }
    }

    for(int a = 0;a<numTypes;a++)
    {
        for(int b = 0;b<numVars;b++)
        {
            if(typelist[a].lexeme == varlist[b].lexeme)
            {
                    ErrorCode(1.4,typelist[a].lexeme); //no issues added points
            }
        }
    }
////////////////////////////////////////////////////////////////////
///////////Correct ID type- types. ///////////
/////if a,b,c is type t1.
////and t1 is int
/////set a, b, c to int for printing properly..

        for(int b = 0;b<numVars;b++)
            {
               //cout<<varlist[b].lexeme<<" "<<varlist[b].token_type<<endl;
            }

        for(int theline = 0;theline < linepos; theline++)
        {
            for(int thename = 0; thename<numnames[theline];thename++)//go through line names (a,b,c)
            {
                for(int c = 0; c < numVars; c++)// go through curr var list
                {
                    if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                    {
                        if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[theline].linetype != 0)
                        {
                            varlist[c].token_type = line[theline].linetype;
                        }
                    }
                }
            }
        }

        for(int a = 0;a<numVars;a++)// go through curr var list
        {
            for(int b = 0;b<numVars;b++)// go through curr var list
            {
                if(varlist[a].type == varlist[b].type )//found same var in v list
                {
                    if(varlist[a].token_type == ID && varlist[a].type.length()>0)
                    {
                        varlist[a].token_type = varlist[b].token_type;
                    }
                }
            }
        }
        /////update type ids/////////
        for(int a = 0;a<numVars;a++)// go through curr var list
        {
            for(int b = 0;b<numTypes;b++)// go through curr var list
            {
                if(varlist[a].type == typelist[b].lexeme )//found same var in v list
                {
                     typelist[b].token_type = varlist[a].token_type;
                }
            }
        }
/////////////////////////////////////////////


    for(int b = 0;b < numTypes;b++)
        {
            //cout<<"type:"<<typelist[b].lexeme<<" T:"<<typelist[b].token_type<<endl;
        }
    for(int b = 0;b < numVars;b++)
        {
           // cout<<"Var:"<<varlist[b].lexeme<<" T:"<<varlist[b].token_type<<endl;
        }
    //cout<<"BOOLEAN";

}

void Parser::parse_decl()
{
    parse_type_decl_section();

    for(int a = 0; a < numTypes; a++){
       // cout<<"ssss: "<< typelist[a].lexeme << endl;
        if(typelist[a].token_type == ID){  //Find id
            for(int b = 0; b<numTypes; b++){
                //cout<<"fixing: "<<typelist[b].lexeme <<" "<<typelist[a].type<<endl;
                //if they match, and aren't already the same, change it.
                if(typelist[b].lexeme == typelist[a].type)
                {
                    for(int c = 0; c<numTypes; c++)
                    {
                        //if they match, and aren't already the same, change it.
                        if(typelist[a].type == typelist[c].lexeme)
                        {
                            typelist[a].token_type = typelist[c].token_type;
                            typelist[a].type = "";//;
                        }
                    }
                }
            }
        }
    }

    parse_var_decl_section();
    //////////////////////
}

void Parser::parse_type_decl_section()
{
    // type_decl_section -> TYPE type_decl_list
    // type_decl_section -> epsilon
    Token t = lexer.GetToken();
    //cout<<"Tdecl_section ="<<t.lexeme<<endl;
    if (t.token_type == TYPE)
    {
        typenumber = 0;
        // type_decl_section -> TYPE type_decl_list
        parse_type_decl_list();
    }
    else if (t.token_type == LBRACE || t.token_type == VAR)
    {
        // type_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error(); //0 points
    }
    typenumber = -1;
}

void Parser::parse_type_decl_list()
{
    // type_decl_list -> type_decl
    // type_decl_list -> type_decl type_decl_list
    parse_type_decl();
    Token t = peek();
    if (t.token_type == LBRACE || t.token_type == VAR)
    {
        typenumber = 1;
        // type_decl_list -> type_decl
    }
    else if (t.token_type == ID)
    {
        // type_decl_list -> type_decl type_decl_list
        parse_type_decl_list();
    }
    else
    {
        syntax_error(); //0 points
    }
    typenumber = -1;
}

void Parser::parse_type_decl()
{
    // type_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
}
/////////////////////////////////////////////////////////////
////////////////////SET TYPE TO ARRAY POS////////////////////
void Parser::parse_type_name()
{

    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID
    //cout<<"GOT DAM"<<endl;
    Token tok = lexer.GetToken();
    //cout<<"typename: "<<t.lexeme<<endl;
    TokenType b = tok.token_type;

    if (tok.token_type == REAL)
    {
        // type_name -> REAL
        b = REAL;
    }
    else if (tok.token_type == INT)
    {
        // type_name -> INT
        b = INT;
    }
    else if (tok.token_type == BOOLEAN)
    {
        // type_name -> BOOLEAN
        b = BOOLEAN;
    }
    else if (tok.token_type == STRING)
    {
        // type_name -> STRING
        b = STRING;
    }
    else if (tok.token_type == LONG)
    {
        // type_name -> LONG
        b = LONG;
    }
    else if (tok.token_type == ID)
    {
        // type_name -> ID
        //cout<<"ID"<<endl;
        b = ID;
    }
    else
    {
        syntax_error(); //0 POINTS
    }
    //cout<<"asdf"<<endl;
    ////////////////////////////////////
        if(numTypes > 0 && typenumber == 0)//sets type type values // typenumber 0 means its a type
            {
                for(int a = numTypes-1; typelist[a].tset!=true && a>-1; a--)
                {
                    bool isidtype = false;
                    for(int c = 0; c<numIdT; c++){
                        if(tok.lexeme == idtypelist[c].lexeme)
                        {
                            isidtype = true; //already an Id type
                        }
                    }
                    if(isidtype == false && b == ID){
                        idtypelist[numIdT].lexeme = tok.lexeme;
                       // cout<<"NEW ID TYPE: "<<idtypelist[numIdT].lexeme<<endl;
                        numIdT++;
                    }
                    //cout<<"4"<<endl;
                    typelist[a].tset = true;
                    typelist[a].token_type = b;
                    if(tok.token_type == ID){
                        typelist[a].type = tok.lexeme;
                    }
                    else{
                        typelist[a].type = "";
                    }
                }
            }
        //cout<<"SEG"<<endl;
        bool found = false;
        bool found22 = false;

        for(int a = 0; a < numVars; a++)
        {
            if(tok.lexeme == varlist[a].lexeme)
            {
                found22 = true; //this loop finds 2.2 error codes
            }
        }
        for(int a = 0; a < numVars; a++)
        {
            for(int i = 0; i<numTypes;i++)
            {
                if(varlist[i].type == typelist[i].lexeme)
                {
                    found = true;
                }
            }
        }
        if(numVars > 0 && typenumber == 1)//sets all var type values for the line
            {
               for(int a = numVars-1; varlist[a].tset != true && a> -1;a--)
               {
                   varlist[a].tset = true;
                   varlist[a].token_type = b;
                   varlist[a].type = tok.lexeme;

                   if(b == ID)
                   {
                        for(int i = 0; i<numTypes;i++)
                        {
                            if(varlist[a].type == typelist[i].lexeme)
                            {
                                found = true;
                                varlist[a].token_type = typelist[i].token_type; //update type if type has type
                            }
                        }
                   }
               }
               //must be a new type, that has not been previously declared. Add to type list
               if(found == false && b == ID)
               {
                //cout<<"new type: "<<tok.lexeme<<endl;
                   if(found22 == true)
                   {
                       ErrorCode(2.2, tok.lexeme);
                   }
                    typelist[numTypes].lexeme = tok.lexeme;
                    typelist[numTypes].type = "*";
                    numTypes++;
               }
        }
    //}
}
///////////////////////////////////////////////////////////////////////////////////

void Parser::parse_var_decl_section()
{
    // var_decl_section -> VAR var_decl_list
    // var_decl_section -> epsilon
    Token t = lexer.GetToken();
    if (t.token_type == VAR)
    {
        typenumber = 1;///////////////////
        parse_var_decl_list();
    }
    else if (t.token_type == LBRACE)
    {
        // var_decl_section -> epsilon
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error(); //0 points
    }
}

void Parser::parse_var_decl_list()
{
    // var_decl_list -> var_decl
    // var_decl_list -> var_decl var_decl_list
    parse_var_decl();
    Token t = peek();
    if (t.token_type == LBRACE)
    {
        // var_decl_list -> var_decl
    }
    else if (t.token_type == ID)
    {
        // var_decl_list -> var_decl var_decl_list
        parse_var_decl_list();
    }
    else
    {
        syntax_error();// 0 points
    }
}

void Parser::parse_var_decl()
{
    // var_decl -> id_list COLON type_name SEMICOLON
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
}

void Parser::parse_id_list() //ORIGINAL
{
     // id_list -> ID
    // id_list -> ID COMMA id_list
    expect(ID);
    Token t = lexer.GetToken();
    if (t.token_type == COMMA)
    {
        // id_list -> ID COMMA id_list
        parse_id_list();
    }
    else if (t.token_type == COLON)
    {
        // id_list -> ID
        lexer.UngetToken(t);
    }
    else
    {
        syntax_error(); //0 points
    }
}


void Parser::parse_body()
{
    // body -> LBRACE stmt_list RBRACE
    //cout<<"body start"<<endl;
    expect(LBRACE);//cout<<"L body"<<endl;
    parse_stmt_list();//cout<<"stmnt end bdy"<<endl;
    expect(RBRACE);//cout<<"R body"<<endl;

}

void Parser::parse_stmt_list()
{
    // stmt_list -> stmt
    // stmt_list -> stmt stmt_list
    parse_stmt();
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID ||
        t.token_type == SWITCH || t.token_type == DO)
    {
        // stmt_list -> stmt stmt_list
        parse_stmt_list();
    }
    else if (t.token_type == RBRACE)
    {
        // stmt_list -> stmt
    }
    else
    {
        syntax_error();// 0 POINTS
    }
}

void Parser::parse_stmt()
{
    // stmt -> assign_stmt
    // stmt -> while_stmt
    // stmt -> do_stmt
    // stmt -> switch_stmt
     for(int thename = 0; thename < numnames[linepos]; thename++) //go through line names (a,b,c)
    {
        for(int c = 0; c < numVars; c++)// go through curr var list
        {
            if(varlist[c].lexeme == line[linepos].names[thename])//found same var in var list
            {
                if((varlist[c].token_type == ID || varlist[c].token_type == 0 || varlist[c].token_type == -1) && line[linepos].linetype != 0)
                {
                    //cout<<"set var: "<<varlist[c].lexeme<<" linetype: "<<line[linepos].linetype<<endl;
                    varlist[c].token_type = line[linepos].linetype;
                }
            }
        }
    }
    /////update type ids/////////
    for(int a = 0;a<numVars;a++)// go through curr var list
    {
        for(int b = 0;b<numTypes;b++)// go through curr var list
        {
            if(varlist[a].type == typelist[b].lexeme )//found same var in v list
            {
                typelist[b].token_type = varlist[a].token_type;
            }
        }
    }
   //////////////////////////////////?
   for(int a = 0; a < numTypes; a++){
                for(int b = 0; b < numTypes; b++)
                {
                    if(typelist[a].type == typelist[b].type && typelist[a].type.length()>0)
                    {
                        if(typelist[a].token_type == ID ||typelist[a].token_type == 0)
                        {
                            typelist[a].token_type = typelist[b].token_type;
                        }
                    }
                }
            }
            ////////////
    wasEq = false;
    bool changed = true;
        while(changed == true)
        {
            changed = false;
            for(int theline = 0; theline < linepos; theline++)
            {
                if(line[theline].linetype == -1 || line[theline].linetype == 0)
                {
                    for(int thename = 0; thename < numnames[theline];thename++)//go through line names (a,b,c)
                    {
                        for(int c = 0; c < numVars; c++)// go through curr var list
                        {
                            if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                            {
                                if(varlist[c].token_type != -1 && varlist[c].token_type != ID)
                                {
                                    changed = true;
                                    line[theline].linetype = varlist[c].token_type; //updates line type..
                                }
                            }
                        }
                    }
                }
            }
            for(int theline = 0;theline < linepos; theline++)
            {
                for(int thename = 0; thename<numnames[theline];thename++)//go through line names (a,b,c)
                {
                    for(int c = 0; c < numVars; c++)// go through curr var list
                    {
                        if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                        {
                           if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[theline].linetype != 0)
                            {
                                varlist[c].token_type = line[theline].linetype;
                            }
                        }
                    }
                }
            }
        }

            for(int a = 0;a<numVars;a++)// go through curr var list
                {
                    for(int b = 0;b<numTypes;b++)// go through type  list
                    {
                        if(varlist[a].type == typelist[b].lexeme && varlist[a].token_type == ID)//found same var in v list
                        {
                            typelist[b].token_type = varlist[a].token_type;

                        }
                    }
                }

            for(int a = 0; a < numTypes; a++)
            {
                for(int b = 0; b < numTypes; b++)
                {
                    if(typelist[a].type == typelist[b].type && typelist[a].type.length()>0 &&typelist[a].type != "*")
                    {
                        if(typelist[a].token_type == ID || typelist[a].token_type == 0)
                        {
                            typelist[a].token_type = typelist[b].token_type;
                        }
                    }
                }
            }
            for(int a = 0; a< numVars; a++) // go through curr var list
            {
                for(int b = 0; b<numTypes; b++) // go through curr type list
                {
                    if(varlist[a].type == typelist[b].lexeme && varlist[a].token_type == ID)//found same var in v list
                    {
                        varlist[a].token_type = typelist[b].token_type;
                    }
                }
            }
    bool plusline = false;
    Token t = peek();
    if (t.token_type == ID)
    {
        // stmt -> assign_stmt
        parse_assign_stmt();
        plusline = true;
    }
    else if (t.token_type == WHILE)
    {
        // stmt -> while_stmt
        parse_while_stmt();
        plusline = true;
    }
    else if (t.token_type == DO)
    {
        // stmt -> do_stmt
        parse_do_stmt();
    }
    else if (t.token_type == SWITCH)
    {
        // stmt -> switch_stmt
        parse_switch_stmt();
    }
    else
    {
        syntax_error(); // 2 POINTS
    }



}
////////////////////////////////////////////
///////////////////////////////////////////////////////////


void Parser::parse_assign_stmt() //ID EQUAL expr SEMICOLON [done]
{
    asslhs = -1;
    leftOp = -1;
    temptype = -1;
    Token toke = peek();

    for(int a = 0; a < numVars; a++)
    {
        if(toke.lexeme == varlist[a].lexeme)
        {
            asslhs = varlist[a].token_type;
        }
    }
    wasnew = true;
    expect(ID);  // no points
    wasEq = true;
    expect(EQUAL);
    parse_expr();               //expr
    expect(SEMICOLON);          //SEMICOLON
    assline = toke.line_no;

    for(int thename = 0; thename < numnames[linepos]; thename++) //go through line names (a,b,c)
    {
        for(int c = 0; c < numVars; c++)// go through curr var list
        {
            if(varlist[c].lexeme == line[linepos].names[thename])//found same var in var list
            {
                if((varlist[c].token_type == ID || varlist[c].token_type == 0 || varlist[c].token_type == -1) && line[linepos].linetype != 0)
                {
                    //cout<<"set var: "<<varlist[c].lexeme<<" linetype: "<<line[linepos].linetype<<endl;
                    varlist[c].token_type = line[linepos].linetype;
                }
            }
        }
    }

    if(asslhs != assrhs && asslhs != -1  && asslhs != ID && assrhs!= -1 && assrhs!=ID)
    {
        //cout<<asslhs<<" "<<assrhs<<endl;
        TypeMismatch(toke.line_no,"C1");
    }
    /////update type ids/////////
    for(int a = 0;a<numVars;a++)// go through curr var list
    {
        for(int b = 0;b<numTypes;b++)// go through curr var list
        {
            if(varlist[a].type == typelist[b].lexeme )//found same var in v list
            {
                typelist[b].token_type = varlist[a].token_type;
            }
        }
    }
   //////////////////////////////////?
   for(int a = 0; a < numTypes; a++)
            {
                //cout<<"type loop:  "<<typelist[a].token_type<<" "<<typelist[a].lexeme<<endl;
                for(int b = 0; b < numTypes; b++)
                {
                    if(typelist[a].type == typelist[b].type && typelist[a].type.length()>0)
                    {
                        if(typelist[a].token_type == ID ||typelist[a].token_type == 0)
                        {
                            typelist[a].token_type = typelist[b].token_type;
                        }
                    }
                }
            }
            ////////////
    wasEq = false;
    bool changed = true;
        while(changed == true)
        {
            changed = false;
            for(int theline = 0; theline < linepos; theline++)
            {
                if(line[theline].linetype == -1 || line[theline].linetype == 0)
                {
                    for(int thename = 0; thename < numnames[theline];thename++)//go through line names (a,b,c)
                    {
                        for(int c = 0; c < numVars; c++)// go through curr var list
                        {
                            if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                            {
                                if(varlist[c].token_type != -1 && varlist[c].token_type != ID)
                                {
                                    changed = true;
                                    line[theline].linetype = varlist[c].token_type; //updates line type..
                                }
                            }
                        }
                    }
                }
            }
            for(int theline = 0;theline < linepos; theline++)
            {
                for(int thename = 0; thename<numnames[theline];thename++)//go through line names (a,b,c)
                {
                    for(int c = 0; c < numVars; c++)// go through curr var list
                    {
                        if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                        {
                           if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[theline].linetype != 0)
                            {
                                varlist[c].token_type = line[theline].linetype;
                            }
                        }
                    }
                }
            }
        }

            for(int a = 0;a<numVars;a++)// go through curr var list
                {
                    for(int b = 0;b<numTypes;b++)// go through type  list
                    {
                        if(varlist[a].type == typelist[b].lexeme && varlist[a].token_type == ID)//found same var in v list
                        {
                            typelist[b].token_type = varlist[a].token_type;

                        }
                    }
                }

            for(int a = 0; a < numTypes; a++)
            {
                for(int b = 0; b < numTypes; b++)
                {
                    if(typelist[a].type == typelist[b].type && typelist[a].type.length()>0 &&typelist[a].type != "*")
                    {
                        if(typelist[a].token_type == ID || typelist[a].token_type == 0)
                        {
                            typelist[a].token_type = typelist[b].token_type;
                        }
                    }
                }
            }
            for(int a = 0; a< numVars; a++) // go through curr var list
            {
                for(int b = 0; b<numTypes; b++) // go through curr type list
                {
                    if(varlist[a].type == typelist[b].lexeme && varlist[a].token_type == ID)//found same var in v list
                    {
                        varlist[a].token_type = typelist[b].token_type;
                    }
                }
            }
        //cout<<"---------"<<endl;
    for(int a = 0;a < numVars;a++)
        {
                //cout<<"vars:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<" "<<varlist[a].type<<endl;
        }
    for(int b = 0;b<numTypes;b++)
        {
                //cout<<"Types: "<<typelist[b].lexeme<<" "<<typelist[b].token_type<<" "<<typelist[b].type<<endl;
        }
    linepos++;
}

void Parser::parse_while_stmt() //WHILE, condition body [DONE]
{
    expect(WHILE);//0 POINTS//consume the 'WHILE'
    parse_condition();//cout<<"cond end"<<endl;
    parse_body();//cout<<"body end"<<endl;
}

void Parser::parse_do_stmt()    //DO body WHILE condition SEMICOLON [DONE]
{
        expect(DO); //cout<<"DO"<<endl; //0 POINTS//consume 'DO'
        parse_body();//cout<<"BODY"<<endl;
        expect(WHILE);//cout<<"WHILE"<<endl; //0 POINTS //Consume 'while'
        parse_condition();//cout<<"condition"<<endl;
        expect(SEMICOLON);//0 POINTS

        for(int thename = 0; thename<numnames[linepos];thename++)//go through line names (a,b,c)
        {
            for(int c = 0; c < numVars; c++)// go through curr var list
            {
                if(varlist[c].lexeme == line[linepos].names[thename])//found same var in var list
                {
                    if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[linepos].linetype != 0)
                    {
                        varlist[c].token_type = line[linepos].linetype;
                    }
                }
            }
        }



    //linepos++; // I guess this is extra.. tbt
    // TODO
}

void Parser::parse_switch_stmt()//SWITCH ID LBRACE case_list RBRACE [DONE] /////////SHOULD BE DONE
{
        for(int a = 0;a < numVars;a++)
        {
                //cout<<"vars:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<" "<<varlist[a].type<<endl;
        }
    for(int b = 0;b<numTypes;b++)
        {
                //cout<<"Types: "<<typelist[b].lexeme<<" "<<typelist[b].token_type<<" "<<varlist[b].type<<endl;
        }
        expect(SWITCH);// 0 POINTS //Consume 'SWITCH;
        Token tok = peek();
        expect(ID);
        for(int a = 0; a< numVars; a++)
        {
            if(varlist[a].lexeme == tok.lexeme)
            {
                if(varlist[a].token_type!= INT && varlist[a].token_type!= NUM)// && varlist[a].token_type!= ID &&varlist[a].token_type!= 0)
                {
                    //cout<<varlist[a].lexeme<<" "<<varlist[a].token_type<<endl;
                    TypeMismatch(tok.line_no,"C5");
                }
                if(varlist[a].token_type == ID &&varlist[a].type.length()>0)
                {
                    for(int b = 0; b<numTypes; b++)
                    {
                        if(varlist[a].type==typelist[b].lexeme)
                        {
                            if(typelist[b].token_type!= INT)
                            {
                                TypeMismatch(tok.line_no,"C5");
                            }
                        }
                    }
                }
            }
        }
        expect(LBRACE); //0 points..
        parse_case_list();
        expect(RBRACE);//0 POINTS
}

void Parser::parse_case_list() // case case_list | case  [done] ///////////SHOULD BE DONE
{
    parse_case();
    Token tok = peek();
    if(tok.token_type == CASE)
    {
        parse_case_list();
    }
    // TODO
}

void Parser::parse_case()//CASE NUM COLON body [done] /////////////SHOULD BE DONE
{
    //cout<<"CASE"<<endl;
    expect(CASE); //0 points
    expect(NUM);  //1 points
    expect(COLON);//0 points
    parse_body(); //body
    // TODO
}

void Parser::parse_expr() //term PLUS expr | term [done]
{
    // TODO

    parse_term(); //SHOULD catch syntax_errors for this function
    //cout<<"TERM"<<endl;
    Token toke = peek();
    if(toke.token_type==PLUS)
    {
        wasOp = true;
        expect(PLUS);//0 points
        parse_expr();
    }
    wasOp = false;

}

void Parser::parse_term() //factor (MULT | DIV) term | factor [done]
{
    parse_factor();

    Token toke = peek();

    if(toke.token_type == MULT)
    {
        wasOp = true;
        expect(MULT);
        parse_term();
    }
    else if(toke.token_type == DIV)
    {
        wasOp = true;
        expect(DIV);
        parse_term();
    }
    //cout<<"somewhere here?"<<endl;
    // TODO
}

void Parser::parse_factor() // LPAREN expr RPAREN | NUM | REALNUM | ID [done]
{
    //cout<<"Factor"<<endl;
    Token tok = peek();
    if(tok.token_type == LPAREN)
    {
        expect(LPAREN);
        parse_expr();
        expect(RPAREN);
    }
    else if(tok.token_type == NUM)
    {
       // parse_new();
        wasnew = true;
        expect(NUM);// 0 points
    }
    else if(tok.token_type==REALNUM)
    {
        //parse_new();
        wasnew = true;
        expect(REALNUM);
    }
    else if(tok.token_type==ID)
    {
        //parse_new();
        wasnew = true;
        expect(ID);
    }
    else
    {
        syntax_error(); //GETS 2 points
    }
        // TODO
}
void Parser::parse_condition() //ID | primary relop primary
{


    leftOp = -1;
    temptype = -1;
    //cout<<"Condition"<<endl;
    Token toke1 = lexer.GetToken();//should get id,num, or realnum
    Token toke2 = peek(); //Should get relop
    lexer.UngetToken(toke1);
    if(toke1.token_type == ID)
    {
        if(toke2.token_type == SEMICOLON || toke2.token_type == LBRACE)
        {
            wasnew = true;
            expect(ID);
            for(int a = 0; a <numVars; a++){
                if(varlist[a].lexeme == toke1.lexeme){
                    if(varlist[a].token_type != BOOLEAN && varlist[a].token_type!= -1 && varlist[a].token_type != ID)
                    {
                        TypeMismatch(toke1.line_no,"C4");//3 points
                    }
                }
            }
        }
        else if(toke2.token_type == GREATER || toke2.token_type==GTEQ ||toke2.token_type == LESS||toke2.token_type ==NOTEQUAL||toke2.token_type == LTEQ) //is relop and not
        {
            wasRelOp = true;
            parse_primary();
            parse_relop();
            toke2 = peek();
            parse_primary();
            ////SET LINE TYPES
                for(int thename = 0; thename < numnames[linepos]; thename++) //go through line names (a,b,c)
                {
                    for(int c = 0; c < numVars; c++)// go through curr var list
                    {
                        if(varlist[c].lexeme == line[linepos].names[thename])//found same var in var list
                        {
                            if((varlist[c].token_type == ID || varlist[c].token_type == 0 || varlist[c].token_type == -1) && line[linepos].linetype != 0)
                            {
                                //cout<<"set var: "<<varlist[c].lexeme<<" linetype: "<<line[linepos].linetype<<endl;
                                varlist[c].token_type = line[linepos].linetype;
                            }
                        }
                    }
                }
        }
    }
    else if(toke1.token_type == NUM || toke1.token_type == REALNUM)
    {
        wasRelOp = true;
        parse_primary();
        parse_relop();
        toke2 = peek();
        parse_primary();
    }
    else
    {
        syntax_error();//1 points
    }
    int p1;
    int p2;
    if(wasRelOp == true)
    {
        int p1, p2;
        if(toke1.token_type != toke2.token_type && toke1.token_type != ID && toke2.token_type!= ID)
        {
            //cout<<"top"<<endl;
            ErrorCode(toke1.line_no,"C3");
        }
        else if(toke1.token_type == ID && toke2.token_type == ID)
        {
            for(int a = 0; a<numVars;a++)
            {
                if(varlist[a].lexeme ==toke1.lexeme)
                {
                    p1 = a;
                }
                if(varlist[a].lexeme == toke2.lexeme)
                {
                    p2 = a;
                }
            }
            if(varlist[p1].token_type != varlist[p2].token_type)
            {
                ErrorCode(toke1.line_no,"C3");
            }
        }
        else
        {
            if(toke1.token_type == NUM)
            {
                toke1.token_type = INT;
            }
            if(toke1.token_type == REALNUM)
            {
                toke1.token_type = REAL;
            }
            if(toke2.token_type == NUM)
            {
                toke2.token_type = INT;
            }
            if(toke2.token_type == REALNUM)
            {
                toke2.token_type = REAL;
            }

            for(int a = 0; a<numVars; a++)
            {
                if(varlist[a].lexeme == toke1.lexeme)
                {
                    p1 = a;
                    if(varlist[a].token_type != toke2.token_type)  // ID < OTHER
                    {
                        if(varlist[a].token_type!= -1 && varlist[a].token_type != 0 && varlist[a].token_type != ID &&toke2.token_type != ID)
                        {
                                //cout<<"mid "<<varlist[a].token_type<<" "<<toke2.token_type <<endl;
                                t13 = true;
                                t13line = toke2.line_no;
                        }
                    }
                }
                if(varlist[a].lexeme == toke2.lexeme)
                {
                    p2 = a;
                    if(toke1.token_type != varlist[a].token_type) // OTHER < ID
                    {
                        if(varlist[a].token_type!= -1 && varlist[a].token_type != 0 && varlist[a].token_type != ID &&toke1.token_type != ID)
                        {
                            //cout<<"a "<<varlist[a].token_type<<" "<<toke1.token_type <<endl;
                            //cout<<"bottom"<<endl;
                               t13 = true;
                               t13line = toke2.line_no;
                        }
                    }
                }
            }

        }
    }
    wasRelOp = false;
    //cout<<"CONDITION t: "<<line[linepos].linetype<<endl;

    linepos++;
    // TODO
}
void Parser::parse_primary()//[done]
{
    Token tok = peek();
    ////////49th POINT/////////////
    if(tok.token_type == ID)
    {
        wasnew = true;
        expect(ID);
    }
    else if(tok.token_type == NUM)
    {
        wasnew = true;
        expect(NUM);
    }
    else if(tok.token_type == REALNUM)
    {
        wasnew = true;
        expect(REALNUM);
    }
    else
    {
        syntax_error(); //GETS 1 POINTS
    }
    // TODO
}

void Parser::parse_relop() // [done]
{
    Token toke = peek();
        if(toke.token_type == GREATER)
        {
            expect(GREATER);
        }
        else if(toke.token_type == GTEQ)
        {
            expect(GTEQ);
        }
        else if(toke.token_type == LESS)
        {
            expect(LESS);
        }
        else if(toke.token_type == NOTEQUAL)
        {
            expect(NOTEQUAL);
        }
        else if(toke.token_type == LTEQ)
        {
            expect(LTEQ);
        }
        else
        {
            //syntax_error();// GETS 0 POINTS
        }
}

void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
}

int typesp[255];
int varsp[255];
int idTsp[255];

int main()
{
    Parser parser;
    parser.ParseInput();
    bool first = true;
    bool nonused = false;
    //cout<<"--print section--"<<endl;
    //cout<<"--print section--"<<endl;


    if(t13 == true)
    {
        TypeMismatch(t13line,"C3"); //actually fixed some things
    }

    for(int a = 0;a < numVars;a++)
        {
                //cout<<"vars:"<<varlist[a].lexeme<<" "<<varlist[a].token_type<<" "<<varlist[a].type<<endl;
                varsp[a] = 0;
        }
    //cout<<"----"<<endl;
    for(int b = 0;b<numTypes;b++)
        {
                //cout<<"Types: "<<typelist[b].lexeme<<" "<<typelist[b].token_type<<" "<<typelist[b].type<<endl;
                typesp[b] = 0;
        }
    //cout<<"----"<<endl;
        for(int c = 0; c < numIdT; c++)
        {
                //cout<<"IDTypes: "<<idtypelist[c].lexeme<<" "<<idtypelist[c].token_type<<endl;
                idTsp[c] = 0;
        }

        // im not sure if you even have to do this.....
        // goes back to previous lines to assign types if a symbol on a line had its type later identified.
        bool changed = true;
        while(changed == true)
        {
            changed = false;
            for(int theline = 0;theline < linepos; theline++)
            {
                if(line[theline].linetype == -1 || line[theline].linetype == 0)
                {
                    for(int thename = 0; thename < numnames[theline];thename++)//go through line names (a,b,c)
                    {
                        for(int c = 0; c < numVars; c++)// go through curr var list
                        {
                            if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                            {
                                if(varlist[c].token_type != -1 && varlist[c].token_type != ID)
                                {
                                    changed = true;
                                    line[theline].linetype = varlist[c].token_type;
                                }
                            }
                        }
                    }
                }
            }
            for(int theline = 0;theline < linepos; theline++)
            {
                for(int thename = 0; thename<numnames[theline];thename++)//go through line names (a,b,c)
                {
                    for(int c = 0; c < numVars; c++)// go through curr var list
                    {
                        if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                        {
                           if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[theline].linetype != 0)
                            {
                                varlist[c].token_type = line[theline].linetype;
                            }
                        }
                    }
                }
            }
        }
        for(int theline = 0;theline < linepos; theline++)
        {
            //cout<<line[theline].linetype<<": ";
            for(int thename = 0; thename<numnames[theline];thename++)//go through line names (a,b,c)
            {
                //cout<<line[theline].names[thename]<<" ";
                //for(int c = 0; c < numVars; c++)// go through curr var list
                {
                    //if(varlist[c].lexeme == line[theline].names[thename])//found same var in var list
                    {
                       // if((varlist[c].token_type == ID || varlist[c].token_type == -1) && line[theline].linetype != 0)
                        {
                            //varlist[c].token_type = line[theline].linetype;
                        }
                    }
                }
            }
           //cout<<endl;
        }





   //cout<<"--print section--"<<endl;

/////////PRINTING SHOULD NOW BE COMPLETED/////////////
//    ___  ___   ___  _    ___   _   _  _
//   | _ )/ _ \ / _ \| |  | __| /_\ | \| |
//   | _ \ (_) | (_) | |__| _| / _ \| .` |
//   |___/\___/ \___/|____|___/_/ \_\_|\_|
//

    cout<<"BOOLEAN";
    for(int b = 0; b < numTypes; b++){
        if(typelist[b].token_type == BOOLEAN && typelist[b].type != "*" && typesp[b] == 0)
        {
            cout<<" "<<typelist[b].lexeme;
            typesp[b] = 1;
            /////prints dot types
            if(typelist[b].type.length()>0){
                if(typelist[b].type != typelist[b+1].type){
                    for(int a = 0; a < numTypes; a++)
                    {
                        if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                        {
                            cout<<" "<<typelist[a].lexeme;
                            typesp[a] = 1;
                        }
                    }
                    for(int a = 0; a < numIdT; a++)
                    {
                        if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                        {
                            cout<<" "<<idtypelist[a].lexeme;
                            idTsp[a] = 1;
                        }
                    }

                }//end b+1 if
            }
                //////end if
            //////
        }
    }
    for(int b = 0; b<numVars; b++){
        if(varlist[b].token_type == BOOLEAN && varsp[b]==0)
        {
            cout<<" "<<varlist[b].lexeme;
            varsp[b] = 1;
            ///////////////for Var dot Types
            if(varlist[b].type.length()>0)
            {
                if(varlist[b].type != varlist[b+1].type)
                {
                    for(int a = 0; a < numTypes; a++){
                        if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                        {
                            cout<<" "<<typelist[a].lexeme;
                            typesp[a] = 1;
                        }
                    }
                    for(int a = 0; a < numIdT; a++){
                        if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                        {
                            cout<<" "<<idtypelist[a].lexeme;
                            idTsp[a] = 1;
                        }
                    }
                }
            }
                ///////////////////////
        }
    }
    cout<<" #\n";

 /////////////////////////////
//    ___ _  _ _____
//   |_ _| \| |_   _|
//    | || .` | | |
//   |___|_|\_| |_|
///////////////////////////////

    cout<<"INT";
    first = true;
    nonused = false;
        for(int b = 0; b < numTypes;b++)
        {
            if(typelist[b].token_type == INT && typelist[b].type != "*" && typesp[b] == 0)// && typesp[b] == 0)
            {
                typesp[b] = 1;
                cout<<" "<<typelist[b].lexeme;
                if(typelist[b].type.length()>0)
                {
                    if(typelist[b].type != typelist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }

                    }//end b+1 if
                }
                //////end if
            }
        }
        for(int b = 0;b<numVars;b++)
        {
            if(varlist[b].token_type == INT&& varsp[b]==0)
            {
                cout<<" "<<varlist[b].lexeme;
                varsp[b]=1;
                ///////////////for Var dot Types
                if(varlist[b].type.length()>0)
                {
                    if(varlist[b].type != varlist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }
                    }
                }
                ///////////////////////
            }
        }
    cout<<" #\n";

//    _    ___  _  _  ___
//   | |  / _ \| \| |/ __|
//   | |_| (_) | .` | (_ |
//   |____\___/|_|\_|\___|
//


    cout<<"LONG";
    first = true;
    nonused = false;
        for(int b = 0;b< numTypes;b++)
        {
            if(typelist[b].token_type == LONG && typelist[b].type != "*" && typesp[b] == 0)// && typesp[b] == 0)
            {

                typesp[b] = 1;
                cout<<" "<<typelist[b].lexeme;

                //////////prints . types
                if(typelist[b].type.length()>0)
                {
                    if(typelist[b].type != typelist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }

                    }//end b+1 if
                }
                //////end if
            }
        }
        for(int b = 0;b<numVars;b++)
        {
            if(varlist[b].token_type == LONG&& varsp[b]==0)
            {
                cout<<" "<<varlist[b].lexeme;
                varsp[b]=1;
                ///////////////for Var dot Types
                if(varlist[b].type.length()>0)
                {
                    if(varlist[b].type != varlist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }
                    }
                }
                ///////////////////////
            }
        }
    cout<<" #\n";
    first = true;
    nonused = false;
/////////////////////////////////
//    ___ ___   _   _
//   | _ \ __| /_\ | |
//   |   / _| / _ \| |__
//   |_|_\___/_/ \_\____|
//
///////////////////////////////
    cout<<"REAL";
    for(int b = 0;b< numTypes;b++)
        {
            if(typelist[b].token_type == REAL && typelist[b].type != "*" && typesp[b] == 0)// && typesp[b] == 0)
            {
                typesp[b] = 1;
                cout<<" "<<typelist[b].lexeme;

                /////////print .types
                if(typelist[b].type.length()>0)
                {
                    if(typelist[b].type != typelist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }

                    }//end b+1 if
                }
                //////end if

            }
        }
        for(int b = 0;b < numVars;b++){
            if(varlist[b].token_type == REAL && varsp[b]==0){

                cout<<" "<<varlist[b].lexeme;
                varsp[b]=1;
                ///////////////for Var dot Types
                if(varlist[b].type.length()>0)
                {
                    if(varlist[b].type != varlist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }
                    }
                }
                ///////////////////////
            }
        }

    cout<<" #\n";
    first = true;
    nonused = false;
    ///////////////////////
    ////STRING///////
    ///////////////////////////
    ///////////////
    cout<<"STRING";
    for(int b = 0;b< numTypes;b++){
            if(typelist[b].token_type == STRING && typelist[b].type != "*" && typesp[b] == 0)// && typesp[b] == 0)
                {
                    typesp[b] = 1;
                    cout<<" "<<typelist[b].lexeme;
                    if(typelist[b].type.length()>0)
                    {
                        if(typelist[b].type != typelist[b+1].type)
                        {
                            for(int a = 0; a < numTypes; a++)
                            {
                                if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                                {
                                    cout<<" "<<typelist[a].lexeme;
                                    typesp[a] = 1;
                                }
                            }
                            for(int a = 0; a < numIdT; a++)
                            {
                                if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                                {
                                    cout<<" "<<idtypelist[a].lexeme;
                                    idTsp[a] = 1;
                                }
                            }

                        }//end b+1 if
                    }
                    //////end if
                }
        }
    for(int b = 0; b<numVars; b++){
        if(varlist[b].token_type == STRING && varsp[b]==0){
            cout<<" "<<varlist[b].lexeme;
            varsp[b]=1;
            ///////////////for Var dot Types
            if(varlist[b].type.length()>0)
            {
                if(varlist[b].type != varlist[b+1].type)
                {
                    for(int a = 0; a < numTypes; a++)
                    {
                        if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                        {
                            cout<<" "<<typelist[a].lexeme;
                            typesp[a] = 1;
                        }
                    }
                    for(int a = 0; a < numIdT; a++)
                    {
                        if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                        {
                            cout<<" "<<idtypelist[a].lexeme;
                            idTsp[a] = 1;
                        }
                    }
                }
            }
            ///////////////////////
        }

    }
    cout<<" #\n";
    /////////Remaining/////////////
    //////////need to finish///////
    /////////////////////////////////////////////////////////////////////////////
//     ___ _____ _  _ ___ ___
//    / _ \_   _| || | __| _ \
//   | (_) || | | __ | _||   /
//    \___/ |_| |_||_|___|_|_\
//

    first = true;
    nonused = false;
    for(int b = 0;b<numTypes;b++)
        {
            if(typelist[b].token_type == ID && typesp[b] == 0 && typelist[b].type != "*")
            {
                if(first == true)
                {
                    cout<<typelist[b].lexeme;
                    typesp[b] = 1;
                    first = false;
                    nonused = true;
                }
                else
                {
                    cout<<" "<<typelist[b].lexeme;
                    typesp[b] = 1;
                }
                if(typelist[b].type.length()>0)
                {
                    if(typelist[b].type != typelist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }

                    }
                }
            }
        }
    for(int b = 0;b<numVars;b++)
        {
            if(varlist[b].token_type == ID&& varsp[b]==0)
            {
                if(first == true)
                {
                    cout<<varlist[b].lexeme;
                    first = false;
                    nonused = true;
                }
                else
                {
                    cout<<" "<<varlist[b].lexeme;
                    nonused = true;
                }

                ///////////////for Var dot Types
                if(varlist[b].type.length()>0)
                {
                    if(varlist[b].type != varlist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }
                    }
                }
                ///////////////////////
            }
        }
    if(nonused == true)
    {
         cout<<" #\n";
    }
//////////////////////////////////////////
//    _____  _______ ___    _
//   | __\ \/ /_   _| _ \  /_\
//   | _| >  <  | | |   / / _ \
//   |___/_/\_\ |_| |_|_\/_/ \_\
//
    first = true;
    nonused = false;
    for(int b = 0;b<numTypes;b++){
            if(typelist[b].token_type == ID && typelist[b].type != "*" && typesp[b] == 0){
                if(first == true)
                {
                    cout<<typelist[b].lexeme;
                    typesp[b] = 1;
                    first = false;
                    nonused = true;
                }
                else
                {
                    cout<<" "<<typelist[b].lexeme;
                    typesp[b] = 1;
                }
                //
                if(typelist[b].type.length()>0)
                {
                    if(typelist[b].type != typelist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[b].type == typelist[a].lexeme && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(typelist[b].type == idtypelist[a].lexeme && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }

                    }
                }
            }
        }
        for(int b = 0;b<numVars;b++)
        {
            if(varlist[b].token_type == -1 && varsp[b]==0)
            {
                if(first == true)
                {
                   cout<<varlist[b].lexeme;
                   varsp[b] = 1;
                    first= false;
                    nonused = true;
                }
                else
                {
                    cout<<" "<<varlist[b].lexeme;
                    varsp[b]=1;
                    nonused = true;
                }
                ///////////////for Var dot Types
                if(varlist[b].type.length()>0)
                {
                    if(varlist[b].type != varlist[b+1].type)
                    {
                        for(int a = 0; a < numTypes; a++)
                        {
                            if(typelist[a].lexeme == varlist[b].type && typesp[a] == 0) //
                            {
                                cout<<" "<<typelist[a].lexeme;
                                typesp[a] = 1;
                            }
                        }
                        for(int a = 0; a < numIdT; a++)
                        {
                            if(idtypelist[a].lexeme == varlist[b].type && idTsp[a] == 0) //
                            {
                                cout<<" "<<idtypelist[a].lexeme;
                                idTsp[a] = 1;
                            }
                        }
                    }
                }
                ///////////////////////
            }
        }
    if(nonused == true)
    {
         cout<<" #\n";
    }
}


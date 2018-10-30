#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stack>
#include <cmath>
using namespace std;

#define dot {putchar(46);putchar(10);}

#define STR_LENGTH 256
#define EPS 0.000000001

int ns = 1;
class Error
{
     const char   *str;
public:
     Error ( const char *s = NULL )
     {
           str = s;
     }
     void print ()
     {
          cout << "Number of string: " << ns << endl;
          if ( str ) cout << "Error: " << str << endl;
     }
};

enum type_of_token
{
     TOK_NULL, 		//0
     TOK_FUNCTION,	//1
     TOK_VAR,		//2
     TOK_BREAK,		//3
     TOK_CONTINUE,  //4
     TOK_IF,		//5
     TOK_THEN,		//6
     TOK_ELSE,		//7
     TOK_DO,     	//8                          
     TOK_WHILE,		//9
     TOK_FOR,     	//10        
     TOK_READ,      //11       
     TOK_WRITE,     //12    
     TOK_NOT,       //13
     TOK_AND,       //14
     TOK_OR,        //15
     TOK_TRUE,      //16
     TOK_FALSE,     //17
     TOK_INT,       //18
     TOK_BOOL,      //19
     TOK_STRING,	//20
     TOK_ID,        //21         
     TOK_NUM,       //22            
     TOK_FIN, 		//23       
     TOK_LFIG,      //24
     TOK_RFIG,      //25
     TOK_COMMA,     //26
     TOK_COLON,     //27
     TOK_ASSIGN,    //28
     TOK_LPAREN,    //29
     TOK_RPAREN,    //30
     TOK_EQ,        //31
     TOK_LSS,       //32
     TOK_GTR,       //33
     TOK_PLUS,      //34
     TOK_MINUS,     //35
     TOK_TIMES,     //36
     TOK_SLASH,     //37
     TOK_LEQ,       //38
     TOK_NEQ,       //39
     TOK_GEQ,       //40
     TOK_SEMICOLON, //41
     TOK_STRINGVAL, //42
     TOK_TOCH, 		//43
	 TOK_RETURN,	//44
	 TOK_UNDEFINED, //45
     TOK_REAL,
     /*POLIZ_GO,                
     POLIZ_FGO,               
     POLIZ_LABEL,  */          
     POLIZ_ADDRESS,
	 POLIZ_JUMP,
	 POLIZ_END_OF_ARGS,
	 POLIZ_CALL_FUNC,
	 //POLIZ_FUNC_DECL,
	 //POLIZ_END_OF_FUNC
};

class Token
{
	type_of_token type;
	int value;
public:
	Token(type_of_token t = TOK_NULL, int v = 0)
	{
		type = t;
		value = v;
	}
	type_of_token get_type ()
	{ 
		return type; 
	}
    int get_value () 
    { 
    	return value; 
    }
    friend ostream& operator << ( ostream &s, Token t )
    {
        s << '(' << t.type << '.' << t.value << ')';
        return s;
    }
};

/////////////////////////тип данных String////////////////////////
class String
{
      char *str;
public:
      String () { }
	  String ( const char * buf )
	  {
		  str = new char [ strlen(buf)+1];
          strcpy ( str , buf );
		  str[strlen(buf)] = 0;
	  }
      char* get_str () const
      {
            return str;
      }
      void put_str ( const char * buf )
      {
           str = new char [ strlen(buf)+1];
           strcpy ( str , buf );
		   str[strlen(buf)] = 0;
      }
      char* operator+ ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            char *s3 = new char[ strlen(s1) + strlen(s2) + 1 ];
            
            strcpy( s3 , s1 );
            strcat( s3 , s2 );
            return s3;
      }
      bool operator== ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) == 0;
      }
      bool operator> ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) > 0;
      }
      bool operator< ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) < 0;
      }
      bool operator<= ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) <= 0;
      }
      bool operator>= ( const String &s )
      {
            char *s1 = get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) >= 0;
      }
      bool operator!= ( const String &s )
      {
            char *s1 = this -> get_str();
            char *s2 = s.get_str();
            return strcmp( s1 , s2 ) != 0;
      }
};

//////////////////////////ÚËÔ ‰‡ÌÌ˚ı Number////////////////////////

enum number_type
{
	TYPE_UNDEFINED,
	TYPE_NAN,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_DOUBLE,
	TYPE_STRING
};

class Number
{
	number_type type;
	bool n_bool;
	int n_int;
	double n_double;
	String n_string;
public:
      Number ()
	  {
		  type = TYPE_UNDEFINED;
	  }
	  number_type get_type()
	  {
		  return type;
	  }
	  bool to_bool()
	  {
		  if (type == TYPE_UNDEFINED) return false;
		  else if (type == TYPE_NAN) return false;
		  else if (type == TYPE_BOOL) return n_bool;
		  else if (type == TYPE_INT) return n_int != 0;
		  else if (type == TYPE_DOUBLE) return (n_double > EPS || n_double < -EPS);
		  else if (type == TYPE_STRING) return n_string != String("");
		  else return false;
	  }
	  int to_int()
	  {
		  if (type == TYPE_UNDEFINED) throw "undefined to int";
		  else if (type == TYPE_NAN) throw "nan to int";
		  else if (type == TYPE_BOOL) return (n_bool ? 1 : 0);
		  else if (type == TYPE_INT) return n_int;
		  else if (type == TYPE_DOUBLE) return (int)n_double;
		  else if (type == TYPE_STRING)
		  {
			  int re = 0;
			  sscanf( n_string.get_str(), "%d", &re );
			  return re;
		  }
		  return 0;
	  }
	  double to_double()
	  {
		  if (type == TYPE_UNDEFINED) throw "undefined to double";
		  else if (type == TYPE_NAN) throw "nan to double";
		  else if (type == TYPE_BOOL) return (n_bool ? 1.0 : 0.0);
		  else if (type == TYPE_INT) return (double)n_int;
		  else if (type == TYPE_DOUBLE) return n_double;
		  else if (type == TYPE_STRING)
		  {
			  double re = 0;
			  sscanf( n_string.get_str(), "%lf", &re );
			  return re;
		  }
		  return 0.;
	  }
	  String to_string()
	  {
		  if (type == TYPE_UNDEFINED) return "undefined";
		  else if (type == TYPE_NAN) return "nan";
		  else if (type == TYPE_BOOL) return (n_bool ? "true" : "false");
		  else if (type == TYPE_INT)
		  {
			  char re[20];
			  sprintf( re, "%d", n_int );
			  return re;
		  }
		  else if (type == TYPE_DOUBLE)
		  {
			  char re[100];
			  sprintf( re, "%10lf", n_double);
			  return re;
		  }
		  else if (type == TYPE_STRING)
			  return n_string;
		  return String("");
	  }
      void set_undefined()
      {
          type = TYPE_UNDEFINED;
      }
	  void set_nan()
	  {
		  type = TYPE_NAN;
	  }
	  void set_bool( bool b )
	  {
		  type = TYPE_BOOL;
		  n_bool = b;
	  }
	  void set_int( int i )
	  {
		  type = TYPE_INT;
		  n_int = i;
	  }
	  void set_double( double d )
	  {
		  type = TYPE_DOUBLE;
		  n_double = d;
	  }
	  void set_string( String s )
	  {
		  type = TYPE_STRING;
		  n_string  = s;
	  }
      Number operator+ ( Number &n )
      {
		  Number re;
		  if (n.get_type() == TYPE_STRING)
		  {
			  re.set_string( to_string() + n.to_string() );
		  }
		  else if (type == TYPE_UNDEFINED)
		  {
			  re.set_undefined();
		  }
		  else if (type == TYPE_NAN)
		  {
			  re.set_nan();
		  }
		  else if (type == TYPE_BOOL)
		  {
			  re.set_bool( to_bool() ^ n.to_bool() );
		  }
		  else if (type == TYPE_INT)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else re.set_int( to_int() + n.to_int() );
			  if (re.to_int() > 1000000000 || re.to_int() < -1000000000)
				  re.set_double( (double)re.to_int() );
		  }
		  else if (type == TYPE_DOUBLE)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else re.set_double( to_double() + n.to_double() );
		  }
		  else if (type == TYPE_STRING)
		  {
			  re.set_string( to_string() + n.to_string() );
		  }

		  return re;
      }
      Number operator- ( Number &n )
      {
      		Number re;
		  if (type == TYPE_UNDEFINED)
		  {
			  re.set_undefined();
		  }
		  else if (type == TYPE_NAN)
		  {
			  re.set_nan();
		  }
		  else if (type == TYPE_BOOL)
		  {
			  re.set_bool( to_bool() ^ n.to_bool() );
		  }
		  else if (type == TYPE_INT)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else re.set_int( to_int() - n.to_int() );
			  if (re.to_int() > 1000000000 || re.to_int() < -1000000000)
				  re.set_double( (double)re.to_int() );
		  }
		  else if (type == TYPE_DOUBLE)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else re.set_double( to_double() - n.to_double() );
		  }
		  else if (type == TYPE_STRING)
		  {
			  re.set_string( to_string() );
		  }

		  return re;
      }
      /*int operator-- (int n)
      {
      		int n3 = n - 1;
      		
      		return n3;
      }
      int operator++ (int n)
      {
      		int n3 = n + 1;
      		
      		return n3;
      }*/
      Number operator* (Number &n)
	  {
		  Number re;
		  if (type == TYPE_UNDEFINED)
		  {
			  re.set_undefined();
		  }
		  else if (type == TYPE_NAN)
		  {
			  re.set_nan();
		  }
		  else if (type == TYPE_BOOL)
		  {
			  re.set_bool( to_bool() && n.to_bool() );
		  }
		  else if (type == TYPE_INT)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else
			  {
				  long long tmp = (long long)to_int() * (long long)n.to_int();
				  if (tmp > 1000000000 || tmp < -1000000000)
						re.set_double( (double)tmp );
				  else re.set_int( (int)tmp );
			  }
		  }
		  else if (type == TYPE_DOUBLE)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else re.set_double( to_double() * n.to_double() );
		  }
		  else if (type == TYPE_STRING)
		  {
			  // ÛÏÌÓÊÂÌËÂ ÒÚÓÍ? ÔÛÒÚ¸ ÚÓÊÂ ·Û‰ÂÚ ·ÂÁ Â‡ÍˆËË
			  re.set_string( to_string() );
		  }

		  return re;
      }
      Number operator/ (Number &n)
      {
      	  Number re;
		  if (type == TYPE_UNDEFINED)
		  {
			  re.set_undefined();
		  }
		  else if (type == TYPE_NAN)
		  {
			  re.set_nan();
		  }
		  else if (type == TYPE_BOOL)
		  {
			  // деление?
			  re.set_bool( to_bool() || n.to_bool() );
		  }
		  else if (type == TYPE_INT)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else
			  {
				  if (n.to_int() == 0)
					  re.set_nan();
				  else if (to_int() % n.to_int() == 0)
					  re.set_int( to_int() / n.to_int() );
				  else re.set_double( (double)to_int() / (double)n.to_int() );
			  }
		  }
		  else if (type == TYPE_DOUBLE)
		  {
			  if (n.get_type() == TYPE_UNDEFINED)
				  re.set_undefined();
			  else if (n.get_type() == TYPE_NAN)
				  re.set_nan();
			  else if ( n.to_double() < EPS && n.to_double() > -EPS )
				  re.set_nan();
			  else
				  re.set_double( to_double() / n.to_double() );
		  }
		  else if (type == TYPE_STRING)
		  {
			  // ‚ÓÚ Â˘Â ÒÚÓÍË Ï˚ ‰Û„ Ì‡ ‰Û„‡ ÌÂ ‰ÂÎËÎË. ·ÂÁ Â‡ÍˆËË
			  re.set_string( to_string() );
		  }

		  return re;
      }
      
      
      Number operator== ( Number &n )
      {
            Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( n.get_type() == TYPE_UNDEFINED );
			else if (type == TYPE_NAN)
				re.set_bool( false );
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( fabs( to_double() - n.to_double() ) < EPS );
				else re.set_bool( to_int() == n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( fabs( to_double() - n.to_double() ) < EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_string() == n.to_string() );
			}
            return re;
      }
      Number operator> ( Number &n )
      {
            Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( false );
			else if (type == TYPE_NAN)
				re.set_bool( false );
			else if (type == TYPE_BOOL)
			{
				re.set_bool( to_bool() == n.to_bool() );
			}
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( to_double() > n.to_double()+EPS );
				else re.set_bool( to_int() > n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_double() > n.to_double()+EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_string() > n.to_string() );
			}
            return re;
      }
      Number operator< ( Number &n )
      {
            Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( false );
			else if (type == TYPE_NAN)
				re.set_bool( false );
			else if (type == TYPE_BOOL)
			{
				re.set_bool( (to_bool()?1:0) < (n.to_bool()?1:0) );
			}
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( to_double() < n.to_double()-EPS );
				else re.set_bool( to_int() < n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_double() < n.to_double()-EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_string() < n.to_string() );
			}
            return re;
      }
      Number operator<= ( Number &n )
      {
            Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( false );
			else if (type == TYPE_NAN)
				re.set_bool( false );
			else if (type == TYPE_BOOL)
			{
				re.set_bool( (to_bool()?1:0) <= (n.to_bool()?1:0) );
			}
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( to_double() < n.to_double()+EPS );
				else re.set_bool( to_int() <= n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_double() < n.to_double()+EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_string() <= n.to_string() );
			}
            return re;
      }
      Number operator>= ( Number &n )
      {
            Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( false );
			else if (type == TYPE_NAN)
				re.set_bool( false );
			else if (type == TYPE_BOOL)
			{
				re.set_bool( (to_bool()?1:0) >= (n.to_bool()?1:0) );
			}
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( to_double() > n.to_double()-EPS );
				else re.set_bool( to_int() >= n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_double() > n.to_double()-EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( false );
				else re.set_bool( to_string() >= n.to_string() );
			}
            return re;
      }
      Number operator!= ( Number &n )
      {
		    Number re;
		    if (type == TYPE_UNDEFINED)
				re.set_bool( n.get_type() != TYPE_UNDEFINED );
			else if (type == TYPE_NAN)
				re.set_bool( true );
			else if (type == TYPE_INT)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( true );
				else if (n.get_type() == TYPE_DOUBLE)
					re.set_bool( fabs( to_double() - n.to_double() ) > EPS );
				else re.set_bool( to_int() != n.to_int() );
			}
			else if (type == TYPE_DOUBLE)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( true );
				else re.set_bool( fabs( to_double() - n.to_double() ) > EPS );
			}
			else if (type == TYPE_STRING)
			{
				if (n.get_type() == TYPE_UNDEFINED || n.get_type() == TYPE_NAN)
					re.set_bool( true );
				else re.set_bool( to_string() != n.to_string() );
			}
            return re;
      }
};

class Identificator
{
	char *name;
    bool declare;
    //type_of_token type;
    //bool assign;
    //int value;
	Number value;
public:
      Identificator ()
      {
            declare = false;
            //assign = false;
      }
      ~Identificator ()
      {
             delete []name;
      }
      char* get_name ()
      {
            return name;
      }
      void put_name ( const char *str )
      {
           name = new char [ strlen(str)+1 ];
           strcpy ( name, str );
		   name[ strlen(str) ] = 0;
      }
      bool get_declare ()
      {
           return declare;
      }
      void put_declare ()
      {
           declare = true;
      }
      void take_declare ()
      {
           declare = false;
      }
      //type_of_token get_type ()
      //{
      //     return type;
      //}
      //void put_type ( type_of_token t )
      //{
      //     type = t;
      //}
      //bool get_assign ()
      //{
      //     return assign;
      //}
      //void put_assign ()
      //{
      //     assign = true;
      //}
      Number get_value ()
      {
           return value;
      }
      void put_value ( Number v )
      {
           value = v;
      }
public:
	
};

class Table_ident
{
      Identificator  *p;
      int size;
      int top;
public:
      Table_ident ( int max_size )
      {
           p = new Identificator [ size = max_size ];
           top = 1;
      }
      /*~Table_ident ()
      {
           delete []p;
      }*/
      Identificator& operator [] ( int k )
      {
           return p[k];
      }
	  int get_top() { return top; }
	  int put_local() { p[top++].put_name( "local" ); return top-1; }
	  void remove_local() { top--; }
      int put ( const char *buf );
	  void print()
	  {
		  char * types [] = { 
		  						(char*)"undefined", 
		  						(char*)"nan", 
		  						(char*)"bool", 
		  						(char*)"int", 
		  						(char*)"double", 
		  						(char*)"string" 
		  					};
		  for (int i=1; i<top; i++)
			  cout << p[i].get_name() << " type=" << types[p[i].get_value().get_type()]
					<< " value=" << p[i].get_value().to_string().get_str() << "\n";
	  }

};

int Table_ident::put ( const char *buf )
{
    for ( int j = 1; j < top; j++ )
         if ( !strcmp( buf, p[j].get_name() ) ) 
         	return j; // строка таблицы соответствует  полученному имени идентификатора
    ///иначе добавляем новую строчку в таблицу
    p[top].put_name( buf );
	++top;
    return top-1; 
}

Table_ident TID(100);

class Scanner
{
      enum state { H, 	 	
      			   IDENT, 
      			   NUMB,  
      			   ALE,  
      			   DELIM,
      			   NEQ 
      			 };
      static char *TW[];
      static type_of_token words[];
      static char *TD[];
      static type_of_token limiter[];
      state CS; 
      string text;
      int c;
      FILE *fp;
      char* buf;
      int  buf_top;
      int  bp;
      
      void clear ()
      {
           buf_top = 0;
           for ( int j = 0; j < 80; ++j ) 
           		buf[j] = '\0';
      }
      void add () 
      {
           buf [ buf_top++ ] = c;
      }

      int look ( const char *buf, char ** list) 
      {
           int i = 1;
           while ( list[i] )
           {
                 if ( !strcmp( buf,list[i] ) ) 
                 	return i;
                 ++i;
           }
           return 0;
      }
      void gc ()
      {
           c = fgetc (fp);
      }
public:
      Token get_token ();
      int END;
      Scanner ( const char * program)
      {
      		  END = 0;
			  buf = (char*)malloc(80);
        	  fp = fopen ( program , "r" );
        	  if ( fp == NULL )
            		cout << "Can't open file"<<endl;
              CS = H; 
              clear();
              gc();
              bp = 0;
      }
	  //для отладки
	  static type_of_token * get_words() { return words; }
	  static char ** get_tw() { return TW; }
	  static type_of_token * get_limiter() { return limiter; }
	  static char ** get_td() { return TD; }
};
 
char *Scanner::TW[] =
{
     NULL,					//0
     (char*)"function", 	//1
     (char*)"var",			//2
     (char*)"break",		//3
     (char*)"continue",		//4
     (char*)"if",			//5
     (char*)"then",			//6
     (char*)"else",			//7
     (char*)"do",			//8
     (char*)"while",		//9
     (char*)"for",			//10
     (char*)"read",			//11
     (char*)"write",		//12
     (char*)"not",			//13
     (char*)"and",			//14
     (char*)"or",			//15
     (char*)"true",			//16
     (char*)"false",		//17
     (char*)"int",			//18
     (char*)"bool",			//19
     (char*)"string",		//20
	 (char*)"return",		//21
     (char*)"undefined",    //22
     (char*)"real",
     NULL
};

type_of_token Scanner::words[] =
{
     TOK_NULL,    //0
     TOK_FUNCTION,//1
     TOK_VAR,	  //2
     TOK_BREAK,	  //3
     TOK_CONTINUE,//4
     TOK_IF,	  //5
     TOK_THEN,	  //6
     TOK_ELSE,	  //7
     TOK_DO,	  //8
     TOK_WHILE,	  //9
     TOK_FOR,	  //10
     TOK_READ,	  //11
     TOK_WRITE,   //12
     TOK_NOT,	  //13
     TOK_AND,	  //14
     TOK_OR,	  //15
     TOK_TRUE,	  //16
     TOK_FALSE,	  //17
     TOK_INT,	  //18
     TOK_BOOL,	  //19
     TOK_STRING,  //20
	 TOK_RETURN,  //21
	 TOK_UNDEFINED, //22
     TOK_REAL,
     TOK_NULL
};

char *Scanner::TD[] =
{
     NULL,			//0
     (char*)"{",	//1
     (char*)"}",	//2
     (char*)";",	//3
     (char*)",",	//4
     (char*)".",    //5
     (char*)":",	//6
     (char*)"=",	//7
     (char*)"(",	//8
     (char*)")",	//9
     (char*)"==",	//10
     (char*)"<",	//11
     (char*)">",	//12
     (char*)"+",	//13
     (char*)"-",	//14
     (char*)"*",	//15
     (char*)"/",	//16
     (char*)"<=",	//17
     (char*)"!=",	//18
     (char*)">=",	//19
     NULL
};

type_of_token Scanner::limiter[] =
{
     TOK_NULL,		//0
     TOK_LFIG, //{    1
     TOK_RFIG, //}    2
     TOK_FIN,  //;    3
     TOK_COMMA, //,   4
     TOK_TOCH,  //.   5    
     TOK_COLON, //:   6
     TOK_ASSIGN, //=  7
     TOK_LPAREN,//(   8
     TOK_RPAREN, //)  9
     TOK_EQ, //==     10
     TOK_LSS, //<     11
     TOK_GTR, //>     12
     TOK_PLUS, //+    13
     TOK_MINUS, //-   14
     TOK_TIMES, //*   15
     TOK_SLASH, // /  16
     TOK_LEQ, //<=    17
     TOK_NEQ, //!=    18
     TOK_GEQ, // >=   19
     TOK_NULL        
};

bool isalpha ( char c )
{
     if ((c>='a')&&(c<='z')) 
     	return true;
     if ((c>='A')&&(c<='Z')) 
     	return true;
     return false;
}
bool isdigit( char c )
{
     if ((c>='0')&&(c<='9')) 
     	return true;
     return false;
}

class Table_String
{
      String  *s;
      int      size;
      int      top;
public:
      Table_String ( int max_size )
      {
          s = new String [ size = max_size ];
          top = 0;
      }
      /*~Table_String ()
      {
          delete []s;
      }*/
      String& operator[] ( int k )
      {
          return s[k];
      }
      int put ( const char *buf )
      {
          s[top].put_str( buf );
          top++;
          return top-1;
      }
};

Table_String TS(100);

////////////////////////////лексический анализ///////////////////////
Token Scanner::get_token ()
{
    int d = 0, j;
    CS = H;
    while(1)
    {
         if (END == 1)
         {
            return Token(TOK_NULL,0);
         }
         switch ( CS )
         {
                case H:
                	 if ( c == EOF )
                	 { 
                    	END = 1;
                    	break;
                	 }
                     else if ( c == ' ' || c == '\n' || c == '\r' || c == '\t' )
                     {
                          if ( c == '\n' ) 
                          		ns++;
                          gc();
                     }
                     else if ( isalpha(c) )
                     {
                          clear();
                          add();
                          gc();
                          CS = IDENT;
                     }
                     else if ( isdigit(c) )
                     {
                          d = c -'0';
                          gc();
                          CS = NUMB;
                     }
                     else if ( c == '}')
                     {
                     	  bp--;
                          if(bp < 0) 
                          		throw Error( "In Scan(). Balance. }.");
                          clear();
                          add();
                          gc();
                          j = look( buf, TD);
                          //cout << buf << endl;
                          return Token( limiter[j], j );
                     }
                     else if ( c == '{' )
                     {
                          bp++;
                          clear();
                          add();
                          gc();
                          j = look( buf, TD );
                          //cout << buf << endl;
                          return Token( limiter[j], j );
                     }
                     else if ( c == '=' || c == '<' || c == '>' )
                     {
                          clear();
                          add();
                          gc();
                          CS = ALE;
                     }
                     else if ( c == '!' )
                     {
                          clear();
                          add();
                          gc();
                          CS = NEQ;
                     }
                     else if ( c == '"')
                     {
                          clear();
                          gc();
                          while ( c != '"' )
                          {
                                  if ( c == '\n' ) 
                                  		ns++;
                                  add();
                                  gc();
                          }
                          gc();
                          j = TS.put( buf );
                         // cout << buf << endl;
                          return ( Token( TOK_STRINGVAL , j ) );
                     }
                     else if (c == '/')
                     {	
                           clear();
                           add();
                           gc();
                           if ( c == '/' )
                           {
                           		do
                           	  		gc();  
                           		while( c != '\n' ) ;
                           		ns++;
                           }
						   else CS = ALE;
                     }
                     else
                     {
                           CS = DELIM;
                     }
                     break;
                     
                case IDENT:
                     if ( isalpha(c) || isdigit(c) || c == '_' )
                     {	  
                          add();
                          gc();
                     }
                     else
                     {
                          if ( (j = look( buf, TW )) )
                          {		
                          		//cout << buf << endl;
                             	return Token( words[j], j);
                          }
                          else
                          {		
                              	j = TID.put( buf );
                              	//cout << buf << endl;
                              	return Token( TOK_ID , j);
                          }
                     }
                     break;
                     
                case NUMB:
                	 
                     if ( isdigit(c) )
                     {
                          	d = d * 10 + (c - '0');
                          	gc();
                     }
                     else if ( isalpha(c) )
                     {
                          	throw Error( "In Scan(). Symbol into number." );
                     }
                     else
                     {
                     		//cout << d << endl;
                         	return Token( TOK_NUM, d );
                     }
                     break;
                     
                case ALE:
                     if ( c == '=' )
                     {
                          add();
                          gc();
                          j = look( buf, TD );
                          //cout << buf << endl;
                          return Token( limiter[j], j );
                     }
                     else 
                     {
                          if ( (j = look( buf , TD )) != 0 )
                          {
                               //	cout << buf << endl;
                                return Token( limiter[j], j );
                          }
                          //cout << buf << endl;
                          return Token( TOK_ASSIGN, j);
                     }
                     break;
                     
                case NEQ:
                     if ( c == '=' )
                     {
                          add();
                          gc();
                          j = look ( buf, TD );
                          ///cout << buf << endl;
                          return Token( TOK_NEQ, j);
                     }
                     else 
                     	  throw Error( "In Scan(). NEQ" );
                     break;
                     
                case DELIM:
                     clear();
                     add();
                     if ( (j = look (buf, TD)) != 0)
                     {	
                          gc();
                          //cout << buf << endl;
                          return Token( limiter[j], j );
                     }
                     else 
                     	  throw Error( "In Scan(). Unknown error.");
                     break;
         }
    } 
}

/////////////////////////    Poliz  /////////////////////////////
 
class Poliz
{
    Token * p;
    int   size;
    int   free;
public:
	Poliz (int max_size)
	{
        p = new Token [size = max_size];
    	free = 0;
    }
    ~Poliz() 
    { 
    	delete [] p; 
    }
    int put_token ( Token t )
	{
        p [free] = t;
        free++;
		return free-1;
    }
    void put_token ( Token t, int place) 
    { 	
    	p [place] = t; 
    }
    void blank()
    { 
    	free++; 
    }
    int get_free()
    { 
    	return free; 
    }
    Token & operator[] (int index)
    {
    	if ( index > size )
        	throw "POLIZ:out of array";
        else
		{
        	if ( index > free )
            	throw "POLIZ:indefinite element of array";
            else
                return p[index];
		}
    }
    void print ()
    {
        for ( int i = 0; i < free; ++i )
		{
			//if (i%4==3) cout << "\n";
			//else cout << "\t";
			//cout << "\n";
			cout << i << ":";
			bool flag = false;
			for ( int j = 1; Scanner::get_words()[j]!=TOK_NULL; j++ )
				if ( p[i].get_type() == Scanner::get_words()[j] )
				{
					cout << Scanner::get_tw()[j] << " ";
					flag = true;
					break;
				}
			if (flag) continue;
			if (p[i].get_type() == POLIZ_JUMP)
			{
				cout << "jump ";
				continue;
			}
			if (p[i].get_type() == POLIZ_ADDRESS)
			{
				cout << "addr" << p[i].get_value() << " ";
				continue;
			}
			if (p[i].get_type() == POLIZ_END_OF_ARGS)
			{
				cout << "args_end ";
				continue;
			}
			if (p[i].get_type() == POLIZ_CALL_FUNC)
			{
				cout << "call ";
				continue;
			}
			if (p[i].get_type() == TOK_FIN)
			{
				cout << "pop ";
				continue;
			}
			for ( int j = 1; Scanner::get_limiter()[j]!=TOK_NULL; j++ )
				if ( p[i].get_type() == Scanner::get_limiter()[j] )
				{
					cout << Scanner::get_td()[j] << " ";
					flag = true;
					break;
				}
			if (flag) continue;
			if (p[i].get_type() == TOK_ID)
				cout << TID[p[i].get_value()].get_name() << " ";
			else if (p[i].get_type() == TOK_NUM)
				cout << p[i].get_value() << " ";
			else
				cout << p[i] << " ";
		}
		cout << "\n";
    }
};
 
///////////////////////////  Parser  /////////////////////////////////
 
class Parser 
{
         Token curr_token;
         type_of_token  c_type;
         int c_val;
         Scanner scan;

         void         program();
		 void         sentense();
		 void 		  function();
         void         operatorr( int break_ind, int cont_ind, bool in_fun );
		 void         variable_declaration();
		 void		  null_statement();
		 void 		  block( int break_ind, int cont_ind, bool in_fun );
         void 		  conditional_operator( int break_ind, int cont_ind, bool in_fun );
         void 		  cycle_operator( int break_ind, int cont_ind, bool in_fun );
         void 		  transition_operator( int break_ind, int cont_ind, bool in_fun );

		 void 		  expression_operator();
         void         expression();
		 void         expression_description();
         void         T();
         void         F();
         
         void gl ()
         {
         	curr_token = scan.get_token();
            c_type = curr_token.get_type();
            c_val = curr_token.get_value();
         }
public:
		Poliz  prog;
    	Parser (const char *program ):scan(program), prog(1000) {}
        void parse();
};
 
void Parser::parse ()
{
	gl();
  	program();
  	//cout << endl << "Yes!!!" << endl;
}

void Parser::program()
{
	while(1)
	{
		if ( c_type == TOK_NULL )
			break;
		sentense();
		//gl();
	}
}

void Parser::sentense()
{
	if ( c_type == TOK_FUNCTION )
	{
		function();
	}
	else
	{
		operatorr( -1, -1, false );
	}
}

///////////////////////////////
void Parser::function ()
{
	if (c_type != TOK_FUNCTION)
		throw curr_token;

	gl(); // function

	if (c_type != TOK_ID)
    	throw curr_token;
  	
	prog.put_token( Token( TOK_ID, c_val ) );
	prog.put_token( Token( TOK_FUNCTION ) );
	gl();

	// тут будет лежать адрес конца функции
	// чтобы Executor перепрыгнул ее при исполнении скрипта
	int end_of_func = prog.put_token( Token( POLIZ_ADDRESS ) );
	prog.put_token( Token( POLIZ_JUMP ) );

	if ( c_type != TOK_LPAREN ) 
		throw curr_token;

	gl(); // (

	int first_arg_ind = prog.get_free();
		
	if (c_type == TOK_RPAREN)
	{
		gl(); // )
		prog.put_token( Token( POLIZ_END_OF_ARGS ) );
		block( -1, -1, true );

		prog.put_token( Token( TOK_UNDEFINED ) );
		prog.put_token( Token( TOK_RETURN ) );
		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), end_of_func );
		return;
	}
	else if (c_type == TOK_ID)
	{
		prog.put_token( Token( TOK_ID, c_val ) );
		gl();
		if (c_type == TOK_RPAREN)
		{
			gl(); // )
			prog.put_token( Token( POLIZ_END_OF_ARGS ) );
			block( -1, -1, true );

			prog.put_token( Token( TOK_UNDEFINED ) );
			prog.put_token( Token( TOK_RETURN ) );
			prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), end_of_func );
			return;
		}

		while(1)
		{
			if (c_type == TOK_RPAREN)
			{
				int last_arg_ind = prog.get_free()-1;
				gl(); // )
				prog.put_token( Token( POLIZ_END_OF_ARGS ) );
				block( -1, -1, true );

				prog.put_token( Token( TOK_UNDEFINED ) );
				prog.put_token( Token( TOK_RETURN ) );
				prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), end_of_func );

				// для нормальной работы Executor'а
				// список аргументов необходимо развернуть
				int i = first_arg_ind, j = last_arg_ind;
				while (i<j)
				{
					Token t1 = prog[i];
					Token t2 = prog[j];
					prog.put_token( t2, i );
					prog.put_token( t1, j );
					i++;
					j--;
				}
				return;
			}
			else if (c_type == TOK_COMMA)
			{
				gl(); // ,
			}
			else throw curr_token;

			if (c_type != TOK_ID)
				throw curr_token;
			prog.put_token( Token( TOK_ID, c_val ) );
			gl();
		}
	}
	else throw curr_token;
}
////////////////////////////// 
void Parser::operatorr ( int break_ind, int cont_ind, bool in_fun )
{
	if (c_type == TOK_VAR)
	{
		variable_declaration();
	}
	else if (c_type == TOK_FIN)
	{
		null_statement();
	}
	else if (c_type == TOK_LFIG)
	{
		block( break_ind, cont_ind, in_fun );
	}
	else if (c_type == TOK_IF)
	{
		conditional_operator( break_ind, cont_ind, in_fun );
	}
	else if ( c_type == TOK_DO || c_type == TOK_WHILE || c_type == TOK_FOR )
	{
		cycle_operator( break_ind, cont_ind, in_fun );
	}
	else if ( c_type == TOK_BREAK || c_type == TOK_CONTINUE || c_type == TOK_RETURN )
	{
		transition_operator( break_ind, cont_ind, in_fun );
	}
	else if ( c_type == TOK_WRITE )
	{
		gl(); // if

		if (c_type != TOK_LPAREN)
			throw curr_token;

		gl(); // (

		expression();	
	    
		if (c_type != TOK_RPAREN)
			throw curr_token;

		gl(); // )

		if (c_type != TOK_FIN)
			throw curr_token;

		gl(); // ;

		prog.put_token( Token( TOK_WRITE ) );
		//prog.put_token( Token( TOK_FIN ) );
	}
	else
	{
		expression_operator();
	}
}      	
///////////////////////////////
void Parser::variable_declaration()
{
	if (c_type != TOK_VAR)
		throw curr_token;

	gl(); // var

	if (c_type != TOK_ID)
		throw curr_token;

	prog.put_token( Token( TOK_ID, c_val ) );
	prog.put_token( Token( TOK_VAR ) );
	gl();

	if (c_type == TOK_ASSIGN)
	{
		gl(); // =
		expression();
		prog.put_token( Token( TOK_ASSIGN ) );
	}
	prog.put_token( Token( TOK_FIN ) );

	while(1)
	{
		if (c_type == TOK_COMMA)
		{
			gl(); // ,
		}
		else if (c_type == TOK_FIN)
		{
			gl(); // ;
			break;
		}
		else throw curr_token;

		if (c_type != TOK_ID)
			throw curr_token;

		prog.put_token( Token( TOK_ID, c_val ) );
		prog.put_token( Token( TOK_VAR ) );
		gl();

		if (c_type == TOK_ASSIGN)
		{
			gl(); // =
			expression();
			prog.put_token( Token( TOK_ASSIGN ) );
		}
		prog.put_token( Token( TOK_FIN ) );
	}
}
///////////////////////////////
void Parser::null_statement ()
{
	if (c_type != TOK_FIN)
		throw curr_token;

	gl(); // ;
}  
///////////////////////////////
void Parser::block ( int break_ind, int cont_ind, bool in_fun )
{
	if (c_type != TOK_LFIG)
		throw curr_token;

	gl(); // {

	operatorr( break_ind, cont_ind, in_fun );
	
	while( 1 )
	{
		if ( c_type == TOK_RFIG )
		{
			gl(); // }
			break;
		}
		else if ( c_type == TOK_NULL )
			throw curr_token;

		operatorr( break_ind, cont_ind, in_fun );
	}
}
///////////////////////////////
void Parser::conditional_operator ( int break_ind, int cont_ind, bool in_fun )
{
	if (c_type != TOK_IF)
		throw curr_token;

	gl(); // if

	if (c_type != TOK_LPAREN)
		throw curr_token;

	gl(); // (

	expression();	
    
    if (c_type != TOK_RPAREN)
		throw curr_token;

	gl(); // )

	int ind1 = prog.put_token( Token( POLIZ_ADDRESS ) );
	int ind2 = prog.put_token( Token( POLIZ_ADDRESS ) );
	prog.put_token( Token( TOK_IF ) );

	prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind1 );
	operatorr( break_ind, cont_ind, in_fun );
  		
  	if (c_type == TOK_ELSE)
  	{   	
        gl(); // else
		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind2 );
        operatorr( break_ind, cont_ind, in_fun );
  	}
	else prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind2 );
}
///////////////////////////////
void Parser::cycle_operator ( int break_ind, int cont_ind, bool in_fun )
{  
    if( c_type == TOK_WHILE )
    {
      	gl(); // while

    	if (c_type != TOK_LPAREN)
			throw curr_token;

		gl(); // (

		int ind0 = prog.get_free();

		expression();	
	    
		if (c_type != TOK_RPAREN)
			throw curr_token;

		gl(); // )

		int ind1 = prog.put_token( Token( POLIZ_ADDRESS ) );
		int ind2 = prog.put_token( Token( POLIZ_ADDRESS ) );
		prog.put_token( Token( TOK_IF ) );

		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind1 );

		operatorr( ind1, ind0, in_fun );

		prog.put_token( Token( POLIZ_ADDRESS, ind0 ) );
		prog.put_token( Token( POLIZ_JUMP ) );
		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind2 );
    }
	else if( c_type == TOK_FOR )
	{	
		gl(); // for

		if (c_type != TOK_LPAREN)
			throw curr_token;
		gl(); // (
		
		if (c_type == TOK_VAR)
		{
			throw curr_token; 
		/*	gl(); // var

			if (c_type != TOK_ID)
				throw curr_token;

			gl();

			if (c_type != TOK_IN)
				throw curr_token;

			gl(); // in

			expression();

			if (c_type != TOK_RPAREN)
				throw curr_token;
			gl(); // )

			operatorr(); */
		}
		else
		{
			if (c_type != TOK_FIN)
			{
				expression();
				prog.put_token( Token( TOK_FIN ) );
			}

			if (c_type != TOK_FIN)
				throw curr_token;
			gl(); // ;

			int ind1 = prog.get_free();

			if (c_type != TOK_FIN)
			{
				expression();
			}
			else prog.put_token( Token( TOK_TRUE ) );

			if (c_type != TOK_FIN)
				throw curr_token;
			gl(); // ;

			int ind2 = prog.put_token( Token( POLIZ_ADDRESS ) );
			int ind3 = prog.put_token( Token( POLIZ_ADDRESS ) );
			prog.put_token( Token( TOK_IF ) );

			int ind4 = prog.get_free();

			if (c_type != TOK_RPAREN)
			{
				expression();
			}

			prog.put_token( Token( POLIZ_ADDRESS, ind1 ) );
			prog.put_token( Token( POLIZ_JUMP ) );

			if (c_type != TOK_RPAREN)
				throw curr_token;
			gl(); // )

			int ind5 = prog.get_free();

			operatorr( ind2, ind1, in_fun );

			prog.put_token( Token( POLIZ_ADDRESS, ind4 ) );
			prog.put_token( Token( POLIZ_JUMP ) );

			int ind6 = prog.get_free();

			prog.put_token( Token( POLIZ_ADDRESS, ind5 ), ind2 );
			prog.put_token( Token( POLIZ_ADDRESS, ind6 ), ind3 );
		}
	}	
	else if( c_type == TOK_DO )
	{
    	gl(); // do

		// поскольку нам заранее неизвестно, где находится
		// условие перехода, мы делаем такой хак:
		//     jump L3
		// L1: jump break_ind
		// L2: jump cont_ind
		// L3:
		// и передаем в operatorr() L1 и L2
		int L0 = prog.put_token( Token( POLIZ_ADDRESS ) );
		prog.put_token( Token( POLIZ_JUMP ) );
		int L1 = prog.put_token( Token( POLIZ_ADDRESS ) );
		prog.put_token( Token( POLIZ_JUMP ) );
		int L2 = prog.put_token( Token( POLIZ_ADDRESS ) );
		prog.put_token( Token( POLIZ_JUMP ) );
		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), L0 );

		int ind0 = prog.get_free();

    	operatorr( L1, L2, in_fun );
    	
    	if (c_type != TOK_WHILE )
			throw curr_token;

  		gl(); // while
  		
		if (c_type != TOK_LPAREN)
		throw curr_token;

		gl(); // (

		int ind_e = prog.get_free();

		expression();	
	    
		if (c_type != TOK_RPAREN)
			throw curr_token;

		gl(); // )

		if (c_type != TOK_FIN)
			throw curr_token;

		gl(); // ;

		int ind1 = prog.put_token( Token( POLIZ_ADDRESS, ind0 ) );
		int ind2 = prog.put_token( Token( POLIZ_ADDRESS ) );
		prog.put_token( Token( TOK_IF ) );

		prog.put_token( Token( POLIZ_ADDRESS, prog.get_free() ), ind2 );

		prog.put_token( Token( POLIZ_ADDRESS, ind1 ), L1 );
		prog.put_token( Token( POLIZ_ADDRESS, ind_e ), L2 );
	}
}
///////////////////////////////
void Parser::transition_operator ( int break_ind, int cont_ind, bool in_fun )
{
	if( c_type == TOK_BREAK )
	{
		if (break_ind==-1) // illegal break
			throw curr_token;

		gl(); // break
		if ( c_type != TOK_FIN )
			throw curr_token;

		gl(); // ;

		// поскольку заранее неизвестно, куда прыгать в случае break,
		// break_ind на самом деле указывает на условие цикла
		// поэтому просто положим FALSE, а потом прыгнем к этому условию
		prog.put_token( Token( TOK_FALSE ) );
		prog.put_token( Token( POLIZ_ADDRESS, break_ind ) );
		prog.put_token( Token( POLIZ_JUMP ) );
	}
	else if ( c_type == TOK_CONTINUE )
	{
		if (cont_ind==-1) // illegal continue
			throw curr_token;

		gl(); // continue
		if ( c_type != TOK_FIN )
			throw curr_token;

		gl(); // ;

		prog.put_token( Token( POLIZ_ADDRESS, cont_ind ) );
		prog.put_token( Token( POLIZ_JUMP ) );
	}
	else if ( c_type == TOK_RETURN )
	{
		if (!in_fun) // illegal return
			throw curr_token;
		gl(); // return

		if ( c_type != TOK_FIN )
		{
			expression();
		}
		else prog.put_token( Token( TOK_UNDEFINED ) );

		if (c_type != TOK_FIN)
			throw curr_token;

		gl(); // ;

		prog.put_token( Token( TOK_RETURN ) );
	}
	else 
		throw curr_token;
}
////////////////////////////////
void Parser::expression_operator ()
{
	expression();
	
	if( c_type != TOK_FIN)
		throw curr_token;
	gl(); // ;
	prog.put_token( Token( TOK_FIN ) );
}
///////////////////////////////
void Parser::expression () 
{
	expression_description();

  	if ( c_type == TOK_EQ || c_type == TOK_LSS || c_type == TOK_GTR || c_type == TOK_LEQ 
  		|| c_type == TOK_GEQ || c_type == TOK_NEQ  || c_type == TOK_ASSIGN )
  	{
		type_of_token op = c_type;
    	gl(); 
    	expression_description(); 
		prog.put_token( Token( op ) );
  	}
}

void Parser::expression_description ()
{
	T();

  	while ( c_type == TOK_PLUS || c_type == TOK_MINUS || c_type == TOK_OR)
  	{
		type_of_token op = c_type;
    	gl();
    	T();
		prog.put_token( Token( op ) );
  	}
  	
}

void Parser::T ()
{
	F();
  	while ( c_type == TOK_TIMES || c_type == TOK_SLASH || c_type == TOK_AND)
  	{
		type_of_token op = c_type;
    	gl();
		F();
		
		prog.put_token( Token( op ) );
  	}
}
 
void Parser::F () 
{ 
	if ( c_type == TOK_ID ) 
  	{
		prog.put_token( Token( TOK_ID, c_val ) );
    	gl();

		if (c_type == TOK_LPAREN) // it's a function call!
		{
			gl(); // (
			Token func_token = prog[prog.get_free()-1];
			prog.put_token( Token( POLIZ_END_OF_ARGS ), prog.get_free()-1 );

			if (c_type == TOK_RPAREN)
			{
				gl(); // )
			}
			else
			{
				expression();

				while(1)
				{
					if (c_type == TOK_RPAREN)
					{
						gl(); // )
						break;
					}
					else if (c_type == TOK_COMMA)
					{
						gl(); // ,
					}
					expression();
				}
			}
			prog.put_token( func_token );
			prog.put_token( Token( POLIZ_CALL_FUNC ) );
		}
  	}
  	else if ( c_type == TOK_NUM ) 
  	{
		prog.put_token( Token( TOK_NUM, c_val ) );
    	gl();
  	}
  	else if ( c_type == TOK_TRUE ) 
  	{
		prog.put_token( Token( TOK_TRUE, c_val ) );
    	gl();
  	}
  	else if ( c_type == TOK_FALSE)
  	{
		prog.put_token( Token( TOK_FALSE, c_val ) );
    	gl();
  	}
  	else if (c_type == TOK_NOT) 
  	{
    	gl(); 
    	F();
		prog.put_token( Token( TOK_NOT, c_val ) );
  	}
  	
  	else if ( c_type == TOK_LPAREN ) 
  	{
    	gl(); // (
    	expression();
    	
    	if ( c_type == TOK_RPAREN)
      		gl(); // )
    	else 
      		throw curr_token;
  	}
  	else if ( c_type == TOK_STRINGVAL )
  	{
		prog.put_token( Token( TOK_STRING, c_val ) );
  		gl();
  	}
  	else 
    	throw curr_token;
}

struct StackElem
{
	Token token;
	Number num;
	StackElem( Token tok, Number nm )
	{
		token = tok;
		num = nm;
	}
};

class Executor
{
private:
	Token curr_token;
	stack< StackElem > args;
	//char **str;
	//int istr;

	int pos;

	int * func_addr;

	int * call_stack;
	int depth;

	int * match_vars;
	int * prev_ids;
	int * var_ids;
	int * id_range;
public:
	Executor()
	{
		func_addr = new int[100];
		for (int i=0; i<100; i++)
			func_addr[i] = -1;

		call_stack = new int[100];
		depth = 1;

		match_vars = new int[100];
		for (int i=0; i<100; i++)
			match_vars[i] = i;
		prev_ids = new int[100];
		var_ids = new int[100];
		id_range = new int[100];
		for (int i=0; i<100; i++)
			id_range[i] = 0;

		/*str=new char *[100];
		for(int i=0; i<100; i++)
			str[i] = new char[STR_LENGTH];
		istr=0;*/
	}
	StackElem pop()
	{
		if (args.size()==0)
			throw "stack is empty";
		StackElem re = args.top();
		args.pop();
		return re;
	}
	StackElem pop( type_of_token tp )
	{
		if (args.size()==0)
			throw "stack is empty";
		StackElem re = args.top();
		args.pop();
		if (re.token.get_type() != tp)
			throw "wrong type of token";
		return re;
	}
	void execute(Poliz & prog);
	//void op_write(int);

	void print()
	{
		stack< StackElem > tmp;
		while (args.size()>0)
		{
			tmp.push( args.top() );
			args.pop();
		}
		char * types [] = { 
							(char*)"undefined", 
							(char*)"nan", 
							(char*)"bool", 
							(char*)"int", 
							(char*)"double", 
							(char*)"string" 
						  };
		cout << "pos=" << pos << " ";
		while (tmp.size()>0)
		{
			StackElem e = tmp.top();
			cout << "[" << e.token.get_type() << " " << e.token.get_value() << " " <<
				types[e.num.get_type()] << " " << e.num.to_string().get_str() << "] ";
			args.push( tmp.top() );
			tmp.pop();
		}
		cout << "\n";
	}
};

void Executor::execute(Poliz & prog)
{
	pos = 0;

	int sz = prog.get_free();

	while ( pos < sz )
	{
		Token curr_token = prog[pos];
		bool flag = true; // будем ли в конце делать pos++
		switch( curr_token.get_type() )
		{
			// сюда скопированы все типы токенов, чтобы ничего не пропустить
			// те, которые для экзегуции не используются - закомментированы

			//case TOK_NULL:
			//	break;

			case TOK_FUNCTION:
			{
				int func_id = args.top().token.get_value();
				if (func_addr[func_id] != -1)
					throw "function redifinition";
				func_addr[func_id] = pos+3;
				pop();
			}
				break;

			case TOK_VAR:
			{
				int var_id = args.top().token.get_value();
				if (TID[var_id].get_declare())
					throw "variable redifinition";
				TID[var_id].put_declare();
			}
				break;

			//case TOK_BREAK:
			//	break;

			//case TOK_CONTINUE:
			//	break;

			case TOK_IF:
			{
				int addr2 = pop( POLIZ_ADDRESS ).token.get_value();
				int addr1 = pop( POLIZ_ADDRESS ).token.get_value();
				bool cond = pop( TOK_NUM ).num.to_bool();
				if (cond) pos = addr1;
				else pos = addr2;
				flag = false;
			}
				break;

			//case TOK_THEN:
			//	break;
				
			//case TOK_ELSE:
			//	break;

			//case TOK_DO:
			//	break;
				
			//case TOK_WHILE:
			//	break;
				
			//case TOK_FOR:
			//	break;
				
			//case TOK_READ:
			//	break;
				
			case TOK_WRITE:
			{
				Number num = pop( TOK_NUM ).num;
				cout << num.to_string().get_str() << "\n";
			}
				break;
				
			case TOK_NOT:
			{
				Number num = pop( TOK_NUM ).num;
				num.set_bool( !num.to_bool() );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_AND:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				Number num;
				num.set_bool( num1.to_bool() && num2.to_bool() );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_OR:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				Number num;
				num.set_bool( num1.to_bool() || num2.to_bool() );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_TRUE:
			{
				Number num;
				num.set_bool( true );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_FALSE:
			{
				Number num;
				num.set_bool( false );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			//case TOK_INT:
			//	break;
				
			//case TOK_BOOL:
			//	break;

			case TOK_STRING:
			{
				int val = curr_token.get_value();
				Number num;
				num.set_string( TS[val] );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_ID:
			{
				int var_id = curr_token.get_value();
				var_id = match_vars[var_id];
				//if (!TID[var_id].get_declare())
				//	throw "undefined variable";
				args.push( StackElem( Token( TOK_NUM, var_id ), TID[var_id].get_value() ) );
			}
				break;
				
			case TOK_NUM:
			{
				int val = curr_token.get_value();
				Number num;
				num.set_int( val );
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;
				
			case TOK_FIN:
				pop( TOK_NUM );
				break;
				
			//case TOK_LFIG:
			//	break;
				
			//case TOK_RFIG:
			//	break;

			//case TOK_COMMA:
			//	break;

			//case TOK_COLON:
			//	break;

			case TOK_ASSIGN:
			{
				Number num = pop( TOK_NUM ).num;
				int var_id = args.top().token.get_value();
				if (var_id == 0)
					throw "illegal assign";
				args.top().num = num;
				TID[var_id].put_value( num );
			}
				break;

			//case TOK_LPAREN:
			//	break;

			//case TOK_RPAREN:
			//	break;

			case TOK_EQ:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 == num2 ) );
			}
				break;

			case TOK_LSS:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 < num2 ) );
			}
				break;

			case TOK_GTR:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 > num2 ) );
			}
				break;

			case TOK_PLUS:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 + num2 ) );
			}
				break;

			case TOK_MINUS:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 - num2 ) );
			}
				break;

			case TOK_TIMES:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 * num2 ) );
			}
				break;

			case TOK_SLASH:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 / num2 ) );
			}
				break;

			case TOK_LEQ:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 <= num2 ) );
			}
				break;

			case TOK_NEQ:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 != num2 ) );
			}
				break;

			case TOK_GEQ:
			{
				Number num2 = pop( TOK_NUM ).num;
				Number num1 = pop( TOK_NUM ).num;
				args.push( StackElem( Token( TOK_NUM ), num1 >= num2 ) );
			}
				break;

			//case TOK_SEMICOLON:
			//	break;

			//case TOK_STRINGVAL:
			//	break;

			//case TOK_TOCH:
			//	break;

			case TOK_RETURN:
			{
				depth--;
				pos = call_stack[depth];

				while (id_range[depth] > id_range[depth-1])
				{
					id_range[depth]--;
					match_vars[var_ids[id_range[depth]]] = prev_ids[id_range[depth]];
					TID.remove_local();
				}
			}
				break;

			case TOK_UNDEFINED:
			{
				Number num;
				num.set_undefined();
				args.push( StackElem( Token( TOK_NUM ), num ) );
			}
				break;

			//case TOK_REAL:
			//	break;

			case POLIZ_ADDRESS:
				args.push( StackElem( curr_token, Number() ) );
				break;

			case POLIZ_JUMP:
			{
				pos = pop( POLIZ_ADDRESS ).token.get_value();
				flag = false;
			}
				break;

			case POLIZ_END_OF_ARGS:
				args.push( StackElem( Token( POLIZ_END_OF_ARGS ), Number() ) );
				break;

			case POLIZ_CALL_FUNC:
			{
				if (depth==100)
					throw "stack overflow";
				int func_id = args.top().token.get_value();
				call_stack[depth] = pos;
				pos = func_addr[func_id];
				pop();
				id_range[depth] = id_range[depth-1];
				while(1)
				{
					if (prog[pos].get_type() == POLIZ_END_OF_ARGS)
					{
						if (args.top().token.get_type() != POLIZ_END_OF_ARGS)
							throw "wrong number of arguments";
						args.pop();
						break;
					}

					if (args.top().token.get_type() == POLIZ_END_OF_ARGS)
						throw "wrong number of arguments";

					int var_id = prog[pos].get_value();
					int new_id = TID.put_local();
					prev_ids[id_range[depth]] = match_vars[var_id];
					var_ids[id_range[depth]] = var_id;
					match_vars[var_id] = new_id;
					id_range[depth]++;

					TID[new_id].put_value( args.top().num );
					pop();
					pos++;
				}
				depth++;
			}
				break;

			default:
				throw "POLIZ: unexpected elem";
		}
		if (flag) pos++;
		//print();
	}

	//cout << "execution has finished\n";
}

 
//////////////////////////////////////////
void Find_the_scrypt( const char* program)
{	
	const char *fname = program;
    
    size_t count=0; 
    char S; 
    
    ifstream f(fname);
    while (!f.eof()) 
    {
        f.get(S);
        count++;
    }
    f.close();  
    
    char *S2 = new char [count];
    for (size_t i=0; i < count; i++ ) 
    	S2[i] = '\0';
    ifstream f2(fname);
    int i = 0;
 
    while (!f2.eof()) 
	{	
        f2.get(S2[i]);
        i++;
    }    
	f2.close();
 
    
    string const page = S2;

   	string const tag = "script";
   	string text;
   
   	string const opening = "<" + tag + ">";
   	string const closing = "</" + tag + ">";
   	string::size_type begin = page.find(opening);
   	if (begin != string::npos) 
   	{
    	begin += opening.length();
    	string::size_type end = page.find(closing, begin);
    	if (end != string::npos) 
    	{
        	text = page.substr(begin, end - begin);
      	}
   	}
    
    ofstream out("helper.txt");
    
    if (!out) 
    {
		cout << "Cannot open file.\n";
		exit(3);
	}
    
    out << text; 
    out.close ();  
    
    delete []S2;

}
////////////////////////////////////////

int main()
{

	Find_the_scrypt("./cgi-bin/indthree.mjs");
	try
	{
        Parser pars("helper.txt");
		pars.parse();
		//cout  << endl;
		//TID.print();
		//cout  << endl;
		//pars.prog.print();
		//cout  << endl;
		Executor exec;
		exec.execute( pars.prog );
		///cout  << endl;
		//TID.print();
		//cout  << endl;
  	}
  	catch (char c)
  	{
    	cout << "unexpected symbol " << c << endl;
    	return 1;
  	}
  	catch (Token t)
  	{
    	cout << "unexpected lexeme " << t << endl;
    	return 1;
  	}
  	catch ( const char *source )
  	{
    	cout << source << endl;
    	return 1;
  	}
	catch ( Error e )
	{
		e.print();
		return 1;
	}
	catch ( ... )
	{
		cout << "unknown error" << endl;
		return 1;
	}
	return 0;
}
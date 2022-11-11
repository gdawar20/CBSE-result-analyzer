//***************************************
//             HEADER FILES
//***************************************
#include<iostream>
#include<conio.h>
#include<fstream>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
using namespace std;
//*************************************************************
//                     FUNCTION PROTOTYPES
//*************************************************************
int char_2_int(char);

int string_2_int(char[]);

void code_2_sub(int,char[]);
//***************************************************
//                     STRUCTURES
//***************************************************
struct subject_code_grade
{
int code;
int marks;
};
struct name_node

 {
 char name[33];
 name_node*next;
 };
 struct data_node
 {
 int code,appeared,plus70,plus75,plus80,plus90,highest;
 name_node*topn;
 data_node*next;
 };
 //**************************************************************
 // CLASS TO READ THE TXT FILE AND WRITE THE DATA IN BINARY FILE
 //**************************************************************
 class read_write
 {
 private:
 char rollno[8],name[33];
 subject_code_grade scg[6];
 char status[5];
 public:
 int txt_2_bin(char[]);
 void add_rollno(char rno[]){strcpy(rollno,rno);}
 void add_name(char nam[]){strcpy(name,nam);}
 void cat_name(char nam[]){strcat(name," "); strcat(name,nam);}
 void add_status(char sta[]){strcpy(status,sta);}
 void add_marks(int mark,int i){scg[i].marks=mark;}
 void add_code(int cod,int i){scg[i].code=cod;}
 char*ret_name(){return name;}
 char*ret_status(){return status;}
 int ret_marks(int i){return scg[i].marks;}
 int ret_code(int i){return scg[i].code;}
 };
 //*********************************************************
 //                MEMBER FUNCTION OF READ_WRITE
 //*********************************************************
 int read_write::txt_2_bin(char txt[])
 {
 read_write R;
 fstream f;
 f.open("REPORT.DAT",ios::out|ios::binary);
 char word[20];
 ifstream i(txt);
 if(!i)
 return 0;
 i>>word;
 while(!i.eof())
 {
     if(strlen(word)==7 && isdigit(word[0]) && isdigit(word[6]))
     {
     R.add_rollno(word);
     int flag=0;
     while(flag==0)
	  {
	  i>>word;
	  if(isalpha(word[0]))
	  R.cat_name(word);
			else
	  flag=1;
			 }
for(int a=0;a<6;a++)
      {
	    if(a==5&&isalpha(word[0]))
   {
   R.add_code(-1,a);
   R.add_marks(-1,a);
   break;
   }
   R.add_code(string_2_int(word),a);
   i>>word;
   if(strcmpi(word,"AB")==0)
   {
   R.add_marks(-1,a);
   i>>word;
   continue;
   }
   R.add_marks(string_2_int(word),a);
   i>>word;
   if(isdigit(word[0]))
   {
   R.add_code(ret_marks(a),a+1);
   a=2;
   while(strcmpi(word,"ABST"))
   {
	 R.add_code(string_2_int(word),a);
	 i>>word;
	 a++;
   }
   goto abst;
   }
   i>>word;
			   }
		    for(a=0;a<3;a++)
		    i>>word;
		    abst:
	  R.add_status(word);
  f.write((char*)&R,sizeof(R));
  }
  else
		   i>>word;
  }
  f.close();
  i.close();
  return 1;
  }
  //***********************************************
  //          ClASS FOR TABULAR ANALYSIS
  //***********************************************
  class data_stack:protected read_write
  {
  data_node*top;
  void push_name();
  public:
  ~data_stack();
  data_stack()
  {
       top=NULL;
  }
  void write_table();
  void datain();
  };
  //***************************************************
  //           DESTRUCTOR OF DATA_STACK
  //***************************************************
  data_stack::~data_stack()
	{
	      name_node*tempn=top->topn;
	      data_node*temp=top;
	      while(top!=NULL)
	       {
   temp=top->next;
   while(top->topn!=NULL)
    {
		    tempn=top->topn->next;
    delete top->topn;
    top->topn=tempn;
    }
    delete temp;
    top=temp;
    }
	  }
//******************************************************************
//            MEMBER FUNCTION TO WRITE TABULAR ANALYSIS
//******************************************************************
void data_stack::write_table()
{
ofstream o("ANALISE4.txt",ios::out);
data_node*temp=top;
char subject[70];
o<<"CODE SUBJECT\t\t\t APPEARED||>=70||>=75||>=80||>=90|| HIGHEST"<<endl;
o<<"----------------------------------------------------------------------------\n";
while(temp!=NULL)
{
code_2_sub(temp->code,subject);
o<<temp->code<<" "<<subject<<"\n\t\t\t\t"<<temp->appeared;
o<<"\t|| "<<temp->plus70<<" || "<<temp->plus75<<" || ";
o<<temp->plus80<<" || "<<temp->plus90<<" || "<<temp->highest<<endl;
o<<"NAME OF STUDENTS- ";
while(temp->topn!=NULL)
       {
o<<"/"<<temp->topn->name;
temp->topn=temp->topn->next;
}
o<<"/\n";
o<<"----------------------------------------------------------------------------\n";
temp=temp->next;
}
o.close();
}
//*************************************************************
//        MEMBER FUNCTION TO PUSH NAMES IN LINKED STACK
//*************************************************************
void data_stack::push_name()
{
  read_write R;
data_node*temp=top;
while(temp!=NULL)
{
fstream f("REPORT.DAT",ios::binary|ios::in);
while(f.read((char*)&R,sizeof(R)))
{
for(int i=0;i=6;i++)
{
if(R.ret_code(i)==temp->highest)
{
if(R.ret_marks(i)==temp->highest)
{
name_node*tempn=new name_node;
strcpy(tempn->name,R.ret_name());
tempn->next=temp->topn;
temp->topn=tempn;
}
	     }
	     }
}
f.close();
temp=temp->next;
   }
}
//***********************************************************
//        MEMBER FUNCTION TO ANALYSIS THE BINARY FILE
//           AND CALL PUSH_NAME() AND WRITE_TABLE()
//***********************************************************
void data_stack::datain()
{
read_write R;
int flag=0;
fstream f("REPORT.DAT",ios::binary|ios::in);
while(f.read((char*)&R,sizeof(R)))
{
if(strcmpi(R.ret_status(),"ABST")==0)
continue;
if(flag==0)
{
     data_node*temp=new data_node;
temp->highest=-1;
temp->appeared=temp->plus70=temp->plus75=0;
temp->plus80=temp->plus90=0;
temp->code=R.ret_code(0);
temp->next=top;
temp->topn=NULL;
top=temp;
flag=1;
}
for(int s=0;s<6;s++)
{
if((R.ret_code(s)==-1)||(R.ret_marks(s)==-1))
break;
int flag1=0;
data_node*check=top;
while(check!=NULL)
{
if(R.ret_code(s)==check->code&&R.ret_marks(s)!=-1)
{

check->appeared++;
if(R.ret_marks(s)>check->highest)
check->highest=R.ret_marks(s);
if(R.ret_marks(s)>=70)
check->plus70++;
if(R.ret_marks(s)>=75)
check->plus75++;
if(R.ret_marks(s)>=80)
check->plus80++;
if(R.ret_marks(s)>=90)
check->plus90++;
flag1++;
}
check=check->next;
}
if(flag1==0)
{
data_node*temp=new data_node;
temp->highest=-1;
temp->appeared=temp->plus70=temp->plus75=0;
temp->plus80=temp->plus90=0;
temp->code=R.ret_code(s);
temp->next=top;
temp->topn=NULL;
top=temp;
temp->appeared++;
if(R.ret_marks(s)>temp->highest)
temp->highest=R.ret_marks(s);
if(R.ret_marks(s)>=70)
temp->plus70++;
if(R.ret_marks(s)>=75)
temp->plus75++;
if(R.ret_marks(s)>=80)
temp->plus80++;
if(R.ret_marks(s)>=90)
temp->plus90++;
	    }
}
}
push_name();
}
//**********************************************************
//                CLASS FOR GENERAL ANALYSIS
//**********************************************************
class analise : public data_stack
{
int appeared,distinct,above90,above4,above5,abovepcm,above90per5;
float top_per[3],top_pcm,top_pcb,top_com,top_hum;
char top_name[3][34];
name_node*topn_pcm,*topn_pcb,*topn_com,*topn_hum;
protected:
void sch_top();
float calcpercent(read_write);
void gen_details();
void push_toppers();
public:
void str_top();
~analise();
analise()
{
topn_pcm=topn_pcb=topn_com=topn_hum=NULL;
top_hum=top_com=top_pcm=top_pcb=0;
}
};
//*****************************************************
//        MEMBER FUNCTION FOR MARGINAL ANALYSIS
//*****************************************************
void analise :: gen_details()
{
appeared=distinct=above90=above4=above5=abovepcm=above90per5=0;
read_write R;
ofstream o("ANALISE1.txt",ios::out);
o<<"Name of students scoring above 90% in five subjects:-\n";
fstream f("REPORT.DAT",ios::binary|ios::in);
while(f.read((char*)&R,sizeof(R)))
{
if(strcmpi(R.ret_status(),"ABST")!=0)
{
appeared++;
int pcm=0,checkpcm=0;
for(int i=0;i=6;i++)
{
if(R.ret_marks(i)==-1)
break;
if(R.ret_marks(i)>=75)
distinct++;
if(R.ret_marks(i)>=90)
above90++;
if(R.ret_code(i)==41||R.ret_code(i)==42||R.ret_code(i)==43)
   {
pcm=pcm+R.ret_marks(i);
checkpcm++;
	       }
	    }
if(calcpercent(R)>=90)
  {
  above90per5++;
  o<<R.ret_name()<<"\n";
 }
if(checkpcm==3)
{
int val=pcm/3;
if(val>=90)
abovepcm++;
}
    }
}
f.close();
o<<"\nTotal students appeared: "<<appeared<<endl;
o<<"Total number of distinctions in various subjects: "<<distinct<<endl;
o<<"Number of scores greater than or equal to 90%: "<<above90<<endl;
o<<"Student scoring aggregate above 90% in PCM: "<<abovepcm<<endl;
o<<"Student scoring aggregate above 90% in Five subjects: "<<above90per5;
o.close();
}
//**********************************************
//            DESTRUCTOR OF ANALISE
//**********************************************
analise::~analise()
{
name_node*temp=topn_pcm;
while(topn_pcm!=NULL)
{
temp=topn_pcm->next;
delete topn_pcm;
topn_pcm=temp;
      }
temp=topn_pcb;
while(topn_pcb!=NULL)
{
temp=topn_pcm->next;
delete topn_pcm;
topn_pcm=temp;
}
temp=topn_com;
while(topn_com!=NULL)
{
temp=topn_com->next;
delete topn_com;
topn_com=temp;
       }
temp=topn_hum;
while(topn_hum!=NULL)
{
temp=topn_hum->next;
delete topn_hum;
topn_hum=temp;
      }
}
//************************************************************
//        MEMBER FUNCTION TO PUSH STREAMWISE TOPPERS
//************************************************************
void analise::push_toppers()
{
read_write R;
fstream f("REPORT.DAT",ios::binary|ios::in);
while(f.read((char*)&R,sizeof(R)))
{
int f_pcm=0,f_pcb=0,f_hum=0,f_com=0;
for(int i=0;i<6;i++)
{
if(R.ret_code(i)==42||R.ret_code(i)==41)
f_pcm++;
if(R.ret_code(i)==42||R.ret_code(i)==44)
f_pcb++;
if(R.ret_code(i)==55)
f_com++;
if(R.ret_code(i)==27)
f_hum++;
}
if(f_pcm==2&&calcpercent(R)==top_pcm)
{
name_node*temp=new name_node;
strcpy(temp->name,R.ret_name());
temp->next=topn_pcm;
topn_pcm=temp;
}
if(f_pcb==2&&calcpercent(R)==top_pcb)
{
name_node*temp1=new name_node;
strcpy(temp1->name,R.ret_name());
temp1->next=topn_pcb;
topn_pcb=temp1;
}
  if(f_com==1&&calcpercent(R)==top_com)
      {
  name_node*temp2=new name_node;
  strcpy(temp2->name,R.ret_name());
  temp2->next=topn_com;
  topn_com=temp2;
  }
  if(f_hum==1&&calcpercent(R)==top_hum)
  {
  name_node*temp3=new name_node;
  strcpy(temp3->name,R.ret_name());
  temp3->next=topn_hum;
  topn_hum=temp3;
  }
  }
  f.close();



  }
//************************************************************
//        MEMBER FUNCTION TO FIND STREAMWISE TOPPERS
//************************************************************
void analise::str_top()
{
read_write R;
fstream f("REPORT.DAT",ios::in|ios::binary);
while(f.read((char*)&R,sizeof(R)))
{
int f_pcm=0,f_pcb=0,f_hum=0,f_com=0;
for(int i=0;i<6;i++)
{
if(R.ret_code(i)==42||R.ret_code(i)==41)
f_pcm++;
if(R.ret_code(i)==42||R.ret_code(i)==44)
f_pcb++;
if(R.ret_code(i)==55)
f_com++;
if(R.ret_code(i)==27)
f_hum++;
}
    if(f_pcm==2&&calcpercent(R)>top_pcm)
	 top_pcm=calcpercent(R);
    if(f_pcb==2&&calcpercent(R)>top_pcb)
	 top_pcb=calcpercent(R);
    if(f_com==1&&calcpercent(R)>top_com)
	 top_com=calcpercent(R);
    if(f_hum==1&&calcpercent(R)>top_hum)
	 top_hum=calcpercent(R);

     }
f.close();
push_toppers();
ofstream o("ANALISE3.txt",ios::out);
o<<"\t\t STREAM-WISE TOPPERS\n";
o<<"\t\t-----------------------------------\n\n";
o<<"In PCM:\t/";
name_node*temp=topn_pcm;
while(temp!=NULL)
{
o<<temp->name<<"/";
temp=temp->next;
}
o<<"\t"<<top_pcm<<endl;
o<<"In PCB:\t/";
temp=topn_pcb;
while(temp!=NULL)
{
o<<temp->name<<"/";
temp=temp->next;
}
o<<"\t"<<top_pcb<<endl;
o<<"In commerce:\t/";
temp=topn_com;
while(temp!=NULL)
{
o<<temp->name<<"/";
temp=temp->next;
}
o<<"\t"<<top_com<<endl;
o<<"In Humanities:\t/";
temp=topn_hum;
while(temp!=NULL)
{
o<<temp->name<<"/";
temp=temp->next;
}
o<<"\t"<<top_hum<<endl;
o.close();
}
//********************************************************
//        MEMBER FUNCTION TO FIND SCHOOL TOPPERS
//********************************************************
void analise::sch_top()
{
read_write R;
for(int i=0;i<3;i++)
  {
top_per[i]=0;
fstream f("REPORT.DAT",ios::in|ios::binary);
while(f.read((char*)&R,sizeof(R)))
      {
      if(calcpercent(R)>top_per[i])
      {
		top_per[i]=calcpercent(R);
		strcpy(top_name[i],R.ret_name());
	    }



	    }
      f.close();
      fstream f1("REPORT.DAT",ios::in|ios::out|ios::binary);
long pos=f1.tellg();
while(f1.read((char*)&R,sizeof(R)))
{
if(strcmpi(top_name[i],R.ret_name())==0&&calcpercent(R)==top_per[i])
{
      break;
}
pos=f1.tellg();
}
f1.seekp(pos);
R.add_marks(-500,0);
f1.write((char*)&R,sizeof(R));
f1.close();
}
remove("REPORT.DAT");
ofstream o("ANALISE2.txt");
o<<"\t\tSCHOOL TOPPERS\n";
o<<"\t\t------------------\n\n";
for(i=0;i=3;i++)
o<<top_name[i]<<"\t"<<top_per[i]<<endl;
o.close();
}
//*********************************************************
//        MEMBER FUNCTION TO CALCULATE PERCENTAGE
//*********************************************************
float analise::calcpercent(read_write R)
{
int smallest=R.ret_marks(1);
float percent=0;
if(R.ret_code(5)==-1)
   for(int i=0;i<5;i++)
   percent+=R.ret_marks(i);
   else
   {
   for(int i=1;i<6;i++)
   {
   if(R.ret_marks(i)<smallest)
   smallest=R.ret_marks(i);
   percent+=R.ret_marks(i);
	       }
   percent-=smallest;
   percent+R.ret_marks(0);
   }
   percent/=5;
   return percent;
   }
   //*****************************************************
   //        FUNCTION TO ASSIGN RESPECTIVE SUBJECT
   //	       TO THE CORRESPONDING SUBJECT CODE
   //*****************************************************
   void code_2_sub(int cod,char subject[70])
   {
   char word[30];
   int code;
   ifstream i("subcode.txt");
   while(!i.eof())
   {
   i>>word;
   code=string_2_int(word);

   if(code==cod)
   {
   i>>word;
   strcpy(subject,word);
   i>>word;
   while(!isdigit(word[0])&&strlen(word)!=3)
   {
     strcat(subject,"  ");
     strcat(subject,word);
     i>>word;
   }
   }
    }
    i.close();
   }
   //********************************************************
   //        FUNCTION TO CONVERT CHARACTER TO INTEGER
   //********************************************************
   int char_2_int(char x)
   {
   return x-48;
   }
   //************************************************************
   //       FUNCTION TO CONVERT NUMERIC STRING TO INTEGER
   //************************************************************
   int string_2_int(char x[])
   {
     int a=0,s=1;
     for(int i=(strlen(x)-1);i>=0;i--)
     {
   if(isdigit(x[i]))
   {
   a+=(char_2_int(x[i])*s);
   s*=10;
   }
      }
      return a;
   }



   //**********************************************
   //                CLASS FOR MENU
   //**********************************************
   class menu:public analise
   {
   void m1();
   void m2();
   void pre_m1();
   void show_txt(char[]);
   public:
   void mainmenu();
   menu()
   {
   cout<<"\n\n\n\n\n\n";
   cout<<"                      |||||||||||||||||||||||||||||"<<endl;
   cout<<"                      |||||||||WELCOME|||||||||||||"<<endl;
   cout<<"                      |||||||||||||||||||||||||||||"<<endl;
   cout<"\n\n";
   cout<<"                    **************************************\n";
   cout<<"                    ****CBSE CLASS XII RESULT ANALYSER****"<<endl;
   cout<<"                    **************************************\n";
   cout<<"\n\n";
   cout<<"                         MADE BY: GOVIND DAWAR ";
	 getch();
	 clrscr();
   }
   };
   //**********************************************************
   //        MEMBER FUNCTION TO DISPLAY THE CONTENTS OF
   //                         GIVEN TXT FILE
   //**********************************************************
   void menu::show_txt(char txt[])
   {
     ifstream i(txt);
     char ch[5000];
     int k=0;
     while(!i.eof())
     {
     if(k==20)                      //no. of lines exceeding screen
     {
	  k=0;
	  cout<<"\n\n\nPress enter to go to next page::";
	  getch();
	  clrscr();
     }
     i.getline(ch,5000);
     cout<<ch<<"\n";
     k++;
  }
  i.close();
  getch();
}

//*********************************************
//        MEMBER FUNCTION FOR MAIN MENU
//*********************************************
void menu::mainmenu()
{
clrscr();
int ch;
cout<<"\t\t\tMAIN MENU\n";
cout<<"\t\t\t*********\n\n";
cout<<"1.Analyser menu\n\n";
cout<<"2.User Manual\n\n";
cout<<"3.Exit\n\n\n";
cout<<"Please enter your choice:";
cin>>ch;
clrscr();
switch(ch)
	    {
case 1: pre_m1(); break;
case 2: m2();             break;
case 3: exit(0);
}
}
//************************************************
//        MEMBER FUNCTION FOR USER'S GUIDE
//************************************************
void menu::m2()
{
cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t************************";
cout<<"\n\t\t\t\t****USER'S GUIDE****";
cout<<"\n\t\t\t\t********************";
getch();
clrscr();
cout<<"\n\n\nThis software helps to analyse the class XII result sent by the CBSE in txt format";

cout<<"\nIt provides the list of:-\n 1.No. of students appeared\n 2.No. of distinctions\n 3.School Toppers\n 4.Streamwise Toppers\n 5.Subjectwise Tabular analysis of:-";
cout<<"\n    5.1.Total students appeared\n  5.2.Distinctions\n  5.3.Particulars Margins Achieved\n  5.4.Highest Marks\n  5.5.Name of Student/s scoring the Highest";
getch();
clrscr();
cout<<"\n\nTo use this software some guidelines should be followed:-";
cout<<"\n\n1.The txt file to be analised should be put in 'BIN' folder of 'Turbo C++'.";
cout<<"\n\n2. The user should have a file with '.txt' extension containing subject codes of CBSE Class XII with their respective subjects for matching of subject codes in the result.";
cout<<"\n\n3.The user needs to remove gender column and school details from the txt file.";
cout<<"\n\n5.Run the software in 'TURBO C++4.5'.";
getch();
mainmenu();
}
//*************************************************
//        MEMBER FUNCTION FOR ANALYSER MENU
//*************************************************
void menu::m1()
{
clrscr();
int ch;
cout<<endl;
cout<<"\t\t\tANALYSER MENU\n";
cout<<"\t\t\t*************\n\n";
cout<<"1. General details\n\n";
cout<<"2. School Toppers\n\n";
cout<<"3. Stream-wise Toppers\n\n";
cout<<"4. Tabular analysis of each subject\n\n";
cout<<"5. Back to main menu\n\n";
cout<<"6. Exit\n\n";
cout<<"Please enter your choice:";
cin>>ch;
clrscr();
switch(ch)
{
case 1: show_txt("ANALISE1.txt");m1();break;
case 2: show_txt("ANALISE2.txt");m1();break;
case 3: show_txt("ANALISE3.txt");m1();break;
case 4: show_txt("ANALISE4.txt");m1();break;
case 5: mainmenu();break;
case 6: exit(0);
default:m1();
}
}
//************************************************************
//        MEMBER FUNCTION FOR ENTRY OF FILE'S NAME AND
//            CALL THE REQUIRED FUNCTION TO ANALISE IT.
//************************************************************
void menu::pre_m1()
{read_write R;
clrscr();
char txt[20];
cout<<"\nPlease enter the name of the txt file to be analysed:";
gets(txt);
int read=R.txt_2_bin(txt);
clrscr();
if(read)
 {
     analise A;
     A.str_top();
A.datain();
A.write_table();
gen_details();
sch_top();
cout<<"\nThe file is analysed successfully and it's analysis is stored in"<<endl;
cout<<"C:/TurboC4/TC/BIN as txt files with prefix as 'ANALISED'.";
getch();
m1();
   }
else
   {
   cout<<"\nTxt file not found";
   getch();
   mainmenu();
   }
}
//************************************
//        MAIN OF THE PROGRAM
//************************************
void main()
{
clrscr();
menu M;
M.mainmenu();
}


















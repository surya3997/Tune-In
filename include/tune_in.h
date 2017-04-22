
struct id3base
{
	int  id;					//program assigned id
	char dir[500];
	char title[31];
	char artist[31];
	char album[31];
	char year[5];
};
struct id3base id3;				// Struct variable for fetching every song



void initialise();
void header();
void about();
void player();
int getid3(char []);
int importer(char []);			//MP3 list to FILEBASE.TXT
void importer1();				//FILEBASE.TXT to STRUCT ID3
void search(int);
void destroyer();
void reset();


int cnt=0,strucnt=0,dircnt=0,cnt1=0,assign,flag=0;
char garbage[50];

#define MAX_USERS 6000000
#define MAX_SIZE 200000

typedef char *KEY;

typedef struct users * USERS;

USERS initUsers ();
int add_users (KEY i, char* log, char* t,char* follower_list,char* following_list , USERS table,int followers,int following);
char* getlogin (USERS table , KEY i);
int searchID (KEY i,USERS table);
void printUSERS(USERS t);
int checkbot (KEY i ,USERS table );
char* getFriends (KEY i,USERS table);
char* getFriendRepo (KEY i,USERS table);
void addRepoToUser (char* repo_id,int owner_id,USERS tabelauser);
char get_state(USERS tUser,int i);
KEY get_id (USERS tUser,int i);

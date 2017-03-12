extern int printf(const char* c, ...);
extern unsigned long long strlen(const char* s);
extern int isdigit(int);
extern int isspace(int);
extern int isalpha(int);
extern int isalnum(int);
extern void* memset( void*, int , unsigned long long );
extern char* strcpy( char*, const char*);
extern int strcmp(const char*, const char*);
extern void exit(int);

#define assert(  expr )                 \
        do{                             \
                if( ! expr ){           \
                        exit(1);        \
                }                       \
        }while(0)

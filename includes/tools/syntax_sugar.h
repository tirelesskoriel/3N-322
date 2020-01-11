#ifndef CLASS_TOOLS
#define CLASS_TOOLS

#include<string>

#define SAVE_DEL(P) do{if(P) delete P;}while(0)
#define WARNING_LOG(W,S) std::cout<< "WARNING:" << (W) << (S) << std::endl
#define LOG(M) std::cout << "log:" << (M) << std::endl
#define COMMON_ERROR_CODE -1
#define MAX_POS(O,N) do{if(O < N) O = N;}while(0)
#define MIN_POS(O,N) do{if(O > N) O = N;}while(0)

bool IS_ERROR(int code);
std::string REPLACE(const char* source,char target, char rep);

#endif
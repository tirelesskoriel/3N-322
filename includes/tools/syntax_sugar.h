#ifndef CLASS_TOOLS
#define CLASS_TOOLS

#define SAVE_DEL(P) do{if(P) delete P;}while(0)
#define WARNING_LOG(W,S) std::cout<< "WARNING:" << (W) << (S) << std::endl
#define LOG(M) std::cout << "log:" << (M) << std::endl
#define COMMON_ERROR_CODE -1

bool IS_ERROR(int code);

#endif
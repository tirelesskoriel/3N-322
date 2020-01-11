#include<tools/syntax_sugar.h>

bool IS_ERROR(int code)
{
    return code == COMMON_ERROR_CODE;
}

std::string REPLACE(const char* source, char target, char rep)
{
    char *result = nullptr;

    unsigned int count = strlen(source);
    for(unsigned int i = 0; i < count; i++)
    {
        if(source[i] == target)
        {
            if(result == nullptr)
            {
                result = (char*)malloc(strlen(source));
                strcpy(result, source);
            }

            result[i] = rep;
        }
            
    }

    if(result == nullptr)
    {
        return std::string(source);
    }

    std::string r = std::string(result);
    free(result);
    return r;
}
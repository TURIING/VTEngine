#include "utility/LogManager.h"

int main(int argc, char* argv[])
{
    Singleton<LogManager>::GetInstance()->Init();
}
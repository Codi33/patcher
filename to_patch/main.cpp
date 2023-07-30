#include <iostream>
#include <Windows.h>

#pragma section(".token", read)

struct init_data_t
{
	DWORD32 pid;
	char str[7];
};

__declspec(allocate(".token")) init_data_t data;

int main()
{
	printf("%d\n", data.pid);
	printf("%s\n", data.str);
	std::cin.get();
	return 0;
}

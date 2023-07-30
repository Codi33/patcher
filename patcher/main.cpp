#include <cstdio>
#include <Windows.h>
#include <fstream>

struct init_data_t
{
	DWORD32 pid;
	char str[7];
};

int main()
{
	const HANDLE h_in = CreateFileA("to_patch.exe", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	const DWORD fsize = GetFileSize(h_in, nullptr);
	auto* fdata = new BYTE[fsize]{ };
	ReadFile(h_in, fdata, fsize, nullptr, nullptr);
	CloseHandle(h_in);

	const auto dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(fdata);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(fdata + dos_headers->e_lfanew);

	const auto sections = IMAGE_FIRST_SECTION(nt_headers);

	init_data_t data{};
	data.pid = GetCurrentProcessId();
	memcpy(data.str, "123456", 7);

	for (WORD i = 0; i < nt_headers->FileHeader.NumberOfSections; i++)
	{
		if (!strcmp(".token", reinterpret_cast<char*>(sections[i].Name)))
		{
			*reinterpret_cast<init_data_t*>(fdata + sections[i].PointerToRawData) = data;
			break;
		}
	}

	const HANDLE n = CreateFileA("new.exe", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	WriteFile(n, fdata, fsize, nullptr, nullptr);
	CloseHandle(n);

	delete[] fdata;

	return 0;
}

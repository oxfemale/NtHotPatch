#pragma once
#define PAGE_SIZE 0x1000
class NtDirect
{
public:
	void* ptr = nullptr;
	unsigned int get_service_id(const char* service_name);
	NtDirect(unsigned int service_id, const char* service_name);
	~NtDirect();
};


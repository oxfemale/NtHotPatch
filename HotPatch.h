#pragma once

#define PAGE_SIZE 0x1000

#ifdef _WIN64
#define HOTPATCH_ADDR_OFFSET 2
static unsigned char hotpatch_stub[] = {
	0x48,0xB8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // mov rax, [Abs Jump Address]
	0xFF,0xE0,                                         // jmp rax
	0xC3,                                              // ret
	0xCC,0xCC,0xCC                                     // padding
};
#else
#define HOTPATCH_ADDR_OFFSET 1
static unsigned char hotpatch_stub[] = {
	0xB8, 0x00, 0x00, 0x00, 0x00, // mov eax, [Abs Jump Address]
	0xFF,0xE0,                    // jmp eax
	0x90                          // ret
};
#endif


class HotPatch
{
public:
	void* patched_address = nullptr;
	void* hotpatch_fptr = nullptr;
	unsigned char backup_bytes[sizeof(hotpatch_stub)];
	HotPatch(void* target);
	BOOL patch(void* dest_fptr);
	BOOL unpatch(void);
	~HotPatch();
};


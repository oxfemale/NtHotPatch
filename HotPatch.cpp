#include <Windows.h>
#include "HotPatch.h"



HotPatch::HotPatch(void* target){
	RtlZeroMemory(this->backup_bytes, sizeof(this->backup_bytes));
	this->patched_address = target;
	this->hotpatch_fptr = nullptr;

}

BOOL HotPatch::patch(void* dest_fptr){
	// Return if already patched.
	if (!this->patched_address || this->hotpatch_fptr) { return FALSE; }
	unsigned char stub_code[sizeof(hotpatch_stub)] = { 0 };
	memcpy(stub_code, hotpatch_stub, sizeof(hotpatch_stub));
	memcpy(stub_code + HOTPATCH_ADDR_OFFSET, &dest_fptr, sizeof(DWORD_PTR));

	// Copy the stub code
	DWORD dwOldProtect = 0;
	if (!VirtualProtect((LPVOID)this->patched_address, PAGE_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect)) { return FALSE; }
	memcpy(backup_bytes, (LPVOID)this->patched_address, sizeof(backup_bytes));
	(void)memcpy((void*)this->patched_address, stub_code, sizeof(stub_code));
	if(!VirtualProtect((LPVOID)this->patched_address, PAGE_SIZE, dwOldProtect, &dwOldProtect)) { return FALSE; }
	this->hotpatch_fptr = dest_fptr;
	return TRUE;
}

BOOL HotPatch::unpatch() {

	// If we haven't set up a target address or no patch is applied, fail out.
	if (!this->patched_address || !this->hotpatch_fptr) { return FALSE; }

	// Copy the stub code
	DWORD dwOldProtect = 0;
	if (!VirtualProtect((LPVOID)this->patched_address, PAGE_SIZE, PAGE_EXECUTE_READWRITE, &dwOldProtect)) { return FALSE; }
	memcpy((LPVOID)this->patched_address, this->backup_bytes, sizeof(this->backup_bytes));
	if (!VirtualProtect((LPVOID)this->patched_address, PAGE_SIZE, dwOldProtect, &dwOldProtect)) { return FALSE; }
	return TRUE;
}

HotPatch::~HotPatch(){
	// Might as well unpatch it if we hit the destructor.
	this->unpatch();
	RtlZeroMemory(this->backup_bytes, sizeof(this->backup_bytes));
	this->patched_address = nullptr;
	this->hotpatch_fptr = nullptr;
}

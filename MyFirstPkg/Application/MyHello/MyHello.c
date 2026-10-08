#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
UefiMain(
	IN EFI_HANDLE		ImageHandle,
	IN EFI_SYSTEM_TABLE	*SystemTable
		)
	{
	Print(L"==========================================\n");
	Print(L" Welcome to My First UEFI App!\n");
	Print(L"==========================================\n");

	Print(L"ImageHandle: %p\n", ImageHandle);
	Print(L"SystemTable: %p\n", SystemTable);
	Print(L"Firmware development journey starts here \n");

	return EFI_SUCCESS;
	}

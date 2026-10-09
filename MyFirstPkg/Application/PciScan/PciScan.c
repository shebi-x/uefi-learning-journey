#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/PciIo.h>

EFI_STATUS
EFIAPI
UefiMain(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE		*Systemtable
		)
	{
	EFI_STATUS		Status;
	EFI_HANDLE		*HandleBuffer = NULL;
	UINTN			HandleCount = 0;
	UINTN			Index;
	EFI_PCI_IO_PROTOCOL	*PciIo;
	UINT16			VendorId;
	UINT16			DeviceId;

	Print(L"===========================================\n");

	//1. find all support PciIO Protocol Handle
	Status = gBS->LocateHandleBuffer(
			ByProtocol,
			&gEfiPciIoProtocolGuid,
			NULL,
			&HandleCount,
			&HandleBuffer
			);
	if (EFI_ERROR(Status)){
		Print(L"Faild to find PCI devices. Status: %r\n", Status);
		return Status;
		}
	// iterate each Handle, open protocol
	for (Index = 0; Index < HandleCount; Index++){
		Status = gBS->OpenProtocol(HandleBuffer[Index],
				&gEfiPciIoProtocolGuid,
				(VOID **)&PciIo,
				ImageHandle,
				NULL,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
				);
		if (EFI_ERROR(Status)){
			continue;
		}

		PciIo->Pci.Read(PciIo,EfiPciIoWidthUint16, 0x00, 1, &VendorId);
		PciIo->Pci.Read(PciIo, EfiPciIoWidthUint16, 0x02, 1, &DeviceId);
		
		Print(L"PCI Device [%02d]: Vendor ID = 0x%04X, Devie ID = 0x%04X\n",Index, VendorId, DeviceId);

		// close Protocol
		gBS->CloseProtocol(HandleBuffer[Index], &gEfiPciIoProtocolGuid, ImageHandle, NULL);

		}
	//free memory
	if (HandleBuffer != NULL){
		FreePool(HandleBuffer);
		}
	Print(L"Scan Complete.\n");
	return EFI_SUCCESS;
	}

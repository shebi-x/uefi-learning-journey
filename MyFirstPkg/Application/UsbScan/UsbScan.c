#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include<Library/MemoryAllocationLib.h>
#include<Protocol/UsbIo.h>

EFI_STATUS
EFIAPI
UefiMain(
	IN EFI_HANDLE			ImageHandle,
	IN EFI_SYSTEM_TABLE		*SystemTable
		)
{
EFI_STATUS		Status;
EFI_HANDLE		*HandleBuffer = NULL;
UINTN			HandleCount = 0;
UINTN			Index;
EFI_USB_IO_PROTOCOL	*UsbIo;
EFI_USB_DEVICE_DESCRIPTOR	DeviceDescriptor;

Print(L"Scanning USB Device...\n");
Print(L"=====================================================");
//1. Find all support USB I/O Protocol Handle
Status = gBS->LocateHandleBuffer(
		ByProtocol,
		&gEfiUsbIoProtocolGuid,
		NULL,
		&HandleCount,
		&HandleBuffer
		);
if (EFI_ERROR(Status) || HandleCount == 0){
	Print(L"No USB devices found or error:%r\n", Status);
	if (HandleBuffer)
		FreePool(HandleBuffer);
	return EFI_SUCCESS;
	}

Print(L"Found %d USB device(s).\n", HandleCount);

//2. itrate pre USB device
for (Index = 0; Index < HandleCount; Index++){
	Status = gBS->OpenProtocol(
			HandleBuffer[Index],
			&gEfiUsbIoProtocolGuid,
			(VOID **)&UsbIo,
			ImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL
			);
	if (EFI_ERROR(Status)){
		continue;
		}
	//3.Read USB Device 
	Status = UsbIo->UsbGetDeviceDescriptor(UsbIo, &DeviceDescriptor);
	if (!EFI_ERROR(Status)){
		Print(L"USB Device [%02d]:\n", Index);
		Print(L"Vendor ID: 0x%04X\n", DeviceDescriptor.IdVendor);
		Print(L"Product ID: 0x%04X\n", DeviceDescriptor.IdProduct);
		Print(L"Class	  :0x%02X\n", DeviceDescriptor.DeviceClass);

		switch (DeviceDescriptor.DeviceClass)
			{
				case 0x00: Print(L"Defind by Interface\n"); break;
				case 0x03: Print(L"HID (Keyboard/mouse)\n"); break;
			 	case 0x08: Print(L"Mass Storage(UDisk)\n"); break;
				case 0x09: Print(L"Hub\n"); break;
				default:   Print(L"Other\n");break;
			}
		}

	//4. close protocol
	gBS->CloseProtocol(
			HandleBuffer[Index],
			&gEfiUsbIoProtocolGuid,
			ImageHandle,
			NULL
			);
	}
Print(L"=========================================================\n");
Print(L"USB Scan Complete.\n");
return EFI_SUCCESS;


}

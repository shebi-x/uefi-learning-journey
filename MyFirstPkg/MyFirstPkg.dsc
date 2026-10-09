[Defines]
PLATFORM_NAME				= MyFirstPkg
PLATFORM_GUID				= 87654321-4321-4321-4321-BA0987654321
PLATFORM_VERSION			= 1.0
DSC_SPECIFICATION			= 0x00010005
OUTPUT_DIRECTORY			= Build/MyFirstPkg
SUPPORTED_ARCHITECTURES			= X64
BUILD_TARGETS				= DEBUG|RELEASE
SKUID_IDENTIFIER			= DEFAULT

[LibraryClasses]
UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
StackCheckLib|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf

[Components]
MyFirstPkg/Application/MyHello/MyHello.inf
MyFirstPkg/Application/PciScan/PciScan.inf

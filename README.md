# UEFI Learning Journey

My hands-on journey from zero to UEFI firmware development on Debian Linux.

## Goal

Transitioning from laptop hardware sales to firmware/BIOS engineering. This
repo documents my daily progress, experiments and lessons learned.

## Environment

- **Host OS**: Debian 13(x86_64)
- **Firmware**: EDK II (upstream+OVMF)
- **Emulator**: QEMU with KVM -**Toolchain**: GCC (with `\-t GCC` flag )

## Directory structure

- `MyFirstPkg/` \-- My first custom UEFI application package.

## Day 1: Hello World from Scratch (2026-10-07)

### What I did today

1.  Created a custom UEFI application package (`MyFirstPkg`) from scratch,
    including `.c`, `.inf`, and `.dsc` files.
2.  Ran into **multiple  dependency resolulation errors** (error 4000) and fixed
    them one by one.
3.  compiled the app successfully and booted it in QEMU, printing real pointer
    values.

### Key Lessons Learned (whack-a-mole phase)

EDK II require **explicit declaration** of every single library in the `.dsc`
file. It does NOT auto-resolve like Windows Visual Studio. The following
libraries were added to the `\[LibraryClasses]`
 section:
 | Library Class | Whyit was Needed  | 
|---|---| 
| `UefiApplicationEntryPoint` | Provides the `UefiMain`entry point | 
| `UefiLib`|Provides `Print()` | `PrintLib`|String formatting |
| `RegisterFilterLib` | New security requirement for `PrintLib` |
| `PcdLib` |Required by `UefiLib` |
| `MemoryAllocationLib` | Required by `UefiLib` |
| `UefiBootServicesTableLib` | Provides `gBS` and `gST` pointers |
| `UefiRuntimeServicesTableLib` | Provides `gRT` pointer |
| `BaseLib` / `BaseMemoryLib` | Core Primitives |
| `DebugLib` / `DebugPrintErrorLevelLib` | Debug output |
| `DevicePathLib` | Required by `UefiLib` |
| `StackCheckLib` / `StackCheckFailureHookLib` | New stack Protection (use Null implementation) |

### Common Pitfall

- `#include <Library/Uefi.h>` is **wrong**. It should be `#include <Uefi.h>`
  (no `Library/` prefix).

## How to Build and Run

### 1.Set up EDK II envronment
cd ~/src/edk2
source edksetup.sh

### 2. Copy this package into the EDK II worspace
cp -r ~/uefi-learning-journey/MyFirstPkg .

### 3. Build
build -a X64 -t GCC -p MyFirstPkg/MyFirstPkg.dsc

### 4.Prepare ESP
mkdir -p /tmp/uefi_esp/EFI/BOOT
cp Build/MyFirstPkg/DEBUG_GCC/X64/MyHello.efi /tmp/uefi_esp/EFI/BOOT/BOOTX64.EFI

### 5.Run in QEMU
qemu-system-x86_64 -enable-kvm -m 2048 -bios /usr/share/ovmf/OVMF.fd -drive file=fat:rw:/tmp/uefi_esp,format=raw -net none


Screenshot

![image](./HelloUEFI.png)

## Day 2: Update PCI Enumeration Works! (2026-10-09)

Successfuly built and ran my custom `PciScan.efi` in QEMU. The application uses the `EFI_PCI_IO_PROTOCOL` to read the configuration space of all PCI devices.

Output in QEMU UEFI Shell:
- `0x8086:0x1237` (Intel Host Bridge)
- `0x8086:0x7000` (Intel ISA Bridge)
- `0x8086:0x7010` (Intel IDE Controller)
- `0x8086:0x7113` (Intel ACPI Controller)
- `0x1234:0x1111` (QEMU Virtual VGA)

This proves  I understand:
1. How to locate handles by protocol (`LocateHandleBuffer`).
2. How to open a protocol (`OpenProtocol`) on a handle.
3. How to invoke function pointers inside a protocol structure (`PciIo->Pci.Read`).
4. How PCI enumeration works at the firmware level.


Day 2 Screenshot

![DeviceId and VendorId](./PciScan.png)



## Day 3: Identifying PCI Device Classes (2026-10-11)

Extended my `PciScan` application to read the **Class Code** (offset 0x0B) and **SubClass** (offset 0x0A) from the PCI configuration space.

This allows me to identify the *type* of hardware based on its Class Code. Here is the actual output from QEMU running my `PciScan.efi`:

![PCI Class](./PrintPciClass.png)
```text
=================================
PCI Device [00]: Vendor ID = 0x8086, Device ID = 0x1237, Class = 0x06, SubClass = 0x00
PCI Device [01]: Vendor ID = 0x8086, Device ID = 0x7000, Class = 0x06, SubClass = 0x01
PCI Device [02]: Vendor ID = 0x8086, Device ID = 0x7010, Class = 0x01, SubClass = 0x01
PCI Device [03]: Vendor ID = 0x8086, Device ID = 0x7113, Class = 0x06, SubClass = 0x00
PCI Device [04]: Vendor ID = 0x1234, Device ID = 0x1111, Class = 0x03, SubClass = 0x00
Scan Complete.
=================================
Decoded device types from the actual output above:
Device [00] & [01] & [03]: Class 0x06 = Bridge Device (Host, ISA, ACPI controllers)
Device [02]: Class 0x01, SubClass 0x01 = Mass Storage Controller (IDE Controller)
Device [04]: Class 0x03, SubClass 0x00 = Display Controller (VGA Compatible)

This is highly relevant to my laptop repair business. When a device is "not recognized," I can now use a firmware-level tool to check if the hardware is even presenting its Class Code correctly to the system (e.g., looking for Class 0x02 for Network Controller).

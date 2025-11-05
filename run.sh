#!/bin/bash
set -e

# Usage: ./run.sh [--qemu|--vbox|--usb]
# Default: --qemu

MODE="qemu"
case "$1" in
	""|"--qemu") MODE="qemu" ;;
	"--vbox") MODE="vbox" ;;
	"--usb") MODE="usb" ;;
	"--help"|"-h")
		echo "Usage: $0 [--qemu|--vbox|--usb]"
		exit 0
		;;
	*)
		echo "Unknown option: $1" >&2
		echo "Usage: $0 [--qemu|--vbox|--usb]" >&2
		exit 1
		;;
esac

if [ "$MODE" = "qemu" ]; then
	if command -v qemu-system-x86_64 >/dev/null 2>&1; then
		qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
	else
		echo "qemu-system-x86_64 not found" >&2
		exit 1
	fi
elif [ "$MODE" = "vbox" ]; then
	echo "Built VirtualBox disk at Image/x64BareBonesImage.vmdk"
	echo "Create a new VM in VirtualBox and attach this VMDK as the disk."
	exit 0
else # usb
	echo "Built raw image at Image/x64BareBonesImage.img"
	echo "To write it to a USB device (DANGEROUS), use:"
	echo "  sudo dd if=Image/x64BareBonesImage.img of=/dev/sdX bs=4M status=progress oflag=sync"
	echo "Replace /dev/sdX with your USB device."
	exit 0
fi

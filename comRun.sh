#!/bin/bash
# Script para compilar y (opcionalmente) ejecutar el proyecto
# Uso: ./comRun.sh [qemu|vbox|usb]
# Por defecto: qemu

set -e

TARGET_ARG="${1:-qemu}"
case "$TARGET_ARG" in
	qemu|vbox|usb) ;;
	*) echo "Uso: $0 [qemu|vbox|usb]" >&2; exit 1 ;;
esac

cd ./Toolchain
make clean
make all
cd ..

make clean
make TARGET="$TARGET_ARG" all

if [ "$TARGET_ARG" = "qemu" ]; then
	# Intentar habilitar audio del PC Speaker en QEMU (PulseAudio). Si falla, correr sin audio.
	if command -v qemu-system-x86_64 >/dev/null 2>&1; then
		# QEMU moderno: usar audiodev y pcspk
		if qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 \
			 -audiodev pa,id=snd0 \
			 -machine pcspk-audiodev=snd0 2>/dev/null; then
			exit 0
		fi
	fi
	# Fallback sin audio explícito
	qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
elif [ "$TARGET_ARG" = "vbox" ]; then
	echo "Imagen para VirtualBox lista: Image/x64BareBonesImage.vmdk"
	echo "Cree una VM en VirtualBox y adjunte este VMDK como disco."
else
	echo "Imagen cruda para USB lista: Image/x64BareBonesImage.img"
	echo "Para grabarla en un pendrive (PELIGROSO), ejecute:"
	echo "  sudo dd if=Image/x64BareBonesImage.img of=/dev/sdX bs=4M status=progress oflag=sync"
	echo "Reemplace /dev/sdX por su dispositivo USB."
fi
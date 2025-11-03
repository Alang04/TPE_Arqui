#!/bin/bash
# Script para compilar y ejecutar el proyecto automáticamente

set -e

cd ./Toolchain

make clean

make all

cd ..

make clean

make all

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
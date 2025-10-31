#!/bin/bash
# Script para compilar y ejecutar el proyecto automáticamente

set -e

cd ./Toolchain

make clean

make all

cd ..

make clean

make all

qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
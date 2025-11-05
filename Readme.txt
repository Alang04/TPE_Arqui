TPE_Arqui (x64BareBones based)
================================

Resumen
-------
Este proyecto es un pequeño sistema operativo para arquitectura x86_64 basado en x64BareBones. 
Incluye:
- Kernel 64-bit con IDT/IRQs, temporizador (PIT), manejo de teclado, consola gráfica en framebuffer, y syscalls vía int 0x80.
- Módulo de usuario con una shell simple y utilidades (benchmarks, reloj/fecha, beep, etc.).
- Un juego Tron en modo texto (1P vs IA o 2P) con música de fondo usando el parlante de PC.

Estructura
----------
- Bootloader/: Pure64 + BMFS para cargar kernel y módulos.
- Kernel/: código del kernel, drivers de video/teclado/sonido, IDT/IRQs, syscalls.
- Userland/: shell, wrappers de syscalls y módulos de usuario (incluye `tron/`).
- Image/: empaquetado a imagen cruda y conversión a VMDK/ QCOW2.
- Toolchain/ModulePacker: herramienta para empaquetar módulos de usuario con el kernel.

Requisitos
----------
Instalar en el host antes de compilar:
- nasm
- qemu
- gcc y binutils (para 64-bit)
- make

Build y ejecución
-----------------
Compilar todo (bootloader, kernel, userland e imagen):
  $ make all                 # por defecto TARGET=qemu
  $ make TARGET=vbox all     # genera VMDK para VirtualBox
  $ make TARGET=usb  all     # genera imagen cruda .img para USB

Arrancar en QEMU:
  $ ./run.sh                 # usa Image/x64BareBonesImage.qcow2
  $ ./run.sh --qemu          # equivalente

Construir y correr con helper script:
  $ ./comRun.sh qemu         # build + ejecutar en QEMU
  $ ./comRun.sh vbox         # build VMDK y mostrar pasos para VirtualBox
  $ ./comRun.sh usb          # build .img y mostrar comando dd (no se ejecuta)

(Opcional) Construir solo el empaquetador:
  $ (cd Toolchain && make all)

Salida e imagenes generadas:
- `Image/x64BareBonesImage.img` cruda (también VMDK y QCOW2 para VMs).
  - con TARGET=qemu: se prioriza `x64BareBonesImage.qcow2`
  - con TARGET=vbox: se prioriza `x64BareBonesImage.vmdk`
  - con TARGET=usb : se prioriza `x64BareBonesImage.img`

Shell de usuario
----------------
Al bootear se inicia una shell mínima. Comandos disponibles (ver `help` en tiempo de ejecución):
- help, clear, printTime, printDate, registers, testDiv0, invOp, playBeep
- + / -     (aumenta/disminuye tamaño de fuente)
- bmFPS, bmCPU, bmMEM, bmKEY (benchmarks simples)
- tron      (lanza el juego Tron)

Juego Tron (modo texto)
-----------------------
- Al ejecutar `tron` se muestra un menú para elegir: 1 Jugador vs IA o 2 Jugadores locales.
- Controles dentro del juego:
  - P1: WASD
  - P2: IJKL
  - p: Pausa / reinicia ronda si terminó
  - q: Volver al menú de Tron
- HUD muestra puntajes, número de step y estado (PAUSA/FIN DE RONDA). 
- Modo texto: la grilla se dibuja con caracteres ('.', '#', '1', '2'); las cabezas de jugadores se marcan como 'A' y 'B'.
- Música de fondo no bloqueante (parlante PC); jingles breves en eventos.

Notas de desarrollo
-------------------
- Syscalls vía int 0x80:
  0 reg snapshot, 1 time, 2 date, 3 read, 4 write, 5/6 tamaño de fuente, 7 beep (bloqueante),
  8 ticks, 9 clear, 10 speaker_start, 11 speaker_off.
- Consola gráfica: framebuffer VBE, fuente bitmap; soporte de backspace y newline, scroll.
- Drivers: teclado con buffer circular; audio PIT ch2 para parlante (bloqueante y no bloqueante).
- Flags de compilación con warnings estrictos para mantener la API encapsulada y libre de código muerto.

Créditos
--------
Base x64BareBones: Rodrigo Rearden (RowDaBoat)
Colaborador: Augusto Nizzo McIntosh

Este trabajo extiende esa base con shell, syscalls adicionales, y el juego Tron en userland.
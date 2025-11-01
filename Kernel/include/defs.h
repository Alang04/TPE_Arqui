#ifndef _defs_
#define _defs_

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80       /* segmento presente en memoria */
#define ACS_CSEG        0x18       /* segmento de codigo */
#define ACS_DSEG        0x10       /* segmento de datos */
#define ACS_READ        0x02       /* segmento de lectura */
#define ACS_WRITE       0x02       /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )
#define ACS_DPL_3       0x60
#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#define MIN_CHAR 0
#define MAX_CHAR 256
#define CANT_SYS 2
#define STDOUT 1
#define STDERR 2
#define TEXT_SIZE 1
#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define X_UPDATE FONT_WIDTH * TEXT_SIZE
#define Y_UPDATE FONT_HEIGHT * TEXT_SIZE
#define BUFF_LENGTH 256
#define REGISTERS_BUFFER_SIZE 512

#endif
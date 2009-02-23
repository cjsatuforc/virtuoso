
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "iferret_log.h"
#include "target-i386/iferret_log_arg_fmt.h"
#include "vslht.h"

// turn info-flow logging on or off
uint8_t iferret_info_flow_on = 0;

// ptr to first byte of info flow log
char *iferret_log_base = NULL;      

// ptr to next byte to be written in info flow log
char *iferret_log_ptr = NULL;      

char *iferret_log_ptr_op_start = NULL;

uint32_t iferret_log_rollup_count = 0;  

char *iferret_keyboard_label=NULL;
uint8_t iferret_keyboard_label_changed = 0;

char* iferret_network_label=NULL;
uint8_t iferret_network_label_changed = 0;

// we'll be using these as "addresses" for registers
unsigned long long ifregaddr[16];


extern unsigned int phys_ram_size;




void iferret_log_syscall_commoner(iferret_syscall_t *sc) {
  // write the std syscall other args.
  iferret_log_uint8_t_write(sc->is_sysenter);  
  iferret_log_uint32_t_write(sc->pid);
  iferret_log_uint32_t_write(sc->callsite_eip);
  iferret_log_string_write(sc->command);

}  


void iferret_log_syscall_common(iferret_syscall_t *sc, va_list op_args) {
  // write the std syscall other args.
  iferret_log_syscall_commoner(sc);
  // write the args specific to this call
  iferret_log_op_args_write(sc->op_num, op_args);
}  


void iferret_log_syscall_write_va(iferret_syscall_t *sc, ...) {
  va_list op_args;

  // write the op and the sentinel
  iferret_log_op_write_prologue(sc->op_num);
  va_start(op_args, sc);
  iferret_log_syscall_common(sc,op_args);
}  


void iferret_log_socketcall_write_va(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, ...) {
  va_list op_args;

  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  va_start(op_args, op_num);
  iferret_log_syscall_common(sc,op_args);
}


void iferret_log_socketcall_write_4(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
				    uint32_t x0) {

  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
}

void iferret_log_socketcall_write_44(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
				      uint32_t x0, uint32_t x1) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
}

void iferret_log_socketcall_write_444(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
				      uint32_t x0, uint32_t x1, uint32_t x2) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
  iferret_log_uint32_t_write(x2);
}


void iferret_log_socketcall_write_4444(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
				       uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
  iferret_log_uint32_t_write(x2);
  iferret_log_uint32_t_write(x3);
}

void iferret_log_socketcall_write_444444444(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
					    uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
					    uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,
					    uint32_t x8) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
  iferret_log_uint32_t_write(x2);
  iferret_log_uint32_t_write(x3);
  iferret_log_uint32_t_write(x4);
  iferret_log_uint32_t_write(x5);
  iferret_log_uint32_t_write(x6);
  iferret_log_uint32_t_write(x7);
  iferret_log_uint32_t_write(x8);
}


void iferret_log_socketcall_write_4444444444(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
					     uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
					     uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,
					     uint32_t x8, uint32_t x9) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
  iferret_log_uint32_t_write(x2);
  iferret_log_uint32_t_write(x3);
  iferret_log_uint32_t_write(x4);
  iferret_log_uint32_t_write(x5);
  iferret_log_uint32_t_write(x6);
  iferret_log_uint32_t_write(x7);
  iferret_log_uint32_t_write(x8);
  iferret_log_uint32_t_write(x9);
}


void iferret_log_socketcall_write_4444444444444444444
(iferret_syscall_t *sc, iferret_log_op_enum_t op_num, 
 uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,
 uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,
 uint32_t x8, uint32_t x9, uint32_t x10, uint32_t x11,
 uint32_t x12, uint32_t x13, uint32_t x14, uint32_t x15,
 uint32_t x16, uint32_t x17, uint32_t x18, uint32_t x19
 ) {
  // write the op and the sentinel
  iferret_log_op_write_prologue(op_num);
  // write the std syscall stuff
  iferret_log_syscall_commoner(sc);
  iferret_log_uint32_t_write(x0);
  iferret_log_uint32_t_write(x1);
  iferret_log_uint32_t_write(x2);
  iferret_log_uint32_t_write(x3);
  iferret_log_uint32_t_write(x4);
  iferret_log_uint32_t_write(x5);
  iferret_log_uint32_t_write(x6);
  iferret_log_uint32_t_write(x7);
  iferret_log_uint32_t_write(x8);
  iferret_log_uint32_t_write(x9);
  iferret_log_uint32_t_write(x10);
  iferret_log_uint32_t_write(x11);
  iferret_log_uint32_t_write(x12);
  iferret_log_uint32_t_write(x13);
  iferret_log_uint32_t_write(x14);
  iferret_log_uint32_t_write(x15);
  iferret_log_uint32_t_write(x16);
  iferret_log_uint32_t_write(x17);
  iferret_log_uint32_t_write(x18);
  iferret_log_uint32_t_write(x19);
}




/*
 iferret_log_socketcall_write_4444444444444444444
*/

void iferret_log_op_args_write(iferret_log_op_enum_t op_num, va_list op_args) {
  char *op_fmt, *p;

  op_fmt = iferret_log_arg_format[op_num];
  //  va_start(op_args, op_num);
  for (p=op_fmt; *p!='\0'; p++) {
    switch (*p) {
    case IFLAT_NONE: // no args at all
      break;
    case IFLAT_UI8:      // uint8_t
      {
	//	uint8_t ui8 = va_arg(op_args,uint8_t);
	uint8_t ui8 = (uint8_t) va_arg(op_args,uint32_t);
	iferret_log_uint8_t_write(ui8);
      }
      break;
    case IFLAT_UI16:     // uint16_t
      {
	//	uint16_t ui16 = va_arg(op_args,uint16_t);
	uint16_t ui16 = (uint16_t) va_arg(op_args,uint32_t);
	iferret_log_uint16_t_write(ui16);      
      }
      break;
    case IFLAT_UI32:     // uint32_t
      {
	uint32_t ui32 = va_arg(op_args,uint32_t);
	iferret_log_uint32_t_write(ui32);
      }
      break;
    case IFLAT_UI64:     // uint64_t
      {
	uint64_t ui64 = va_arg(op_args,uint64_t);
	iferret_log_uint64_t_write(ui64);
      }
      break;
    case IFLAT_STR:      // string
      {
	char *str = va_arg(op_args,char *);
	iferret_log_string_write(str);
      }
      break;
    }
  }
  va_end(op_args);
}


// read an info-flow op and all its args from the log
// op_fmt is a string telling us how to interpret the elements in op_args
// op_args is a va_list containing the *addresses* of the arguments.
void iferret_log_op_args_read(iferret_op_t *op) {
  char *p;
  char buf[MAX_STRING_LEN];
  int i;

  // NB: we've already read the op and checked the sentinel...

  if (op->num >= IFLO_SYS_CALLS_START) {
    // its a syscall.  read in the other stuff.
    op->syscall->is_sysenter = iferret_log_uint8_t_read();
    op->syscall->pid = iferret_log_uint32_t_read();
    op->syscall->callsite_eip = iferret_log_uint32_t_read();
    iferret_log_string_read(op->syscall->command);
  }

  // now we iterate through the fmt string for this op
  // and read each type of thing from the log and into
  // the right place in op.  

  i=0;
  op->num_args = 0;
  for (p=iferret_log_arg_format[op->num]; *p!='\0'; p++) {
    switch (*p) {
    case '1':      // a 1-byte unsigned int
      op->arg[i].type = IFLAT_UI8;
      op->arg[i].val.u8 = iferret_log_uint8_t_read();
      op->num_args++;
      break;
    case '2':     // a 2-byte unsigned int
      op->arg[i].type = IFLAT_UI16;
      op->arg[i].val.u16 = iferret_log_uint16_t_read();
      op->num_args++;
      break;
    case '4':     // a 4-byte unsigned int
      op->arg[i].type = IFLAT_UI32;
      op->arg[i].val.u32 = iferret_log_uint32_t_read();
      op->num_args++;
      break;
    case '8':     // an 8-byte unsigned int
      op->arg[i].type = IFLAT_UI64;
      op->arg[i].val.u64 = iferret_log_uint64_t_read();
      op->num_args++;
      break;
    case 's':      // a string
      op->arg[i].type = IFLAT_STR;
      iferret_log_string_read(buf);
      op->arg[i].val.str = strdup(buf);
      op->num_args++;
      break;
    default: 
      break;
    }
    i ++;
  }

}

 

void iferret_spit_op(iferret_op_t *op) {
  int i;
  printf ("(%s", iferret_op_num_to_str(op->num));
  if (op->num >= IFLO_SYS_CALLS_START) {
    printf (",(is_sysenter,%d)", op->syscall->is_sysenter);
    printf (",(pid,%d)", op->syscall->pid);
    printf (",(callsite_eip,%d)", op->syscall->callsite_eip);
    printf (",(command,%s)", op->syscall->command);
  }
  for (i=0; i<op->num_args; i++) {
    switch (op->arg[i].type) {
    case IFLAT_NONE:
      exit(1);
    case IFLAT_UI8:
      printf (",(ui8,%x)", op->arg[i].val.u8);
      break;
    case IFLAT_UI16:
      printf (",(ui16,%x)", op->arg[i].val.u16);
      break;      
    case IFLAT_UI32: 
      printf (",(ui32,%lx)", (long unsigned int) op->arg[i].val.u32);
      break;
    case IFLAT_UI64:
      printf (",(ui64,%llx)", (long long unsigned int) op->arg[i].val.u64);
      break;
    case IFLAT_STR:
      printf (",(str,%s)", op->arg[i].val.str);
      break;
    }
  }
  printf (")\n");
}


// this gets called from the qemu monitor in order to 
// change the current keyboard label
void iferret_set_keyboard_label(const char *label) {
  if (iferret_keyboard_label == NULL) 
    iferret_keyboard_label = (char *) malloc(IFERRET_MAX_KEYBOARD_LABEL_LEN);
  assert (iferret_keyboard_label != NULL);
  printf ("iferret_set_keyboard_label: label is henceforth [%s]\n", label);
  strncpy(iferret_keyboard_label, label, IFERRET_MAX_KEYBOARD_LABEL_LEN);
  iferret_keyboard_label_changed = 1;
}

// this gets called from the qemu monitor in order to 
// change the current network label
void iferret_set_network_label(const char *label) {
  if (iferret_network_label == NULL) 
    iferret_network_label = (char *) malloc(IFERRET_MAX_NETWORK_LABEL_LEN);
  assert (iferret_network_label != NULL);
  printf ("iferret_set_network_label: label is henceforth [%s]\n", label);
  strncpy(iferret_network_label, label, IFERRET_MAX_NETWORK_LABEL_LEN);
  iferret_network_label_changed = 1;
}

void iferret_log_create() {
  int i;
  // initial info flow log allocation.
  iferret_log_ptr = iferret_log_base = (char *) calloc (IFERRET_LOG_SIZE,1);
  iferret_set_keyboard_label("keyboard_startup");
  iferret_set_network_label("network_startup");
  // set up ifregaddr array.
  for (i=0; i<16; i++) {
    // all addresses memory addresses will be on the interval 
    // 0 .. phys_ram_size-1.  
    // We'll pretend the registers live starting at phys_ram_size.
    // and we'll make them 8 bytes just for fun.
    ifregaddr[i] = (unsigned long long) ( ((uint64_t) phys_ram_size) + i*8);
    printf("Register i=%d is at 0x%x\r\n", i, (unsigned int) ifregaddr[i]);
  }

}




// save current if log to a file for some reason    
void iferret_log_write_to_file() {
  char filename[1024];
  FILE *fp;

  printf ("iferret_log_ptr - iferret_log_base = %Lu\n", (unsigned long long) (iferret_log_ptr - iferret_log_base));
  printf ("IFERRET_LOG_SIZE = %d\n", IFERRET_LOG_SIZE);

  snprintf (filename, 1024, "/scratch/tmp2/ifl-%d-%d", getpid(), iferret_log_rollup_count);

  fp = fopen (filename, "w");

  fwrite(iferret_log_base, 1, iferret_log_ptr-iferret_log_base, fp);
  fclose(fp);
  
  printf ("wrote if log to %s\n", filename);
  iferret_log_rollup_count ++;


  // processing complete; ready to write over old log.  
  iferret_log_ptr = iferret_log_base; 

}


// info-flow log is full.  Dump it to a file. 
void iferret_log_rollup() {
  iferret_log_write_to_file();
}


#include <cstdint>
#include <cstdio>
#include <cstdlib>

//
// @TODO
// Variables / sym table
// Stack
// Func args
// local variables
// Loop with Opcodes
//

// Instruction set
enum OpCode : uint8_t {
  OP_HALT,
  OP_NOP,

  OP_MOV_RR,
  OP_MOV_MR,
  OP_MOV_MM,
  OP_MOV_RM,
  OP_MOV_CR,
  OP_MOV_CM,

  OP_JMP,
  OP_JE,
  OP_JNE,
  OP_JZ,
  OP_JG,
  OP_JGE,
  OP_JL,
  OP_JLE,

  OP_CMP,

  OP_NOT,
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_NEG,

  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,

  OP_FADD,
  OP_FSUB,
  OP_FMUL,
  OP_FDIV,

  OP_INC,
  OP_DEC,
};

// VM State
enum VmState : uint32_t {
  VM_HALT     = 0,
  VM_CMP_E    = 1<<0,
  VM_CMP_NE   = 1<<1,
  VM_CMP_Z    = 1<<2,
  VM_CMP_G    = 1<<3,
  VM_CMP_GE   = 1<<4,
  VM_CMP_L    = 1<<5,
  VM_CMP_LE   = 1<<6,
  VM_DIV_ZERO = 1<<7
};


static uint32_t machine_status = 0;
static void *ip;
#define REGISTER_NUMBER 16
static uint32_t registers[REGISTER_NUMBER];

#define HEAP_SIZE 4 * 1024 * 1024
static uint8_t *code = NULL;

struct SymTable {
  uint32_t  identifier;
  uintptr_t addr;
  char      *symbol;
};


//
// Instruction pointer routines
//
void ip_i8() {
  ip = ((uint8_t *)ip) + sizeof(uint8_t);
}

void ip_i32() {
  ip = ((uint32_t *)ip) + sizeof(uint32_t);
}


void gen_program() {
  ip = &code;

  *(uint8_t *)ip = OP_ADD;
  ip_i8();
  *(uint32_t *)ip = 256;
  ip_i32();
  *(uint8_t *)ip = OP_HALT;
}

//
// VM routines
//

void vm_setup() {
  if(code) free(code);
  code = (uint8_t *)malloc(HEAP_SIZE);  
}

void vm_reset() {
  printf("vm_reset():\n)";
  machine_status = 0;
  ip = &code;
}

void vm_loop() {
  printf("vm_loop():start\n");

  for(;;) {
    switch(*(uint8_t *)ip) {
    case OP_ADD:
      printf("%d\n", *(uint8_t *)ip);
      ip_i8();
      printf("%d\n", *(uint32_t *)ip);
      ip_i32();
    case OP_HALT:
      printf("vm_loop():end\n");
      return;
      break;
    }
  }
}

void vm_clear() {
  if(code) free(code);
}

int main(int argc, char *argv[]) {
  gen_program();
  vm_reset();
  vm_loop();
  vm_clear();
}


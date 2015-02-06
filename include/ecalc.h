#ifndef __ECALC_H__
# define __ECALC_H__

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <math.h>
# include <ctype.h>

#define IS_EFFECTIVE_PTR(ptr) (((ptr) != NULL) && (ptr) != ((void*)0xdeadbeef))
#define PTR(x, type) type *x = NULL;
#define MALLOC(x, type, size) do { \
if (IS_EFFECTIVE_PTR(x)) { \
  ERROR("Already allocated pointer:%s", #x); \
  exit(-1); \
} \
x = (type*) malloc((sizeof(type)) * size); \
} while (0)
#define FREE(x) do { \
if (!IS_EFFECTIVE_PTR(x)) { \
  ERROR("Already done free: %s", #x); \
  exit(-1); \
} \
free(x); \
x = ((void*)0xdeadbeef); \
} while(0)
# if defined(NDEBUG)
#  define CHK_DEBUG()
#  define DEBUG(x)
# else
#  define CHK_DEBUG() fprintf(stderr, "%s:%d <%s>\n", __FILE__, __LINE__, __FUNCTION__)
#  define DEBUG(x, ...) do {\
  fprintf(stderr, "%s:%d <%s> -> ", __FILE__, __LINE__, __FUNCTION__);\
  fprintf(stderr, x, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
} while (0)
# endif

# define ERROR(...) fprintf(stderr, "ERROR occured: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\nat: file: %s, function: %s, line: %d\n\n", __FILE__, __FUNCTION__, __LINE__)

# define DIRECTION_FOR(counter, init, condition, dir) for (counter = init; condition; counter = counter + ((int)dir)*1)

# define EACH_ARRAY(func, array, size) do { \
  uint __each_counter = 0; \
  for (__each_counter = 0; __each_counter < size; func(array[__each_counter++])); \
} while(0)
typedef enum {
  D_FORWARD = 1,
  D_BACK = -1
} Direction;

typedef enum {
  NONE = 0,
  BINARY = 2,
  DECIMAL = 10,
  HEXADECIMAL = 16
} numtypes_t;

typedef enum {
  T_ADD,
  T_SUB,
  T_MUL,
  T_DIV,
  T_POW,
  T_OR,
  T_AND,
  T_NOT,
  T_XOR,
  T_NUM,
  T_OPEN_BRACKET,
  T_CLOSE_BRACKET,
  T_EOF,
} TokenKind;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_POW,
  OP_OR,
  OP_AND,
  OP_NOT,
  OP_XOR,
  OP_NUM,
} Opcode;

typedef struct {
  TokenKind type;
  char value;
} Token;

typedef unsigned int uint;

typedef void* LLData_t;
typedef void (*fptr_lleach_t)(LLData_t);
typedef struct linkedlist_t *LinkedList;

/* at main.c */
extern char *g_expression;
extern LinkedList g_token;

extern Token get_token(void);
extern void unget_token(void);

/* at parse.c */
extern void parse(void);
extern void parse_expression(void);
extern void parse_primary_expression(void);
extern void parse_term(void);
extern void parse_not(void);
extern void parse_number(void);

/* at lex.c */
extern void lexical_analyze(void);

/* at vm.c */
extern LinkedList g_bytecode;
extern void vm_init(void);
extern void vm_free(void);
extern void vm_execute(void);
extern void vm_add_opcode(Opcode opcode, ...);

/* at LinkedList.c */
extern LinkedList ll_get_instance(void);
extern void ll_add(LinkedList, LLData_t);
extern void ll_set_pos(LinkedList, uint);
extern void ll_set_pos_relative(LinkedList, int);
extern uint ll_get_pos(LinkedList);
extern LLData_t ll_get_data(LinkedList, uint);
extern LLData_t ll_get_current_data(LinkedList);
extern LLData_t ll_remove_data(LinkedList, uint);
extern void ll_foreach(LinkedList, fptr_lleach_t, uint);
extern size_t ll_count(LinkedList);
extern void ll_free(LinkedList);

#endif
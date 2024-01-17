#include <stdlib.h>

#define STACK_MAX	256

typedef struct s_object	t_object;
typedef struct s_gc	t_gc;

struct s_object
{
    t_object        *next;
    unsigned char   marked; 
	size_t          size;
	void            *base;
};

struct s_gc
{
    t_object    **stack;
    t_object    *root;
    size_t      cap;
    size_t      len;
};

t_gc        *new_gc();
int         push_object(t_gc *gc, t_object *obj);
t_object    *pop_object(t_gc *gc);
t_object    *new_object(t_gc *gc, size_t size); /* entry to gc table */
void        gc_run(t_gc *gc);
void        gc_end(t_gc *gc);

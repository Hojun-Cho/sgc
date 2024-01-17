#include <stdio.h>
#include "sgc.h"

t_gc    *new_gc()
{
    t_gc    *gc;

    gc = calloc(1, sizeof(t_gc));
    if (gc == NULL)
        return (NULL);
    gc->stack = calloc(STACK_MAX, sizeof(t_object *));
    if (gc->stack == NULL)
    {
        free(gc);
        return (NULL);
    }
    gc->cap = STACK_MAX;
    gc->len = 0;
    return (gc);
}

int    push_object(t_gc *gc, t_object *obj)
{
    if (gc->len + 1 == gc->cap)
    {
        fprintf(stderr, "Error: Can't push anymore\n");
        return (-1);
    }
    gc->stack[gc->len++] = obj;
    return (0);
}

t_object    *pop_object(t_gc *gc)
{
    if (gc->len == 0)
        return (NULL);
    return (gc->stack[--gc->len]);
}

t_object    *new_object(t_gc *gc, size_t size)
{
    t_object    *obj;

    obj = calloc(sizeof(t_object *), STACK_MAX);
    if (obj == NULL)
        return (NULL);
    *obj = (t_object){
        .base = calloc(1, size),
        .size = size,
        .marked = 0,
    };
    if (obj->base == NULL)
    {
        free(obj);
        return (NULL); 
    }
    obj->next = gc->root;
    gc->root = obj;
    return (obj);
}

static void    _mark(t_object *obj)
{
    if (obj->marked)
        return ;
    obj->marked = 1; 
}

static void    mark_all(t_gc *gc)
{
    size_t  i;

    i = 0;
    while (i < gc->cap)
    {
        if (gc->stack[i])
            _mark(gc->stack[i]); // mark all of heap;
        i++;
    }
}

static void    sweep_all(t_gc *gc)
{
    t_object    **obj;
    t_object    *unreacheable;

    obj = &gc->root;
    while (*obj)
    {
        if ((*obj)->marked == 0) 
        {
            unreacheable = *obj;
            *obj = unreacheable->next;
            free(unreacheable->base);
            free(unreacheable);
        }
        else
        {
            (*obj)->marked = 0;
            obj = &(*obj)->next;
        }
    }    
}

void    gc_run(t_gc *gc)
{
    mark_all(gc);
    sweep_all(gc);
}

void    gc_end(t_gc *gc)
{
    mark_all(gc);
    sweep_all(gc);
    free(gc->stack);
    free(gc);
}

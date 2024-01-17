#include "sgc.h"

void test(t_gc *gc)
{
    new_object(gc, 4);
    new_object(gc, 4);
}

int main()
{
    t_gc *gc = new_gc();

    {
        t_object *obj = new_object(gc, 4);
        new_object(gc, 4);
        test(gc);
        new_object(gc, 4);
        /*push_object(gc, obj);*/
        gc_end(gc); 
    }
}

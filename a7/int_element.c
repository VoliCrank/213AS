#include "int_element.h"
#include <stdio.h>
#include <stdlib.h>
#include "refcount.h"

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */


/* Forward reference to a int_element. You get to define the structure. */
struct int_element{
    struct element_class *class;
    int val;
};



void int_print(struct element* thisv){
    struct int_element *this = (struct int_element*)thisv;
    printf("%d", this->val);
}

int int_compare(struct element * arg1, struct element * arg2){
    if (is_int_element(arg2) == 1 && 1 == is_int_element(arg1)){
        struct int_element* this1 = (struct int_element*)arg1;
        struct int_element* this2 = (struct int_element*)arg2;
        if (this1->val > this2->val){
            printf("%d",this1->val);
            return 1;
        } else if (this1->val < this2->val){
            return -1;
        }
        return 0;
    } else if(is_int_element(arg1)){
        return -1;
    } 
    return 1;
}
/* Static function that determines whether this is an int_element. */

void int_finalizer (void* e){

}

struct element_class int_class = {int_print, int_compare};

int is_int_element(struct element* e){
    if ((e->class - &int_class) % sizeof(struct int_element*) == 0){
        return 1;
    }
    return 0;
}
/* Static constructor that creates new integer elements. */
struct int_element *int_element_new(int value){
    struct int_element * ret= rc_malloc(sizeof(*ret),int_finalizer);
    ret->val = value;
    ret->class = &int_class;
    return ret;
}




/* Static function that obtains the value held in an int_element. */
int int_element_get_value(struct int_element * e){
    return e->val;
}


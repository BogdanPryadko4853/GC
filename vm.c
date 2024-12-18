#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

VM* newVM() {
    VM* vm = (VM*)malloc(sizeof(VM));
    vm->stackSize = 0;
    vm->firstObject = NULL;
    vm->numObjects = 0;
    vm->maxObjects = IGCT;
    return vm;
}

void push(VM* vm, Object* value) {
    vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm) {
    return vm->stack[--vm->stackSize];
}

void mark(Object* object) {
    if (object->marked) return;
    object->marked = 1;
    if (object->type == TWIN) {
        mark(object->head);
        mark(object->tail);
    }
}

void markAll(VM* vm) {
    for (int i = 0; i < vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}

void markspeep(VM* vm) {
    Object** object = &vm->firstObject;
    while (*object) {
        if (!(*object)->marked) {
            Object* unreached = *object;
            *object = unreached->next;
            free(unreached);
            vm->numObjects--;
        } else {
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void gc(VM* vm) {
    int numObjects = vm->numObjects;
    markAll(vm);
    markspeep(vm);
    vm->maxObjects = vm->numObjects * 2;
    printf("Collected %d objects, %d left.\n", numObjects - vm->numObjects, vm->numObjects);
}

Object* newObject(VM* vm, ObjectType type) {
    if (vm->numObjects == vm->maxObjects) gc(vm);
    Object* object = (Object*)malloc(sizeof(Object));
    object->type = type;
    object->next = vm->firstObject;
    vm->firstObject = object;
    object->marked = 0;
    vm->numObjects++;
    return object;
}

void pushInt(VM* vm, int intValue) {
    Object* object = newObject(vm, INT);
    object->value = intValue;
    push(vm, object);
}

Object* pushPair(VM* vm) {
    Object* object = newObject(vm, TWIN);
    object->tail = pop(vm);
    object->head = pop(vm);
    push(vm, object);
    return object;
}

void objectPrint(Object* object) {
    switch (object->type) {
        case INT:
            printf("%d", object->value);
            break;
        case TWIN:
            printf("(");
            objectPrint(object->head);
            printf(", ");
            objectPrint(object->tail);
            printf(")");
            break;
    }
}

void freeVM(VM* vm) {
    vm->stackSize = 0;
    gc(vm);
    free(vm);
}
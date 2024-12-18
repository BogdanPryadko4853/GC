#ifndef VM_H
#define VM_H

#define STACK_MAX_SIZE 256
#define IGCT 8

typedef enum {
    INT,
    TWIN
} ObjectType;

typedef struct sObject {
    ObjectType type;
    unsigned char marked;
    struct sObject* next;
    union {
        int value;
        struct {
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;

typedef struct {
    Object* stack[STACK_MAX_SIZE];
    int stackSize;
    Object* firstObject;
    int numObjects;
    int maxObjects;
} VM;

VM* newVM();
void push(VM* vm, Object* value);
Object* pop(VM* vm);
void mark(Object* object);
void markAll(VM* vm);
void markspeep(VM* vm);
void gc(VM* vm);
Object* newObject(VM* vm, ObjectType type);
void pushInt(VM* vm, int intValue);
Object* pushPair(VM* vm);
void objectPrint(Object* object);
void freeVM(VM* vm);

#endif // VM_H
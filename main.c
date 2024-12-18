#include <stdio.h>
#include "vm.h"

void first_test() {
	printf("1: Objects on the stack are preserved.\n");
	VM* vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	gc(vm);
	freeVM(vm);
}

void second_test() {
	printf("2: Unreached objects are collected.\n");
	VM* vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	pop(vm);
	pop(vm);
	gc(vm);
	freeVM(vm);
}

void third_test() {
	printf("3: Reach the nested objects.\n");
	VM* vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	pushPair(vm);
	pushInt(vm, 3);
	pushInt(vm, 4);
	pushPair(vm);
	pushPair(vm);
	gc(vm);
	freeVM(vm);
}

void another_test() {
	printf("4: Cycles.\n");
	VM* vm = newVM();
	pushInt(vm, 1);
	pushInt(vm, 2);
	Object* a = pushPair(vm);
	pushInt(vm, 3);
	pushInt(vm, 4);
	Object* b = pushPair(vm);
	a->tail = b;
	b->tail = a;
	gc(vm);
	freeVM(vm);
}

void performance() {
	printf("Performance of GC.\n");
	VM* vm = newVM();
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 20; j++) {
			pushInt(vm, i);
		}
		for (int k = 0; k < 20; k++) {
			pop(vm);
		}
	}
	freeVM(vm);
}

int main(int argc, const char** argv) {
	first_test();
	second_test();
	third_test();
	another_test();
	performance();
	return 0;
}
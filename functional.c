#include "functional.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void for_each(void (*func)(void *), array_t list) {
	int i = 0;
	if (list.len > 0) {
		while (i < list.len) {
			func((char *)list.data + i * list.elem_size);
			i++;
		}
	}
}

array_t map(void (*func)(void *, void *),
			int new_list_elem_size,
			void (*new_list_destructor)(void *),
			array_t list) {
	array_t new_list;
	int i;
	char *new_list_data, *list_data, *destuctor_list_data;

	// alocare memorie lista
	new_list.elem_size = new_list_elem_size;
	new_list.len = list.len;
	new_list.destructor = new_list_destructor;
	new_list.data = malloc(new_list_elem_size * list.len);

	// aplicare pe fiecare element
	for (i = 0; i < list.len; i++) {
		new_list_data = (char *)new_list.data + i * new_list_elem_size;
		list_data = (char *)list.data + i * list.elem_size;
		func(new_list_data, list_data);
	}

	if (list.destructor) {
		for (i = 0; i < list.len; i++) {
			destuctor_list_data = (char *)list.data + i * list.elem_size;
			list.destructor(destuctor_list_data);
		}
	}
	free(list.data);

	return new_list;
}

array_t filter(boolean(*func)(void *), array_t list) {
	array_t new_list;
	int i;
	char *list_data, *new_list_data, *destructor_list_data;

	// alocare memorie
	new_list.elem_size = list.elem_size;
	new_list.len = 0;
	new_list.destructor = list.destructor;
	new_list.data = malloc(list.elem_size * list.len);

	// aplicare pe fiecare element
	for (i = 0; i < list.len; i++) {
		list_data = (void *)list.data + i * list.elem_size;
		new_list_data = (void *)new_list.data + new_list.len *
						new_list.elem_size;
		if (func((void *)list.data + i * list.elem_size) == 1) {
			memcpy(new_list_data, list_data, list.elem_size);
			new_list.len++;
		}
	}

	if (list.destructor) {
		for (i = 0; i < list.len; i++) {
			destructor_list_data = (void *)list.data + i * list.elem_size;
			list.destructor(destructor_list_data);
		}
	}
	free(list.data);

	return new_list;
}

void *reduce(void (*func)(void *, void *), void *acc, array_t list) {
	int i = 0;
	if (list.len > 0) {
		while (i < list.len) {
			func(acc, (char *)list.data + i * list.elem_size);
			i++;
		}
	}

	return acc;
}

void for_each_multiple(void(*func)(void **), int varg_c, ...) {
	int min = 1000000, i, j;
	void **elem;
	array_t *list = malloc(varg_c * sizeof(array_t));
	char *list_destructor;

	va_list args;
	va_start(args, varg_c);

	for (i = 0; i < varg_c; i++) {
		list[i] = va_arg(args, array_t);
		if (list[i].len < min)
			min = list[i].len;
	}

	va_end(args);

	for (i = 0; i < min; i++) {
		elem = malloc(varg_c * sizeof(void *));
		for (j = 0; j < varg_c; j++)
			elem[j] = (char *)list[j].data + i * list[j].elem_size;
		func(elem);
		free(elem);
	}

	for (i = 0; i < varg_c; i++) {
		if (list[i].destructor) {
			for (j = 0; j < list[i].len; j++) {
				list_destructor = (char *)list[i].data + j * list[i].elem_size;
				list[i].destructor(list_destructor);
			}
		}
	}
	free(list);
}

array_t map_multiple(void (*func)(void *, void **),
					 int new_list_elem_size,
					 void (*new_list_destructor)(void *),
					 int varg_c, ...) {
	array_t new_list;
	int min = 1000000, i, j;
	void **elem;
	array_t *list = malloc(varg_c * sizeof(array_t));
	char *new_list_data, *list_destructor;

	va_list args;
	va_start(args, varg_c);

	for (i = 0; i < varg_c; i++) {
		list[i] = va_arg(args, array_t);
		if (list[i].len < min)
			min = list[i].len;
	}

	va_end(args);

	new_list.data = malloc(new_list_elem_size * min);
	new_list.elem_size = new_list_elem_size;
	new_list.len = min;
	new_list.destructor = new_list_destructor;

	for (i = 0; i < min; i++) {
		elem = malloc(varg_c * sizeof(void *));
		new_list_data = (char *)new_list.data + i * new_list_elem_size;
		for (j = 0; j < varg_c; j++)
			elem[j] = (char *)list[j].data + i * list[j].elem_size;
		func(new_list_data, elem);
		free(elem);
	}

	for (i = 0; i < varg_c; i++) {
		if (list[i].destructor) {
			for (j = 0; j < list[i].len; j++) {
				list_destructor = (char *)list[i].data + j * list[i].elem_size;
				list[i].destructor(list_destructor);
			}
		}
		free(list[i].data);
	}
	free(list);

	return new_list;
}

void *reduce_multiple(void(*func)(void *, void **), void
*acc, int varg_c, ...) {
	int min = 1000000, i, j;
	void **elem;
	array_t *list = malloc(varg_c * sizeof(array_t));
	char *list_destructor;

	va_list args;
	va_start(args, varg_c);

	for (i = 0; i < varg_c; i++) {
		list[i] = va_arg(args, array_t);
		if (list[i].len < min)
			min = list[i].len;
	}

	va_end(args);

	for (i = 0; i < min; i++) {
		elem = malloc(varg_c * sizeof(void *));
		for (j = 0; j < varg_c; j++)
			elem[j] = (char *)list[j].data + i * list[j].elem_size;
		func(acc, elem);
		free(elem);
	}

	for (i = 0; i < varg_c; i++) {
		if (list[i].destructor) {
			for (j = 0; j < list[i].len; j++) {
				list_destructor = (char *)list[i].data + j * list[i].elem_size;
				list[i].destructor(list_destructor);
			}
		}
	}
	free(list);

	return acc;
}


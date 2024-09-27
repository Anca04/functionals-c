#include "functional.h"
#include "tasks.h"
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void reverse_list(void *acc, void *elem) {
	array_t *list = (array_t *)acc;
	void *new_data = malloc((list->len + 1) * list->elem_size);
	char *new_data_list = (char *)new_data + list->elem_size;
	int *new_elem = (int *)elem;

	memcpy(new_data, new_elem, list->elem_size);

	memcpy(new_data_list, list->data, list->len * list->elem_size);

	free(list->data);
	list->data = new_data;
	list->len++;
}

array_t reverse(array_t list) {
	array_t new_list_rev;

	// list goala
	new_list_rev.len = 0;
	new_list_rev.elem_size = list.elem_size;
	new_list_rev.destructor = list.destructor;
	new_list_rev.data = NULL;

	reduce(reverse_list, &new_list_rev, list);

	return new_list_rev;
}

void create_func(void *new_elem, void **old_array_list) {
	number_t *number = (number_t *)new_elem;

	int *integer = (int *)old_array_list[0];
	int *fractional = (int *)old_array_list[1];

	// alocare memorie pentru string
	number->string = malloc(100);
	// atribuire parte intreaga si fractionala
	number->integer_part = *integer;
	number->fractional_part = *fractional;

	sprintf(number->string, "%d.%d", number->integer_part,
			number->fractional_part);
}

// functie pentru a elibera memoria pentru string-ul din structura number_t
void number_destructor(void *elem) {
	number_t *number = (number_t *)elem;
	free(number->string);
}

array_t create_number_array(array_t integer_part, array_t fractional_part) {
	array_t number_array;

	number_array = map_multiple(create_func, sizeof(number_t),
								number_destructor, 2, integer_part,
								fractional_part);

	return number_array;
}

void map_extract_name(void *new_elem, void *old_elem) {
	student_t *new_student = (student_t *)new_elem;
	student_t *old_student = (student_t *)old_elem;
	// alocare memorie pentru nume
	new_student->name = malloc(strlen(old_student->name) * sizeof(char *));

	// copiere vechiul nume in noul nume
	strcpy(new_student->name, old_student->name);
}

array_t get_passing_students_names(array_t list) {
	array_t map_list;

	map_list = map(map_extract_name, list.elem_size, list.destructor, list);

	return map_list;
}

array_t check_bigger_sum(array_t list_list, array_t int_list) {
	(void)list_list;
	(void)int_list;
	return (array_t){0};
}

array_t get_even_indexed_strings(array_t list) {
	(void)list;
	return (array_t){0};
}

array_t generate_square_matrix(int n) {
	(void)n;
	return (array_t){0};
}

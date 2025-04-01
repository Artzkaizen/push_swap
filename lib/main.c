#include "./push_swap.h"

int g_move_count = 0;




t_stack *get_stack_for_location(t_tabs *stacks, t_chunk_location location) {
    if (location == LOC_TOP_A || location == LOC_BOTTOM_A) return (&stacks->a);
    else return (&stacks->b);
}

int get_chunk_element_value(t_tabs *stacks, t_chunk *chunk, int index_from_start) {
    t_stack *stk = get_stack_for_location(stacks, chunk->location);
    if (!stk || !stk->head || index_from_start <= 0 || index_from_start > chunk->size) return INT_MIN;

    t_node *current;
    int i = 1; // Start counter for while loop
    if (chunk->location == LOC_TOP_A || chunk->location == LOC_TOP_B) {
        current = stk->head;
        while (i < index_from_start) { current = current->next; i++; }
    } else { // LOC_BOTTOM_A or LOC_BOTTOM_B
        current = stk->head->prev;
        while (i < index_from_start) { current = current->prev; i++; }
    }
    return current->value;
}

int get_chunk_max_value(t_tabs *stacks, t_chunk *chunk) {
     t_stack *stk = get_stack_for_location(stacks, chunk->location);
     if (!stk || !stk->head || chunk->size <= 0) return INT_MIN;
     t_node *current; int max_val = INT_MIN; int i = 0;
     if (chunk->location == LOC_TOP_A || chunk->location == LOC_TOP_B) {
        current = stk->head;
        while (i < chunk->size) { if (current->value > max_val) max_val = current->value; if(i < chunk->size -1) current = current->next; i++; }
     } else { current = stk->head->prev;
        while (i < chunk->size) { if (current->value > max_val) max_val = current->value; if(i < chunk->size - 1) current = current->prev; i++; } }
    return max_val;
}

int get_chunk_min_value(t_tabs *stacks, t_chunk *chunk) {
    t_stack *stk = get_stack_for_location(stacks, chunk->location);
    if (!stk || !stk->head || chunk->size <= 0) return INT_MAX;
    t_node *current; int min_val = INT_MAX; int i = 0;
     if (chunk->location == LOC_TOP_A || chunk->location == LOC_TOP_B) {
        current = stk->head;
        while (i < chunk->size) { if (current->value < min_val) min_val = current->value; if(i < chunk->size - 1) current = current->next; i++; }
     } else { current = stk->head->prev;
        while (i < chunk->size) { if (current->value < min_val) min_val = current->value; if(i < chunk->size - 1) current = current->prev; i++; } }
    return min_val;
}

void calculate_three_way_pivots(t_chunk *chunk, int min_val, int max_val, int *pivot1_val, int *pivot2_val) {
    
    (void)chunk; // Not used in this function, but kept for consistency with original)
    int range = max_val - min_val + 1;
    if (range < 3) { *pivot1_val = min_val + range / 3; *pivot2_val = min_val + (2 * range) / 3;
        if (*pivot2_val <= *pivot1_val && range > 1) *pivot2_val = *pivot1_val + 1;
         if (*pivot1_val < min_val) *pivot1_val = min_val; if (*pivot2_val < min_val) *pivot2_val = min_val; }
    else { *pivot1_val = min_val + range / 3; *pivot2_val = min_val + (2 * range) / 3; }
}

void determine_partition_destinations(t_chunk_location s, t_chunk *l, t_chunk *m, t_chunk *h) {
    if (s == LOC_TOP_A) { l->location = LOC_BOTTOM_B; m->location = LOC_TOP_B; h->location = LOC_TOP_A; }
    else if (s == LOC_BOTTOM_A) { l->location = LOC_BOTTOM_B; m->location = LOC_TOP_B; h->location = LOC_TOP_A; }
    else if (s == LOC_TOP_B) { l->location = LOC_BOTTOM_B; m->location = LOC_BOTTOM_A; h->location = LOC_TOP_A; }
    else if (s == LOC_BOTTOM_B) { l->location = LOC_TOP_B; m->location = LOC_BOTTOM_A; h->location = LOC_TOP_A; }
}

void initialize_partition_sizes(t_chunk *low, t_chunk *mid, t_chunk *high) { low->size = 0; mid->size = 0; high->size = 0; }

int move_element(t_tabs *stacks, t_chunk_location from, t_chunk_location to) {
    if (from == LOC_TOP_A) { if (to == LOC_TOP_B) move_and_print(stacks, PB); else if (to == LOC_BOTTOM_A) move_and_print(stacks, RA); else if (to == LOC_BOTTOM_B) { move_and_print(stacks, PB); move_and_print(stacks, RB); } else if (to == LOC_TOP_A) move_and_print(stacks, RA); else return 0; }
    else if (from == LOC_TOP_B) { if (to == LOC_TOP_A) move_and_print(stacks, PA); else if (to == LOC_BOTTOM_B) move_and_print(stacks, RB); else if (to == LOC_BOTTOM_A) { move_and_print(stacks, PA); move_and_print(stacks, RA); } else if (to == LOC_TOP_B) move_and_print(stacks, RB); else return 0; }
    else if (from == LOC_BOTTOM_A) { if (to == LOC_TOP_A) move_and_print(stacks, RRA); else if (to == LOC_TOP_B) { move_and_print(stacks, RRA); move_and_print(stacks, PB); } else if (to == LOC_BOTTOM_B) { move_and_print(stacks, RRA); move_and_print(stacks, PB); move_and_print(stacks, RB); } else if (to == LOC_BOTTOM_A) move_and_print(stacks, RRA); else return 0; }
    else if (from == LOC_BOTTOM_B) { if (to == LOC_TOP_B) move_and_print(stacks, RRB); else if (to == LOC_TOP_A) { move_and_print(stacks, RRB); move_and_print(stacks, PA); } else if (to == LOC_BOTTOM_A) { move_and_print(stacks, RRB); move_and_print(stacks, PA); move_and_print(stacks, RA); } else if (to == LOC_BOTTOM_B) move_and_print(stacks, RRB); else return 0; }
    else return 0;
    return 1;
}

/* --- Optimization Helper Implementations --- */
void sort_int_triple(int *a, int *b, int *c) { int t; if(*a>*b){t=*a;*a=*b;*b=t;} if(*a>*c){t=*a;*a=*c;*c=t;} if(*b>*c){t=*b;*b=*c;*c=t;} }
bool are_values_consecutive(int a, int b, int c, int d) { sort_int_triple(&a, &b, &c); return ((b - a == 1) && (c - b == 1) && (d - c == 1)); }

bool is_stack_a_sorted_from_node(t_tabs *stacks, int start_index) {
    t_stack *a = &stacks->a;
    if (!a->head || start_index <= 0 || start_index > a->size) return TRUE;
    if (a->size < start_index + 1) return TRUE;

    t_node *start_node = a->head;
    int i = 1; // Counter for while loop
    while (i < start_index) { start_node = start_node->next; i++; }

    t_node *current = start_node;
    int expected_value = current->value;
    while(current->next != a->head) {
         expected_value++;
         if(current->next->value != expected_value) return FALSE;
         current = current->next;
    }
    return TRUE;
}

void optimize_high_chunk_placement(t_tabs *stacks, t_chunk *high_chunk_dest) {
    t_stack *a = &stacks->a;
    if (high_chunk_dest->location != LOC_TOP_A || a->size < 3) return;

    // Condition 1 simplified check
    int v0 = a->head->value; int v1 = a->head->next->value; int v2 = a->head->next->next->value;
    int total_size = stacks->a.size + stacks->b.size;
    bool top_three_max = (v0 >= total_size - 3 && v1 >= total_size - 3 && v2 >= total_size - 3);
    if (high_chunk_dest->size >= 3 && top_three_max && is_stack_a_sorted_from_node(stacks, 4)) {
        t_chunk temp_chunk = {LOC_TOP_A, 3}; sort_chunk_three(stacks, &temp_chunk); high_chunk_dest->size -= 3; return;
    }
    // Condition 2 check
    if (high_chunk_dest->size >= 1 && a->size >= 3) {
         if (a->head->next->value == a->head->next->next->value - 1 && a->head->value > a->head->next->value && is_stack_a_sorted_from_node(stacks, 3)) {
             move_and_print(stacks, SA); high_chunk_dest->size--; return;
         } }
    // Condition 3 check
    if (high_chunk_dest->size >= 1 && a->size >= 1) {
         if (is_stack_a_sorted_from_node(stacks, 2)) {
             if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1)) {
                 high_chunk_dest->size--; return;
             } } }
}

/* --- Main Sorting Logic Implementation (Refactored Loops) --- */


void partition_chunk(t_tabs *stacks, t_chunk *chunk_to_split, t_partition_dest *destinations) {
    int pivot1_val, pivot2_val;
    int min_val = get_chunk_min_value(stacks, chunk_to_split);
    int max_val = get_chunk_max_value(stacks, chunk_to_split);
    int original_size = chunk_to_split->size;
    calculate_three_way_pivots(chunk_to_split, min_val, max_val, &pivot1_val, &pivot2_val);

    int i = 0; // Counter for while loop
    while (i < original_size) {
        int current_value = get_chunk_element_value(stacks, chunk_to_split, 1);
        if (current_value >= pivot2_val) {
            destinations->high_chunk.size += move_element(stacks, chunk_to_split->location, destinations->high_chunk.location);
            optimize_high_chunk_placement(stacks, &destinations->high_chunk);
        } else if (current_value >= pivot1_val) {
            destinations->mid_chunk.size += move_element(stacks, chunk_to_split->location, destinations->mid_chunk.location);
        } else {
            destinations->low_chunk.size += move_element(stacks, chunk_to_split->location, destinations->low_chunk.location);
        }
        i++;
    }
}

void sort_chunk_recursive(t_tabs *stacks, t_chunk *chunk_to_sort) {
    t_partition_dest destinations;
    if (chunk_to_sort->size <= 3) {
        if (chunk_to_sort->size == 3) sort_chunk_three(stacks, chunk_to_sort);
        else if (chunk_to_sort->size == 2) sort_chunk_two(stacks, chunk_to_sort);
        else if (chunk_to_sort->size == 1) sort_chunk_one(stacks, chunk_to_sort);
        return;
    }
    initialize_partition_sizes(&destinations.low_chunk, &destinations.mid_chunk, &destinations.high_chunk);
    determine_partition_destinations(chunk_to_sort->location, &destinations.low_chunk, &destinations.mid_chunk, &destinations.high_chunk);
    partition_chunk(stacks, chunk_to_sort, &destinations);
    sort_chunk_recursive(stacks, &destinations.high_chunk);
    sort_chunk_recursive(stacks, &destinations.mid_chunk);
    sort_chunk_recursive(stacks, &destinations.low_chunk);
}



/* --- Main --- */
int main(int argc, char **argv) {
    t_tabs *tabs;

    if (argc < 2)
		return (0);
	if (argc == 2)
		tabs= create_tabs(ft_split(argv[1], ' '));
	else
		tabs = create_tabs(argv + 1);
    if (!tabs) return 1;
    g_move_count = 0;
    push_swap(tabs);
    free_tabs(tabs);
    return 0;
}
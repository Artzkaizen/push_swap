#include "push_swap.h"

void rrr(t_tabs *tabs)
{
    if (!tabs) return;
    rra(tabs);
    rrb(tabs);
}

void move_stack(t_tabs *tabs, t_stack_move move)
{
    switch (move)
    {
    case SA:  sa(tabs);  break;
    case SB:  sb(tabs);  break;
    case SS:  ss(tabs);  break;
    case PA:  pa(tabs);  break;
    case PB:  pb(tabs);  break;
    case RA:  ra(tabs);  break;
    case RB:  rb(tabs);  break;
    case RR:  rr(tabs);  break;
    case RRA: rra(tabs); break;
    case RRB: rrb(tabs); break;
    case RRR: rrr(tabs); break;
    }
}

void move_and_print(t_tabs *tabs, t_stack_move move)
{
    if (move == SA)       printf("sa\n");
    else if (move == SB)  printf("sb\n");
    else if (move == SS)  printf("ss\n");
    else if (move == PA)  printf("pa\n");
    else if (move == PB)  printf("pb\n");
    else if (move == RA)  printf("ra\n");
    else if (move == RB)  printf("rb\n");
    else if (move == RR)  printf("rr\n");
    else if (move == RRA) printf("rra\n");
    else if (move == RRB) printf("rrb\n");
    else if (move == RRR) printf("rrr\n");
    move_stack(tabs, move);
    g_move_count++;
}

int move_from_to(t_tabs *data, t_loc from, t_loc to)
{
    if (from == TOP_A) {
        if (to == TOP_B) { move_and_print(data, PB); }
        else if (to == BOTTOM_A) { move_and_print(data, RA); }
        else if (to == BOTTOM_B) { move_and_print(data, PB); move_and_print(data, RB); }
        else if (to == TOP_A) { move_and_print(data, RA); }
        else return (0);
    } else if (from == TOP_B) {
        if (to == TOP_A) { move_and_print(data, PA); }
        else if (to == BOTTOM_B) { move_and_print(data, RB); }
        else if (to == BOTTOM_A) { move_and_print(data, PA); move_and_print(data, RA); }
        else if (to == TOP_B) { move_and_print(data, RB); }
        else return (0);
    } else if (from == BOTTOM_A) {
        if (to == TOP_B) { move_and_print(data, RRA); move_and_print(data, PB); }
        else if (to == BOTTOM_B) { move_and_print(data, RRA); move_and_print(data, PB); move_and_print(data, RB); }
        else if (to == BOTTOM_A || to == TOP_A) { move_and_print(data, RRA); }
        else return (0);
    } else if (from == BOTTOM_B) {
         if (to == TOP_A) { move_and_print(data, RRB); move_and_print(data, PA); }
         else if (to == BOTTOM_A) { move_and_print(data, RRB); move_and_print(data, PA); move_and_print(data, RA); }
         else if (to == TOP_B || to == BOTTOM_B) { move_and_print(data, RRB); }
         else return (0);
    } else
        return (0);
    return (1);
}
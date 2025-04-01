#include "push_swap.h"

/**
 * @brief Initializes the variable structure for check_reduce_cond1.
 * @param data Pointer to t_tabs.
 * @param vars Pointer to the variable structure to initialize.
 */
void init_reduce_cond1_vars(t_tabs *data, t_reduce_cond1_vars *vars)
{
    vars->v0 = data->a.head->value;
    vars->v1 = data->a.head->next->value;
    vars->v2 = data->a.head->next->next->value;
    vars->total_size = data->a.size + data->b.size;
    vars->n_m1 = vars->total_size - 1;
    vars->n_m2 = vars->total_size - 2;
    vars->n_m3 = vars->total_size - 3;
    vars->is_top_3_max = FALSE; // Initialize flag
}

/**
 * @brief Calculates the is_top_3_max flag within the variable structure.
 * @param vars Pointer to the variable structure.
 */
void calculate_top_3_max(t_reduce_cond1_vars *vars)
{
    // Check if v0, v1, v2 are precisely n-1, n-2, n-3 in some order
    vars->is_top_3_max = ((vars->v0 == vars->n_m1 || vars->v0 == vars->n_m2 || vars->v0 == vars->n_m3) && (vars->v1 == vars->n_m1 || vars->v1 == vars->n_m2 || vars->v1 == vars->n_m3) && (vars->v2 == vars->n_m1 || vars->v2 == vars->n_m2 || vars->v2 == vars->n_m3) && (vars->v0 != vars->v1 && vars->v0 != vars->v2 && vars->v1 != vars->v2)); // Ensure distinct
}

/**
 * @brief Helper for split_max_reduction: Checks Condition 1.
 * @param data Pointer to the t_tabs structure.
 * @return t_bool TRUE if condition met, FALSE otherwise.
 */
t_bool check_reduce_cond1(t_tabs *data)
{
    t_reduce_cond1_vars vars; // Only 1 variable + param

    if (data->a.size < 3) // Pre-check to avoid accessing invalid memory
        return (FALSE);
    init_reduce_cond1_vars(data, &vars);
    calculate_top_3_max(&vars);
    if (vars.is_top_3_max == TRUE && a_partly_sort(data, 4) == TRUE)
        return (TRUE);
    return (FALSE);
}

/**
 * @brief Helper for split_max_reduction: Checks Condition 2.
 * @param data Pointer to the t_tabs structure.
 * @return t_bool TRUE if condition met, FALSE otherwise.
 */
t_bool check_reduce_cond2(t_tabs *data)
{
    t_stack *a; // Only 1 variable + param

    a = &data->a;
    if (a->size < 3)
        return (FALSE);
    if (a->head->next->value == a->head->next->next->value - 1 && a->head->value > a->head->next->value && a_partly_sort(data, 3) == TRUE)
    {
        return (TRUE);
    }
    return (FALSE);
}

/**
 * @brief Helper for split_max_reduction: Checks Condition 3.
 * @param data Pointer to the t_tabs structure.
 * @return t_bool TRUE if condition met, FALSE otherwise.
 */
t_bool check_reduce_cond3(t_tabs *data)
{
    t_stack *a; // Only 1 variable + param

    a = &data->a;
    if (a->size < 1)
        return (FALSE);
    if (a_partly_sort(data, 2) == TRUE)
    {
        if (a->size == 1 || (a->size > 1 && a->head->value == a->head->next->value - 1))
        {
            return (TRUE);
        }
    }
    return (FALSE);
}
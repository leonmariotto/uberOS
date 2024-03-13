#ifndef TASKS_H
# define TASKS_H

/*
** This function enable a thread in threads static arrays
** argument and attr are ignored for now TODO
*/
int osTaskNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr);

#endif

#ifndef _H_TLIBC_LIST
#define _H_TLIBC_LIST

typedef struct _TLIBC_LIST_HEAD
{
	struct _TLIBC_LIST_HEAD *next, *prev;
}TLIBC_LIST_HEAD;

#define tlibc_list_init(_head)\
{\
	(_head)->next = _head;\
	(_head)->prev = _head;\
}

#define __tlibc_list_add(_new, _prev, _next)\
{\
	(_new)->next = (_next);\
	(_new)->prev = (_prev);\
	(_new)->next->prev = (_new);\
	(_new)->prev->next = (_new);\
}

#define tlibc_list_add(_new, _head)\
{\
	__tlibc_list_add((_new), (_head), ((_head)->next));\
}

#define tlibc_list_add_tail(_new, _head)\
{\
	__tlibc_list_add((_new), (_head)->prev, (_head));\
}

#define __tlibc_list_del(_prev, _next)\
{\
	(_next)->prev = _prev;\
	(_next)->prev->next = (_next);\
}

#define tlibc_list_del(entry)\
{\
	__tlibc_list_del((entry)->prev, (entry)->next);\
}

#define tlibc_list_fix(_prev, _next)\
{\
	(_prev)->next = _next;\
	(_prev)->next->prev = _prev;\
}

#endif

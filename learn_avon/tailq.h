//
//  tailq.h
//  learn_avon
//
//  Created by wangwang on 2019/7/13.
//  Copyright © 2019 wangwang. All rights reserved.
//

#ifndef tailq_h
#define tailq_h

// 定义队列的头结点结构体的宏
#define TAILQ_HEAD(name, type) \
struct name {   \
    struct type *tqh_first; /* first element */ /* 指向队列的第一个元素的地址 */    \
    struct type **tqh_last; /* addr of last next element */ /* 指向队列最后一个元素的tqe_next的地址 */  \
}

#define TAILQ_HEAD_INITIALIZER(head)    \
{   NULL,   &(head).tqh_first   }

// 这个结构体是队列中元素的一个字段的类型，类型为下面这个匿名结构体，该结构体保存着2个指针
#define TAILQ_ENTRY(type)   \
struct {    \
    struct type *tqe_next;  /* next element */  /* 指向下一个元素的地址 */   \
    struct type **tqe_prev; /* address of previous next element */  /* 指向上一个元素的tqe_next的地址 */   \
}

/*
 * tail queue access methods
 */

#define TAILQ_FIRST(head)   ((head)->tqh_first)

#define TAILQ_END(head) NULL

#define TAILQ_NEXT(elm, field)  ((elm)->field.tqe_next) // 获取队列的下一个元素的指针

// (head)->tqh_last，表示队列中最后一个元素的tqe_next的地址，也即是TAILQ_ENTRY(type)的地址
// (struct headname *)((head)->tqh_last)，表示把TAILQ_ENTRY(type)的地址转化成TAILQ_HEAD(name, type)的地址，因为它们在内存上空间布局相同，可以互转
// (((struct headname *)((head)->tqh_last))->tqh_last)，经过上一步的类型转换，tqh_last相当于TAILQ_ENTRY(type)的tqe_prev，表示队列中倒数第二个元素的tqe_next的地址
// (*(((struct headname *)((head)->tqh_last))->tqh_last))，表示指向最后一个元素的指针，类型为struct type *
#define TAILQ_LAST(head, headname)  \
(*(((struct headname *)((head)->tqh_last))->tqh_last))

#define TAILQ_PREV(elm, headname, field)    \
(*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))

#define TAILQ_EMPTY(head)   \
(TAILQ_FIRST(head) == TAILQ_END(head))

// 遍历队列的宏
#define TAILQ_FOREACH(var, head, field) \
for((var) = TAILQ_FIRST(head);  \
(var) != TAILQ_END(head);   \
(var) = TAILQ_NEXT(var, field))

#define TAILQ_FOREACH_REVERSE(var, head, headname, field)   \
for((var) = TAILQ_LAST(head, headname); \
(var) != TAILQ_END(head);   \
(var) = TAILQ_PREV(var, headname, field))

/*
 * Tail queue functions.
 */

#define TAILQ_INIT(head)    do {    \
    (head)->tqh_first = NULL;   \
    (head)->tqh_last = &(head)->tqh_first;  \
} while (0)

// 在队列头插入元素elm
#define TAILQ_INSERT_HEAD(head, elm, field) do {    \
    if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)    \
        (head)->tqh_first->field.tqe_prev = &(elm)->field.tqe_next; \
    else    \
        (head)->tqh_last = &(elm)->field.tqe_next;  \
    (head)->tqh_first = (elm);  \
    (elm)->field.tqe_prev = &(head)->tqh_first; \
} while (0)

// 在队列head的尾部插入元素elm
#define TAILQ_INSERT_TAIL(head, elm, field) do {    \
    (elm)->field.tqe_next = NULL;   \
    (elm)->field.tqe_prev = (head)->tqh_last;   \
    *(head)->tqh_last = (elm);  /* 运算符优先级->高于* */   \
    (head)->tqh_last = &(elm)->field.tqe_next;  \
} while (0)

// 在队列head的listelm的后面插入元素elm
#define TAILQ_INSERT_AFTER(head, listelm, elm, field)   do {    \
    if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != NULL)    \
        (elm)->field.tqe_next->field.tqe_prev = &(elm)->field.tqe_next; \
    else    \
        (head)->tqh_last = &(elm)->field.tqe_next;  \
    (listelm)->field.tqe_next = (elm);  \
    (elm)->field.tqe_prev = &(listelm)->field.tqe_next; \
} while (0)

// 在元素listelm前面插入elm
#define TAILQ_INSERT_BEFORE(listelm, elm, field)    do {    \
    (elm)->field.tqe_prev = (listelm)->field.tqe_prev;  \
    (elm)->field.tqe_next = (listelm);  \
    *(listelm)->field.tqe_prev = (elm);  /* 运算符优先级.高于->高于* */   \
    (listelm)->field.tqe_prev = &(elm)->field.tqe_next; \
} while (0)

// 在队列head中删除元素elm
// 注意：需要在之后释放elm
#define TAILQ_REMOVE(head, elm, field)  do {    \
    if (((elm)->field.tqe_next) != NULL)    \
        (elm)->field.tqe_next->field.tqe_prev = (elm)->field.tqe_prev;  \
    else    \
        (head)->tqh_last = (elm)->field.tqe_prev;   \
    *(elm)->field.tqe_prev = (elm)->field.tqe_next;  /* 运算符优先级.高于->高于* */   \
} while (0)

// 在队列head中用elm2替换elm
// 注意：需要在之后释放elm
#define TAILQ_REPLACE(head, elm, elm2, field)   do {    \
    if (((elm2)->field.tqe_next = (elm)->field.tqe_next) != NULL)   \
        (elm2)->field.tqe_next->field.tqe_prev = &(elm2)->field.tqe_next;   \
    else    \
        (head)->tqh_last = &(elm2)->field.tqe_next; \
    (elm2)->field.tqe_prev = (elm)->field.tqe_prev; \
    *(elm2)->field.tqe_prev = (elm2);   \
} while (0)

#endif /* tailq_h */

//
//  main.c
//  learn_avon
//
//  Created by wangwang on 2019/7/13.
//  Copyright © 2019 wangwang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "tailq.h"

struct item {
    int value;
    TAILQ_ENTRY(item) entries;
};

TAILQ_HEAD(queue_item, item) queue;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    struct item *elem, *elem2, *elem3;
    TAILQ_INIT(&queue);
    
    if (TAILQ_EMPTY(&queue)) {
        printf("The queue is null now\n");
    }
    
    // 插入元素0 1 2 3 4 5 6 7 8 9
    for (int i = 0; i < 10; ++i) {
        elem = (struct item*)malloc(sizeof(struct item));
        elem->value = i;
        TAILQ_INSERT_TAIL(&queue, elem, entries);
    }
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    // 0 1 23 4 5 6 7 8 100 9
    elem2 = (struct item*)malloc(sizeof(struct item));
    elem2->value = 100;
    TAILQ_INSERT_BEFORE(elem, elem2, entries);
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    // 200 0 1 2 3 4 5 6 7 8 100 9
    elem2 = (struct item*)malloc(sizeof(struct item));
    elem2->value = 200;
    TAILQ_INSERT_HEAD(&queue, elem2, entries);
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    // 200 2000 0 1 2 3 4 5 6 7 8 100 9
    elem = elem2;
    elem2 = (struct item*)malloc(sizeof(struct item));
    elem2->value = 2000;
    TAILQ_INSERT_AFTER(&queue, elem, elem2, entries);
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    elem = elem2->entries.tqe_next;
    elem2 = elem->entries.tqe_next;
    // 200 2000 1 2 3 4 5 6 7 8 100 9
    TAILQ_REMOVE(&queue, elem, entries);
    free(elem);
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    elem = (struct item*)malloc(sizeof(struct item));
    elem->value = 11;
    // 200 2000 11 2 3 4 5 6 7 8 100 9
    TAILQ_REPLACE(&queue, elem2, elem, entries);
    free(elem2);
    TAILQ_FOREACH(elem3, &queue, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    elem = TAILQ_FIRST(&queue);
    printf("The first element is %d \n", elem->value);
    elem = TAILQ_NEXT(elem, entries);
    printf("The second element is %d \n", elem->value);
    elem = TAILQ_LAST(&queue, queue_item);
    printf("The last element is %d \n", elem->value);
    elem = TAILQ_PREV(elem, queue_item, entries);
    printf("The second last element is %d \n", elem->value);
    
    TAILQ_FOREACH_REVERSE(elem3, &queue, queue_item, entries) {
        printf("%d ", elem3->value);
    }
    printf("\n");
    
    printf("Hello, World!\n");
    return 0;
}

#ifndef CHRISTIAN_LICHTSCHRANKE_H
#define CHRISTIAN_LICHTSCHRANKE_H

void IRAM_ATTR gpio_isr_handler(void* arg);
void gpio_task_example(void* arg);
void app_lichtschranke();

#endif

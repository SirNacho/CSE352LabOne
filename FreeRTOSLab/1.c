void task_one(void *pvParameters) {
    // This loop runs FOREVER — tasks never return!
    for(;;) {
        printf("Task 1 Running!\n");
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void task_two(void *pvParameters) {
    for(;;) {
        printf("Task 2 Running!\n");  // Print to serial terminal
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {

    // Create tasks: (function, name, stack, params, priority, handle)
    xTaskCreate(task_one,   "Task 1",   256, NULL, 1, NULL);  // Priority 1
    xTaskCreate(task_two, "Task Two", 256, NULL, 1, NULL);  // Priority 1 (same)

    vTaskStartScheduler();  // Start FreeRTOS! (this line NEVER returns)
    return 0;  // We never get here, but the compiler likes it
}

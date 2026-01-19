// Virtual timer implementation

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrf.h"

#include "virtual_timer.h"
#include "virtual_timer_linked_list.h"

// This is the interrupt handler that fires on a compare event
void TIMER4_IRQHandler(void) {

  //Disabling interrupts temporarily.
  __disable_irq();

  // This should always be the first line of the interrupt handler!
  // It clears the event so that it doesn't happen again
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;

  // Place your interrupt handler code here
  
  // Update CC[0] register from the remaining timer values
  checkTimers();
}

void checkTimers(){
	// update CC[0] value by looking at the linked list
	// node after the current one, and update CC[0] using
	// the timer_value from this node

  /*
  1. Remove the first node and place at the end of the linked list after adding the current timer value. 
  Use read_timer() to get the current timer value. Use the linked list function that both inserts the 
  node and also sorts the list based on the timer value.
  2. Now get the first node of the updated list. This will be the next timer value to trigger the new COMPARE EVENT at.
  3. Check if the timer vallue in this node is already reached. 
  In that case, call the callback function. The callback function can be called using the following command: timer_node->cbFunc();
  */
  

  node_t* list_head = list_get_first();

  if (list_head == NULL) 
  {
    __enable_irq();
    return;
  }

  node_t* expired = list_head;
  
  //execute callback function.
  if (expired->cbFunc != NULL)
  {
    (*(expired->cbFunc))(); //<---call the callback.
  }

  if (expired->period != 0) 
  {
    expired->timer_value = read_timer() + expired->period;
    list_remove(expired);
    list_insert_sorted(expired);
    node_t* new_head = list_get_first();
    if (new_head != NULL)
    {
    NRF_TIMER4->CC[0] = new_head->timer_value;
    }
  }
  else
  {
    list_remove(expired); //<---NOTE: Don't forget to remove the expired node before freeing it or the mc will crash and yell at you.
    free(expired);
    node_t* new_head = list_get_first();
    if (new_head != NULL) 
    {
      NRF_TIMER4->CC[0] = new_head->timer_value;
    }
  }

  __enable_irq();
}


// Read the current value of the timer counter
uint32_t read_timer(void) {
  // Same function as the regular timers lab from CSE 351
  // Should return the value of the internal counter for TIMER4
  
  NRF_TIMER4->TASKS_CAPTURE[3] = 0x01;
  return NRF_TIMER4->CC[3];
  //return 0;
}

// Initialize TIMER4 as a free running timer
// 1) Set to be a 32 bit timer
// 2) Set to count at 1MHz
// 3) Enable the timer peripheral interrupt (look carefully at the INTENSET register!)
// 4) Clear the timer
// 5) Start the timer
void virtual_timer_init(void) {
  // Place your timer initialization code here
  
  //Stopping and clearing timer4.
  NRF_TIMER4->TASKS_STOP = 0x01;
  NRF_TIMER4->TASKS_CLEAR = 0x01;

  //Setting up the timers values.
  NRF_TIMER4->PRESCALER = 4; //<---- This should set the count to be at 1mhz
  NRF_TIMER4->BITMODE = 3;
  NRF_TIMER4->MODE = 0;
  NRF_TIMER4->INTENSET = 1 << 16; // <---Set this intenset to COMPARE[0] interrupt by setting bit 16.
  
  //Setting up interrupt and priority.
  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 0);
  
  //Clearing and start timer4.
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  
}

// Start a timer. This function is called for both one-shot and repeated timers
// To start a timer:
// 1) Create a linked list node (This requires `malloc()`. Don't forget to free later)
// 2) Setup the linked list node with the correct information
//      - You will need to modify the `node_t` struct in "virtual_timer_linked_list.h"!
// 3) Place the node in the linked list
// 4) Setup the compare register so that the timer fires at the right time
// 5) Return a timer ID
//
// Your implementation will also have to take special precautions to make sure that
//  - You do not miss any timers
//  - You do not cause consistency issues in the linked list (hint: you may need the `__disable_irq()` and `__enable_irq()` functions).
//
// Follow the lab manual and start with simple cases first, building complexity and
// testing it over time.

//This function was created to check if the id's are all good.
void timer_id_check(void) 
{
  return 0;
}

static uint32_t timer_start(uint32_t microseconds, virtual_timer_callback_t cb, bool repeated) {
  
  //Sets up new node.

  node_t* new_node = (node_t*)malloc(sizeof(node_t));

  new_node->timer_value = read_timer() + microseconds;
  new_node->cbFunc = cb;

  //Checks if the timer is periodic
  if (repeated)
  {
    new_node->period = microseconds;
  }
  else
  {
    new_node->period = 0;
  }
  //new_node->period = 2000000;
  new_node->next = NULL;

  //insert new node to the list.
  list_insert_sorted(new_node);
  
  //Checks if this is the first node, then update CC[0].

  node_t* list_head = list_get_first();
  if (list_head == new_node)
  {
    NRF_TIMER4->CC[0] = new_node->timer_value;
  }
  
  printf("node id: %d\n", new_node->id);

  //Test to see if I can setup three nodes as a test.
  /*
  //setting up the nodes.
  node_t* node_One = (node_t*)malloc(sizeof(node_t));
  node_One->timer_value = 2000000;
  node_One->id = 1;
  node_One->cbFunc = NULL;
  node_One->period = 2000000;

  node_t* node_Two = (node_t*)malloc(sizeof(node_t));
  node_Two->timer_value = 5000000;
  node_Two->id = 2;
  node_Two->cbFunc = NULL;
  node_Two->period = 2000000;

  node_t* node_Three = (node_t*)malloc(sizeof(node_t));
  node_Three->timer_value = 10000000;
  node_Three->id = 3;
  node_Three->cbFunc = NULL;
  node_Three->period = 2000000;

  //assigning what's next to each node.
  node_One->next = node_Two;
  node_Two->next = node_Three;
  node_Three->next = NULL;
  node_t* list_head = list_get_first();

  //Inserting the nodes to the list.
  list_insert_sorted(node_One);
  list_insert_sorted(node_Two);
  list_insert_sorted(node_Three);
  */

  //Printing the linked list nodes.
  //printf("Node One id: %d\nNode Two id: %d\nNode Three id: %d\n", node_One->id, node_Two->id, node_Three->id);
  //list_print();
  //printf("The first node in the linked list is: Node %d.\n\n", list_head->id);
  return new_node->id;
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start(uint32_t microseconds, virtual_timer_callback_t cb) {
  return timer_start(microseconds, cb, false);
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start_repeated(uint32_t microseconds, virtual_timer_callback_t cb) {
  return timer_start(microseconds, cb, true);
}

// Remove a timer by ID.
// Make sure you don't cause linked list consistency issues!
// Do not forget to free removed timers.
void virtual_timer_cancel(uint32_t timer_id) {
}


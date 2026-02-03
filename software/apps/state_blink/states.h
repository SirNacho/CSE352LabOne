typedef enum {
	STATE_INIT,
	STATE_PRESSED,
	STATE_RELEASED
} led_state_t;

int main() {
	led_state_t state = STATE_INIT; 
	while (;;) {
		switch(state) {
		case STATE_INIT:
			current_state = STATE_IDLE;
            break;		
       	case STATE_PRESSED:
       		break;
        }
	}
	return 0;
}
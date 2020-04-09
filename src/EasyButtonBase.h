#ifndef _EasyButtonBase_h
#define _EasyButtonBase_h
#endif

#include <Arduino.h>
#include "Sequence.h"

#ifdef ESP8266
#define EASYBUTTON_FUNCTIONAL_SUPPORT 1
#endif

#ifdef EASYBUTTON_FUNCTIONAL_SUPPORT
#include <functional>
#include "FunctionalInterrupt.h"
#endif

#define MAX_SEQUENCES 5


class EasyButtonBase
{    
public:
    //Common functions
    #ifdef EASYBUTTON_FUNCTIONAL_SUPPORT
        typedef std::function<void()> callback_t;
    #else
        typedef void (*callback_t)();
    #endif
    EasyButtonBase(bool active_low):_active_low(active_low){}
    virtual void begin() = 0;													// Initialize a button object and the pin it's connected to.
    virtual bool read() = 0;                                        			// Returns the current debounced button state, true for pressed, false for released.
    void onPressed(callback_t callback);										// Call a callback function when the button has been pressed and released.
    void onPressedFor(uint32_t duration, callback_t callback);					// Call a callback function when the button has been held for at least the given number of milliseconds.
    void onSequence(uint8_t sequences, uint32_t duration, callback_t callback); // Call a callback function when the given sequence has matched.

protected:
    //Common variables
    Sequence _sequences[MAX_SEQUENCES];
	uint16_t _sequences_count;
    uint32_t _held_threshold;	                                                // Held threshold.

	callback_t _pressed_sequence_callbacks[MAX_SEQUENCES];
    callback_t _pressed_callback;	                                            // Callback function for pressed events.
	callback_t _pressed_for_callback;                                           // Callback function for pressedFor events.
    bool _held_callback_called;                                                 // Indicate if button long press has been notified.
    bool _active_low;				                                            // Inverts button logic. If true, low = pressed else high = pressed.
    bool _current_state;		                                                // Current button state, true = pressed.
	bool _last_state;			                                                // Previous button state, true = pressed.
	bool _changed;				                                                // Has the state change since last read.
	uint32_t _time;				                                                // Time of current state.
	uint32_t _last_change;		                                                // Time of last state change.
    bool _was_btn_held;			                                                // Indicate if button was held.
};
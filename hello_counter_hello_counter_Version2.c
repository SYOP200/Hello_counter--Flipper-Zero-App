/*
 hello_counter.c
 A tiny Flipper Zero app that displays a counter and updates on button presses.

 Place this folder under flipperzero-firmware/applications_user/hello_counter
 and follow the README to build.

 Notes:
 - The Flipper/Furi API names vary across SDK versions. If build errors occur,
   consult your firmware version's examples and adjust include paths / function names.
*/

#include <furi.h>
#include <gui/gui.h>
#include <gui/view_port.h>
#include <input/input.h>
#include <stdio.h>

static int32_t counter = 0;
static ViewPort* view_port = NULL;

// Draw callback: paint the counter and controls hint
static void draw_callback(Canvas* canvas, void* context) {
    // Clear background (canvas is cleared by framework usually)
    // Print title
    canvas_set_font(canvas, FontPrimary);
    char buf[64];
    snprintf(buf, sizeof(buf), "Hello Counter");
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, buf);

    // Print counter
    snprintf(buf, sizeof(buf), "%d", counter);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, buf);

    // Print controls
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 56, "A: +   B: -   Back: Exit");
}

// Input callback: update the counter or exit
static void input_callback(InputEvent* event, void* ctx) {
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyOk) { // A button / OK
            counter++;
        } else if(event->key == InputKeyBack) { // Back button (B/back)
            // Treat Back short as decrement (some devices map B differently)
            counter--;
        } else if(event->key == InputKeyLeft) {
            // Optional mapping: left to decrement
            counter--;
        } else if(event->key == InputKeyRight) {
            // Optional mapping: right to increment
            counter++;
        }
        // Trigger a redraw by invalidating the view_port from the GUI thread
        if(view_port) {
            view_port_update(view_port);
        }
    } else if(event->type == InputTypeLong && event->key == InputKeyBack) {
        // Long press Back -> exit app
        // The framework's app entry loop will check for this via message queue;
        // we instead set a custom flag by posting to the application queue.
        // For simplicity we use furi_porinter to stop by deleting the viewport
        // and returning from main. Actual exit is handled in the main loop below.
        // We post an event to the running thread by using furi_message_queue — but
        // to keep this example short we will set counter to a sentinel value and
        // the main loop will detect it. (See main below.)
        counter = INT32_MIN; // sentinel -> exit
    }
}

int32_t hello_counter_app(void* p) {
    UNUSED(p);

    // Allocate and setup viewport
    view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, NULL);

    // Add viewport to GUI
    Gui* gui = furi_record_get(FURI_RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // Main loop: wait for exit sentinel
    while(1) {
        // A tiny delay to avoid busy-looping; the GUI/input system calls callbacks
        // on events. We poll the counter sentinel here.
        furi_delay_ms(50);

        if(counter == INT32_MIN) break;
    }

    // Cleanup
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    view_port = NULL;

    return 0;
}
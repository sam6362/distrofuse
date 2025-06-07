#ifndef UI_H
#define UI_H

// Main UI functions
void draw_ui();
void ui_set_package_status(const char* package, const char* manager);
void ui_update_status(const char* status);

// Progress functions
void ui_update_download_progress();
void ui_update_install_progress();
void ui_update_remove_progress();

// Utility functions
void ui_show_error(const char* error_msg);
void ui_show_success(const char* message);
void ui_finish();

// Terminal control
void clean_terminal();
void get_terminal_size();
void hide_cursor();
void show_cursor();

#endif
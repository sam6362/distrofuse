#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include "ui.h"

// Global state
static int terminal_width = 80;
static int terminal_height = 24;
static int current_progress = 0;
static char current_package[256] = "";
static char current_manager[256] = "";
static char current_status[256] = "Ready";
static int animation_frame = 0;

// Colors and styling
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define CYAN        "\033[96m"
#define GREEN       "\033[92m"
#define YELLOW      "\033[93m"
#define BLUE        "\033[94m"
#define MAGENTA     "\033[95m"
#define RED         "\033[91m"
#define WHITE       "\033[97m"
#define BG_DARK     "\033[100m"

// Animation characters
static const char* spinner[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
static const char* progress_chars[] = {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};

void get_terminal_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        terminal_width = w.ws_col;
        terminal_height = w.ws_row;
    }
    if (terminal_width < 60) terminal_width = 60;
}

void clean_terminal() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void move_cursor_up(int lines) {
    printf("\033[%dA", lines);
}

void hide_cursor() {
    printf("\033[?25l");
    fflush(stdout);
}

void show_cursor() {
    printf("\033[?25h");
    fflush(stdout);
}

void draw_line(char ch, const char* color) {
    printf("%s", color);
    for (int i = 0; i < terminal_width; i++) {
        printf("%c", ch);
    }
    printf("%s\n", RESET);
}

void draw_modern_header() {
    printf(BOLD CYAN);
    printf("╭");
    for (int i = 0; i < terminal_width - 2; i++) {
        printf("─");
    }
    printf("╮\n");
    
    // Title with gradient effect
    printf("│");
    int title_len = strlen("🚀 DISTFUSE - Universal Package Manager");
    int padding = (terminal_width - 2 - title_len) / 2;
    
    for (int i = 0; i < padding; i++) printf(" ");
    printf(BOLD WHITE "🚀 DISTFUSE" CYAN " - Universal Package Manager");
    for (int i = 0; i < terminal_width - 2 - padding - title_len; i++) printf(" ");
    printf("│\n");
    
    printf("╰");
    for (int i = 0; i < terminal_width - 2; i++) {
        printf("─");
    }
    printf("╯" RESET "\n");
}

void draw_package_card() {
    if (strlen(current_package) == 0) return;
    
    printf(DIM "┌");
    for (int i = 0; i < terminal_width - 2; i++) printf("─");
    printf("┐" RESET "\n");
    
    printf(DIM "│" RESET " " BOLD GREEN "📦 Package: " RESET WHITE "%s\n", current_package);
    printf(DIM "│" RESET " " BOLD BLUE "⚡ Manager: " RESET WHITE "%s\n", current_manager);
    
    printf(DIM "└");
    for (int i = 0; i < terminal_width - 2; i++) printf("─");
    printf("┘" RESET "\n");
}

void draw_modern_progress_bar() {
    int bar_width = terminal_width - 20;
    if (bar_width < 20) bar_width = 20;

    printf(BOLD YELLOW "Progress " RESET "[");
    
    int filled = (current_progress * bar_width) / 100;
    int partial = ((current_progress * bar_width) % 100) / 12; // 0-7
    
    // Draw filled part
    for (int i = 0; i < filled; i++) {
        printf(GREEN "█" RESET);
    }
    
    // Draw partial part
    if (filled < bar_width && partial > 0) {
        printf(YELLOW "%s" RESET, progress_chars[partial]);
        filled++;
    }
    
    // Draw empty part
    for (int i = filled; i < bar_width; i++) {
        printf(DIM "░" RESET);
    }
    
    printf("] " BOLD "%3d%%" RESET "\n", current_progress);
}

void draw_status_with_spinner() {
    printf(BOLD MAGENTA "%s " RESET, spinner[animation_frame % 10]);
    printf(WHITE "Status: " RESET "%s", current_status);
    
    // Clear rest of line
    int status_len = strlen(current_status) + 9;
    for (int i = status_len; i < terminal_width; i++) {
        printf(" ");
    }
    printf("\n");
}

void draw_ui() {
    clean_terminal();
    hide_cursor();
    get_terminal_size();
    
    clean_terminal();
    draw_modern_header();
    printf("\n");
    draw_package_card();
    printf("\n");
    draw_modern_progress_bar();
    printf("\n");
    draw_status_with_spinner();
    printf("\n");
    
    fflush(stdout);
}

void ui_set_package_status(const char* package, const char* manager) {
    strncpy(current_package, package, sizeof(current_package) - 1);
    current_package[sizeof(current_package) - 1] = '\0';
    
    strncpy(current_manager, manager, sizeof(current_manager) - 1);
    current_manager[sizeof(current_manager) - 1] = '\0';
    
    current_progress = 0;
    strcpy(current_status, "Initializing...");
}

void ui_update_status(const char* status) {
    strncpy(current_status, status, sizeof(current_status) - 1);
    current_status[sizeof(current_status) - 1] = '\0';
}

void update_ui_display() {
    // Clear terminal and redraw everything
    clean_terminal();
    hide_cursor();
    get_terminal_size();
    
    draw_modern_header();
    printf("\n");
    draw_package_card();
    printf("\n");
    draw_modern_progress_bar();
    printf("\n");
    draw_status_with_spinner();
    printf("\n");
    
    fflush(stdout);
}

void ui_update_download_progress() {
    ui_update_status("🔍 Searching package repositories...");
    draw_ui();
    usleep(500000);
    
    for (int i = 5; i <= 40; i += 3) {
        current_progress = i;
        animation_frame++;
        
        if (i < 15) {
            ui_update_status("🔍 Locating package...");
        } else if (i < 30) {
            ui_update_status("📥 Preparing download...");
        } else {
            ui_update_status("⬇️  Downloading package...");
        }
        
        update_ui_display();
        usleep(200000);
    }
}

void ui_update_install_progress() {
    for (int i = 41; i <= 100; i += 4) {
        current_progress = i;
        animation_frame++;
        
        if (i < 60) {
            ui_update_status("📦 Extracting package...");
        } else if (i < 85) {
            ui_update_status("⚙️  Installing components...");
        } else if (i < 95) {
            ui_update_status("🔗 Setting up dependencies...");
        } else {
            ui_update_status("✅ Finalizing installation...");
        }
        
        update_ui_display();
        usleep(150000);
    }
}

void ui_update_remove_progress() {
    ui_update_status("🔍 Locating installed package...");
    draw_ui();
    usleep(300000);
    
    for (int i = 5; i <= 100; i += 5) {
        current_progress = i;
        animation_frame++;
        
        if (i < 20) {
            ui_update_status("🔍 Checking dependencies...");
        } else if (i < 50) {
            ui_update_status("🗑️  Removing files...");
        } else if (i < 80) {
            ui_update_status("🧹 Cleaning up...");
        } else {
            ui_update_status("✅ Package removed successfully!");
        }
        
        update_ui_display();
        usleep(100000);
    }
}

void ui_show_error(const char* error_msg) {
    printf("\n" BOLD RED "❌ Error: " RESET RED "%s" RESET "\n", error_msg);
    show_cursor();
    fflush(stdout);
}

void ui_show_success(const char* message) {
    printf("\n" BOLD GREEN "✅ Success: " RESET GREEN "%s" RESET "\n", message);
    show_cursor();
    fflush(stdout);
}

void ui_finish() {
    current_progress = 100;
    ui_update_status("✅ Operation completed successfully!");
    
    // Full UI update for completion
    update_ui_display();
    
    printf(BOLD GREEN "🎉 All done! Package operation completed successfully." RESET "\n");
    show_cursor();
    fflush(stdout);
}
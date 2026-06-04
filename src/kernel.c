void init_gdt(void);
void init_idt(void);

void putchar(char c);
void print(const char *str);

int cursor_x = 0;
int cursor_y = 0;

int min_cursor_x = 2; 

void clear_screen(void) {
    char *video_memory = (char*) 0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void print_prompt(void) {
    print("> ");
    min_cursor_x = cursor_x; 
}

void backspace(void) {
    char *video_memory = (char*) 0xB8000;

    if (cursor_x <= min_cursor_x) {
        return;
    }

    if (cursor_x > 0) {
        cursor_x--;
    }

    int offset = (cursor_y * 80 + cursor_x) * 2;
    video_memory[offset] = ' ';
    video_memory[offset + 1] = 0x07;
}

void putchar(char c) {
    if (c == '\b') {
        backspace();
        return;
    }

    char *video_memory = (char*) 0xB8000;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        
        print_prompt();
    } 
    else {
        int offset = (cursor_y * 80 + cursor_x) * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = 0x0F;

        cursor_x++;
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= 25) {
        clear_screen();
        print_prompt();
    }
}

void print(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putchar(str[i]);
        i++;
    }
}

void kernel_main(void) {
    init_gdt();
    init_idt();

    __asm__ volatile("sti");

    clear_screen();
    print("Welcome to OneOS!\n");
    print("Keyboard driver active (AZERTY layout).\n"); 

    while(1) {
        __asm__ volatile("hlt");
    }
}
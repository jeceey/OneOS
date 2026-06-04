void kernel_main(void) {
    char *video_memory = (char*) 0xB8000;

    const char *str = "Welcome to OneOS!";
    
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07; 
    }

    int i = 0;
    while (str[i] != '\0') {
        video_memory[i * 2] = str[i];
        video_memory[i * 2 + 1] = 0x0F; 
        i++;
    }
}
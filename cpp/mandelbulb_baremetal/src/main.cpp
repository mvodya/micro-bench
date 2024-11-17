extern "C" void main() {
    const char *message = "Mandelbulb Baremetal benchmark!";
    volatile char *video_memory = (volatile char *)0xB8000;
    int offset = 0;

    // Clear screen
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';      // Empty char
        video_memory[i + 1] = 0x07; // White on black
    }

    // Print message
    for (const char *p = message; *p; ++p) {
        video_memory[offset++] = *p;   // Empty char
        video_memory[offset++] = 0x07; // White on black
    }

    while (true) {
        __asm__("hlt");
    }
}

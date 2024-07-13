#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// Function to initialize readline and set up a simple prompt
void initialize_readline() {
    // Clear the history at the start
    rl_clear_history();
}

// Function to handle the user input
void handle_input(const char *input) {
    if (input == NULL) {
        printf("\nExiting...\n");
        rl_clear_history();
        exit(0);
    }

    // Add the input to history
    add_history(input);

    printf("You entered: %s\n", input);

}

int main() {
    
    return 0;
}


int main(void) 
{
    // signal
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

    initialize_readline();

    // Main loop to read input from the user 
    while (true) {
        // Display the prompt and read input
        char *input = readline("my_shell> ");

        // Handle the user input
        handle_input(input);

        // Free the input buffer
        free(input);
        // testowanie
    }
    // rl_clear_history();
    return 0;
}

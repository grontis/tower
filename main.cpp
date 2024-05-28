#include <iostream>
#include <csignal>
#include <thread>

bool running = true;

void signalHandler(int signal) {
    if (signal == SIGINT) {
        running = false;
    }
}

void printLine(std::string line) {    
    for(int i = 0; i < line.length(); i++) {
        std::cout << line[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << std::endl;
}

void printMainMenu() { 
    printLine("T O W E R");

    printLine("1. Start Game");
    printLine("2. Load Game");
    printLine("3. Exit");
}

int processInput() {
    int choice;
    std::cin >> choice;

    return choice;
}

void update(int choice) {
    switch(choice) {
        case 1:
            printLine("Starting Game...");
            break;
        case 2:
            printLine("Loading Game...");
            break;
        case 3:
            printLine("Exiting Game...");
            break;
        default:
            printLine("Invalid Choice");
            break;
    }

}

void renderNext() {
    system("clear"); //todo interface for system indedepent clear
    //TODO render next frame
    //TODO scenario class
}

int main() {
    std::signal(SIGINT, signalHandler);

    printMainMenu();
    while (running) {
        int choice = processInput();
        update(choice);
        renderNext();
    }

    return 0;
}
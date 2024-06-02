#include <iostream>
#include <csignal>
#include <thread>
#include <vector>
#include <algorithm>

void printLine(std::string line, int printDelay = 50) {    
    for(int i = 0; i < line.length(); i++) {
        std::cout << line[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(printDelay));
    }

    std::cout << std::endl;
}

class Option {
    public:
        Option() : nextScenarioId(), text("Default Option"){}
        Option(int nextScenarioId, std::string text) : nextScenarioId(nextScenarioId), text(text) {}

        int GetNextScenarioId() {
            return nextScenarioId;
        }

        std::string GetText() {
            return text;
        }

    private:
        int nextScenarioId;
        std::string text;
};

class Scenario {
    public:
        Scenario() : id(0), title(), options() {}
        Scenario(int id, std::string title, std::vector<Option> options) : id(id), title(title), options(options) {}

        void Print() {
            printLine(title);
            for (int i = 0; i < options.size(); i++) {
                printLine(std::to_string(i + 1) + ". " + options[i].GetText());
            }
        }

        int GetId() {
            return id;
        }

        Option GetOption(int index) {
            return options[index];
        }
        
    private:
        int id;
        std::string title;
        std::vector<Option> options;
};



bool running = true;
int currentScenarioId = 1;
std::vector<Scenario> scenarios;

void initializeScenarios() {
    scenarios.push_back(Scenario(1, "T O W E R", 
    {
        Option(3, "Start Game"), 
        Option(2, "Load Game"), //TODO save game scenario
        Option(2, "Exit"),
    }));

    scenarios.push_back(Scenario(3, "The dark marshes of the forest are silent with death. The air is thick with the smell of decay. My boots fill with mud and water as I tread along, deeper into the forest. My brother said he was going to find a relic for the church, but has never returned. Is he still alive? Gods I hope so...", 
    {
        Option(4, "Continue"),
    }));

    scenarios.push_back(Scenario(4, "I'm not far from the ancient tower, but it is starting to get dark... As the sun sets, the forest grows darker. The trees seem to close in around me. It might be wise to set up camp for the night.", 
    {
        Option(5, "Set up camp"),
        Option(7, "Press onward into the darkness of the night"),
    }));

    scenarios.push_back(Scenario(5, "I've found a small clearing next to the creek. It's a good spot to set up camp. I gather some wood and start a fire. The warmth is comforting. I eat some of the food I brought with me and settle in for the night. I hope my brother is safe...", 
    {
        Option(6, "Continue"),
    }));

    scenarios.push_back(Scenario(6, "The fire crackles and pops as I sit, staring into the darkness. I take a long draw from my pipe and exhale. As the smoke drifts away, I close my eyes...", 
    {
        Option(6, "Continue"),
    }));

    scenarios.push_back(Scenario(7, "I press onward into the darkness of the night. The forest is alive with the sounds of the night. The trees seem to whisper to me as I walk. I can feel the eyes of the forest upon me. I must be close to the tower now... I hope my brother is safe...",
    {
        Option(7, "Continue"),
    }));
}

void signalHandler(int signal) {
    if (signal == SIGINT) {
        running = false;
    }
}

Option processInput() {
    int choice;
    std::cin >> choice;

    return scenarios[currentScenarioId].GetOption(choice - 1);
}

void update(Option choice) {
    currentScenarioId = choice.GetNextScenarioId();
}

void renderNext() {
    system("clear");

    auto findScenario = [](int id) {
        return std::find_if(scenarios.begin(), scenarios.end(), [id](Scenario& scenario) {
            return scenario.GetId() == id;
        });
    };

    auto it = findScenario(currentScenarioId);
    if (it != scenarios.end()) {
        it->Print();
    } else {
        printLine("Invalid scenario id: " + std::to_string(currentScenarioId));
    }
}

int main() {
    try{
        std::signal(SIGINT, signalHandler);

        initializeScenarios();

        renderNext();
        while (running) {
            Option choice = processInput();
            update(choice);
            renderNext();
        }
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught an unknown exception." << std::endl;
    }

    return 0;
}
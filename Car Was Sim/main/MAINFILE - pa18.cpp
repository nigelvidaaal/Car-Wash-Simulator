
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <stdexcept>

// Simulation struct
struct SimulationResult {
    // scheduled simulation time
    int duration;

    // count of all cars
    int total_cars;

    // time of cars in the line
    int longest_wait;
    double average_wait;
};

// following the optional OO design suggestions
class CarWashSimulator {
public:
    // default constructor
    /**
    * @brief a car wash simulator with initialized random number generators.
    */
    CarWashSimulator() : random(std::random_device{}()), distribute(0, 3) {}

    // making this of a member struct
    /**
    * @brief Does a single car wash simulation for specified duration.
    * @param minutes Duration of simulation in minutes
    */
    SimulationResult run(int minutes) {
        // self explanatory variables
        std::queue<int> car_queue;
        int current_time = 0;
        int cars_washed = 0;
        int total_wait_time = 0;
        int max_wait_time = 0;

        // while the store is open AND while also the line's not empty..
        while (current_time < minutes || !car_queue.empty()) {

            // wash cars during open hours
            if (current_time < minutes) {
                // generating random num
                int arrival_case = distribute(random);

                // adheres via "Simulation Model" section
                // add cars to the queue
                if (arrival_case == 2) {
                    car_queue.push(current_time);

                // if multiple cars arrive at the same time
                } else if (arrival_case == 3) {
                    car_queue.push(current_time);
                    car_queue.push(current_time);
                }
            }

            // process car washing (one car per minute)
            if (!car_queue.empty()) {
                // process first car in line via queue
                int arrival_time = car_queue.front();

                // remove Mitsubishi Lancer 2008 from queue and start washing
                car_queue.pop();

                // and then calculate wait time for each
                int wait_time = current_time - arrival_time;

                // sum of wait times
                total_wait_time += wait_time;

                // keep track of the cars washed
                cars_washed++;

                // track of the max wait time if there's one
                if (wait_time > max_wait_time) {
                    max_wait_time = wait_time;
                }
            }

            // always keep the time moving
            current_time++;
        }

        // create the result of the simulation
        SimulationResult result;

        // overall duration
        result.duration = minutes;

        // total cars washed
        result.total_cars = cars_washed;

        // longest time recorded
        result.longest_wait = max_wait_time;

        // avg time recorded time of waiting
        result.average_wait = (cars_washed > 0) ?
        static_cast<double>(total_wait_time) / cars_washed : 0.0;

        return result;
    }

    /**
    * @brief Does a series of car wash simulations in doubling sequence
    * @param max_minutes The maximum duration in minutes
    */
    void run_all(int max_minutes) {
        // to empty for new simulations
        results.clear();

        // for the doubling sequence to commence
        int time_duration = 30;

        // conditional var
        bool resume = true;

        // for short simulations via "Simulation Behavior" section in the desc
        // example for ./pa18 -m 25
        if (max_minutes < 30) {
            // pushback track record to the results
            results.push_back(run(max_minutes));
            resume = false;
        }

        // while the store's running and hasn't exceeded the max mins
        while (resume && time_duration <= max_minutes) {
            // push back recorded times in the results
            results.push_back(run(time_duration));

            // decide whether to continue doubling
            if (time_duration <= max_minutes / 2) {
                time_duration *= 2;
            } else {
                // otherwise, stop
                resume = false;
            }
        }

        // simulation check to make sure
        // only if max is to 30, no results yet, OR last run's smaller than max
        if (max_minutes >= 30 && (results.empty()
        || results.back().duration < max_minutes)) {
            results.push_back(run(max_minutes));
        }
    }

    /**
    * @brief Displays simulation results
    */
    // adhere to the given instructions on "Formatting Rules"
    void display_results() const {
        std::cout << std::setw(15) << "Time (minutes)"
                  << std::setw(15) << "Cars Washed"
                  << std::setw(15) << "Average Wait"
                  << std::setw(15) << "Longest Wait"
                  << "\n";
        std::cout << std::string(60, '-') << "\n";

        for (const auto& result : results) {
            std::cout << std::setw(15) << result.duration
                      << std::setw(15) << result.total_cars
                      << std::setw(15) << std::fixed << std::setprecision(2)
                      << result.average_wait
                      << std::setw(15) << result.longest_wait
                      << "\n";
        }
    }

private:
    std::default_random_engine random;
    std::uniform_int_distribution<int> distribute;
    std::vector<SimulationResult> results;
};

// "3. Create an argument parser class"
class ArgumentParser {
public:
    // constructor; number of command line, argument vector
    /**
    * @brief An argument parser and processes command line arguments.
    * @param argc Argument count from main()
    * @param argv Argument values/vector from main()
    */
    ArgumentParser(int argc, char* argv[]) {
        // argument loop, starts at 1; passing the program name
        for (int i = 1; i < argc; i++) {
            // processes the line
            std::string arg = argv[i];

            // pa18 lecture: 1:48:15 reference
            // if user types "h", displays the help message
            if (arg == "-h") {
                help = true;

            // also verifies if there's a value after -m
            } else if (arg == "-m") {
                // making sure if there's another argument after typing -m
                if (i + 1 < argc) {
                    try {
                        // converting that typed argument to an int using stoi
                        max = std::stoi(argv[i + 1]);
                        // also for ./pa18 -m 0, to fallback to default
                        if (max <= 0) {
                            max = 43200;
                        }
                        i++;
                    /* this catches all the errors whenever user types something
                    not supposed to be typed */
                    } catch (const std::invalid_argument&) {
                        max = 43200;
                    } catch (const std::out_of_range&) {
                        max = 43200;
                    }
                }
            }
        }
    }

    /**
    * @brief Checks if help flag (-h) was entered
    * @return help If help was requested, false otherwise
    */
    bool help_requested() const {
        return help;
    }

    /**
    * @brief Gets the maximum simulation duration
    * @return max The maximum simulation time
    */
    int max_minutes() const {
        return max;
    }

private:
    bool help = false;
    int max = 43200;
};

// *** FUNCTION HEADER ***
void display_help(const std::string& program_name);

int main(int argc, char* argv[]) {
    // laid out by the project description
    ArgumentParser args(argc, argv);

    // if help is requested,then display help interface
    if (args.help_requested()) {
        display_help(argv[0]);
    }
    // else, run all necessities/the program itself
    else {
        CarWashSimulator simulator;
        simulator.run_all(args.max_minutes());
        simulator.display_results();
    }

    return 0;
}

// function to display help message
void display_help(const std::string& program_name) {
    // making sure we don't hard code our program name to ensure correctness
    // even when we rename the file
    std::cout << "Usage: " << program_name << " [-m MINUTES] [-h]\n"
              << "Simulate a car wash queue.\n"
              << "  -m MINUTES   Set simulation upper limit (default: 43200)\n"
              << "  -h           Show this help message and exit\n";
}

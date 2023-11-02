#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
class Seat {
public:
    Seat(int num, int p, const std::string& N, const std::string& avail) : number(num), price(p), Name(N), availability(avail) {}
    int number;
    int price;
    std::string Name;
    std::string availability;

    void SetName(std::string NameNew) {
        Name = NameNew;
    }
    void SetAv(std::string Av) {
        availability = Av;
    }
};

/* Flight{
public:
    Flight(int date, int p, const std::string& N, const std::string& avail) : number(num), price(p), Name(N), availability(avail) {}
    int number;
    int price;
    std::string Name;
    std::string availability;

    void SetName(std::string NameNew) {
        Name = NameNew;
    }
    void SetAv(std::string Av) {
        availability = Av;
    }
};
*/
class FileConfig {
public:
    FileConfig() {}

    void ReadReturn(const std::string& name) {
        std::ifstream file(name);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string date, flightNumber, token;
            int seatCount;
            iss >> date >> flightNumber >> seatCount;

            std::vector<std::string> data = { date, flightNumber, std::to_string(seatCount) };

            while (iss >> token) {
                data.push_back(token);
            }

            config[flightNumber] = std::move(data);

            initializeSeatsAvailability(flightNumber);
            std::cout << "Flight Number: " << flightNumber << ", Data: ";
            for (const auto& item : config[flightNumber]) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }

        file.close();
    }

    int getTotalSeats(const std::string& flightNumber) const {
        auto it = config.find(flightNumber);
        if (it == config.end()) {
            std::cerr << "Flight not found." << std::endl;
            return -1;
        }

        const std::vector<std::string>& flightData = it->second;
        int totalSeats = 0;
        for (size_t i = 3; i < flightData.size(); i += 2) { // Start from the fourth element and skip prices
            std::vector<std::string> tokens = splitString(flightData[i], "-");
            int start = std::stoi(tokens[0]);
            int end = std::stoi(tokens[1]);
            totalSeats += end - start + 1;
        }
        return totalSeats;
    }

    void initializeSeatsAvailability(const std::string& flightNumber) {
        auto it = config.find(flightNumber);
        if (it == config.end()) {
            std::cerr << "Flight not found." << std::endl;
            return;
        }

        const std::vector<std::string>& flightData = it->second;
        for (size_t i = 3; i < flightData.size(); i += 2) { // Start from the fourth element and skip prices
            std::vector<std::string> tokens = splitString(flightData[i], "-");
            int start = std::stoi(tokens[0]);
            int end = std::stoi(tokens[1]);
            int price = std::stoi(flightData[i + 1].substr(0, flightData[i + 1].size() - 1)); // remove '$' and convert to int
            for (int seat = start; seat <= end; ++seat) {
                seatsAvailability[flightNumber].push_back(Seat(seat, price,"", "Free"));
            }
        }
    }
    std::map<std::string, std::vector<Seat>> print_Avail() {
        return seatsAvailability;
    }
private:
    std::map<std::string, std::vector<std::string>> config;
    std::map<std::string, std::vector<Seat>> seatsAvailability;
    std::vector<std::string> myVector = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M", "O", "P", "Q","R", "S","T","V","X","Y","Z"};


    std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) const {
        std::vector<std::string> result;
        size_t start = 0, end = str.find(delimiter);
        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        result.push_back(str.substr(start, end));
        return result;
    }
};

int main() {
    FileConfig config;
    config.ReadReturn("C:/Users/Давід/source/repos/OOP1/OOP1/config.txt.txt");


    std::map<std::string, std::vector<Seat>> r=config.print_Avail();
    for (const auto& pair : r) {
        std::cout << "Flight Number: " << pair.first << std::endl;
        for (const Seat& seat : pair.second) {
            std::cout << "Seat Number: " << seat.number
                << ", Price: " << seat.price
                << "$, Availability: " << seat.availability
                << std::endl;
        }
        std::cout << "------------------------------------" << std::endl;
    }

    return 0;
}

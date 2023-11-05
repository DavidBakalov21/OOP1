#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <random>
class Seat {
public:
    Seat(int num, std::string p,  std::string N,  std::string avail,  std::string id) : number(num), price(p), Name(N), availability(avail), ID(id) {}
    int number;
    std::string price;
    std::string Name;
    std::string availability;
    std::string ID;

    void SetName(std::string NameNew) {
        Name = NameNew;
    }
    void SetAv(std::string Av) {
        availability = Av;
    }
};

class Flight {
public:
    Flight(std::string date, std::string number, std::vector<Seat> S) : FDate(date), No(number), seats(S) {}
    std::string FDate;
    std::string No;
    std::vector<Seat> seats;
};
class FileConfig {
public:
    FileConfig() {}

    void ReadReturn(const std::string& name) {
        std::ifstream file(name);
        if (!file.is_open()) {
            std::cout << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string date;
            std::string flightNumber;
            std::string seatCount;
            std::string firstInter;
            std::string firstPrice;
            std::string secondInt;
            std::string secondPrice;
            iss >> date;
            iss >> flightNumber; 
            iss >> seatCount;
            iss >> firstInter;
            iss >> firstPrice;
            iss >> secondInt;
            iss >> secondPrice;
            std::vector<std::string> data = { date, flightNumber, seatCount,firstInter,firstPrice,secondInt,secondPrice };

            config[flightNumber] = std::move(data);
            std::string informaString = "";
            SetSeats(flightNumber);
            for (int i = 0; i < config[flightNumber].size(); i++)
            {
               informaString+= config[flightNumber][i]+" ";
            }
            std::cout << informaString << std::endl;

           
        }

        file.close();
    }
    void SetSeats(const std::string& flightNumber) {
        auto el = config.find(flightNumber);
        std::vector<std::string>& flightData = el->second;
        std::vector<Seat> seatsFlight;

        int seatCountPerRow = std::stoi(flightData[2]);
        int row = 1;
        int seatRow = 0;

        for (size_t i = 3; i < flightData.size(); i += 2) {
            std::vector<std::string> tokens = splitString(flightData[i]);
            int start = std::stoi(tokens[0]);
            int end = std::stoi(tokens[1]);
            std::string priceStr = flightData[i + 1];
            

            for (int seatNumber = start; seatNumber <= end; ++seatNumber) {
                if (seatRow >= seatCountPerRow) {
                    row++;
                    seatRow = 0;
                }
                std::string seatName = std::to_string(row) + Alphabet[seatRow];
                seatsFlight.emplace_back(seatNumber, priceStr, seatName, "Free","");

                seatRow++;
            }
        }
        flights.emplace_back(flightData[0], flightNumber, seatsFlight);
    }



    std::vector<Flight> getFlights() {
        return flights;
    }

private:
    std::map<std::string, std::vector<std::string>> config;
    std::vector<std::string> Alphabet = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M", "O", "P", "Q","R", "S","T","V","X","Y","Z"};
    std::vector<Flight> flights;
    std::vector<std::string> splitString(const std::string& str) {
        std::vector<std::string> result;
        int DelimPos = str.find('-');
        result.push_back(str.substr(0, DelimPos));
        result.push_back(str.substr(DelimPos + 1)); 
        
        return result;
    }

};

class FlightManager {
    FileConfig config; 
    std::vector<Flight> FlighVec;
    std::string FPath; 

public:
    FlightManager(const std::string& path) : FPath(path) {
        config.ReadReturn(FPath);
        FlighVec = config.getFlights(); 
    }


    void Check(std::string date, std::string No) {
       
        for (int j = 0; j < FlighVec.size(); j++) {
          
            if (FlighVec[j].FDate == date && FlighVec[j].No == No) {
                for (int i = 0; i < FlighVec[j].seats.size(); i++)
                {
                    if (FlighVec[j].seats[i].availability == "Free") {
                        std::cout << FlighVec[j].seats[i].Name << ", " << FlighVec[j].seats[i].price << std::endl;
                    }
                    
                }
            }
        }
    }

    void Book(std::string Date, std::string FlightNo, std::string place, std::string name) {
        for (int j = 0; j < FlighVec.size(); j++) {
            if (FlighVec[j].FDate == Date && FlighVec[j].No == FlightNo) {
                for (int i = 0; i < FlighVec[j].seats.size(); i++)
                {
                    if (FlighVec[j].seats[i].availability == "Free" && FlighVec[j].seats[i].Name == place) {
                        FlighVec[j].seats[i].availability = name;
                        FlighVec[j].seats[i].ID = GenerateId();
                        std::cout << "----------" << std::endl;
                        std::cout <<"confirmed with ID " << FlighVec[j].seats[i].ID << std::endl;
                    }

                }
            }
        }
    }

    void returnTicket(std::string ID) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].seats.size(); i++)
            {
                if (FlighVec[j].seats[i].ID == ID) {
                    std::cout << "----------" << std::endl;
                    std::cout << FlighVec[j].seats[i].price << "returned to" << FlighVec[j].seats[i].availability << std::endl;
                    FlighVec[j].seats[i].availability = "Free";
                    FlighVec[j].seats[i].ID.clear();
                
                }
            }
        
        
        }
    }


    void View(std::string ID) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].seats.size(); i++)
            {
                if (FlighVec[j].seats[i].ID == ID) {
                    std::cout << "----------" << std::endl;
                    std::cout << FlighVec[j].seats[i].price << std::endl;
                    std::cout << FlighVec[j].No << std::endl;
                    std::cout << FlighVec[j].FDate << std::endl;
                    std::cout << FlighVec[j].seats[i].Name << std::endl;

                }
            }


        }
    }

    void ViewUser(std::string Username) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].seats.size(); i++)
            {
                if (FlighVec[j].seats[i].availability == Username) {
                    std::cout << "----------" << std::endl;
                    std::cout<<"FlightNO:" << FlighVec[j].No <<",Date: " << FlighVec[j].FDate <<", seat:"<<FlighVec[j].seats[i].Name << ", price:" << FlighVec[j].seats[i].price <<std::endl;
                }
            }


        }
    }
private:
    std::mt19937 rng;
    std::string GenerateId() {
        std::string result = "";
        std::uniform_int_distribution<int> dist(1, 9);

        for (int i = 0; i < 15; i++) {
            int iRand = dist(rng);
            result += std::to_string(iRand);
        }
        return result;
    }

};

int main() {
    FlightManager man("C:/Users/Давід/source/repos/OOP1/OOP1/config.txt.txt");
    man.Check("11.12.2022", "FQ12");
    std::cout << "AAAAAAA----asasxasdx------ssssssss-dsd-------ddvvvvvvvv------------" << std::endl;
    man.Book("11.12.2022", "FQ12", "1A", "Vasya Pupkin");
    man.Book("11.12.2022", "FQ12", "2A", "Tanya Pupkin");
    man.ViewUser("Vasya Pupkin");
    std::cout << "--eeeeeee--------ssssssss------ggggggggggggg-----xxxx---dsvsds------" << std::endl;
    man.Check("11.12.2022", "FQ12");

    //std::cout << GenerateId() << std::endl;
    return 0;
}

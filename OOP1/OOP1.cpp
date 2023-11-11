#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits> 
#include <iterator>
#include <random>

enum CLI
{
    CHECK = 1,
    BOOK = 2,
    RETURN = 3,
    VIEW=4,
    ViewUser=5
   
};

class Seat {
public:
    Seat(std::string p,  std::string N,  std::string avail,  std::string id) : price(p), Name(N), availability(avail), ID(id) {}
    Seat(const Seat& other) : price(other.price), Name(other.Name), availability(other.availability), ID(other.ID) {};
    Seat(Seat&& other) : price(std::move(other.price)), Name(std::move(other.Name)), availability(std::move(other.availability)), ID(std::move(other.ID)) {
    }

    void SetName(std::string NameNew) {
        Name = NameNew;
    }
    void SetAv(std::string Av) {
        availability = Av;
    }
    void SetId(std::string newId) {
        ID = newId;
    }

    std::string getName() {
        return Name;
    }
    std::string getPrice() {
        return price;
    }
    std::string getAvail() {
        return availability;
    }
    std::string getID() {
        return ID;
    }
private:
    std::string price;
    std::string Name;
    std::string availability;
    std::string ID;

};

class Flight {
public:
    Flight(std::string date, std::string number, std::vector<Seat> S) : FDate(date), No(number), seats(S) {}
   

    std::string getFdate() {
        return FDate;
    }
    std::string getNo() {
        return No;
    }
    std::vector<Seat>& getSeats() {
        return seats;
    }

private:
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
            iss >> date;
            iss >> flightNumber;
            iss>> seatCount;
            std::vector<std::string> data = { date, flightNumber, seatCount };
            std::string interval, price;

            while (iss >> interval >> price) {
                data.push_back(interval);
                data.push_back(price);
            }

            config[flightNumber] = std::move(data);
            SetSeats(flightNumber); 

            std::string informaString = "";
            for (int i = 0; i < config[flightNumber].size(); i++)
            {
                informaString += config[flightNumber][i] + " ";
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
            

            for (int seatNumber = start; seatNumber <= end; seatNumber++) {
                if (seatRow >= seatCountPerRow) {
                    row++;
                    seatRow = 0;
                }
                std::string seatName = std::to_string(row) + Alphabet[seatRow];
                seatsFlight.emplace_back(priceStr, seatName, "Free","");

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
    FlightManager(std::string path) : FPath(path) {
        config.ReadReturn(FPath);
        FlighVec = config.getFlights(); 
    }


    void Check(std::string& date, std::string& No) {
       
        for (int j = 0; j < FlighVec.size(); j++) {
          
            if (FlighVec[j].getFdate() == date && FlighVec[j].getNo() == No) {
                for (int i = 0; i < FlighVec[j].getSeats().size(); i++)
                {
                    if (FlighVec[j].getSeats()[i].getAvail() == "Free") {
                        std::cout << FlighVec[j].getSeats()[i].getName() << ", " << FlighVec[j].getSeats()[i].getPrice() << std::endl;
                    }
                    
                }
            }
        }
    }

    void Book(std::string& Date, std::string& FlightNo, std::string& place, std::string& name) {
        for (int j = 0; j < FlighVec.size(); j++) {
            if (FlighVec[j].getFdate() == Date && FlighVec[j].getNo() == FlightNo) {
                for (int i = 0; i < FlighVec[j].getSeats().size(); i++)
                {
                    if (FlighVec[j].getSeats()[i].getAvail() == "Free" && FlighVec[j].getSeats()[i].getName() == place) {
                        FlighVec[j].getSeats()[i].SetAv(name);
                        FlighVec[j].getSeats()[i].SetId(GenerateId());
                        std::cout << "----------" << std::endl;
                        std::cout <<"confirmed with ID " << FlighVec[j].getSeats()[i].getID() << std::endl;
                    }

                }
            }
        }
    }

    void returnTicket(std::string& ID) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].getSeats().size(); i++)
            {
                if (FlighVec[j].getSeats()[i].getID() == ID) {
                    std::cout << "----------" << std::endl;
                    std::cout << FlighVec[j].getSeats()[i].getPrice() << "returned to" << FlighVec[j].getSeats()[i].getAvail() << std::endl;
                    FlighVec[j].getSeats()[i].SetAv("Free");
                    FlighVec[j].getSeats()[i].SetId("");
                
                }
            }
        
        
        }
    }


    void View(std::string& ID) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].getSeats().size(); i++)
            {
                if (FlighVec[j].getSeats()[i].getID() == ID) {
                    std::cout << "----------" << std::endl;
                    std::cout << FlighVec[j].getSeats()[i].getID() << std::endl;
                    std::cout << FlighVec[j].getNo() << std::endl;
                    std::cout << FlighVec[j].getFdate() << std::endl;
                    std::cout << FlighVec[j].getSeats()[i].getName() << std::endl;
                    std::cout << FlighVec[j].getSeats()[i].getAvail() << std::endl;
                    std::cout << FlighVec[j].getSeats()[i].getPrice() << std::endl;
                }
            }


        }
    }

    void ViewUser(std::string& Username) {
        for (int j = 0; j < FlighVec.size(); j++) {
            for (int i = 0; i < FlighVec[j].getSeats().size(); i++)
            {
                if (FlighVec[j].getSeats()[i].getAvail() == Username) {
                    std::cout << "----------" << std::endl;
                    std::cout<<"FlightNO:" << FlighVec[j].getNo() <<",Date: " << FlighVec[j].getFdate() <<", seat:"<<FlighVec[j].getSeats()[i].getName() << ", price:" << FlighVec[j].getSeats()[i].getPrice() <<std::endl;
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
class Command {
    FlightManager flightManager;

public:
    Command(std::string path) : flightManager(path) {}
    void commandLoop() {

        std::cout << "1-check, 2-book, 3-return, 4-viewId, 5-Username\n";
       
        while (true) {
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch (choice)
            {
            case CHECK:
            {
                std::string Date;
                std::string No;
                std::cin >> Date;
                std::cin >> No;
                flightManager.Check(Date,No);
                break;
            }
            case BOOK: {
                std::string Date;
                std::string No;
                std::string Place;
                std::string Name;
                std::cin >> Date;
                std::cin >> No;
                std::cin >> Place;
                std::cin >> Name;
                flightManager.Book(Date, No, Place,Name);
                break;
            }

            case RETURN: {
                std::string ID;
                std::cin >> ID;
                flightManager.returnTicket(ID);
                break;
            }
            case VIEW: {
                std::string ID2;
                std::cin >> ID2;
                flightManager.View(ID2);
                break;
            }
            case ViewUser: {
                std::string UserName;
                std::cin >> UserName;
                flightManager.ViewUser(UserName);
                break;
            }
      
            default:
                std::cout << "Wrong numba\n";
                break;
            }
        }

    }
};
int main() {
    Command com("C:/Users/Давід/source/repos/OOP1/OOP1/config.txt.txt");
    std::cout << "AAAAAAA----asasxasdx------ssss666666666ssss-dsd-------ddvvvvvvvv------------" << std::endl;
    com.commandLoop();
    /*FlightManager man("C:/Users/Давід/source/repos/OOP1/OOP1/config.txt.txt");
    man.Check("11.12.2022", "FQ12");
    std::cout << "AAAAAAA----asasxasdx------ssssssss-dsd-------ddvvvvvvvv------------" << std::endl;
    man.Book("11.12.2022", "FQ12", "1A", "NPC1");
    man.Book("11.12.2022", "FQ12", "2A", "NPC2");
    man.ViewUser("NPC1");
    std::cout << "--eeeeeee--------ssssssss------ggggggggggggg-----xxxx---dsvsds---2222---" << std::endl;
    man.Check("11.12.2022", "FQ12");
    */
    //std::cout << GenerateId() << std::endl;
    return 0;
}

#include<bits/stdc++.h>
// #include"Parser.h"
#ifdef IMPL
#include "Vehicle.h"
#include "Road.h"
typedef std::vector<Road*> Model;
#endif
int main(int argc, char **argv){
  // Add a check here
  std::ifstream configFile;
  configFile.open(argv[1]);
  if(configFile.fail()){
    std::cout << "[ ERROR ] File is corrupted/not found."<< std::endl;
    std::exit(1);
  }
  else{
    bool defmode = true;
    #ifdef IMPL
    Model model;
    #endif
    std::string line;
    while(std::getline(configFile,line)){
      if (! line.length()) continue;  // IGN empty
      if (line[0] == '#') continue;  // IGN with #
      {if(defmode){
        // All definitions go here
        if(line.find("Road_Id") != std::string::npos){
          // Create and add new road;
          int id = std::atoi(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          Road newroad(id);
          model.push_back(&newroad);
          #else
          std::cout << "Road ID : " << id << std::endl;
          #endif
        }
        if(line.find("Road_Length") != std::string::npos){
          // Create and add new road;
          double length = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->length = length;
          #else
          std::cout << "Length : "<<length<< std::endl;
          #endif
        }
        if(line.find("Road_Width") != std::string::npos){
          // Create and add new road;
          double width = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->width = width;
          #else
          std::cout << "Width : "<<width<< std::endl;
          #endif
        }
        if(line.find("Road_Signal") != std::string::npos){
          // Create and add new road;
          double signal = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->signalPosition = signal;
          #else
          std::cout << "Signal : "<<signal<< std::endl;
          #endif
        }

        // Default Params
        if(line.find("Default_MaxSpeed") != std::string::npos){
          // Create and add new road;
          double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->default_maxspeed = maxsp;
          #else
          std::cout << "Maxspeed : "<<maxsp<< std::endl;
          #endif
        }
        if(line.find("Default_Acceleration") != std::string::npos){
          // Create and add new road;
          double acc = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->default_acceleration = acc;
          #else
          std::cout << "Acceleration : "<<acc<< std::endl;
          #endif
        }
        if(line.find("Default_Type") != std::string::npos){
          // Create and add new road;
          std::string type = line.substr(line.find("=") + 1);
          #ifdef IMPL
          model.back()->default_type = type;
          #else
          std::cout << "Type : "<<type<< std::endl;
          #endif
        }
        if(line.find("Default_Skill") != std::string::npos){
          // Create and add new road;
          int skill = std::atoi(line.substr(line.find("=") + 1).c_str());
          if(skill < 0){
            skill = 0;
          }
          if( skill > 2){
            skill =2 ;
          }
          #ifdef IMPL
          model.back()->default_skill = skill;
          #else
          std::cout << "Skill : "<<skill<< std::endl;
          #endif
        }
        if(line.find("Vehicle_Type") != std::string::npos){
          // Create and add new road;
          std::string vtype = line.substr(line.find("=") + 2);
          #ifdef IMPL
          Vehicle newVehicle(vtype);
          model.back()->addVehicle(&newVehicle);
          #else
          std::cout << "New Vehicle Type : "<<vtype<< std::endl;
          #endif
        }
        if(line.find("Vehicle_Length") != std::string::npos){
          // Create and add new road;
          double length = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->vehicles.back()->length  = length;
          #else
          std::cout << "Vehicle Length : "<<length<< std::endl;
          #endif
        }
        if(line.find("Vehicle_Width") != std::string::npos){
          // Create and add new road;
          double width = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->vehicles.back()->width  = width;
          #else
          std::cout << "Vehicle width : "<<width<< std::endl;
          #endif
        }
        if(line.find("Vehicle_MaxSpeed") != std::string::npos){
          // Create and add new road;
          double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->vehicles.back()->maxspeed = maxsp;
          #else
          std::cout << "Vehicle maxspeed : "<<maxsp<< std::endl;
          #endif
        }
        if(line.find("Vehicle_Acceleration") != std::string::npos){
          // Create and add new road;
          double acc = std::atof(line.substr(line.find("=") + 1).c_str());
          #ifdef IMPL
          model.back()->vehicles.back()->acceleration = acc;
          #else
          std::cout << "Vehicle Acceleration : "<<acc<< std::endl;
          #endif
        }
        // CHANGE MODE
        if(line.find("SIMULATION") != std::string::npos){
          defmode = false;
        }
      }
      else{
        // For simulation
        std::cout << "~ ~ ~ ~ SIMULATION ~ ~ ~ ~"<<std::endl;
        if(line.find("END") != std::string::npos){
          break;
        }
      }
    }
    }
    std::cout<<"~ ~ ~ ~ THE END ~ ~ ~ ~"<<std::endl;
  }
}

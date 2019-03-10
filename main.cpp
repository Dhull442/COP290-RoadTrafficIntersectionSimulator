#include<bits/stdc++.h>
// #include"Parser.h"
#ifdef IMPL
#include "Vehicle.h"
#include "Road.h"
typedef std::vector<Road*> Model;
typedef std::vector<Vehicle*> vv;
#endif
std::string preprocess(std::string a){
  std::string ans = "";
  for(int i=0;i<a.length();i++){
    if(a[i]=='#')
      break;
    if(a[i] >= 65 && a[i] <= 90){
      ans += (a[i] + 32);
    }
    if(a[i] >= 97 && a[i] <= 122){
      ans += a[i];
    }
    else continue;
  }
  return ans;
}
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
    vV vehicles;
    #endif
    std::string line;
    int num_rules=0,safety_skill;
    double safety_maxspeed,safety_acceleration,safety_length,safety_width;
    while(std::getline(configFile,line)){
      if (! line.length()) continue;  // IGN empty
      if (line[0] == '#') continue;  // IGN with #
      {
        if(defmode){
          // All definitions go here
          if(line.find("Safety_MaxSpeed") != std::string::npos){
            // Create and add new road;
            safety_maxspeed = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            #ifdef IMPL
            #else
            std::cout << "Safety_MaxSpeed : " << safety_maxspeed << std::endl;
            #endif
          }
          if(line.find("Safety_Acceleration") != std::string::npos){
            // Create and add new road;
            safety_acceleration = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            #ifdef IMPL
            #else
            std::cout << "Safety_Acceleration : " << safety_acceleration << std::endl;
            #endif
          }
          if(line.find("Safety_Length") != std::string::npos){
            // Create and add new road;
            safety_length = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            #ifdef IMPL
            #else
            std::cout << "Safety_Length : " << safety_length << std::endl;
            #endif
          }
          if(line.find("Safety_Width") != std::string::npos){
            // Create and add new road;
            safety_width = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            #ifdef IMPL
            #else
            std::cout << "Safety_width : " << safety_width << std::endl;
            #endif
          }
          if(line.find("Safety_Skill") != std::string::npos){
            // Create and add new road;
            safety_skill = std::atoi(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            #ifdef IMPL
            #else
            std::cout << "Safety_Skill : " << safety_skill << std::endl;
            #endif
          }
          if(line.find("Road_Id") != std::string::npos){
            // Create and add new road;
            if(num_rules!= 5){
              std::cout << "[ ERROR ] Please Specify all the rules."<<std::endl;
              std::exit(1);
            }
            int id = std::atoi(line.substr(line.find("=") + 1).c_str());
            #ifdef IMPL
            Road newroad(id);
            newroad.setDefaults(safety_maxspeed,safety_acceleration,safety_length,safety_width,safety_skill);
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
            std::string type = preprocess(line.substr(line.find("=") + 1));
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
            std::string vtype = preprocess(line.substr(line.find("=") + 1));
            #ifdef IMPL
            Vehicle newVehicle(vtype);
            vehicles.push_back(&newVehicle);
            #else
            std::cout << "New Vehicle Type : "<<vtype<< std::endl;
            #endif
          }
          if(line.find("Vehicle_Length") != std::string::npos){
            // Create and add new road;
            double length = std::atof(line.substr(line.find("=") + 1).c_str());
            #ifdef IMPL
            vehicles.back()->length  = length;
            #else
            std::cout << "Vehicle Length : "<<length<< std::endl;
            #endif
          }
          if(line.find("Vehicle_Width") != std::string::npos){
            // Create and add new road;
            double width = std::atof(line.substr(line.find("=") + 1).c_str());
            #ifdef IMPL
            vehicles.back()->width  = width;
            #else
            std::cout << "Vehicle width : "<<width<< std::endl;
            #endif
          }
          if(line.find("Vehicle_MaxSpeed") != std::string::npos){
            // Create and add new road;
            double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
            #ifdef IMPL
            vehicles.back()->maxspeed = maxsp;
            #else
            std::cout << "Vehicle maxspeed : "<<maxsp<< std::endl;
            #endif
          }
          if(line.find("Vehicle_Acceleration") != std::string::npos){
            // Create and add new road;
            double acc = std::atof(line.substr(line.find("=") + 1).c_str());
            #ifdef IMPL
            vehicles.back()->acceleration = acc;
            #else
            std::cout << "Vehicle Acceleration : "<<acc<< std::endl;
            #endif
          }
          // CHANGE MODE
          if(line.find("START") != std::string::npos){
            defmode = false;
          }
        }
        else{
          // For simulation
          std::cout << "* * * * * * * * * ~ ~ ~ ~ SIMULATION ~ ~ ~ ~ * * * * * * * * *"<<std::endl;
          if(line.find("END") != std::string::npos){
            break;
          }
        }
      }
    }
    std::cout<<"* * * * * * * * * ~ ~ ~ ~ ~ THEEND ~ ~ ~ ~ ~ * * * * * * * * *"<<std::endl;
  }
}

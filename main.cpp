#include<bits/stdc++.h>
// #include"Parser.h"
// #ifdef IMPL
#include "Vehicle.h"
#include "Road.h"
typedef std::vector<Road*> Model;
typedef std::vector<Vehicle*> vv;
// #endif
// #ifdef VEHICLE_H
//   #ifdef ROAD_H
//     #ifndef IMPL
//     #define IMPL
    // #endif
  // #endif
// #endif

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
void simulationActions(
  // #ifdef IMPL
  Road* road,
  vv vehicles,
  // #endif
  std::vector<std::string> tokens){
    // std::cout <<"Road with ID "<<road -> id <<std::endl;
    double delT=0;
  for(int i=0;i<tokens.size();i++){
    // Parse the value and function
    std::string function = tokens[i].substr(0,tokens[i].find("="));
    std::string value = tokens[i].substr(tokens[i].find("=")+1);

    // Signal change routine
    if(!function.compare("Signal")){
      // #ifdef IMPL
      road->setSignal(value);
      // #else
        std::cout<<"Road Signal = "<<value<<std::endl;
      // #endif
      continue;
    }

    // Passing time routine
    if(!function.compare("Pass")){
      double time = std::atof(value.c_str());
      delT += time;
      // #ifdef IMPL
      // #else
        std::cout<<"Pass time = "<<time<<std::endl;
      // #endif
      continue;
    }

    // addition of vehicles routine
    // #ifdef IMPL
    bool found = false;
    for(int v = 0; v < vehicles.size(); v++ ){
      if(!vehicles[v]->type.compare(preprocess(function))){  // preprocessing to ignore any fuss due to Capitals
        road->addVehicle(vehicles[v],value);
        found = true;
        break;
      }
    }
    if(found)continue;
    // #else
    // #endif
    // None found
    {
      std::cout<<"No function defined for "<<function<<" with value "<<value<<std::endl;
      std::exit(1);
    }
  }
  // #ifdef IMPL

    std::cout<<"Running Simulation with ΔT = "<<delT<<std::endl;
    road->runSim(delT);
  // #else
  // #endif
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
    // #ifdef IMPL
    Model model;
    vv vehicles;
    // #endif
    std::string line;
    int num_rules=0,safety_skill;
    double safety_maxspeed,safety_acceleration,safety_length,safety_width,safety_lanes;
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
            // #ifdef IMPL
            // #else
            std::cout << "Safety_MaxSpeed : " << safety_maxspeed << std::endl;
            // #endif
          }
          if(line.find("Safety_Acceleration") != std::string::npos){
            // Create and add new road;
            safety_acceleration = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            // #ifdef IMPL
            // #else
            std::cout << "Safety_Acceleration : " << safety_acceleration << std::endl;
            // #endif
          }
          if(line.find("Safety_Length") != std::string::npos){
            // Create and add new road;
            safety_length = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            // #ifdef IMPL
            // #else
            std::cout << "Safety_Length : " << safety_length << std::endl;
            // #endif
          }
          if(line.find("Safety_Width") != std::string::npos){
            // Create and add new road;
            safety_width = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            // #ifdef IMPL
            // #else
            std::cout << "Safety_width : " << safety_width << std::endl;
            // #endif
          }
          if(line.find("Safety_Skill") != std::string::npos){
            // Create and add new road;
            safety_skill = std::atoi(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            // #ifdef IMPL
            // #else
            std::cout << "Safety_Skill : " << safety_skill << std::endl;
            // #endif
          }
          if(line.find("Safety_Lanes") != std::string::npos){
            // Create and add new road;
            safety_lanes = std::atoi(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            // #ifdef IMPL
            // #else
            std::cout << "Safety_Lanes : " << safety_skill << std::endl;
            // #endif
          }
          if(line.find("Road_Id") != std::string::npos){
            // Create and add new road;
            if(num_rules!= 6){
              std::cout << "[ ERROR ] Please Specify all the rules."<<std::endl;
              std::exit(1);
            }
            int id = std::atoi(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            Road* newroad= new Road(id);
            newroad->setDefaults(safety_maxspeed,safety_acceleration,safety_length,safety_width,safety_skill);
            model.push_back(newroad);
            model.back()->lanes = safety_lanes;
            // #else
            std::cout << "Road ID : " << newroad->id << std::endl;
            // #endif
          }
          if(line.find("Road_Length") != std::string::npos){
            // Create and add new road;
            double length = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->length = length;
            // #else
            std::cout << "Length : "<<length<< std::endl;
            // #endif
          }
          if(line.find("Road_Width") != std::string::npos){
            // Create and add new road;
            double width = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->width = width;
            // #else
            std::cout << "Width : "<<width<< std::endl;
            // #endif
          }
          if(line.find("Road_Lanes") != std::string::npos){
            // Create and add new road;
            int lanes = std::atoi(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->lanes = lanes;
            // #else
            std::cout << "Lanes : "<<lanes<< std::endl;
            // #endif
          }
          if(line.find("Road_Signal") != std::string::npos){
            // Create and add new road;
            double signal = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->signalPosition = signal;
            // #else
            std::cout << "Signal : "<<signal<< std::endl;
            // #endif
          }

          // Default Params
          if(line.find("Default_MaxSpeed") != std::string::npos){
            // Create and add new road;
            double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->default_maxspeed = maxsp;
            // #else
            std::cout << "Maxspeed : "<<maxsp<< std::endl;
            // #endif
          }
          if(line.find("Default_Acceleration") != std::string::npos){
            // Create and add new road;
            double acc = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            model.back()->default_acceleration = acc;
            // #else
            std::cout << "Acceleration : "<<acc<< std::endl;
            // #endif
          }
          // if(line.find("Default_Type") != std::string::npos){
          //   // Create and add new road;
          //   std::string type = preprocess(line.substr(line.find("=") + 1));
          //   // #ifdef IMPL
          //   model.back()->default_type = type;
          //   // #else
          //   std::cout << "Type : "<<type<< std::endl;
          //   // #endif
          // }
          if(line.find("Default_Skill") != std::string::npos){
            // Create and add new road;
            int skill = std::atoi(line.substr(line.find("=") + 1).c_str());
            if(skill < 0){
              skill = 0;
            }
            if( skill > 2){
              skill =2 ;
            }
            // #ifdef IMPL
            model.back()->default_skill = skill;
            // #else
            std::cout << "Skill : "<<skill<< std::endl;
            // #endif
          }
          if(line.find("Vehicle_Type") != std::string::npos){
            // Create and add new road;
            std::string vtype = preprocess(line.substr(line.find("=") + 1));
            // #ifdef IMPL
            Vehicle* newVehicle = new Vehicle(vtype);
            vehicles.push_back(newVehicle);
            // #else
            std::cout << "New Vehicle Type : "<<vtype<< std::endl;
            // #endif
          }
          if(line.find("Vehicle_Length") != std::string::npos){
            // Create and add new road;
            double length = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            vehicles.back()->length  = length;
            // #else
            std::cout << "Vehicle Length : "<<length<< std::endl;
            // #endif
          }
          if(line.find("Vehicle_Width") != std::string::npos){
            // Create and add new road;
            double width = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            vehicles.back()->width  = width;
            // #else
            std::cout << "Vehicle width : "<<width<< std::endl;
            // #endif
          }
          if(line.find("Vehicle_MaxSpeed") != std::string::npos){
            // Create and add new road;
            double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            vehicles.back()->maxspeed = maxsp;
            // #else
            std::cout << "Vehicle maxspeed : "<<maxsp<< std::endl;
            // #endif
          }
          if(line.find("Vehicle_Acceleration") != std::string::npos){
            // Create and add new road;
            double acc = std::atof(line.substr(line.find("=") + 1).c_str());
            // #ifdef IMPL
            vehicles.back()->acceleration = acc;
            // #else
            std::cout << "Vehicle Acceleration : "<<acc<< std::endl;
            // #endif
          }
          // CHANGE MODE
          if(line.find("START") != std::string::npos){
            defmode = false;
          }
        }
        else{
          // Catch END statement
          if(line.find("END") != std::string::npos){
            break;
          }
          // For simulation
          std::string delimiter = ";";
          std::vector<std::string> tokens;
          bool roadSpecified = false;
          int road_id;
          int pos=0;
          // #ifdef IMPL
          Road* road;
          // #endif
          // Extract tokens from string
          while ((pos = line.find(delimiter)) != std::string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
          }
          // No Tokens found
          if(tokens.size()<1){
            std::cout<<"[ ERROR ] Please follow the config file syntax!" <<std::endl;
            std::exit(1);
          }
          // Check if road defined
          if(tokens[0].find("Road") != std::string::npos){
            roadSpecified = true;
            road_id = std::atoi(tokens[0].substr(tokens[0].find("=") + 1).c_str());
            // #ifdef IMPL
              for(int num = 0; num < model.size(); num++){
                if(model[num]->id == road_id){
                  road = model[num];
                  break;
                }
              }
              if(road == NULL){
                std::cout <<"[ ERROR ] No Road with id "<<road_id<<" exists" <<std::endl;
                std::exit(1);
              }
            // #else
            std::cout<<"Road Id: "<<road_id<<std::endl;
            // #endif
            tokens.erase(tokens.begin());
          }
          if(roadSpecified){
            // #ifdef IMPL
              simulationActions(road,vehicles,tokens);
            // #else
              // std::cout<<"Functions are going on with road" <<road_id<<std::endl;
              // simulationActions(tokens);
            // #endif
          }
          if(!roadSpecified){
            // #ifdef IMPL
              if(model.size()<1){
                std::cout<<"[ ERROR ] No Roads exist"<<std::endl;
                std::exit(1);
              }
              simulationActions(model.back(),vehicles,tokens);
            // #else
              // std::cout<<"Functions are going on with latest road added"<<std::endl;
              // simulationActions(tokens);
            // #endif
          }
          // std::cout << "* * * * * * * * * ~ ~ ~ ~ SIMULATION ~ ~ ~ ~ * * * * * * * * *"<<std::endl;
        }
      }
    }

    // For each road in model, terminate the Road
    for(auto road: model) {
        road->engine.endSim();
    }

    std::cout<<"* * * * * * * * * ~ ~ ~ ~ ~ THEEND ~ ~ ~ ~ ~ * * * * * * * * *"<<std::endl;
  }
}

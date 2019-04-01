#include <bits/stdc++.h>
#include "Vehicle.h"
#include "Road.h"
typedef std::vector < Road * > Model;
typedef std::vector < Vehicle * > vv;

// Preprocesses a line
std::string preprocess(std::string a) {
  std::string ans = "";
  for (int i = 0; i < a.length(); i++) {
    if (a[i] == '#')
      break;

    if (a[i] >= 65 && a[i] <= 90) {
      ans += (a[i] + 32);
    }

    if (a[i] >= 97 && a[i] <= 122) {
      ans += a[i];
    } else {
      continue;
    }
  }
  return ans;
}

// Runs the simulation
void simulationActions(
  Road * road,
  vv vehicles,
  std::vector < std::string > tokens
) {
  double delT = 0;
  for (int i = 0; i < tokens.size(); i++) {
    // Parse the value and function
    std::string
    function = tokens[i].substr(0, tokens[i].find("="));
    std::string value = tokens[i].substr(tokens[i].find("=") + 1);

    // Signal change routine
    if (! function.compare("Signal")) {
      road -> setSignal(value);
      std::cout << "Road Signal = " << value << std::endl;
      continue;
    }

    // Passing time routine
    if (! function.compare("Pass")) {
      double time = std::atof(value.c_str());
      delT += time;
      std::cout << "Pass time = " << time << std::endl;
      continue;
    }

    // Addition of vehicles routine
    bool found = false;
    for (int v = 0; v < vehicles.size(); v++) {
      if (!vehicles[v] -> type.compare(preprocess(function))) { // prepnrocessing to ignore any fuss due to Capitals
        // std::cout << "ADDING " << vehicles[v]->type << std::endl;
        road -> addVehicle(vehicles[v], value);
        found = true;
        break;
      }
    }

    if (found) continue;

    {
      std::cout << "No function defined for " << function << " with value " << value << std::endl;
      std::exit(1);
    }
  }

  if (delT > 0) {
    // Run the simulation
    std::cout << "Running Simulation with ΔT = " << delT << std::endl;
    road->runSim(delT);
  }
}
int main(int argc, char ** argv) {
  // Add a check here
  std::ifstream configFile;
  configFile.open(argv[1]);
  if (configFile.fail()) {
    std::cout << "[ ERROR ] File is corrupted/not found." << std::endl;
    std::exit(1);
  } else {
    bool defmode = true;
    // Models are a vector of roads
    Model model;
    vv vehicles;
    std::string line;
    int num_rules = 0, safety_skill;
    double safety_maxspeed, safety_acceleration, safety_length, safety_width, safety_lanes, safety_distance;
    while (std::getline(configFile, line)) {
      if (!line.length()) continue; // IGN empty
      if (line[0] == '#') continue; // IGN with #
      {
        if (defmode) {
          // All definitions go here
          if (line.find("Safety_MaxSpeed") != std::string::npos) {
            // Create and add new road;
            safety_maxspeed = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_MaxSpeed : " << safety_maxspeed << std::endl;
          }
          if (line.find("Safety_Acceleration") != std::string::npos) {
            // Create and add new road;
            safety_acceleration = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_Acceleration : " << safety_acceleration << std::endl;
          }
          if (line.find("Safety_Length") != std::string::npos) {
            // Create and add new road;
            safety_length = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_Length : " << safety_length << std::endl;
          }
          if (line.find("Safety_Width") != std::string::npos) {
            // Create and add new road;
            safety_width = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_width : " << safety_width << std::endl;
          }
          if (line.find("Safety_Skill") != std::string::npos) {
            // Create and add new road;
            safety_skill = std::atoi(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_Skill : " << safety_skill << std::endl;
          }
          if (line.find("Safety_Lanes") != std::string::npos) {
            // Create and add new road;
            safety_lanes = std::atoi(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_Lanes : " << safety_skill << std::endl;
          }

          if (line.find("Safety_Distance") != std::string::npos) {
            // Create and add new road;
            safety_distance = std::atof(line.substr(line.find("=") + 1).c_str());
            num_rules++;
            std::cout << "Safety_Acceleration : " << safety_acceleration << std::endl;
          }

          if (line.find("Road_Id") != std::string::npos) {
            // Create and add new road;
            if (num_rules != 7) {
              std::cout << "[ ERROR ] Please Specify all the rules." << std::endl;
              std::exit(1);
            }
            int id = std::atoi(line.substr(line.find("=") + 1).c_str());
            Road * newroad = new Road(id);
            newroad -> setDefaults(safety_maxspeed, safety_acceleration, safety_length, safety_width, safety_skill, safety_distance);
            model.push_back(newroad);
            model.back() -> initLanes(safety_lanes);
            std::cout << "Road ID : " << newroad -> id << std::endl;
          }

          if (line.find("Road_Length") != std::string::npos) {
            // Create and add new road;
            double length = std::atof(line.substr(line.find("=") + 1).c_str());
            model.back() -> length = length;
            model.back() -> engine.initializeMap();
            std::cout << "Length : " << length << std::endl;
          }

          if (line.find("Road_Width") != std::string::npos) {
            // Create and add new road;
            double width = std::atof(line.substr(line.find("=") + 1).c_str());
            model.back() -> width = width;
            model.back() -> engine.initializeMap();
            std::cout << "Width : " << width << std::endl;
          }

          if (line.find("Road_Lanes") != std::string::npos) {
            // Create and add new road;
            int lanes = std::atoi(line.substr(line.find("=") + 1).c_str());
            model.back() -> initLanes(lanes);
            std::cout << "Lanes : " << lanes << std::endl;
          }

          if (line.find("Road_Signal") != std::string::npos) {
            // Create and add new road;
            double signal = std::atof(line.substr(line.find("=") + 1).c_str());
            model.back() -> signalPosition = signal;
            std::cout << "Signal : " << signal << std::endl;
          }

          // Default Params
          if (line.find("Default_MaxSpeed") != std::string::npos) {
            double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
            model.back() -> default_maxspeed = maxsp;
            std::cout << "Maxspeed : " << maxsp << std::endl;
          }

          if (line.find("Default_Acceleration") != std::string::npos) {
            // Create and add new road;
            double acc = std::atof(line.substr(line.find("=") + 1).c_str());
            model.back() -> default_acceleration = acc;
            std::cout << "Acceleration : " << acc << std::endl;
          }

          if (line.find("Default_Skill") != std::string::npos) {
            // Create and add new road;
            int skill = std::atoi(line.substr(line.find("=") + 1).c_str());
            if (skill < 0) {
              skill = 0;
            }
            if (skill > 2) {
              skill = 2;
            }
            model.back() -> default_skill = skill;
            std::cout << "Skill : " << skill << std::endl;
          }

          if (line.find("Vehicle_Type") != std::string::npos) {
            // Create and add new road;
            std::string vtype = preprocess(line.substr(line.find("=") + 1));
            Vehicle * newVehicle = new Vehicle(vtype);
            vehicles.push_back(newVehicle);
            std::cout << "New Vehicle Type : " << vtype << std::endl;
          }

          if (line.find("Vehicle_Length") != std::string::npos) {
            // Create and add new road;
            double length = std::atof(line.substr(line.find("=") + 1).c_str());
            vehicles.back() -> length = length;
            std::cout << "Vehicle Length : " << length << std::endl;
          }

          if (line.find("Vehicle_Width") != std::string::npos) {
            // Create and add new road;
            double width = std::atof(line.substr(line.find("=") + 1).c_str());
            vehicles.back() -> width = width;
            std::cout << "Vehicle width : " << width << std::endl;
          }

          if (line.find("Vehicle_MaxSpeed") != std::string::npos) {
            // Create and add new road;
            double maxsp = std::atof(line.substr(line.find("=") + 1).c_str());
            vehicles.back() -> maxspeed = maxsp;
            std::cout << "Vehicle maxspeed : " << maxsp << std::endl;
          }

          if (line.find("Vehicle_Acceleration") != std::string::npos) {
            // Create and add new road;
            double acc = std::atof(line.substr(line.find("=") + 1).c_str());
            vehicles.back() -> acceleration = acc;
            std::cout << "Vehicle Acceleration : " << acc << std::endl;
          }
          if (line.find("Vehicle_SafetyDistance") != std::string::npos) {
            // Create and add new road;
            double sdistance = std::atof(line.substr(line.find("=") + 1).c_str());
            vehicles.back() -> safedistance = sdistance;
            std::cout << "Vehicle Safety Distance : " << sdistance << std::endl;
          }
          // CHANGE MODE
          if (line.find("START") != std::string::npos) {
            defmode = false;
          }
        } else {
          // Catch END statement
          if (line.find("END") != std::string::npos) {
            break;
          }
          // For simulation
          std::string delimiter = ";";
          std::vector < std::string > tokens;
          bool roadSpecified = false;
          int road_id;
          int pos = 0;

          Road * road;

          // Extract tokens from string
          while ((pos = line.find(delimiter)) != std::string::npos) {
            tokens.push_back(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
          }
          // No Tokens found
          if (tokens.size() < 1) {
            std::cout << "[ ERROR ] Please follow the config file syntax!" << std::endl;
            std::exit(1);
          }
          // Check if road defined
          if (tokens[0].find("Road") != std::string::npos) {
            roadSpecified = true;
            road_id = std::atoi(tokens[0].substr(tokens[0].find("=") + 1).c_str());
            for (int num = 0; num < model.size(); num++) {
              if (model[num] -> id == road_id) {
                road = model[num];
                break;
              }
            }
            if (road == NULL) {
              std::cout << "[ ERROR ] No Road with id " << road_id << " exists" << std::endl;
              std::exit(1);
            }
            std::cout << "Road Id: " << road_id << std::endl;
            tokens.erase(tokens.begin());
          }
          if (roadSpecified) {
            simulationActions(road, vehicles, tokens);
          }

          if (!roadSpecified) {
            if (model.size() < 1) {
              std::cout << "[ ERROR ] No Roads exist" << std::endl;
              std::exit(1);
            }
            simulationActions(model.back(), vehicles, tokens);
          }
         }
      }
    }

    // For each road in model, terminate the Road
    for (auto road: model) {
      road -> engine.endSim();
    }

    std::cout << "* * * * * * * * * ~ ~ ~ ~ ~ THEEND ~ ~ ~ ~ ~ * * * * * * * * *" << std::endl;
  }
}

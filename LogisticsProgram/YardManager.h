//
// Created by qinusty on 15/11/16.
//

#ifndef Server_YardManager_H
#define Server_YardManager_H

#include <map>
#include <ostream>
#include "vehicle.h"
#include "siding.h"

class YardManager {
private:
    std::vector<siding*> sidings;

    bool* points = NULL;
    void loadWagons(std::string wagonConfigPath, std::vector<vehicle*>& wagons);
    void loadSidings(std::string sidingConfigPath, std::vector<vehicle*>& wagons);
    void loadPoints();
public:
    YardManager(std::string sidingConfigPath, std::string wagonConfigPath);
    void take(int sidingNo, int num);
    void put(int sidingNo, int num);
    virtual ~YardManager();
    std::string getConfigCommand();

    std::string getLoadCommand();


    friend std::ostream &operator<<(std::ostream &os, const YardManager &manager);

    void updatePoints(std::string &response);
};


#endif //SERVER_YardManager_H

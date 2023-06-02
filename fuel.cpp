#include "fuel.h"

// Name: FuelSys() Default Constructor
// Desc - Instantiates an object of type FuelSys, sets m_current null ptr
// Preconditions - None
// Postconditions - None
FuelSys::FuelSys(){
    m_current = nullptr;
}
// Name: FuelSys() Destructor
// Desc - deallocates everything than set's m_current == nullptr
// Preconditions - None
// Postconditions - None
FuelSys::~FuelSys(){
    if (m_current != nullptr) {
        clearTank();
    }
    m_current = nullptr;
}
// Name: FuelSys()addTank
// Desc - adds tanks in a circular linked list
// Preconditions - need tankID, tankCap, and totalFuel
// Postconditions - Adds all the tanks to the circular linked list
bool FuelSys::addTank(int tankID, int tankCap, int tankFuel = 0) {
    if (tankCap >= MINCAP) {
        if(tankFuel >= 0) {
            if(tankFuel <= tankCap) {
                if (tankID >= 0) {
                    if (m_current != nullptr) {
                        Tank *tempTank = m_current->m_next;
                        while (tempTank != m_current) {
                            if (tempTank->m_tankID == tankID) { // checks if tankID already exist
                                return false;
                            }
                            tempTank = tempTank->m_next;
                        }
                        int iD = tankID;
                        int cap = tankCap;
                        int fuel = tankFuel;
                        //sets all the values also sets next Tank to m_current->m_next
                        m_current->m_next = new Tank(iD, cap, fuel, nullptr, m_current->m_next);
                        m_current = m_current->m_next;
                        return true;
                    } else {
                        int iD = tankID;
                        int cap = tankCap;
                        int fuel = tankFuel;
                        Tank *newTank = new Tank(iD, cap, fuel);
                        m_current = newTank;
                        m_current->m_next = m_current;
                        return true;
                    }
                } else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return false;
}
// Name: FuelSys() removeTank
// Desc - deletes all tanks but first deletes all pumps first so no memory leaks
// Preconditions - need tankID
// Postconditions - entire thing is empty if we so choose
bool FuelSys::removeTank(int tankID) {
    bool tankFound = findTank(tankID);

    if (tankFound) {
        Tank *tempTank = m_current->m_next;
        Tank *prevTank = m_current;

        if (m_current->m_next == m_current) { // only one tank in the circular linked list
            Pump *tempPump = tempTank->m_pumps;

            if (tempPump == nullptr) { // no pumps

                delete tempPump;
            } else if (tempPump->m_next == nullptr) { // one pump

                delete tempPump;
            } else {
                Pump *nextPump;
                while (tempPump != nullptr) { // multiple pumps
                    nextPump = tempPump->m_next;
                    removePump(m_current->m_next->m_tankID, tempPump->m_pumpID);
                    tempPump = nextPump;
                }
            }

            delete m_current;
            m_current = nullptr;
            return true;

        } else { // multiples tanks

            Pump *tempPump = tempTank->m_pumps;
            if (tempPump == nullptr) { // no pumps

            } else if (tempPump->m_next == nullptr) { // one pump
                delete tempPump;
                tempTank->m_pumps = nullptr;
            } else {
                Pump *nextPump;
                while (tempPump != nullptr) { // multiple pumps
                    nextPump = tempPump->m_next;
                    removePump(tempTank->m_tankID, tempPump->m_pumpID);
                    tempPump = nextPump;
                }
            }

            Tank *nextTank = tempTank->m_next;
            delete m_current->m_next;
            prevTank->m_next = nextTank;
            return true;
        }
    }
    return false;
}
// Name: FuelSys() findTank
// Desc - finds tank you looking for
// Preconditions - need tankID
// Postconditions - finds the tank that you want to find also m_current set to the one before
bool FuelSys::findTank(int tankID){

    if(m_current != nullptr) {
        Tank* tempTank = m_current->m_next;
        while(tempTank != m_current) {
            if (tempTank->m_next->m_tankID == tankID) {
                m_current = tempTank;
                return true;
            }

            tempTank = tempTank->m_next;
        }
        if (m_current->m_next->m_tankID == tankID) {
            return true;
        }
    }
    return false;
}

// Name: FuelSys() addPump
// Desc - add pumps to a specific tank
// Preconditions - need tankID, pumpID, targetTank
// Postconditions - adds all the pumps you want to each tank if you want
bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {
    if ((findTank(targetTank) && findTank(tankID)) && tankID != targetTank) {
            Tank *tempTank = m_current->m_next;
            while (tempTank->m_tankID != tankID) {
                tempTank = tempTank->m_next;
            }

            Pump *tempPump = tempTank->m_pumps;
            if (tempPump != nullptr) {
                while (tempPump != nullptr) {

                    if (tempPump->m_pumpID == pumpID) {
                        return false;
                    }
                    tempPump = tempPump->m_next;
                }
            }

            int pID = pumpID;
            int target = targetTank;
            tempTank->m_pumps = new Pump (pID, target, tempTank->m_pumps);
            return true;
        }

    return false;
}
// Name: FuelSys() remove tank
// Desc - removes tank at a specific spot
// Preconditions - need tankID, pumpID
// Postconditions - removes the pumps from a tank
bool FuelSys::removePump(int tankID, int pumpID){
    bool tankFound = findTank(tankID);

    if(tankFound) { // finds the tank you want, used alot
        Tank *currTank = m_current->m_next;
        while(currTank->m_tankID != tankID) {
            currTank = currTank->m_next;
        }

        Pump *tempPump = currTank->m_pumps;
        Pump *prevPump;

        if(tempPump == nullptr) {
            return false;
        }
        if (tempPump->m_next == nullptr) {
            if (tempPump->m_pumpID == pumpID) {
                delete currTank->m_pumps;
                currTank->m_pumps = nullptr;
                return true;
            }
        }
        prevPump = currTank->m_pumps;
        while(tempPump != nullptr) {
            if (tempPump->m_pumpID == pumpID) {

                if(prevPump == tempPump && prevPump == currTank->m_pumps){
                    m_current->m_next->m_pumps = tempPump->m_next;
                } else{
                    prevPump->m_next = tempPump->m_next;
                }

                delete tempPump;
                return true;
            }
            prevPump = tempPump;
            tempPump = tempPump->m_next;
        }
    }
    return false;
}
// Name: FuelSys() totalFuel
// Desc - gets the total fuel of all tanks
// Preconditions -
// Postconditions - gets the total fuel of all tanks
int FuelSys::totalFuel() const{
    int total_fuel = 0;

    if (m_current != nullptr) {

        Tank *tempTank = m_current->m_next;

        while (m_current != nullptr && tempTank != m_current) {
            total_fuel += tempTank->m_tankFuel;
            tempTank = tempTank->m_next;
        }
    }
    return total_fuel;
}
// Name: FuelSys() drain
// Desc - drains fromTankID and transfers fuel to TargetTank
// Preconditions -tankID, PumpID, fuel
// Postconditions - targetTank gets the fuel
bool FuelSys::drain(int tankID, int pumpID, int fuel){
    bool found = findPump(pumpID) && findTank(tankID);

        if(found) {
            Tank * tempTank = m_current->m_next;
            Pump * tempPump = m_current->m_pumps->m_next;

            if (tempTank->m_tankID == tankID && tempPump->m_pumpID == pumpID) {
                if(fuel > tempTank->m_tankCapacity) {
                    fuel = tempTank->m_tankFuel;
                    tempTank->m_tankFuel = 0;
                    fill(tempPump->m_target, fuel); // calls fill to transfer fuel
                    return true;
                }
                else if(fuel < tempTank->m_tankFuel){
                    tempTank->m_tankFuel = tempTank->m_tankFuel - fuel;
                    fill(tempPump->m_target, fuel);
                    return true;
                }
                else if(fuel > tempTank->m_tankFuel){
                    fuel = tempTank->m_tankFuel;
                    tempTank->m_tankFuel = 0;
                    fill(tempPump->m_target, fuel);
                    return true;
                }
                else {
                    tempTank->m_tankFuel = tempTank->m_tankFuel - fuel;
                    fill(tempPump->m_target, fuel);
                    return true;
                }
            }
            else {
            }
        }
    return false;
}
// Name: FuelSys() fill
// Desc - fills the tanks
// Preconditions - tankID, fuel
// Postconditions - fills the tankID with proper fuel
bool FuelSys::fill(int tankID, int fuel){
    bool tank_found = findTank(tankID);

    if(tank_found) {
        m_current->m_next->m_tankCapacity += fuel;
        if (m_current->m_next->m_tankFuel > m_current->m_next->m_tankCapacity){
            m_current->m_next->m_tankFuel = m_current->m_next->m_tankCapacity;
        }
        return true;
    }
    return false;
}
// Name: FuelSys() overload assignment operator
// Desc - makes a deep copy
// Preconditions -
// Postconditions - deallocates memory with cleartank than adds the tanks and pumps making a deep copy
const FuelSys & FuelSys::operator=(const FuelSys & rhs){
    Tank *tempTank;
    if(&rhs != this) {
        if (m_current != nullptr) {
            clearTank();
             m_current = nullptr;
        }
    }
        // creates tank 1 and 3 with pumps but not 2
        tempTank = rhs.m_current->m_next;
        if (rhs.m_current != nullptr) {

            while(rhs.m_current != nullptr && tempTank != rhs.m_current){

                addTank(tempTank->m_tankID, tempTank->m_tankCapacity, tempTank->m_tankFuel);

                Pump *tempPump = tempTank->m_pumps;
                Pump *currPump = m_current->m_pumps;
                while (tempPump != nullptr) {
                    Pump *newPump = new Pump(tempPump->m_pumpID, tempPump->m_target);

                    if(currPump == nullptr) {
                        m_current->m_pumps = newPump;
                        currPump = newPump;
                    }
                    else {
                        currPump->m_next = newPump;
                        currPump = newPump;
                    }

                    tempPump = tempPump ->m_next;
                }

                tempTank = tempTank->m_next;
            }
            // adds tanks 2 and pumps, making it right
            addTank(tempTank->m_tankID, tempTank->m_tankCapacity, tempTank->m_tankFuel);
            Pump *tempPump = tempTank->m_pumps;
            Pump *currPump = m_current->m_pumps;
            while (tempPump != nullptr) {
                Pump *newPump = new Pump(tempPump->m_pumpID, tempPump->m_target);

                if(currPump == nullptr) {
                    m_current->m_pumps = newPump;
                    currPump = newPump;
                }
                else {
                    currPump->m_next = newPump;
                    currPump = newPump;
                }

                tempPump = tempPump ->m_next;
            }
        }

    return *this;
}


void FuelSys::dumpSys() const{
    if (m_current != nullptr){
        Tank* tempTank = m_current->m_next;//we start at front item
        //we continue visting nodes until we reach the cursor
        while(m_current != nullptr && tempTank != m_current){
            cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel << " kg)" << endl;
            // now dump the targets for all pumps in this tank
            // we need to traverse the list of pumps
            dumpPumps(tempTank->m_pumps);
            tempTank = tempTank->m_next;
        }
        //at the end we visit the cursor (current)
        //this also covers the case that there is only one item
        cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel << " kg)" << endl;
        dumpPumps(tempTank->m_pumps);
        cout << "The current tank is " << m_current->m_tankID << endl;
    }
    else
        cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump* pumps) const{
    // this traverses the linked list to the end
    Pump* tempPump = pumps;
    while (tempPump != nullptr){
        cout << " => pump " << tempPump->m_pumpID << "(To tank " << tempPump->m_target << ")" << endl;
        tempPump = tempPump->m_next;
    }
}
// Name: FuelSys() clear tank
// Desc - clears everything, for destructor
// Preconditions -
// Postconditions - everything is deallocated
void FuelSys::clearTank() { // clears all tanks, calls clears pump to clear pumps to, ultimate clear
    while(m_current != nullptr) {
        removeTank(m_current->m_tankID);
    }
}

// Name: FuelSys() findPumps
// Desc - find the pump you looking for
// Preconditions - pumpID
// Postconditions - can't use findTank for pumps so create helper function to find it
bool FuelSys::findPump(int pumpID) {

    if(m_current != nullptr) {
        Pump* tempPump = m_current->m_next->m_pumps;
        while(tempPump != m_current->m_pumps) {

            if (tempPump->m_next->m_pumpID == pumpID) {
                m_current->m_pumps = tempPump;
                return true;
            }
            tempPump = tempPump->m_next;
        }
        if (m_current->m_pumps->m_pumpID == pumpID) {
            return true;
        }
    }
    return false;
}
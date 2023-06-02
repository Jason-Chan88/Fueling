#include "fuel.h"
#include <random>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};
class Tester{
public:

    bool testAddTankNormal(){ // creates 3 tanks with different values and check that they exist with checking values
        FuelSys sys;

        bool result = true;

        result = sys.addTank(1, 4500, 2500);
        result = result && sys.addTank(2,7000,5000);
        result = result && sys.addTank(3,2000,1);


        result = result && sys.findTank(1);
        result = result && sys.m_current->m_next->m_tankID == 1;
        result = result && sys.m_current->m_next->m_tankCapacity == 4500;
        result = result && sys.m_current->m_next->m_tankFuel == 2500;

        result = result && sys.findTank(2);
        result = result && sys.m_current->m_next->m_tankID == 2;
        result = result && sys.m_current->m_next->m_tankCapacity == 7000;
        result = result && sys.m_current->m_next->m_tankFuel == 5000;

        result = result && sys.findTank(3);
        result = result && sys.m_current->m_next->m_tankID == 3;
        result = result && sys.m_current->m_next->m_tankCapacity == 2000;
        result = result && sys.m_current->m_next->m_tankFuel == 1;

        return result;
    }

    bool testAddTankError(FuelSys &sys){ // test if -1 tankID would work, should return false
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=2;i<=numTanks;i++){
            result = result && (sys.addTank(2000,DEFCAP,randFuel.getRandNum()));
        }
        sys.dumpSys();
        return result;
    }

    bool testRemoveTankNormal(FuelSys &sys) { // removes all the tanks from the list, and checks they are gone
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }
        result = result && (sys.removeTank(1));
        result = result && (sys.removeTank(2));
        result = result && (sys.removeTank(3));

        result = result &&(sys.m_current == nullptr);

        return result;
    }

    bool testRemoveTankError(FuelSys &sys) { // tries to remove tank 0 that doesn't exist
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }
        result = result && (sys.removeTank(0));

        return result;
    }

    bool testFindTankNormal(FuelSys &sys) { // tries and find a tank in the list
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }
        result = result && (sys.findTank(2));

        return result;
    }

    bool testFindTankError(FuelSys &sys) { // tries to find a tank that's not in the list
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }
        result = result && (sys.findTank(4));

        return result;
    }

    bool testTotalFuelNormal(FuelSys &sys) { // see's if it returns the same value
        int result;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            sys.addTank(i,DEFCAP,randFuel.getRandNum());
        }
        result = sys.totalFuel();

        return result;
    }

    bool testTotalFuelError(FuelSys &sys) { // if it returns 0 which is an error case
        int result;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        result = sys.totalFuel();

        return result;
    }

    bool testAddPumpNormal(FuelSys &sys) { // adds the pumps to the tanks like the driver test
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,2,randTank.getRandNum()));
        }

        return result;
    }

    bool testAddPumpError(FuelSys &sys) { // adding pumps with the same id shouldn't work
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
        }

        return result;
    }

    bool testRemovePumpNormal(FuelSys &sys) { //removes the pumps in the tanks
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,2,randTank.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++) {
            result = result && (sys.removePump(i, 1));
            result = result && (sys.removePump(i, 2));
        }
        result = result && (sys.m_current->m_pumps == nullptr);
        return result;
    }

    bool testRemovePumpError(FuelSys &sys) { // removing from a non-existent tank aka tank 4
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,2,randTank.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++) {
            result = result && (sys.removePump(4, 1));
            result = result && (sys.removePump(4, 2));
        }

        return result;
    }

    bool testDrainNormal(FuelSys &sys) { // test drain with tank fuel being above tank capacity
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,2,randTank.getRandNum()));
        }

        result = result && (sys.drain(1,1,10000));

        return result;
    }

    bool testDrainError(FuelSys &sys) { // trying to use drain on a tank that doesn't exist tank 4
        bool result = true;
        int numTanks = 3;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=numTanks;i++){
            result = result && (sys.addPump(i,1,randTank.getRandNum()));
            result = result && (sys.addPump(i,2,randTank.getRandNum()));
        }

        result = result && (sys.drain(4,1,1000));

        return result;
    }

    bool testAssignmentOperatorNormal() { // copies contents of other to sys
        FuelSys sys;
        FuelSys other;

        bool result = true;
        int numTanks = 6;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=3;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=4;i<=6;i++){
            result = result && (other.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=3;i++){
            (sys.addPump(i,1,randTank.getRandNum()));

            (sys.addPump(i,2,randTank.getRandNum()));

        }

        for (int i=3;i<=6;i++){
            (other.addPump(i,1,randTank.getRandNum()));
            (other.addPump(i,2,randTank.getRandNum()));
        }
        sys = other;
        result = result && (sys.m_current->m_tankID == other.m_current->m_tankID);


        return result;
    }

    bool testAssignmentOperatorErros() { // copies contents of other to sys
        FuelSys sys;
        FuelSys other;

        bool result = true;
        int numTanks = 6;
        Random randFuel(MINCAP,DEFCAP);
        Random randTank(1,numTanks);

        for (int i=1;i<=3;i++){
            result = result && (sys.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=4;i<=6;i++){
            result = result && (other.addTank(i,DEFCAP,randFuel.getRandNum()));
        }

        for (int i=1;i<=3;i++){
            (sys.addPump(i,1,randTank.getRandNum()));

            (sys.addPump(i,2,randTank.getRandNum()));

        }

        for (int i=3;i<=6;i++){
            (other.addPump(i,1,randTank.getRandNum()));
            (other.addPump(i,2,randTank.getRandNum()));
        }
        sys = sys;

        result = result;


        return result;
    }

};
int main(){
    Tester tester;

    // add tank
    FuelSys sys;
    FuelSys sys2;

    //remove tank
    FuelSys sys3;
    FuelSys sys4;

    //find tank
    FuelSys sys5;
    FuelSys sys6;

    // total fuel
    FuelSys sys7;
    FuelSys sys8;

    // add pump
    FuelSys sys9;
    FuelSys sys10;

    // remove pump
    FuelSys sys11;
    FuelSys sys12;

    //drain
    FuelSys sys13;
    FuelSys sys14;

    // driver
    FuelSys sys20;

    if (tester.testAddTankNormal()) // should return true
        cout << "\ttestAddTankNormal() returned true." << endl;
    else
        cout << "\ttestAddTankNormal() returned false." << endl;

    if (tester.testAddTankError(sys2)) // should return false
        cout << "\ttestAddTankError() returned true." << endl;
    else
        cout << "\ttestAddTankError() returned false." << endl;

    if (tester.testRemoveTankNormal(sys3)) // should return true
        cout << "\ttestRemoveTankNormal() returned true." << endl;
    else
        cout << "\ttestRemoveTankNormal() returned false." << endl;

    if (tester.testRemoveTankError(sys4)) // should return false
        cout << "\ttestRemoveTankError() returned true." << endl;
    else
        cout << "\ttestRemoveTankError() returned false." << endl;

    if (tester.testFindTankNormal(sys5)) // should return true
        cout << "\ttestFindTankNormal() returned true." << endl;
    else
        cout << "\ttestFindTankNormal() returned false." << endl;

    if (tester.testFindTankError(sys6)) // should return false
        cout << "\ttestFindTankErrorl() returned true." << endl;
    else
        cout << "\ttestFindTankError() returned false." << endl;

    if (tester.testTotalFuelNormal(sys7)) // should return true
        cout << "\ttestTotalFuelNormal() returned true." << endl;
    else
        cout << "\ttestTotalFuelNormal() returned false." << endl;

    if (tester.testTotalFuelError(sys8)) // should return false
        cout << "\ttestTotalFuelError() returned true." << endl;
    else
        cout << "\ttestTotalFuelError() returned false." << endl;

    if (tester.testAddPumpNormal(sys9)) // should return true
        cout << "\ttestTAddPumpNormal() returned true." << endl;
    else
        cout << "\ttestAddPumpNormal() returned false." << endl;

    if (tester.testAddPumpError(sys10)) // should return false
        cout << "\ttestAddPumpError() returned true." << endl;
    else
        cout << "\ttestAddPumpError() returned false." << endl;

    if (tester.testRemovePumpNormal(sys11)) // should return true
        cout << "\ttestRemovePumpNormal() returned true." << endl;
    else
        cout << "\ttestRemovePumpNormal() returned false." << endl;

    if (tester.testRemovePumpError(sys12)) // should return false
        cout << "\ttestRemovePumpError() returned true." << endl;
    else
        cout << "\ttestRemovePumpError() returned false." << endl;

    if (tester.testDrainNormal(sys13)) // should return true
        cout << "\ttestDrainNormal() returned true." << endl;
    else
        cout << "\ttestDrainNormal() returned false." << endl;

    if (tester.testDrainError(sys14)) // should return false
        cout << "\ttestDrainError() returned true." << endl;
    else
        cout << "\ttestDrainError() returned false." << endl;

    if (tester.testAssignmentOperatorNormal()) // should return true
        cout << "\ttestAssignmentOperatorNormal() returned true." << endl;
    else
        cout << "\ttestAssignmentOperatorNormal() returned false." << endl;



    // driver test
    int numTanks = 3;
    Random randFuel(MINCAP,DEFCAP);
    Random randTank(1,numTanks);


    for (int i=1;i<=numTanks;i++){
       sys20.addTank(i,DEFCAP,randFuel.getRandNum());

    }
    cout << "After creating tanks: \n\n"; sys20.dumpSys();cout << endl;
    for (int i=1;i<=numTanks;i++){
        sys20.addPump(i,1,randTank.getRandNum());
        sys20.addPump(i,2,randTank.getRandNum());
    }
    cout << "After adding pumps: \n\n"; sys20.dumpSys();cout << endl;
     //can't remove the second pump by itself but works when you remove with pump 1

    sys20.dumpSys();

    FuelSys concorde;
    concorde = sys20;
    cout << "Dumping Concorde (after using assignment operator): \n\n"; concorde.dumpSys();cout << endl;
    return 0;
}
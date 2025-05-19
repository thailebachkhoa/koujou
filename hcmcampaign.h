/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};


// dlinked list
template <typename T> // class unitnode
class Node
{
public:
    T data;
    Node *next;

    Node(T data = 0, Node *next = 0) : data(data), next(next) {}
    void add(T data)
    {
        Node *newNode = new Node(data);
        if (next == nullptr)
        {
            next = newNode;
        }
        else
        {
            Node *temp = next;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    int size()
    {
        int count = 0;
        Node *temp = this;
        while (temp != nullptr)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

template <typename T>
void deleteList(Node<T> *node)
{
    while (node)
    {
        Node<T> *temp = node;
        node = node->next;
        delete temp;
    }
}

// classify Unit
enum UnitClassify
{
    VEHICLE,
    INFANTRY
};

// just copy army
class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;
    // vector<pair<double (*)(Unit*), Unit*>> EXPBonuses;
    // vector<pair<double (*)(Unit*), Unit*>> LFPenalties;
public:
    virtual void fightIfDefense(Army *enemy) = 0;
    virtual void fightIfAttack(Army *enemy) = 0;
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false);
    virtual string str() const = 0;

    // getter, setter
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }
    UnitList *getUnitList() const { return unitList; }
    BattleField *getBattleField() const { return battleField; }
    void setLF(double LF)
    {
        int temp = ceil(LF);
        this->LF = max(0, min(1000, temp));
    }
    void setEXP(double EXP)
    {
        int temp = ceil(EXP);
        this->EXP = max(0, min(500, temp));
    }

    void resetLF_EXP();
    string getName() const { return name; }
    
 
    // tesh
    
    // void calculateEXPAndLF(bool withEffect = true);

    // bool removeUnit(Unit *unit, bool destroyUnit = false);
    // bool addUnit(Unit *unit);

    // void addEXPBonus(double (*fx)(Unit* unit), Unit* unitArg);
    // void addLFPenalty(double (*fx)(Unit* unit), Unit* unitArg);

    // UnitNode *getHead();

    // /**
    //  * Update each Unit by return value of cb and auto ceiling
    //  */
    // void updateQuantityEach(double (*cb)(int quantity));
    // /**
    //  * Update each Unit by return value of cb and auto ceiling
    //  */
    // void updateWeightEach(double (*cb)(int quantity)); 
   
};
class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
        : Army(unitArray, size, name, battleField) {}

    void fightIfAttack(Army *enemy);
    void fightIfDefense(Army *enemy);

    string str() const override;
};
class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
        : Army(unitArray, size, name, battleField) {}

    void fightIfAttack(Army *enemy){}
    void fightIfDefense(Army *enemy){}

    string str() const override;
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) : r(r), c(c) {}
    Position(const string &str_pos)
    {
        // format: (x, y)
        string temp = str_pos.substr(1, str_pos.size() - 2);
        // step 2: split by ,
        size_t pos = temp.find(",");
        string x = temp.substr(0, pos);
        string y = temp.substr(pos + 1);
        // step 3: convert to int
        this->r = stoi(x);
        this->c = stoi(y);
    } // Example: str_pos = "(1,15)"
    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }
    string str() const
    {
        stringstream ss;
        ss << "(" << r << "," << c << ")";
        return ss.str();
    } // Example: returns "(1,15)"
    Position operator&=(const Position &other) const
    {
        return Position(r & other.r, c & other.c);
    }
};

// Unit group
class Unit
{
protected:
    int quantity, weight;
    Position pos;
    UnitClassify unitClassify;
    int tempAttackScore;
public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    Position getCurrentPosition() const;
    // phương thức ảo
    virtual int getAttackScore() = 0;
    virtual string str() const = 0;

    // getter, setter
    void setQuantity(double quantity) { this->quantity = ceil(quantity); }
    void setWeight(double weight) { this->weight = ceil(weight); }
    void setPos(Position pos) { this->pos = pos; }
    int getQuantity() const { return quantity; }
    int getWeight() const { return weight; }
    Position getPos() const { return pos; }
    int getTempAttackScore() const { return tempAttackScore; }
    void setTempAttackScore(double tempAttackScore) { this->tempAttackScore = ceil(tempAttackScore); }
    UnitClassify getUnitClassify() const { return unitClassify; }
};
class Vehicle : public Unit
{
private:
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
        : Unit(quantity, weight, pos), vehicleType(vehicleType) { this->unitClassify = VEHICLE;
             tempAttackScore = this->getAttackScore(); }
    int getAttackScore() override;
    string str() const override;
    // getter, setter
    VehicleType getVehicleType() const { return vehicleType; }

};
class Infantry : public Unit
{
private:
    InfantryType infantryType;

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
        : Unit(quantity, weight, pos), infantryType(infantryType) { unitClassify = INFANTRY;
             tempAttackScore = this->getAttackScore(); }
    int getAttackScore() override;
    string str() const override;
    // getter, setter
    InfantryType getInfantryType() const { return infantryType; }
};

class UnitList
{
private:
    int capacity;
    int count_vehicle;
    int count_infantry;
    vector<Unit *> array; // array of unit
    Army *holder_of_this; // owner of this list
    Unit* findUnit(Unit *unit) // return the unit same type with parameter unit
    { 
        for (int i = 0; i < capacity; i++)
        {
            if (!array[i]) continue;
            if (unit->getUnitClassify() == VEHICLE) {
                auto v1 = static_cast<Vehicle*>(unit);
                auto v2 = dynamic_cast<Vehicle*>(array[i]);
                if (v2 && v2->getVehicleType() == v1->getVehicleType())
                    return v2;
            } else {
                auto i1 = static_cast<Infantry*>(unit);
                auto i2 = dynamic_cast<Infantry*>(array[i]);
                if (i2 && i2->getInfantryType() == i1->getInfantryType())
                    return i2;
            }
        }
        return nullptr;
    }
    // TODO
public:
    UnitList(Army *army);
    bool insert(Unit *unit);                             // return true if insert successfully
    Unit *getVehicleViaType(VehicleType vehicleType);    // return true if it exists
    Unit *getInfantryViaType(InfantryType infantryType); // return true if it exists
    bool isContain(VehicleType vehicleType);             // return true if it exists
    bool isContain(InfantryType infantryType);           // return true if it exists
    string str() const;
    // TODO

    // getter, setter
    int getCapacity() const { return capacity; }
    int getCount() const { return count_vehicle + count_infantry; }
    void removeUnit(Unit *unit)
    {
        if (unit == nullptr)
            return;
        for (int i = 0; i < capacity; i++)
        {
            if (array[i] == unit)
            {
                delete array[i];
            }
        }
        for (int i = 0; i < capacity - 1; i++)
        {
            if (array[i] == nullptr)
            {
                array[i] = array[i + 1];
                array[i + 1] = nullptr;
            }
        }
        if (unit->getUnitClassify() == VEHICLE)
            count_vehicle--;
        else
            count_infantry--;
    }
    Unit *getUnit(int index) const
    { // đang fix
        return array[index];
    }
};

// battlefield group
// class TerrainElement
// {
// public:
//     TerrainElement();
//     ~TerrainElement();
//     virtual void getEffect(Army *army) = 0;
// };

/* Terrain group */ // jusst copy
class TerrainElement
{
protected:
    virtual void getEffectOnLibArmy(LiberationArmy *liberationArmy) = 0;
    virtual void getEffectOnARVN(ARVN *ARVN) = 0;
    Position pos;

public:
    TerrainElement(Position pos) : pos(pos) {}
    virtual ~TerrainElement() = default;
    void getEffect(Army *army)
    {
        army->getName() == "LiberationArmy"
            ? getEffectOnLibArmy(static_cast<LiberationArmy *>(army))
            : getEffectOnARVN(static_cast<ARVN *>(army));
    }
    Position getPos() const { return pos; }
    void setPos(Position pos) { this->pos = pos; }
};

class Road : public TerrainElement
{
public:
    Road(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override {}
    void getEffectOnARVN(ARVN *ARVN) override {}
};

class Mountain : public TerrainElement
{
public:
    Mountain(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class River : public TerrainElement
{
public:
    River(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class Urban : public TerrainElement
{
public:
    Urban(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class Fortification : public TerrainElement
{
public:
    Fortification(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
class SpecialZone : public TerrainElement
{
public:
    SpecialZone(Position pos) : TerrainElement(pos) {}

private:
    void getEffectOnLibArmy(LiberationArmy *liberationArmy) override;
    void getEffectOnARVN(ARVN *ARVN) override;
};
/* Terrain group */

// setup group
class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement ***terrain;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str();
    // getter, setter
    TerrainElement ***getTerrain() const { return terrain; }
    TerrainElement *getTerrainElement(int r, int c) const { return terrain[r][c]; }
    int getNumRows() const { return n_rows; }
    int getNumCols() const { return n_cols; }
};

class Configuration // chưa check
{
private:
    int num_rows, num_cols;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    Unit **liberationUnits, **ARVNUnits; 
    int count_liberUnit, count_ARVNUnit;
    int eventCode;
    // support function
    void extractUnits(string input, Node<Unit *> *list_UncludeHo, Node<Unit *> *list_3que);

public:
    Configuration(const string &config_file_path);
    ~Configuration();
    string str();
    // getter, setter
    int getNumRows() const { return num_rows; }
    int getNumCols() const { return num_cols; }

    vector<Position *> getArrayForest() const { return arrayForest; }
    vector<Position *> getArrayRiver() const { return arrayRiver; }
    vector<Position *> getArrayFortification() const { return arrayFortification; }
    vector<Position *> getArrayUrban() const { return arrayUrban; }
    vector<Position *> getArraySpecialZone() const { return arraySpecialZone; }

    Unit **getLiberationUnits() const { return liberationUnits; }
    Unit **getARVNUnits() const { return ARVNUnits; }
    int getEventCode() const { return eventCode; }
    int getliberUnitsSize() const { return count_liberUnit; }
    int getARVNUnitsSize() const { return count_ARVNUnit; }
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *VNCHARMY;
    // void afterfightMethod();
public:
    HCMCampaign(const string &config_file_path);
    ~HCMCampaign();
    void run();
    string printResult();
};

#endif
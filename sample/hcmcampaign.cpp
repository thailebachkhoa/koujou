#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

/*
Question 1: getattackscore in infantry, how calculate again, check the conditions ??
Question 2: army thay đổi chỉ số exp và lf thì unit có thay đổi\
2. Weight mình đã trả lời là lấy weight lớn nhất, pos giữ nguyên đơn vị đã tồn tại trong danh sách.https://lms.hcmut.edu.vn/mod/forum/discuss.php?d=26022

*/

// Implement the method LiberationArmy
void LiberationArmy::fightIfAttack(Army *enemy)
{
    this->setEXP(this->getEXP() * 1.5);
    this->setLF(this->getLF() * 1.5);
    double enemyLF = enemy->getLF(), enemyEXP = enemy->getEXP();

    bool victory = false;
    bool battle = false;
    sp::fightIfDefense_helper_LiberationArmy helper(this);
    vector<Vehicle *> vehicles = helper.combinationA();
    vector<Infantry *> infantries = helper.combinationB();

    if (vehicles.size() == 0 && infantries.size() == 0)
    {
        battle = false;
        victory = false;
    }
    else if (vehicles.size() != 0 && infantries.size() != 0)
    {
        victory = true;
        battle = true;
    }
    else
    {
        if (vehicles.size() != 0)
        {
            // what if vehicles.size() == 0
            if (vehicles.size() == 0)
            {
                int conbinators_point = sp::sumPointsofUnitList(infantries);
                vector<Infantry *> infantries_enemy = enemy->getUnitList()->getInfantries();
                int enemy_point = sp::sumPointsofUnitList(infantries_enemy);
                if (conbinators_point > enemy_point)
                {
                    victory = true;
                    battle = true;
                }
            }
            else
            {
                int conbinators_point = sp::sumPointsofUnitList(vehicles);
                vector<Vehicle *> vehicles_enemy = enemy->getUnitList()->getVehicles();
                int enemy_point = sp::sumPointsofUnitList(vehicles_enemy);
                if (conbinators_point > enemy_point)
                {
                    victory = true;
                    battle = true;
                }
            }
        }
    }

    if (!battle)
    {
        // giảm điểm nếu không giao tranh
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            unit->setWeight(unit->getWeight() * 0.9);
        }
        resetLF_EXP();
        return;
    }
    else
    {
        this->unitList->remove(vehicles);
        this->unitList->remove(infantries);
    }
    if (victory)
    {
        UnitList *enemyUnitList = enemy->getUnitList();
        // insert enemyUnitList to this->unitList
        for (int i = 0; i < enemyUnitList->getCount(); i++)
        {
            this->unitList->insert(enemyUnitList->getUnit(i));
        }
        resetLF_EXP();
    }
}
void LiberationArmy::fightIfDefense(Army *enemy)
{
    this->setEXP(this->getEXP() * 1.3);
    this->setLF(this->getLF() * 1.3);

    while (true)
    {
        if (this->EXP >= enemy->getEXP() && this->LF >= enemy->getLF())
            return;

        if (this->EXP < enemy->getEXP() && this->LF < enemy->getLF())
        {
            for (int i = 0; i < unitList->getCount(); i++)
            {
                if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
                {
                    Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
                    vehicle->setQuantity(sp::closestFibonacci(vehicle->getQuantity()));
                }
                else
                {
                    Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
                    infantry->setQuantity(sp::closestFibonacci(infantry->getQuantity()));
                }
            }
            continue;
        }
        else
        {
            for (int i = 0; i < unitList->getCount(); i++)
            {
                if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
                {
                    Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
                    vehicle->setQuantity(vehicle->getQuantity() * 0.9);
                }
                else
                {
                    Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
                    infantry->setQuantity(infantry->getQuantity() * 0.9);
                }
            }
            break;
        }
    }
}
string LiberationArmy::str() const
{
    // format: LiberationArmy[name=<name>, LF=<LF>, EXP=<EXP>]
    stringstream ss;
    ss << "LiberationArmy[name=" << name
       << ",LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << unitList->str()
       << ",battleField=" << battleField->str()
       << "]";
    return ss.str();
}
// Implement the method ARVN
void ARVN::fightIfAttack(Army *enemy)
{
    for (int i = 0; i < unitList->getCount(); i++)
    {
        if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
            vehicle->setQuantity(vehicle->getQuantity() * 0.8);
            if (vehicle->getQuantity() == 1)
            {
                unitList->remove(vehicle);
            }
        }
        else
        {
            Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
            infantry->setQuantity(infantry->getQuantity() * 0.8);
            if (infantry->getQuantity() == 1)
            {
                unitList->remove(infantry);
            }
        }
    }
    this->resetLF_EXP();
}
void ARVN::fightIfDefense(Army *enemy)
{

    if (unitList->getCount() == 0)
        return;

    for (int i = 0; i < unitList->getCount(); i++)
    {
        if (unitList->getUnit(i)->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(unitList->getUnit(i));
            vehicle->setWeight(vehicle->getWeight() * 0.8);
        }
        else
        {
            Infantry *infantry = static_cast<Infantry *>(unitList->getUnit(i));
            infantry->setWeight(infantry->getWeight() * 0.8);
        }
    }
    unitList = nullptr; // tránh kẻo xóa các đơn vị bị trưng dựng
}
string ARVN::str() const
{
    // format: ARVN[name=<name>, LF=<LF>, EXP=<EXP>]
    stringstream ss;
    ss << "ARVN[name=" << name
       << ",LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << unitList->str()
       << ",battleField=" << battleField->str()
       << "]";
    return ss.str();
}
// Implement the method UnitList

Vehicle *UnitList::getVehicleViaType(VehicleType vehicleType) // return vehicle if it exists
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(current->unit);
            if (vehicle->getVehicleType() == vehicleType)
                return vehicle;
        }
        else
            break; // unitclassofy == INFANTRY
    }
    return nullptr;
}
Infantry *UnitList::getInfantryViaType(InfantryType infantryType)
{
    for (UnitNode *current = tail; current != nullptr; current = current->prev)
    {
        if (current->unit->getUnitClassify() == UnitClassify::INFANTRY)
        {
            Infantry *infantry = static_cast<Infantry *>(current->unit);
            if (infantry->getInfantryType() == infantryType)
                return infantry;
        }
        else
            break; // unitclassofy == VEHICLE
    }
    return nullptr;
}
vector<Vehicle *> UnitList::getVehicles() const
{
    vector<Vehicle *> vehicles;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
            vehicles.push_back(static_cast<Vehicle *>(current->unit));
        else
            break;
    }
    return vehicles;
}
vector<Infantry *> UnitList::getInfantries() const
{
    vector<Infantry *> infantries;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::INFANTRY)
            infantries.push_back(static_cast<Infantry *>(current->unit));
        else
            break;
    }
    return infantries;
}
vector<Unit *> UnitList::getUnits() const
{
    vector<Unit *> units;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        units.push_back(current->unit);
    }
    return units;
}
void UnitList::clear()
{
    for (UnitNode *current = head; current != nullptr; current = head)
    {
        head = current->next;
        delete current;
    }
    head = tail = nullptr;
    count = 0;
}

void UnitList::remove(Unit *unit)
{
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit == unit) // so sánh con trỏ
        {
            if (current->prev)
                current->prev->next = current->next;
            if (current->next)
                current->next->prev = current->prev;
            if (current == head)
                head = current->next;
            if (current == tail)
                tail = current->prev;
            delete current;
            count--;
            return;
        }
    }
}
void UnitList::remove(vector<Vehicle *> vehicles)
{
    for (Vehicle *vehicle : vehicles)
    {
        remove(vehicle);
    }
}
void UnitList::remove(vector<Infantry *> infantries)
{
    for (Infantry *infantry : infantries)
    {
        remove(infantry);
    }
}
string UnitList::str() const
{
    // format: UnitList[count_vehicle=<v_c>;count_infantry=<i_c>;<unit_list>]
    stringstream ss;

    ss << "UnitList[count_vehicle=";
    int count_vehicle = 0;
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        if (current->unit->getUnitClassify() == UnitClassify::VEHICLE)
            count_vehicle++;
    }
    ss << count_vehicle;

    ss << ";";
    ss << "count_infantry=";
    int count_infantry = count - count_vehicle;
    ss << count_infantry;

    ss << ";";
    for (UnitNode *current = head; current != nullptr; current = current->next)
    {
        ss << current->unit->str();
        if (current->next != nullptr)
            ss << ",";
    }
    ss << "]";

    return ss.str();
}
// Implement the method Position
Position::Position(const string &str)
{
    // format: (x, y)
    // step 1: remove ( and )
    string temp = str.substr(1, str.size() - 2);
    // step 2: split by ,
    size_t pos = temp.find(",");
    string x = temp.substr(0, pos);
    string y = temp.substr(pos + 1);
    // step 3: convert to int
    this->r = stoi(x);
    this->c = stoi(y);
}
string Position::str() const
{
    // format: (x, y)
    stringstream ss;
    ss << "(" << r << ", " << c << ")";
    return ss.str();
}

/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
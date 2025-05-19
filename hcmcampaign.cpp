#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

namespace sp
{
    // common funcs

    bool isSquare(int n)
    {
        if (n < 0)
            return false;
        int root = static_cast<int>(sqrt(n));
        return n == root * root;
    }
    //  shortened, see in test file
    int personalNumber(int n, int Y)
    {
        if (n < 0 || Y < 0)
            return -1;
        int sum = n + Y;
        return (sum == 0) ? 0 : (sum - 1) % 9 + 1;
    }

    double euclidDistance(const Position &pos1, const Position &pos2)
    {
        return sqrt(pow(pos1.getRow() - pos2.getRow(), 2) + pow(pos1.getCol() - pos2.getCol(), 2));
    }

    int closestFibonacci(int n) // ???
    {
        if (n <= 0)
            return 0;
        int f1 = 0, f2 = 1;
        while (f2 <= n)
        {
            f2 += f1;
            f1 = f2 - f1;
        }
        return f2;
    }

    bool isSpecialNumber(int n, int k)
    { // đổi số sang hệ k phân
        if (n <= 0)
            return false; //  n > 0
        if (n == 1)
            return true;

        while (n > 0)
        {
            if (n % k > 1)
                return false; // Nếu chữ số nào lớn hơn 1, không thể phân tích
            n /= k;
        }
        return true;
    }

    void findCombination(vector<Unit *> &arr, int index, int target,
                         vector<Unit *> current, vector<Unit *> &result,
                         int &minSum)
    {
        int currentSum = 0;
        for (auto unit : current)
            currentSum += unit->getAttackScore();

        if (currentSum > target && currentSum < minSum)
        {
            minSum = currentSum;
            result = current;
        }

        if (index >= arr.size())
            return;

        // Include current unit
        current.push_back(arr[index]);
        findCombination(arr, index + 1, target, current, result, minSum);

        // Exclude current unit
        current.pop_back();
        findCombination(arr, index + 1, target, current, result, minSum);
    }
    vector<Unit *> smallestCombinationGreaterThanTarget(UnitList *list, int target, bool (*validator)(Unit *))
    {
        vector<Unit *> arr;
        for (int i = 0; i < list->getCount(); ++i)
        {
            Unit *unit = list->getUnit(i);
            if (validator(unit))
                arr.push_back(unit);
        }

        vector<Unit *> result;
        int minSum = INT_MAX;
        findCombination(arr, 0, target, {}, result, minSum);

        return (minSum == INT_MAX) ? vector<Unit *>() : result;
    }

    bool isVehicle(Unit *unit)
    {
        return unit->getUnitClassify() == UnitClassify::VEHICLE;
    }

    bool isInfantry(Unit *unit)
    {
        return unit->getUnitClassify() == UnitClassify::INFANTRY;
    }
}
// clean code part
const string map_vehicle[] = {
    "TRUCK",
    "MORTAR",
    "ANTIAIRCRAFT",
    "ARMOREDCAR",
    "APC",
    "ARTILLERY",
    "TANK"};

const string map_infantries[] = {
    "SNIPER",
    "ANTIAIRCRAFTSQUAD",
    "MORTARSQUAD",
    "ENGINEER",
    "SPECIALFORCES",
    "REGULARINFANTRY"};

/*Unit group*/
// Implement the method virtual class Unit
Unit::Unit(int quantity, int weight, Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}
Unit::~Unit() {}
Position Unit::getCurrentPosition() const { return pos; }
// Implement the method vehicle
int Vehicle::getAttackScore()
{
    double score = vehicleType * 304 + quantity * weight;
    // có biến đổi applyEffects
    return ceil(score / 30.0);
}
string Vehicle::str() const
{
    // format: Vehicle[attr_name=<attr_value>], vehicleType -> quantity, weight, pos
    stringstream ss;
    ss << "Vehicle[";
    ss << "vehicleType=" << map_vehicle[vehicleType] << ",";
    ss << "quantity=" << quantity << ",";
    ss << "weight=" << weight << ",";
    ss << "position=" << pos.str() << "]";
    return ss.str();
}
// Implement the method Infantry
int Infantry::getAttackScore() // remarkable part
{
    // fomula: score = typeValue ∗ 56 + quantity ∗ weight
    int score = infantryType * 56 + quantity * weight;
    if (infantryType == InfantryType::SPECIALFORCES && sp::isSquare(weight))
        score += 75;

    int personalNumberin1975 = sp::personalNumber(score, 1975); // ? cách dùng
    if (personalNumberin1975 > 7)
        setQuantity(ceil(quantity * 1.2));
    else if (personalNumberin1975 < 3)
        setQuantity(ceil(quantity * 0.9));
    else
        return score;
    // applyEffects
    return infantryType * 56 + quantity * weight;
}
string Infantry::str() const
{
    stringstream ss;
    ss << "Infantry[";
    ss << "infantryType=" << map_infantries[infantryType] << ",";
    ss << "quantity=" << quantity << ",";
    ss << "weight=" << weight << ",";
    ss << "pos=" << pos.str() << "]";
    return ss.str();
}
// Implement the method UnitList
UnitList::UnitList(Army *army)
{
    this->holder_of_this = army;
    int S = army->getEXP() + army->getLF();
    bool isSpecialof2 = sp::isSpecialNumber(S, 2);
    bool isSpecialof3 = sp::isSpecialNumber(S, 3);
    bool isSpecialof5 = sp::isSpecialNumber(S, 5);
    bool isSpecialof7 = sp::isSpecialNumber(S, 7);
    if (isSpecialof3 & isSpecialof5 & isSpecialof7 & isSpecialof2)
        capacity = 12;
    else
        capacity = 8;

    array.resize(capacity);
    for (int i = 0; i < capacity; i++)
        array[i] = nullptr;

    count_vehicle = 0;
    count_infantry = 0;
}

bool UnitList::insert(Unit *unit)
{
    if (count_infantry + count_vehicle >= capacity)
        return false;
    Unit *unitExist = findUnit(unit);
    if (unitExist != nullptr)
    {
        unitExist->setQuantity(unitExist->getQuantity() + unit->getQuantity());
        if (unit->getWeight() > unitExist->getWeight())
        {
            unitExist->setWeight(unit->getWeight());
        }
        return true;
    }
    if (unit->getUnitClassify() == VEHICLE)
    {
        array.push_back(static_cast<Vehicle *>(unit));
        count_vehicle++;
    }
    else
    {
        array.insert(array.begin(), static_cast<Infantry *>(unit));
        count_infantry++;
    }

    return true;
}
bool UnitList::isContain(VehicleType vehicleType)
{
    for (int i = 0; i < capacity; i++)
    {
        if (array[i] != nullptr && array[i]->getUnitClassify() == VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(array[i]);
            if (vehicle->getVehicleType() == vehicleType)
                return true;
        }
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType)
{
    for (int i = 0; i < capacity; i++)
    {
        if (array[i]->getUnitClassify() == INFANTRY)
        {
            Infantry *infantry = static_cast<Infantry *>(array[i]);
            if (infantry->getInfantryType() == infantryType)
                return true;
        }
        else
            return false;
    }
}
Unit *UnitList::getVehicleViaType(VehicleType vehicleType)
{
    for (int i = 0; i < capacity; i++)
    {
        if (array[i]->getUnitClassify() == VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(array[i]);
            if (vehicle->getVehicleType() == vehicleType)
                return vehicle;
        }
    }
    return nullptr;
}
Unit *UnitList::getInfantryViaType(InfantryType infantryType)
{
    for (int i = 0; i < capacity; i++)
    {
        if (array[i]->getUnitClassify() == INFANTRY)
        {
            Infantry *infantry = static_cast<Infantry *>(array[i]);
            if (infantry->getInfantryType() == infantryType)
                return infantry;
        }
        else
            return nullptr;
    }
}
string UnitList::str() const
{
    stringstream ss;
    ss << "UnitList[";
    ss << "count_vehicle=" << count_vehicle << ";";
    ss << "count_infantry=" << count_infantry << ";";
    for (int i = 0; i < capacity; i++)
    {
        if (array[i] != nullptr)
        {
            ss << array[i]->str();
            if (i != capacity - 1)
                ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
/*Army group*/
// Implement the method Army

void Army::resetLF_EXP()
{
    // Calculate LF and EXP
    LF = 0;
    EXP = 0;

    for (int i = 0; i < unitList->getCount(); i++)
    {
        Unit *unit = unitList->getUnit(i);
        if (unit->getUnitClassify() == VEHICLE)
        {
            Vehicle *vehicle = static_cast<Vehicle *>(unit);
            LF += vehicle->getAttackScore();
        }
        else
        {
            Infantry *infantry = static_cast<Infantry *>(unit);
            EXP += infantry->getAttackScore();
        }
    }
    LF = max(0, min(1000, LF));
    EXP = max(0, min(500, EXP));
}
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
{
    this->battleField = battleField;
    this->name = name;
    this->unitList = new UnitList(this);
    // insert to unitList via unitArray
    for (int i = 0; i < size; i++)
    {
        unitList->insert(unitArray[i]);
    }
    this->resetLF_EXP();
}
void Army::fight(Army *enemy, bool defense)
{
    defense ? fightIfDefense(enemy) : fightIfAttack(enemy);
}

void LiberationArmy::fightIfAttack(Army *enemy)
{
    // liberationArmy action

    // tăng chỉ số
    this->setEXP(this->getEXP() * 1.5);
    this->setLF(this->getLF() * 1.5);

    auto A = sp::smallestCombinationGreaterThanTarget(this->unitList, enemy->getEXP(), sp::isInfantry);
    auto B = sp::smallestCombinationGreaterThanTarget(this->unitList, enemy->getLF(), sp::isVehicle);

    bool foundInfantryComb = A.size() > 0;
    bool foundVehicleComb = B.size() > 0;
    bool battle = false;

    if (foundInfantryComb && foundVehicleComb)
    {
        battle = true;
    }
    else if (!foundInfantryComb && !foundVehicleComb)
    { /*battel = false*/
    }
    else
    {
        // chỉ số tấn công toàn đội
        int attackScore = 0;
        for (int i = 0; i < this->unitList->getCount(); ++i)
        {
            attackScore += this->unitList->getUnit(i)->getAttackScore();
        }
        // chỉ số tấn công địch
        int enemyAttackScore = 0;
        for (int i = 0; i < enemy->getUnitList()->getCount(); ++i)
        {
            enemyAttackScore += enemy->getUnitList()->getUnit(i)->getAttackScore();
        }
        // nếu đội ta > địch
        if (attackScore > enemyAttackScore)
            battle = true;
    }

    if (battle)
    {
        // xóa tổ hợp ko thỏa mãn
        if (foundInfantryComb)
        {
            for (Unit *unit : A)
            {
                unitList->removeUnit(unit);
            }
        }
        if (foundVehicleComb)
        {
            for (Unit *unit : B)
            {
                unitList->removeUnit(unit);
            }
        }
        // trưng dụng, thêm toàn bộ unitlist của enemy vào LiberationArmy
        for (int i = 0; i < enemy->getUnitList()->getCount(); i++)
        {
            Unit *unit = enemy->getUnitList()->getUnit(i);
            unitList->insert(unit);
        }
    }
    else
    {
        // mỗi đơn vị quân sự xóa 10% trọng số
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            unit->setWeight(unit->getWeight() - unit->getWeight() * 0.1);
        }
    }

    resetLF_EXP();
}
void LiberationArmy::fightIfDefense(Army *enemy)
{
    // TODO
    this->setEXP(this->getEXP() * 1.3);
    this->setLF(this->getLF() * 1.3);

    // so sánh chỉ số chiến đấu
    if (LF >= enemy->getLF() && EXP >= enemy->getEXP())
    {
    }
    else if (LF < enemy->getLF() && EXP < enemy->getEXP())
    {
        // cập nhập số lượng mỗi quân lên số fibo gần nhất
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            int fibo = sp::closestFibonacci(unit->getQuantity());
            unit->setQuantity(fibo);
        }
    }
    else
    {
        // xóa mỗi đơn vị 10% số lượng
        for (int i = 0; i < unitList->getCount(); i++)
        {
            Unit *unit = unitList->getUnit(i);
            unit->setQuantity(unit->getQuantity() - unit->getQuantity() * 0.1);
        }
    }

    // ARVN action
    // XÓA mỗi đơn vị quân sự enemy 20% số lượng
    for (int i = 0; i < enemy->getUnitList()->getCount(); i++)
    {
        Unit *unit = enemy->getUnitList()->getUnit(i);
        unit->setQuantity(unit->getQuantity() - unit->getQuantity() * 0.2);
        if (unit->getQuantity() == 1)
        {
            enemy->getUnitList()->removeUnit(unit);
            i--;
        }
    }
    resetLF_EXP();
}

string ARVN::str() const
{
    stringstream ss;
    ss << "ARVN[";
    ss << "LF=" << getLF() << ",";
    ss << "EXP=" << getEXP() << ",";
    ss << "unitList=" << unitList->str() << ",";
    ss << "battleField=";
    if (battleField != nullptr)
    {
        ss << battleField->str();
    }
    ss << "]";
    return ss.str();
}

string LiberationArmy::str() const
{
    stringstream ss;
    ss << "LiberationArmy[";
    ss << "LF=" << getLF() << ",";
    ss << "EXP=" << getEXP() << ",";
    ss << "unitList=" << unitList->str() << ",";
    ss << "battleField=";
    if (battleField != nullptr)
    {
        ss << battleField->str();
    }
    ss << "]";
    return ss.str();
}

// fight trong battlefield hay fight ngẫu nhiên

/*setup group*/

/*TerrainElement */

// Implement the method Mountain
void Mountain::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 4;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        auto unit = ARVN->getUnitList()->getUnit(i);
        int gapToMountain = sp::euclidDistance(unit->getPos(), pos);
        if (gapToMountain <= impactRange)
        {
            if (unit->getUnitClassify() == UnitClassify::VEHICLE)
                ARVN->setLF(ARVN->getLF() - 0.05 * static_cast<Vehicle *>(unit)->getAttackScore());
            else
                ARVN->setEXP(ARVN->getEXP() + 0.2 * static_cast<Infantry *>(unit)->getAttackScore());
        }
    }
}
void Mountain::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        auto unit = liberationArmy->getUnitList()->getUnit(i);
        int gapToMountain = sp::euclidDistance(unit->getPos(), pos);
        if (gapToMountain <= impactRange)
        {
            if (unit->getUnitClassify() == UnitClassify::VEHICLE)
                liberationArmy->setLF(liberationArmy->getLF() - 0.1 * static_cast<Vehicle *>(unit)->getAttackScore());
            else
                liberationArmy->setEXP(liberationArmy->getEXP() + 0.3 * static_cast<Infantry *>(unit)->getAttackScore());
        }
    }
}
// Implement the method River
void River::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 2;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            unit->setTempAttackScore(unit->getTempAttackScore() * 0.9);
    }
}
void River::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::INFANTRY)
            continue;
        if (sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            unit->setTempAttackScore(unit->getTempAttackScore() * 0.9);
    }
}
// IMPLEMENT THE METHOD Urban
void Urban::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 3;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
            continue;
        Infantry *infantry = static_cast<Infantry *>(unit);
        if (infantry->getInfantryType() != InfantryType::SPECIALFORCES)
            continue;
        int Distance = sp::euclidDistance(unit->getPos(), pos);

        if (Distance <= impactRange)
        {
            // unit->setWeight(unit->getWeight);
            double increase = 3.0 * infantry->getAttackScore() / (2.0 * Distance);
            infantry->setTempAttackScore(infantry->getTempAttackScore() + increase);
        }
    }
}
void Urban::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        if (unit->getUnitClassify() == UnitClassify::VEHICLE)
        {
            const int impactRange = 2;
            Vehicle *vehicle = static_cast<Vehicle *>(unit);
            if (vehicle->getVehicleType() == VehicleType::ARTILLERY &&
                sp::euclidDistance(unit->getPos(), pos) <= impactRange)
            {
                vehicle->setTempAttackScore(vehicle->getTempAttackScore() * 0.5);
            }
            else
            {
                const int impactRange = 5;
                Infantry *infantry = static_cast<Infantry *>(unit);
                if (infantry->getInfantryType() == InfantryType::SPECIALFORCES || infantry->getInfantryType() == InfantryType::REGULARINFANTRY)
                {
                    int Distance = sp::euclidDistance(unit->getPos(), pos);
                    if (Distance <= impactRange)
                    {
                        double increase = 2.0 * infantry->getAttackScore() / Distance;
                        infantry->setTempAttackScore(infantry->getTempAttackScore() * 0.5);
                    }
                }
            }
        }
    }
}
// Implement the method Fortification
void Fortification::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 2;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        int Distance = sp::euclidDistance(unit->getPos(), pos);
        if (Distance <= impactRange)
        {
            unit->setTempAttackScore(unit->getTempAttackScore() * 1.2);
        }
    }
}
void Fortification::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        int Distance = sp::euclidDistance(unit->getPos(), pos);
        if (Distance <= impactRange)
        {
            unit->setTempAttackScore(unit->getTempAttackScore() * 0.8);
        }
    }
}
// Implement the method SpecialZone
void SpecialZone::getEffectOnARVN(ARVN *ARVN)
{
    const int impactRange = 1;
    for (int i = 0; i < ARVN->getUnitList()->getCount(); i++)
    {
        Unit *unit = ARVN->getUnitList()->getUnit(i);
        int Distance = sp::euclidDistance(unit->getPos(), pos);
        if (Distance <= impactRange)
        {
            unit->setTempAttackScore(0);
        }
    }
}
void SpecialZone::getEffectOnLibArmy(LiberationArmy *liberationArmy)
{
    const int impactRange = 2;
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        int Distance = sp::euclidDistance(unit->getPos(), pos);
        if (Distance <= impactRange)
        {
            unit->setTempAttackScore(0);
        }
    }
}

// Implement the method Configuration
Configuration::Configuration(const string &config_file_path)
{
    Node<Unit *> *list_UncludeHo = new Node<Unit *>();
    Node<Unit *> *list_3que = new Node<Unit *>();

    ifstream file(config_file_path);
    string line;
    while (getline(file, line))
    {
        if (line.find("NUM_ROWS") != string::npos)
            num_rows = stoi(line.substr(line.find("=") + 1));
        else if (line.find("NUM_COLS") != string::npos)
            num_cols = stoi(line.substr(line.find("=") + 1));
        else if (line.find("EVENT_CODE") != string::npos)
            eventCode = max(0, stoi(line.substr(line.find("=") + 1)) % 100);
        else if (line.find("ARRAY_") != string::npos)
        {
            vector<Position *> *array = nullptr;
            if (line.find("FOREST") != string::npos)
                array = &arrayForest;
            else if (line.find("RIVER") != string::npos)
                array = &arrayRiver;
            else if (line.find("FORTIFICATION") != string::npos)
                array = &arrayFortification;
            else if (line.find("URBAN") != string::npos)
                array = &arrayUrban;
            else if (line.find("SPECIAL_ZONE") != string::npos)
                array = &arraySpecialZone;

            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // phá dấu []
            size_t pos = 0;
            while ((pos = temp.find("),")) != string::npos)
            {
                array->push_back(new Position(temp.substr(0, pos + 1)));
                temp.erase(0, pos + 2);
            }
            if (!temp.empty())
                array->push_back(new Position(temp));
        }
        else if (line.find("UNIT_LIST=") != string::npos)
        {
            // format: UNIT_LIST=[TANK(5,2,(1,2),0),TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)]
            string temp = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1); // phá dấu []
            /*
            UNIT_NAME(quantity,weight,position,armyBelong Với armyBelong=0 tương ứng với
            Quân Giải phóng, armyBelong=1 tương ứng với Quân đội chính quyền Sài Gòn
            */
            size_t pos = 0;
            while ((pos = temp.find("), ")) != string::npos)
            {
                string unit_str = temp.substr(0, pos + 1); // TANK(5,2,(1,2),0)
                extractUnits(unit_str, list_UncludeHo, list_3que);
                temp.erase(0, pos + 2); // TANK(5,2,(3,2),1),REGULARINFANTRY(5,2,(1,1),0),REGULARINFANTRY(5,2,(3,3),1)
            }
            if (!temp.empty())
                extractUnits(temp, list_UncludeHo, list_3que);
        }
    }
    file.close();

    this->count_ARVNUnit = list_UncludeHo->size();
    this->count_liberUnit = list_3que->size();
    this->liberationUnits = new Unit *[count_liberUnit];
    this->ARVNUnits = new Unit *[count_ARVNUnit];

    // copy the data from the linked list to the array
    Node<Unit *> *temp = list_UncludeHo;
    for (int i = 0; i < count_ARVNUnit; i++)
    {
        liberationUnits[i] = temp->data;
        temp = temp->next;
    }
    temp = list_3que;
    for (int i = 0; i < count_liberUnit; i++)
    {
        ARVNUnits[i] = temp->data;
        temp = temp->next;
    }
    delete list_UncludeHo;
    delete list_3que;
}
void Configuration::extractUnits(string input, Node<Unit *> *list_UncludeHo, Node<Unit *> *list_3que)
{
    // format: TANK(5,2,(1,2),0)
    // step 1: remove ( and ) but hold the string TANK
    string temp = input.substr(0, input.find("(")); // TANK
    // step 2: remove the string TANK and ( but hold the string 5,2,(1,2),0)
    string temp2 = input.substr(input.find("(") + 1); // 5,2,(1,2),0
    // step 3: split by , and save int, int, pos, int
    size_t pos = 0;
    int quantity = stoi(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    int weight = stoi(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    Position position(temp2.substr(0, temp2.find(",")));
    temp2.erase(0, temp2.find(",") + 1);
    int armyBelong = stoi(temp2);

    // step 4: create the unit
    Unit *unit = nullptr;
    if (temp == "TRUCK")
        unit = new Vehicle(quantity, weight, position, VehicleType::TRUCK);
    else if (temp == "MORTAR")
        unit = new Vehicle(quantity, weight, position, VehicleType::MORTAR);
    else if (temp == "ANTIAIRCRAFT")
        unit = new Vehicle(quantity, weight, position, VehicleType::ANTIAIRCRAFT);
    else if (temp == "ARMOREDCAR")
        unit = new Vehicle(quantity, weight, position, VehicleType::ARMOREDCAR);
    else if (temp == "APC")
        unit = new Vehicle(quantity, weight, position, VehicleType::APC);
    else if (temp == "ARTILLERY")
        unit = new Vehicle(quantity, weight, position, VehicleType::ARTILLERY);
    else if (temp == "TANK")
        unit = new Vehicle(quantity, weight, position, VehicleType::TANK);
    else if (temp == "SNIPER")
        unit = new Infantry(quantity, weight, position, InfantryType::SNIPER);
    else if (temp == "ANTIAIRCRAFTSQUAD")
        unit = new Infantry(quantity, weight, position, InfantryType::ANTIAIRCRAFTSQUAD);
    else if (temp == "MORTARSQUAD")
        unit = new Infantry(quantity, weight, position, InfantryType::MORTARSQUAD);
    else if (temp == "ENGINEER")
        unit = new Infantry(quantity, weight, position, InfantryType::ENGINEER);
    else if (temp == "SPECIALFORCES")
        unit = new Infantry(quantity, weight, position, InfantryType::SPECIALFORCES);
    else if (temp == "REGULARINFANTRY")
        unit = new Infantry(quantity, weight, position, InfantryType::REGULARINFANTRY);

    if (armyBelong == 0)
        list_3que->add(unit);
    else if (armyBelong == 1)
        list_UncludeHo->add(unit);
    else
        throw invalid_argument("Invalid army belong");
}
Configuration::~Configuration()
{
    for (Position *pos : arrayForest)
        delete pos;
    for (Position *pos : arrayRiver)
        delete pos;
    for (Position *pos : arrayFortification)
        delete pos;
    for (Position *pos : arrayUrban)
        delete pos;
    for (Position *pos : arraySpecialZone)
        delete pos;

    for (int i = 0; i < count_liberUnit; i++)
        delete liberationUnits[i];
    for (int i = 0; i < count_ARVNUnit; i++)
        delete ARVNUnits[i];
    delete[] liberationUnits;
    delete[] ARVNUnits;
}
string Configuration::str()
{
    stringstream ss;
    ss << "["
       << "num_rows=" << num_rows << ","
       << "num_cols=" << num_cols << ",";

    auto formatArray = [&](const string &name, const vector<Position *> &array)
    {
        ss << name << "=[";
        for (size_t i = 0; i < array.size(); ++i)
        {
            ss << array[i]->str();
            if (i != array.size() - 1)
                ss << ",";
        }
        ss << "],";
    };

    formatArray("arrayForest", arrayForest);
    formatArray("arrayRiver", arrayRiver);
    formatArray("arrayFortification", arrayFortification);
    formatArray("ARRAY_URBAN", arrayUrban);
    formatArray("ARRAY_SPECIAL_ZONE", arraySpecialZone);

    ss << "liberationUnits=[";
    for (int i = 0; i < count_liberUnit; ++i)
    {
        ss << liberationUnits[i]->str();
        if (i != count_liberUnit - 1)
            ss << ",";
    }
    ss << "],";

    ss << "ARVNUnits=[";
    for (int i = 0; i < count_ARVNUnit; ++i)
    {
        ss << ARVNUnits[i]->str();
        if (i != count_ARVNUnit - 1)
            ss << ",";
    }
    ss << "],";

    ss << "eventCode=" << eventCode << "]";

    return ss.str();
}

// Implement the method BattleField
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                         vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
{
    this->n_rows = n_rows;
    this->n_cols = n_cols;
    // TODO
    // create 2d array of TerrainElement with default is Road
    terrain = new TerrainElement **[n_rows];
    for (int i = 0; i < n_rows; i++)
    {
        terrain[i] = new TerrainElement *[n_cols];
        for (int j = 0; j < n_cols; j++)
        {
            terrain[i][j] = new Road(Position(i, j));
        }
    }
    // update the terrain with the given positions
    for (Position *pos : arrayForest)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Mountain(*pos);
    }
    for (Position *pos : arrayRiver)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new River(*pos);
    }
    for (Position *pos : arrayFortification)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Fortification(*pos);
    }
    for (Position *pos : arrayUrban)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new Urban(*pos);
    }
    for (Position *pos : arraySpecialZone)
    {
        delete terrain[pos->getRow()][pos->getCol()];
        terrain[pos->getRow()][pos->getCol()] = new SpecialZone(*pos);
    }
}
string BattleField::str()
{
    // format: BattleField[attr_name=<attr_value>]
    stringstream ss;
    ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return ss.str();
}
BattleField::~BattleField()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            delete terrain[i][j];
        }
        delete[] terrain[i];
    }
    delete[] terrain;
}

// Implement the method HCMCampaign
void HCMCampaign::run()
{

    // Địa hình ảnh hưởng đến quân đội 2 bên
    for (int i = 0; i < battleField->getNumRows(); i++)
    {
        for (int j = 0; j < battleField->getNumCols(); j++)
        {
            battleField->getTerrainElement(i, j)->getEffect(VNCHARMY);
            battleField->getTerrainElement(i, j)->getEffect(liberationArmy);
        }
    }

    // Giao tranh
    int eventCode = config->getEventCode();
    if (eventCode < 75)
        liberationArmy->fight(VNCHARMY, false);
    else if (eventCode >= 75)
    {
        liberationArmy->fight(VNCHARMY, true);
        liberationArmy->fight(VNCHARMY, false);
    }

    // loại bỏ các đơn vị đã chết, chưa kiểm tra
    for (int i = 0; i < VNCHARMY->getUnitList()->getCount(); i++)
    {
        Unit *unit = VNCHARMY->getUnitList()->getUnit(i);
        if (unit->getTempAttackScore() <= 5)
        {
            VNCHARMY->getUnitList()->removeUnit(unit);
            i--;
        }
    }
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        if (unit->getTempAttackScore() <= 5)
        {
            liberationArmy->getUnitList()->removeUnit(unit);
            i--;
        }
    }

    // đặt lại các thông số cho các đơn vị
    for (int i = 0; i < VNCHARMY->getUnitList()->getCount(); i++)
    {
        Unit *unit = VNCHARMY->getUnitList()->getUnit(i);
        unit->setTempAttackScore(unit->getAttackScore());
    }
    for (int i = 0; i < liberationArmy->getUnitList()->getCount(); i++)
    {
        Unit *unit = liberationArmy->getUnitList()->getUnit(i);
        unit->setTempAttackScore(unit->getAttackScore());
    }
    liberationArmy->resetLF_EXP();
    VNCHARMY->resetLF_EXP();
}
HCMCampaign::HCMCampaign(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    battleField = new BattleField(config->getNumRows(), config->getNumCols(),
                                  config->getArrayForest(), config->getArrayRiver(),
                                  config->getArrayFortification(), config->getArrayUrban(),
                                  config->getArraySpecialZone());
    liberationArmy = new LiberationArmy(config->getLiberationUnits(), config->getliberUnitsSize(),
                                        "LiberationArmy", battleField);
    VNCHARMY = new ARVN(config->getARVNUnits(), config->getARVNUnitsSize(),
                        "ARVN", battleField);
}
string HCMCampaign::printResult()
{
    stringstream ss;
    ss << "LIBERATIONARMY[";
    ss << "LF=" << liberationArmy->getLF() << ",";
    ss << "EXP=" << liberationArmy->getEXP();
    ss << "]-";
    ss << "ARVN[";
    ss << "LF=" << VNCHARMY->getLF() << ",";
    ss << "EXP=" << VNCHARMY->getEXP();
    ss << "]";
    return ss.str();
}

HCMCampaign::~HCMCampaign()
{

    delete VNCHARMY;
    delete liberationArmy;
    delete battleField;
    delete config;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
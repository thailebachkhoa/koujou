
// void findCombination(vector<Unit *> &arr, int index, int target,
//                      vector<Unit *> current, vector<Unit *> &result,
//                      int &minSum)
// {

//     int currentSum = 0;
//     for (auto unit : current)
//     {
//         currentSum += unit->getAttackScore();
//     }

//     if (currentSum > target && currentSum < minSum)
//     {
//         minSum = currentSum;
//         result = current;
//     }

//     if (index >= arr.size())
//     {
//         return;
//     }

//     current.push_back(arr[index]);
//     findCombination(arr, index + 1, target, current, result, minSum);

//     current.pop_back();
//     findCombination(arr, index + 1, target, current, result, minSum);
// }

// vector<Unit *> smallestCombinationGreaterThanTarget(UnitList *list, int target, bool (*validator)(Unit *))
// {
//     vector<Unit *> arr;
//     vector<Unit *> current;
//     vector<Unit *> result;
//     int minSum = INT_MAX;

//     for (auto i = list->getHead(); i != nullptr; i = i->next)
//     {
//         if (validator(i->unit))
//         {
//             arr.push_back(i->unit);
//         }
//     }

//     findCombination(arr, 0, target, {}, result, minSum);

//     if (minSum == INT_MAX)
//     {
//         return {};
//     }

//     return result;
// }
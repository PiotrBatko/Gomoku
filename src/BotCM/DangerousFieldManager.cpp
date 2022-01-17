#include "DangerousFieldManager.hpp"

#include "../Coordinates.hpp"
#include "../Board.hpp"

namespace CM {

DangerousFieldManager::DangerousFieldManager() {
}

DangerousFieldManager::~DangerousFieldManager() {
}

void DangerousFieldManager::Initialize(const Board* const board) {
    this->board = board;
}

void DangerousFieldManager::AddDangerousFields(Coordinates& dangerousFieldA, Coordinates& dangerousFieldB) {
    DangerousFieldsEntryT currentDangerousFields
            = {dangerousFieldA, dangerousFieldB};

    dangerousFields.push_back(currentDangerousFields);
}

std::optional<Coordinates> DangerousFieldManager::GetDangerousField() {
    if (dangerousFields.size() != 0u) {
        std::list<DangerousFieldsEntryT>::iterator it = dangerousFields.begin();
        while (it != dangerousFields.end()) {

            DangerousFieldsEntryT& currentDangerousFields = *it;
            Coordinates dangerousField1 = currentDangerousFields[0];
            Coordinates dangerousField2 = currentDangerousFields[1];

            if (   !board->IsFieldEmpty(dangerousField1)
                || !board->IsFieldEmpty(dangerousField2)) {
                // Remove no longer relevant dangerous fields.
                dangerousFields.erase(it++);
                continue;
            }

            dangerousFields.erase(it);
            return dangerousField1;
        }
    }
    return std::nullopt;
}

}

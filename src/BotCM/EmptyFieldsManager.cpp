#include <BotCM/EmptyFieldsManager.hpp>

#include "../DebugInfo.hpp"
#include "../Coordinates.hpp"

namespace CM {

EmptyFieldsManager::EmptyFieldsManager()
    : fieldsCollectionInitialized(false) {
}

EmptyFieldsManager::~EmptyFieldsManager() {
}

void EmptyFieldsManager::InitializeCollection(const std::size_t boardWidth, const std::size_t boardHeight) {
    // For each row:
    for (std::size_t rowId = 0u; rowId < boardHeight; ++rowId) {
        emptyFieldsCollection.push_back(std::list<std::size_t>());
        std::list<std::size_t>& currentRow = emptyFieldsCollection.at(rowId);

        // For each column:
        for (std::size_t columnId = 0u; columnId < boardHeight; ++columnId) {
            currentRow.push_back(columnId);
        }
    }

    fieldsCollectionInitialized = true;
}

bool EmptyFieldsManager::IsFieldsCollectionInitialized() {
    return fieldsCollectionInitialized;
}

bool EmptyFieldsManager::SetFieldNotEmpty(Coordinates& coordinates) {
    const std::size_t rowCount = emptyFieldsCollection.size();
    if (coordinates.y > rowCount) {
        LOG_ERROR("coordinates.y > rowsCount");
        return false;
    }
    std::list<std::size_t>& currentRow = emptyFieldsCollection[coordinates.y];

    bool fieldRemoved = false;
    for (std::size_t x : currentRow) {
        if (x == coordinates.x) {
            currentRow.remove(x);
            fieldRemoved = true;
            break;
        }
    }
    if (!fieldRemoved) {
        LOG_ERROR("Field (x,y): ", coordinates.x, ",", coordinates.y, " not found in emptyFieldsCollection.");
        return false;
    }

    LOG_LN("Field ", coordinates.x, ",", coordinates.y, " erased.");
    return true;
}

}

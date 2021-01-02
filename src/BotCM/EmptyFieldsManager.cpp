#include <BotCM/EmptyFieldsManager.hpp>

#include "../DebugInfo.hpp"
#include "../Coordinates.hpp"
#include "../Random.hpp"

namespace CM {

EmptyFieldsManager::EmptyFieldsManager()
    : fieldsCollectionInitialized(false), emptyFieldsCount(0u) {
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
            emptyFieldsCount++;
        }
    }

    fieldsCollectionInitialized = true;
}

bool EmptyFieldsManager::IsFieldsCollectionInitialized() {
    return fieldsCollectionInitialized;
}

bool EmptyFieldsManager::SetFieldNotEmpty(const Coordinates& coordinates) {
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

    // For debugging:
    //LOG_LN("Field ", coordinates.x, ",", coordinates.y, " erased.");

    if (emptyFieldsCount == 0u) {
        LOG_ERROR("Trying to decrease zero value of 'emptyFieldsCount'.");
    } else {
        emptyFieldsCount--;
    }
    return true;
}

bool EmptyFieldsManager::RandomizeEmptyField(Coordinates& randomizedEmptyField) {
    if (emptyFieldsCount == 0u) {
        LOG_ERROR("No empty fields.");
    }
    bool fieldRandomized = false;

    const std::size_t randomizedValue =
            static_cast<std::size_t>(
                    Random::RandomizeInt(static_cast<unsigned int>(emptyFieldsCount)));

    std::size_t emptyFieldsSum = 0u;
    for (size_t currentRowId = 0u; currentRowId < emptyFieldsCollection.size(); ++currentRowId) {
        std::list<std::size_t>& currentRow = emptyFieldsCollection[currentRowId];

        emptyFieldsSum += currentRow.size();

        if (randomizedValue < emptyFieldsSum) {
            const std::size_t indexInCurrentRow = emptyFieldsSum - currentRow.size();

            std::list<std::size_t>::iterator it = currentRow.begin();
            for (std::size_t i = 0u; i < indexInCurrentRow; ++i) {
                it++;
            }
            const std::size_t randomizedFieldColumn = *it;

            randomizedEmptyField.x = randomizedFieldColumn;
            randomizedEmptyField.y = currentRowId;
            fieldRandomized = true;
            break;
        }
    }

    if (!fieldRandomized) {
        LOG_ERROR("Field not randomized.");
        return false;
    }

    const bool result = SetFieldNotEmpty(randomizedEmptyField);
    return result;
}

}

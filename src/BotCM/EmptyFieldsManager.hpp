#ifndef _BOTCM_EMPTY_FIELDS_MANAGER_HPP_
#define _BOTCM_EMPTY_FIELDS_MANAGER_HPP_

#include <vector>
#include <list>

class Coordinates;

namespace CM {

class EmptyFieldsManager {
public:
    EmptyFieldsManager();
    virtual ~EmptyFieldsManager();

    // Fill 'emptyFieldsCollection' with all fields of the game board.
    void InitializeCollection(std::size_t boardWidth, std::size_t boardHeight);

    bool IsFieldsCollectionInitialized();

    bool SetFieldNotEmpty(const Coordinates& coordinates);

    bool RandomizeEmptyField(Coordinates& randomizedEmptyField);

    bool IsFieldEmpty(const Coordinates& coordinates) const;

private:
    // First dimension (vector): index of row in the game board.
    // Second dimension (list): list of column ids of empty fields, for given row.
    std::vector<std::list<std::size_t>> emptyFieldsCollection;

    bool fieldsCollectionInitialized;

    std::size_t emptyFieldsCount;

    // Function for debugging.
    void DisplayEmptyFieldsCollection();
};

}

#endif

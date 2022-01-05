#ifndef _BOT_CM_DANGEROUS_FIELD_MANAGER_HPP_
#define _BOT_CM_DANGEROUS_FIELD_MANAGER_HPP_

#include <list>
#include <array>
#include <optional>

class Coordinates;
class Board;

namespace CM {

using DangerousFieldsEntryT = std::array<Coordinates, 2>;

class DangerousFieldManager {
public:

    void Initialize(const Board* board);
    void AddDangerousFields(Coordinates& dangerousFieldA, Coordinates& dangerousFieldB);

    // Get first dangerous field found in the dangerous fields collection. If none found,
    // std::nullopt is returned.
    std::optional<Coordinates> GetDangerousField();

    DangerousFieldManager();
    virtual ~DangerousFieldManager();

private:
    // This collection contains coordinates of empty fields F which are in places like: FXXXF
    // where X is the opponent pawn and F is the empty field.
    std::list<DangerousFieldsEntryT> dangerousFields;

    const Board* board;
};

}

#endif

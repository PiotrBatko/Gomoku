#ifndef COMMONENUMS_HPP
#define COMMONENUMS_HPP

enum class PlayerType
{
    HUMAN_CONSOLE = 1,
    HUMAN,
    BOT_RANDOMIZER,
    BOT_CM,
    BOT_BATOBOT,

    ELEMENT_COUNT
};

enum class GameplayFileManagementMode {
    NO_FILE_MANAGEMENT,
    GAMEPLAY_SAVING,
    GAMEPLAY_LOADING
};

#endif

#ifndef _FILE_APP_CONFIG_LOADER_H
#define _FILE_APP_CONFIG_LOADER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "FileAppConfigContainer.h"
#include "../CommonUtils.h"

#define APP_CONFIG_VALUES_FILE_PATH          "AppConfigValues.ini"
#define FILE_APP_CONFIG_LOADER_CONST_KEYWORD "const"
#define TAB_CHAR                             '\t'
#define SPACE_CHAR                           ' '
#define EQUAL_CHAR                           '='
#define SEMICOLON_CHAR                       ';'
#define QUOTATION_MARK_CHAR                  '"'
#define INT_TYPE_STRING                      "int"
#define LONG_LONG_INT_TYPE_STRING            "long long int"
#define BOOL_TYPE_STRING                     "bool"
#define STD_STRING_TYPE_STRING               "std::string"
#define TRUE_STRING                          "true"
#define FALSE_STRING                         "false"

// Enum defining file application configuration loader error state.
enum FileAppConfigErrorState {
    /// No error.
    FileAppConfigErrorState_NoError,
    /// File opening error (e.g. there is no file having given path).
    FileAppConfigErrorState_FileOpenError,
    /// There is more defined constants (in file with only definitions, without
    /// initializations) than values for constants.
    FileAppConfigErrorState_NotEqualsConstantsDeclsAndDefsCount,
    /// Constant hasn't written type.
    FileAppConfigErrorState_NoTypeForConstant,
    /// Constant type in file with constants initializations is not matching
    /// type in file with constants declarations.
    FileAppConfigErrorState_TypeNotMatchingDeclaration,
    /// Constant hasn't written name.
    FileAppConfigErrorState_NoNameForConstant,
    /// Constant hasn't written value.
    FileAppConfigErrorState_NoValueForConstant,
    /// Constant name in file with constants initializations is not matching
    /// name in file with constants declarations.
    FileAppConfigErrorState_NameNotMatchingDeclaration,
    /// Wrong value for constant.
    FileAppConfigErrorState_ConstantWrongValue
};

// This class represents loader of application configuration constants
// from external file(s) (where these constants are defined and initialized)
// to RAM memory.
class FileAppConfigLoader {
public:
    /// Class constructor.
    FileAppConfigLoader();

    /// Load application configuration constants from external file to given
    /// container of file application configuration. There is assumed that there are
    /// two files with application configuration constants:
    /// - first file with constant declarations using DEFINE_CONSTANT macro
    /// (AppConfigDefinitions.h),
    /// - second file with constant definitions with initializations (AppConfigValues.ini)
    /// There has to be the same order of entries with constant values in second file,
    /// as in first file
    bool LoadFileAppConfigConstants(FileAppConfigContainer& fileAppConfigContainer);

private:
    /// Pointer to application configuration constants external file.
    std::unique_ptr<std::ifstream> appConfigValuesFile;
    /// String for current read line from application configuration file.
    std::string currentReadLine;
    /// Container of application configuration constants defined in external file(s)
    /// and loaded from them to RAM memory.
    FileAppConfigContainer* fileAppConfigContainer;
    /// Constant string with constant definition text.
    const std::string constantDefinitionFixedText;

    /// Load application configuration constants from external file to given
    /// container of file application configuration. There is assumed that there are
    /// two files with application configuration constants:
    /// - first file with constant declarations using DEFINE_CONSTANT macro
    /// (AppConfigDefinitions.h),
    /// - second file with constant definitions with initializations (AppConfigValues.ini)
    /// There has to be the same order of entries with constant values in second file,
    /// as in first file.
    FileAppConfigErrorState loadFileAppConfigConstants(FileAppConfigContainer& fileAppConfigContainer);
    /// Read selected constant from application configuration external file.
    /// There is assumed that following calls of this method must have parameters
    /// (constant type and name) convinient with following constants in external file
    /// with constants to read. This method reads file following lines and reads
    /// constant value if read constant type and names are the same as type and name
    /// given with current method parameters.
    FileAppConfigErrorState readAppConfigConstant(void* outputValue, std::string& strType, std::string& constantNameString);
    /// Check if current line contains valid constant definition.
    bool checkIfCurrentLineContainsConstantDefinition(size_t& indexOfOccurenceStart);
    /// Convert loaded constant string value to appropriate type and write in place
    /// for output value.
    bool convertValueStringToDesiredTypeAndWrite(std::string& constantTypeString, std::string& valueString, void* outputValue);
    /// Compare two strings with additional checks.
    int compareStringsWithAdditionalChecks(std::string& str1, std::string& str2, int countOfCharsToCheck);
    /// Parse constant type string from current read line.
    FileAppConfigErrorState parseConstantTypeString(int indexOfTypeStringStart, std::string& constantTypeString, int& indexOfSpaceAfterType);

    /// Check if given character is white character.
    bool isCharacterWhite(char character);
    /// Check if given character is equal character.
    bool isCharacterEqual(char character);
    /// Check if given character is semicolon character.
    bool isCharacterSemicolon(char character);
    /// Check if given character is quotation mark character.
    bool isCharacterQuotationMark(char character);

	bool checkLoadedValuesCorrectness(FileAppConfigContainer& fileAppConfigContainer);
};

#endif // _FILE_APP_CONFIG_LOADER_H

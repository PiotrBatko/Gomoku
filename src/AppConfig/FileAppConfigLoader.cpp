#include "FileAppConfigLoader.hpp"

#include "../CommonEnums.hpp"
#include "../DebugInfo.hpp"

FileAppConfigLoader::FileAppConfigLoader()
	: constantDefinitionFixedText(FILE_APP_CONFIG_LOADER_CONST_KEYWORD) {
    appConfigValuesFile = nullptr;
    fileAppConfigContainer = nullptr;
}

// Load application configuration constants from external file to given
// container of file application configuration. There is assumed that there are
// two files with application configuration constants:
// - first file with constant declarations using DEFINE_CONSTANT macro
// (AppConfigDefinitions.h),
// - second file with constant definitions with initializations (AppConfigValues.ini)
// There has to be the same order of entries with constant values in second file,
// as in first file.
// Return true if success, false otherwise.
bool FileAppConfigLoader::LoadFileAppConfigConstants(FileAppConfigContainer& fileAppConfigContainer) {
    FileAppConfigErrorState fileAppConfigErrorState = loadFileAppConfigConstants(fileAppConfigContainer);
    bool result = false;
    if (fileAppConfigErrorState == FileAppConfigErrorState_NoError) {
        result = true;
    } else {
        std::string errorText = "ERROR while loading application configuration data from external file: ";
        if (fileAppConfigErrorState == FileAppConfigErrorState_FileOpenError) {
            errorText.append("File open error.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_NotEqualsConstantsDeclsAndDefsCount) {
            errorText.append("Not equals count of constant declarations and initializations.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_NoTypeForConstant) {
            errorText.append("There is no type for constant.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_TypeNotMatchingDeclaration) {
            errorText.append("Type in initialization is not matching type in declaration.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_NoNameForConstant) {
            errorText.append("There is no name for constant.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_NoValueForConstant) {
            errorText.append("There is no value for constant.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_NameNotMatchingDeclaration) {
            errorText.append("Name in file with constants initializations not matching name in file with constants declarations.");
            result = false;
        } else if (fileAppConfigErrorState == FileAppConfigErrorState_ConstantWrongValue) {
            errorText.append("ERROR: File open error.");
            result = false;
        } else {
            // Unknown error.
            errorText.append("Unknown error.");
            result = false;
        }
        LOG_ERROR(errorText);
    }

	result = checkLoadedValuesCorrectness(fileAppConfigContainer);
	return result;
}

/***************************************************************************//**
* @brief Load application configuration constants from external file to given
* container of file application configuration. There is assumed that there are
* two files with application configuration constants:
* - first file with constant declarations using DEFINE_CONSTANT macro
* (AppConfigDefinitions.h),
* - second file with constant definitions with initializations (AppConfigValues.ini)
* There has to be the same order of entries with constant values in second file,
* as in first file.
*
* @param fileAppConfigContainer - container for loaded constants.
* @return File application configuration loader error state.
*******************************************************************************/
FileAppConfigErrorState FileAppConfigLoader::loadFileAppConfigConstants(FileAppConfigContainer& fileAppConfigContainer) {
    this->fileAppConfigContainer = &fileAppConfigContainer;

    // 1. Open external file.
    appConfigValuesFile = std::make_unique<std::ifstream>(APP_CONFIG_VALUES_FILE_PATH, std::ios::in);
    if (!appConfigValuesFile->is_open()) {
        return FileAppConfigErrorState_FileOpenError;
    }
    std::string constantTypeString;
    std::string constantNameString;
    FileAppConfigErrorState errorState = FileAppConfigErrorState_NoError;

    // 2. Read values for each application configuration constant.
    #define DEFINE_CONSTANT(type, name) \
        constantTypeString = #type ; \
        constantNameString = #name ; \
        errorState = readAppConfigConstant(&(fileAppConfigContainer. name ), constantTypeString, constantNameString ); \
        if (errorState != FileAppConfigErrorState_NoError) { \
            return errorState; \
        }
    #include "AppConfigDefinitions.hpp"
    #undef DEFINE_CONSTANT
    return FileAppConfigErrorState_NoError;
}

/***************************************************************************//**
* @brief Read selected constant from application configuration external file.
* There is assumed that following calls of this method must have parameters
* (constant type and name) convinient with following constants in external file
* with constants to read. This method reads file following lines and reads
* constant value if read constant type and names are the same as type and name
* given with current method parameters.
*
* @param outputValue - pointer to variable in 'fileAppConfigContainer', where read
* constant value will be written,
* @param constantTypeString - string with type of read constant,
* @param constantNameString - string with name of read constant.
* @return true if success, false otherwise.
*******************************************************************************/
FileAppConfigErrorState FileAppConfigLoader::readAppConfigConstant(void* outputValue, std::string& constantTypeString, std::string& constantNameString) {
    //-----------------------------------------------------------------------------------
    // 1. Get line with constant definition.
    //-----------------------------------------------------------------------------------
    size_t indexOfOccurenceStart = -1;
    bool doesCurrentLineContainConstant = false;
    // Process this loop while current read line doesn't contain any constant definition.
    while (!doesCurrentLineContainConstant) {
        // Get next line from file. If there is no more lines, it means that there is more
        // defined constants (in file with only definitions, without initializations) than
        // values for constants.
        if (!std::getline(*appConfigValuesFile, currentReadLine)) {
            return FileAppConfigErrorState_NotEqualsConstantsDeclsAndDefsCount;
        }
        // Check if current line contains valid constant definition.
        doesCurrentLineContainConstant = checkIfCurrentLineContainsConstantDefinition(indexOfOccurenceStart);
    }

    size_t constantDefinitionFixedTextLength = constantDefinitionFixedText.size();
    char indexOfSpaceAfterConstString = indexOfOccurenceStart + constantDefinitionFixedTextLength + 1;
    //-----------------------------------------------------------------------------------
    // 2. Get index of type string start.
    //-----------------------------------------------------------------------------------
    int indexOfTypeStringStart = -1;
    for (int i = indexOfSpaceAfterConstString; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (!isCharacterWhite(currentChar)) {
            indexOfTypeStringStart = i;
            break;
        }
    }
    if (indexOfTypeStringStart == -1) {
        return FileAppConfigErrorState_NoTypeForConstant;
    }

    //-----------------------------------------------------------------------------------
    // 3. Get string with type of loaded constant.
    //-----------------------------------------------------------------------------------
    int indexOfSpaceAfterType = -1;
    FileAppConfigErrorState fileAppConfigErrorState = parseConstantTypeString(indexOfTypeStringStart, constantTypeString, indexOfSpaceAfterType);
    if (fileAppConfigErrorState != FileAppConfigErrorState_NoError) {
        return fileAppConfigErrorState;
    }

    //-----------------------------------------------------------------------------------
    // 4. Get index of constant name string start.
    //-----------------------------------------------------------------------------------
    int indexOfConstantNameStart = -1;
    for (int i = indexOfSpaceAfterType; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (!isCharacterWhite(currentChar)) {
            indexOfConstantNameStart = i;
            break;
        }
    }
    if (indexOfConstantNameStart == -1) {
        return FileAppConfigErrorState_NoNameForConstant;
    }

    //-----------------------------------------------------------------------------------
    // 5. Get string with name of loaded constant.
    //-----------------------------------------------------------------------------------
    int indexOfSpaceAfterConstantName = -1;
    std::string constantNameStringRead;
    for (int i = indexOfConstantNameStart; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (isCharacterWhite(currentChar)) {
            indexOfSpaceAfterConstantName = i;
            break;
        }
        constantNameStringRead.push_back(currentChar);
    }
    if (constantNameStringRead != constantNameString) {
        return FileAppConfigErrorState_NameNotMatchingDeclaration;
    }
    if (indexOfSpaceAfterConstantName == -1) {
        return FileAppConfigErrorState_NoValueForConstant;
    }

    //-----------------------------------------------------------------------------------
    // 6. Get index of equal character.
    //-----------------------------------------------------------------------------------
    int indexOfEqualSign = -1;
    for (int i = indexOfSpaceAfterConstantName; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (isCharacterEqual(currentChar)) {
            indexOfEqualSign = i;
            break;
        } else {
            if (!isCharacterWhite(currentChar)) {
                return FileAppConfigErrorState_ConstantWrongValue;
            }
        }
    }
    if (indexOfEqualSign == -1) {
        return FileAppConfigErrorState_NoValueForConstant;
    }

    //-----------------------------------------------------------------------------------
    // 7. Get index of constant value start.
    //-----------------------------------------------------------------------------------
    int indexOfConstantValueStart = -1;
    for (int i = indexOfEqualSign + 1; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (!isCharacterWhite(currentChar)) {
            indexOfConstantValueStart = i;
            break;
        }
    }
    if (indexOfConstantValueStart == -1) {
        return FileAppConfigErrorState_NoValueForConstant;
    }

    //-----------------------------------------------------------------------------------
    // 8. Get string with constant value.
    //-----------------------------------------------------------------------------------
    bool isConstantTypeString = false;
    int indexOfConstantValueToReadStart = indexOfConstantValueStart;
    if (constantTypeString == STD_STRING_TYPE_STRING) {
        isConstantTypeString = true;
        indexOfConstantValueToReadStart++;
    }
    std::string valueString;
    for (int i = indexOfConstantValueToReadStart; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (!isConstantTypeString) {
            if (isCharacterWhite(currentChar) || isCharacterSemicolon(currentChar)) {
                break;
            }
        } else {
            if (isCharacterQuotationMark(currentChar)) {
                break;
            }
        }
        valueString.push_back(currentChar);
    }

    //-----------------------------------------------------------------------------------
    // 9. Convert value string to appropriate type and write in place for output value.
    //-----------------------------------------------------------------------------------
    bool result = convertValueStringToDesiredTypeAndWrite(constantTypeString, valueString, outputValue);
    if (!result) {
        return FileAppConfigErrorState_ConstantWrongValue;
    }
    return FileAppConfigErrorState_NoError;
}

/***************************************************************************//**
* @brief Check if current line contains valid constant definition.
*
* @param indexOfOccurenceStart - output index of constant valid definition
* occurence start in current read line.
* @return checking result.
*******************************************************************************/
bool FileAppConfigLoader::checkIfCurrentLineContainsConstantDefinition(size_t& indexOfOccurenceStart) {
    // Get index of 'constantDefinitionFixedText' occurence in current read line.
    indexOfOccurenceStart = currentReadLine.find(constantDefinitionFixedText);
    // If there is no occurence of constant fixed test in current line, go to next line.
    if (indexOfOccurenceStart == std::string::npos) {
        return false;
    }

    // Check if current occurence of 'defineConstantFixedText' is valid, because it could
    // be written as comment.
    for (int i = 0; i < (int)indexOfOccurenceStart; ++i) {
        char currentChar = currentReadLine.at(i);
        if (currentChar != ' ' && currentChar != TAB_CHAR) {
            // Line constans non-white characters before constant definition occurence,
            // so constant definition occurence is not valid.
            return false;
        }
    }
    return true;
}

/***************************************************************************//**
* @brief Check if given character is white character.
*
* @param character - input character.
* @return checking result.
*******************************************************************************/
bool FileAppConfigLoader::isCharacterWhite(char character) {
    if (character == SPACE_CHAR || character == TAB_CHAR) {
        return true;
    }
    return false;
}

/***************************************************************************//**
* @brief Check if given character is equal character.
*
* @param character - input character.
* @return checking result.
*******************************************************************************/
bool FileAppConfigLoader::isCharacterEqual(char character) {
    if (character == EQUAL_CHAR) {
        return true;
    }
    return false;
}

/***************************************************************************//**
* @brief Check if given character is semicolon character.
*
* @param character - input character.
* @return checking result.
*******************************************************************************/
bool FileAppConfigLoader::isCharacterSemicolon(char character) {
    if (character == SEMICOLON_CHAR) {
        return true;
    }
    return false;
}

/***************************************************************************//**
* @brief Check if given character is quotation mark character.
*
* @param character - input character.
* @return checking result.
*******************************************************************************/
bool FileAppConfigLoader::isCharacterQuotationMark(char character) {
    if (character == QUOTATION_MARK_CHAR) {
        return true;
    }
    return false;
}

/***************************************************************************//**
* @brief Convert loaded constant string value to appropriate type and write in
* place for output value.
*
* @param constantTypeString - input string with type of read constant,
* @param valueString - input string with value of read constant,
* @param outputValue - pointer to variable in 'fileAppConfigContainer', where read
* constant value will be written.
* @return true if success, false otherwise.
*******************************************************************************/
bool FileAppConfigLoader::convertValueStringToDesiredTypeAndWrite(std::string& constantTypeString, std::string& valueString, void* outputValue) {
    if (constantTypeString == INT_TYPE_STRING) {
        try {
            int convertedStringToInt = std::stoi(valueString);
            int& finalValuePlace = *((int*)outputValue);
            finalValuePlace = convertedStringToInt;
        } catch (...) {
            // Convertion error.
            return false;
        }
    } else if (constantTypeString == LONG_LONG_INT_TYPE_STRING) {
        try {
            long long int convertedStringToInt = std::stoll(valueString);
            long long int& finalValuePlace = *((long long int*)outputValue);
            finalValuePlace = convertedStringToInt;
        } catch (...) {
            // Convertion error.
            return false;
        }
    } else if (constantTypeString == BOOL_TYPE_STRING) {
        bool convertedStringToBool;
        if (valueString == TRUE_STRING) {
            convertedStringToBool = true;
        } else if (valueString == FALSE_STRING) {
            convertedStringToBool = false;
        } else {
            return false;
        }
        bool& finalValuePlace = *((bool*)outputValue);
        finalValuePlace = convertedStringToBool;
    } else if (constantTypeString == STD_STRING_TYPE_STRING) {
        std::string& finalValuePlace = *((std::string*)outputValue);
        std::string valueStringCopy = valueString;
        finalValuePlace = valueStringCopy;
    } else {
        // Unsupported constant type.
        return false;
    }
    return true;
}

/***************************************************************************//**
* @brief Compare two strings with additional checks.
*
* @param str1 - first string,
* @param str2 - second string,
* @param countOfCharsToCheck - count of characters to check.
* @return information about check:
* 0 - strings are different, when checked 'countOfCharsToCheck' starting chars,
* 1 - strings are exactly the same,
* 2 - strings are the same, when checked 'countOfCharsToCheck' starting chars,
* 3 - count of chars to check is greater than size of longer of input string.
*******************************************************************************/
int FileAppConfigLoader::compareStringsWithAdditionalChecks(std::string& str1, std::string& str2, int countOfCharsToCheck) {
    size_t str1Size = str1.size();
    size_t str2Size = str2.size();
    if (countOfCharsToCheck > (int)str1Size || countOfCharsToCheck > (int)str2Size) {
        return 3;
    }
    for (int i = 0; i < countOfCharsToCheck; ++i) {
        char char1 = str1.at(i);
        char char2 = str2.at(i);
        if (char1 != char2) {
            return 0;
        }
    }
    int maxSize = str1Size >= str2Size ? str1Size : str2Size;
    if (countOfCharsToCheck == maxSize) {
        return 1;
    }
    return 2;
}

/***************************************************************************//**
* @brief Parse constant type string from current read line.
*
* @param indexOfTypeStringStart - input index of type string start,
* @param constantTypeString - string with type of read constant,
* @param indexOfSpaceAfterType - output index of space char after type string.
* @return file application configuration error state.
*******************************************************************************/
FileAppConfigErrorState FileAppConfigLoader::parseConstantTypeString(int indexOfTypeStringStart, std::string& constantTypeString, int& indexOfSpaceAfterType) {
    bool isConstantTypeLongLongInt = false;
    if (constantTypeString == LONG_LONG_INT_TYPE_STRING) {
        isConstantTypeLongLongInt = true;
    }
    std::string constantTypeStringRead;
    for (int i = indexOfTypeStringStart; i < (int)currentReadLine.size(); ++i) {
        char currentChar = currentReadLine.at(i);
        if (isCharacterWhite(currentChar)) {
            if (!isConstantTypeLongLongInt) {
                indexOfSpaceAfterType = i;
                break;
            // If read constant type is long long int, space can't break loading type string.
            } else {
                int compareState = compareStringsWithAdditionalChecks(constantTypeStringRead, constantTypeString, i - indexOfTypeStringStart);
                if (compareState == 0 || compareState == 3) {
                    return FileAppConfigErrorState_TypeNotMatchingDeclaration;
                // Strings are exactly the same.
                } else if (compareState == 1) {
                    indexOfSpaceAfterType = i;
                    break;
                }
            }
        }
        constantTypeStringRead.push_back(currentChar);
    }
    if (constantTypeString != constantTypeStringRead) {
        return FileAppConfigErrorState_TypeNotMatchingDeclaration;
    }
    if (indexOfSpaceAfterType == -1) {
        return FileAppConfigErrorState_NoNameForConstant;
    }
    return FileAppConfigErrorState_NoError;
}

bool FileAppConfigLoader::checkLoadedValuesCorrectness(FileAppConfigContainer& fileAppConfigContainer) {
	if (fileAppConfigContainer.BoardSize < 1) {
		LOG_ERROR("fileAppConfigContainer.BoardSize < 1");
		return false;
	}

	const int PlayerMinId = 1;
	const int PlayerMaxId = static_cast<int>(PlayerType::ELEMENT_COUNT);
	if (fileAppConfigContainer.PlayerWhite < PlayerMinId || fileAppConfigContainer.PlayerWhite > PlayerMaxId) {
		LOG_ERROR("fileAppConfigContainer.Player1 has wrong value.");
		return false;
	}
	if (fileAppConfigContainer.PlayerBlack < PlayerMinId || fileAppConfigContainer.PlayerBlack > PlayerMaxId) {
		LOG_ERROR("fileAppConfigContainer.Player2 has wrong value.");
		return false;
	}
	return true;
}

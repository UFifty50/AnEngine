#ifndef FILEDIALOGUES_H
#define FILEDIALOGUES_H

#include <optional>
#include <string>


namespace AnEngine {
    struct Dialogues {
        static std::optional<std::string> OpenFileDialogue(const char* filter);
        static std::optional<std::string> SaveFileDialogue(const char* filter);
    };
}  // namespace AnEngine

#endif

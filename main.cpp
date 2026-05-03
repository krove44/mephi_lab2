#include <curses.h>

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Sequence/ArraySequence.h"
#include "Sequence/BitSequence.h"
#include "Sequence/ListSequence.h"

namespace {

int RunMenu(const std::string& title, const std::vector<std::string>& options) {
    int selected = 0;
    keypad(stdscr, TRUE);

    while (true) {
        clear();
        mvprintw(1, 2, "%s", title.c_str());
        mvprintw(2, 2, "Use Up/Down and Enter");

        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            if (i == selected) {
                attron(A_REVERSE);
            }
            mvprintw(4 + i, 4, "%s", options[i].c_str());
            if (i == selected) {
                attroff(A_REVERSE);
            }
        }

        refresh();
        int key = getch();
        if (key == KEY_UP) {
            selected = (selected - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
        } else if (key == KEY_DOWN) {
            selected = (selected + 1) % static_cast<int>(options.size());
        } else if (key == 10 || key == KEY_ENTER) {
            return selected;
        }
    }
}

std::string PromptString(const std::string& label) {
    echo();
    curs_set(1);
    clear();
    mvprintw(1, 2, "%s", label.c_str());
    mvprintw(3, 2, "> ");
    char buffer[256] = {};
    getnstr(buffer, 255);
    noecho();
    curs_set(0);
    return buffer;
}

void ShowMessage(const std::string& message) {
    clear();
    mvprintw(1, 2, "%s", message.c_str());
    mvprintw(3, 2, "Press any key...");
    refresh();
    getch();
}

template <typename T>
bool TryParseValue(const std::string& text, T& value) {
    std::istringstream input(text);
    input >> value;
    return !input.fail() && input.eof();
}

template <>
bool TryParseValue<std::string>(const std::string& text, std::string& value) {
    value = text;
    return true;
}

template <>
bool TryParseValue<uint8_t>(const std::string& text, uint8_t& value) {
    int parsed = 0;
    std::istringstream input(text);
    input >> parsed;
    if (input.fail() || !input.eof() || parsed < 0 || parsed > 1) {
        return false;
    }
    value = static_cast<uint8_t>(parsed);
    return true;
}

template <typename T>
T PromptValue(const std::string& label) {
    while (true) {
        T value {};
        std::string input = PromptString(label);
        if (TryParseValue<T>(input, value)) {
            return value;
        }
        ShowMessage("Invalid input");
    }
}

int PromptIndex(const std::string& label) {
    return PromptValue<int>(label);
}

template <typename T>
std::string ToText(const T& value) {
    std::ostringstream output;
    output << value;
    return output.str();
}

template <>
std::string ToText<uint8_t>(const uint8_t& value) {
    return std::to_string(static_cast<int>(value));
}

template <typename T>
std::string SequenceToString(const ISequence<T>& sequence) {
    std::ostringstream output;
    output << "[";
    for (int i = 0; i < sequence.GetLength(); ++i) {
        if (i > 0) {
            output << ", ";
        }
        output << ToText(sequence.Get(i));
    }
    output << "]";
    return output.str();
}

template <typename T, typename SequenceType>
void HandleConcat(SequenceType& sequence) {
    int count = PromptIndex("Concat length");
    if (count < 0) {
        ShowMessage("Length must be non-negative");
        return;
    }

    SequenceType extra;
    for (int i = 0; i < count; ++i) {
        extra.Append(PromptValue<T>("Value #" + std::to_string(i)));
    }
    sequence.Concat(&extra);
    ShowMessage("Concat done");
}

template <typename T, typename SequenceType>
void RunSequenceUi(const std::string& title) {
    SequenceType sequence;

    while (true) {
        int choice = RunMenu(
            title,
            {
                "Show sequence",
                "Append",
                "Prepend",
                "InsertAt",
                "Get by index",
                "Get first",
                "Get last",
                "Get subsequence",
                "Concat",
                "Back"
            }
        );

        try {
            if (choice == 0) {
                ShowMessage("Current: " + SequenceToString<T>(sequence));
            } else if (choice == 1) {
                sequence.Append(PromptValue<T>("Append value"));
                ShowMessage("Append done");
            } else if (choice == 2) {
                sequence.Prepend(PromptValue<T>("Prepend value"));
                ShowMessage("Prepend done");
            } else if (choice == 3) {
                T value = PromptValue<T>("Insert value");
                int index = PromptIndex("Insert index");
                sequence.InsertAt(value, index);
                ShowMessage("Insert done");
            } else if (choice == 4) {
                int index = PromptIndex("Index");
                ShowMessage("Value: " + ToText(sequence.Get(index)));
            } else if (choice == 5) {
                ShowMessage("First: " + ToText(sequence.GetFirst()));
            } else if (choice == 6) {
                ShowMessage("Last: " + ToText(sequence.GetLast()));
            } else if (choice == 7) {
                int start = PromptIndex("Start index");
                int end = PromptIndex("End index");
                std::unique_ptr<ISequence<T>> part(sequence.GetSubsequence(start, end));
                ShowMessage("Subsequence: " + SequenceToString<T>(*part));
            } else if (choice == 8) {
                HandleConcat<T>(sequence);
            } else {
                return;
            }
        } catch (const std::exception& error) {
            ShowMessage(std::string("Error: ") + error.what());
        }
    }
}

void RunArraySequenceMenu() {
    while (true) {
        int choice = RunMenu(
            "Choose ArraySequence type",
            {"int", "double", "string", "Back"}
        );

        if (choice == 0) {
            RunSequenceUi<int, ArraySequence<int>>("ArraySequence<int>");
        } else if (choice == 1) {
            RunSequenceUi<double, ArraySequence<double>>("ArraySequence<double>");
        } else if (choice == 2) {
            RunSequenceUi<std::string, ArraySequence<std::string>>("ArraySequence<string>");
        } else {
            return;
        }
    }
}

void RunListSequenceMenu() {
    while (true) {
        int choice = RunMenu(
            "Choose ListSequence type",
            {"int", "double", "string", "Back"}
        );

        if (choice == 0) {
            RunSequenceUi<int, ListSequence<int>>("ListSequence<int>");
        } else if (choice == 1) {
            RunSequenceUi<double, ListSequence<double>>("ListSequence<double>");
        } else if (choice == 2) {
            RunSequenceUi<std::string, ListSequence<std::string>>("ListSequence<string>");
        } else {
            return;
        }
    }
}

void RunBitSequenceMenu() {
    RunSequenceUi<uint8_t, BitSequence>("BitSequence");
}

}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (true) {
        int choice = RunMenu(
            "Choose sequence",
            {"ArraySequence", "ListSequence", "BitSequence", "Exit"}
        );

        if (choice == 0) {
            RunArraySequenceMenu();
        } else if (choice == 1) {
            RunListSequenceMenu();
        } else if (choice == 2) {
            RunBitSequenceMenu();
        } else {
            break;
        }
    }

    endwin();
    return 0;
}

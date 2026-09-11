/*
 * glo/log.h
 *
 * A basic logging class for debug, info, warning, and error messages. Clients
 * must provide a module identifier with the message. Messages are colour coded
 * for easy identification in the terminal, and each is displayed with a
 * timestamp.
 *
 * Messages can be any printable object --- they do not have to be strings.
 *
 * TODO: allow filtering by message class, and module name
 *
 * © Tim Clarke November 2025
 */

#pragma once

#include <sstream>
#include <string>
#include <string_view>

class Log
{
    // the various colours that messages may be printed in
    static inline constexpr const char * GREY = "\033[90m";
    static inline constexpr const char * GREEN = "\033[32m";
    static inline constexpr const char * ORANGE = "\033[33m";
    static inline constexpr const char * RED = "\033[31m";
    static inline constexpr const char * RESET = "\033[0m";

  public:
    template <typename T> static void d(const char * module, const T & value)
    {
        std::ostringstream oss;
        oss << value;

        // currently disabled to allow "instruction for use" to be read
        // print(GREY, "debug", module, oss.str());
    }

    template <typename T> static void i(const char * module, const T & value)
    {
        std::ostringstream oss;
        oss << value;
        print(GREEN, "information", module, oss.str());
    }

    template <typename T> static void w(const char * module, const T & value)
    {
        std::ostringstream oss;
        oss << value;
        print(ORANGE, "warning", module, oss.str());
    }

    template <typename T> static void e(const char * module, const T & value)
    {
        std::ostringstream oss;
        oss << value;
        print(RED, "error", module, oss.str());
    }

  private:
    static void print(const char * const colour, const char * severity,
        const char * const module, const std::string_view & s);

    static std::string getTimestamp();
};

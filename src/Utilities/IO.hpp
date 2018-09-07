#pragma once
#include <iostream>

#include <string>
#include <rang.hpp>

namespace Labyrinth {

    struct GStreams{
        static std::ostream& PrintOut(bool Retarget = false, std::ostream& Target = std::cout) {
            static std::ostream* mPrintOut = &Target;
            if(Retarget) { mPrintOut = &Target; }
            return *mPrintOut;
        } 
        static std::ostream& ErrorOut(bool Retarget = false, std::ostream& Target = std::cerr) {
            static std::ostream* mErrorOut = &Target;
            if(Retarget) { mErrorOut = &Target; }
            return *mErrorOut;
        } 
    };

    template<typename... T>
    inline std::ostream& print(T&&... Args) {
        return ((GStreams::PrintOut() << rang::fg::reset << "[ ]: ") << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& warn(T&&... Args) {
        return ((GStreams::ErrorOut()  << rang::fgB::yellow << "[*]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& error(T&&... Args) {
        return ((GStreams::ErrorOut()  << rang::fgB::red << "[!]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& notice(T&&... Args) {
        return ((GStreams::PrintOut()  << rang::fgB::cyan << "[-]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& success(T&&... Args) {
        return ((GStreams::PrintOut()  << rang::fgB::green << "[^]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }


    template<typename... T>
    inline std::ostream& println(T&&... Args) {
        return print(std::forward<T&&>(Args)...) << '\n';
    }
    template<typename... T>
    inline std::ostream& warnln(T&&... Args) {
        return warn(std::forward<T&&>(Args)...) << '\n';
    }
    template<typename... T>
    inline std::ostream& errorln(T&&... Args) {
        return error(std::forward<T&&>(Args)...) << '\n';
    }
    template<typename... T>
    inline std::ostream& noticeln(T&&... Args) {
        return notice(std::forward<T&&>(Args)...) << '\n';
    }
    template<typename... T>
    inline std::ostream& successln(T&&... Args) {
        return success(std::forward<T&&>(Args)...) << '\n';
    }


}
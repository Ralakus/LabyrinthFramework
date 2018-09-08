#pragma once
#include <iostream>

#include <string>
#include <vector>
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
        static std::vector<std::string>& StringBuffer() {
            static std::vector<std::string> mStringBuffer;
            return mStringBuffer;
        }
        template<typename... T>
        static std::string StrConcat(T&&... Args){
            return ( ... + std::forward<T&&>(Args));
        }
        static bool Buffer(bool ResetBuffer = false, bool SetBuffer = true) {
            static bool mBuffer = SetBuffer;
            if(ResetBuffer) { mBuffer = SetBuffer; }
            return mBuffer; 
        }
    };

    template<typename... T>
    inline std::ostream& print(T&&... Args) {
        if(GStreams::Buffer()) { GStreams::StringBuffer().emplace_back(GStreams::StrConcat(std::string("[ ]: "), std::forward<T&&>(Args)...)); }
        return ((GStreams::PrintOut() << rang::fg::reset << "[ ]: ") << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& warn(T&&... Args) {
        if(GStreams::Buffer()) { GStreams::StringBuffer().emplace_back(GStreams::StrConcat(std::string("[*]: "), std::forward<T&&>(Args)...)); }
        return ((GStreams::ErrorOut()  << rang::fgB::yellow << "[*]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& error(T&&... Args) {
        if(GStreams::Buffer()) { GStreams::StringBuffer().emplace_back(GStreams::StrConcat(std::string("[!]: "), std::forward<T&&>(Args)...)); }
        return ((GStreams::ErrorOut()  << rang::fgB::red << "[!]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& notice(T&&... Args) {
        if(GStreams::Buffer()) { GStreams::StringBuffer().emplace_back(GStreams::StrConcat(std::string("[-]: "), std::forward<T&&>(Args)...)); }
        return ((GStreams::PrintOut()  << rang::fgB::cyan << "[-]: " << rang::fg::reset) << ... << std::forward<T&&>(Args));
    }

    template<typename... T>
    inline std::ostream& success(T&&... Args) {
        if(GStreams::Buffer()) { GStreams::StringBuffer().emplace_back(GStreams::StrConcat(std::string("[^]: "), std::forward<T&&>(Args)...)); }
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
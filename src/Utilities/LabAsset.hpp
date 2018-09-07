#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <memory>

#include "Detail.hpp"
#include "IO.hpp"

namespace Labyrinth {

    class LabAsset;

    constexpr auto INVALID_LABASSET = "AE_INVALID_LABASSET";

    class LabAssetNode {

        friend class LabAsset;

    public:

        enum class Type : unsigned char {
            Aesset, String
        };

    private:

        std::string mContent;
        std::map<std::string, std::unique_ptr<LabAssetNode>> mNodes;
        LabAsset* mParent;
        bool mNeedsParse;

        Type mType;

    public:

        LabAssetNode(const std::string& Data, Type NodeType, LabAsset* Parent)
            : mContent(Data), mType(NodeType), mParent(Parent), mNeedsParse(true) { Parse(); }

        int Parse();

        static LabAssetNode& InvalidLabAsset() {
            static LabAssetNode mNode = LabAssetNode(INVALID_LABASSET, Type::String, nullptr);
            return mNode;
        }

        LabAssetNode& operator[](const std::string& Key);

        template<class T>
		T As() {
			T R;
			if (!mContent.empty() && mContent != INVALID_LABASSET) {
				try {
					R = Detail::FromString<T>(mContent);
				}
				catch (std::exception& e) {
                    errorln("Failed to parse: \"", mContent, "\" into requested type!");
				}
			}
			return R;
		}

		template<class T>
		T As(T Default) {
			T R = Default;
			if (!mContent.empty() && mContent != INVALID_LABASSET) {
				try {
					R = Detail::FromString<T>(mContent);
				}
				catch (std::exception& e) {
                    errorln("Failed to parse: \"", mContent, "\" into requested type!");
                    noticeln("Returning default value...");
				}
			}
			return R;
		}

        const std::string& String() const { return mContent; }

    };

    class LabAsset {

        std::string mContent;
        std::map<std::string, std::unique_ptr<LabAssetNode>> mNodes;
        bool mNeedsParse;

    public:

        LabAsset() : mNeedsParse(true) {}
        LabAsset(std::istream& InputStream);

        int Parse();

        bool Load(std::istream& InputStream);
        bool Load(const std::string& Data);

        LabAssetNode& operator[](const std::string& Key);

    };




    LabAsset::LabAsset(std::istream& InputStream) {
        std::stringstream ss;
        ss << InputStream.rdbuf();
        mContent = ss.str();
        Parse();
    }

    int LabAsset::Parse() {

        enum ParseState {
            Scanning, Name, Value,
        } State;
        for(size_t i = 0; i < mContent.size();) {

            i = mContent.find('<', i);
            if(i == std::string::npos) {
                continue;
            }
            size_t ValueIndex = mContent.find('/', i);
            if(ValueIndex == std::string::npos) {
                errorln("Expected a value definition!");
                //continue;
                return -1;
            }
            ValueIndex++;
            if(ValueIndex == mContent.size()) {
                continue;
            }

            size_t NextClosing = mContent.find('>', ValueIndex);
            size_t PassedOpenings = std::count(mContent.begin() + ValueIndex, mContent.begin() + NextClosing, '<');
            size_t PassedClosings;
            for(PassedClosings = PassedOpenings; PassedClosings >= 1; PassedClosings--) {
                NextClosing = mContent.find('>', NextClosing + 1);
                if(NextClosing == std::string::npos) {
                    errorln("Expected a closing statement for \"", mContent.substr(i + 1, ValueIndex - i - 2), "\"!");
                    break;
                }
            }
            if(PassedClosings != 0) {
                errorln("Failed to find nested closing statement!");
                return -2;
            }

            mNodes[mContent.substr(i + 1, ValueIndex - i - 2)] = std::make_unique<LabAssetNode>(
                    mContent.substr(ValueIndex, NextClosing - ValueIndex),
                    PassedOpenings > 0 ? LabAssetNode::Type::Aesset : LabAssetNode::Type::String,
                    this
            );

            i = NextClosing;

        }

        mNeedsParse = false;
        return 0;

    }

    bool LabAsset::Load(std::istream& InputStream) {
        std::stringstream ss;
        ss >> InputStream.rdbuf();
        mContent = ss.str();
        return Parse() >= 0;
    }
    bool LabAsset::Load(const std::string& Data)   { mContent = Data; return Parse() >= 0;}

    LabAssetNode& LabAsset::operator[](const std::string& Key) {
        if(mNodes.count(Key) < 1)   { return LabAssetNode::InvalidLabAsset(); }
        if(mNodes[Key]->mNeedsParse) { return mNodes[Key]->Parse() >= 0 ? *mNodes[Key] : LabAssetNode::InvalidLabAsset(); }
        return *mNodes[Key];
    }







    int  LabAssetNode::Parse() {

        enum ParseState {
            Scanning, Name, Value,
        } State;
        for(size_t i = 0; i < mContent.size();) {

            i = mContent.find('<', i);
            if(i == std::string::npos) {
                continue;
            }
            size_t ValueIndex = mContent.find('/', i);
            if(ValueIndex == std::string::npos) {
                errorln("Expected a value definition!");
                //continue;
                return -1;
            }
            ValueIndex++;
            if(ValueIndex == mContent.size()) {
                continue;
            }

            size_t NextClosing = mContent.find('>', ValueIndex);
            size_t PassedOpenings = std::count(mContent.begin() + ValueIndex, mContent.begin() + NextClosing, '<');
            size_t PassedClosings;
            for(PassedClosings = PassedOpenings; PassedClosings >= 1; PassedClosings--) {
                NextClosing = mContent.find('>', NextClosing + 1);
                if(NextClosing == std::string::npos) {
                    errorln("Expected a closing statement for \"", mContent.substr(i + 1, ValueIndex - i - 2), "\"!");
                    break;
                }
            }
            if(PassedClosings != 0) {
                errorln("Failed to find nested closing statement!");
                return -2;
            }

            mNodes[mContent.substr(i + 1, ValueIndex - i - 2)] = std::make_unique<LabAssetNode>(
                    mContent.substr(ValueIndex, NextClosing - ValueIndex),
                    PassedOpenings > 0 ? LabAssetNode::Type::Aesset : LabAssetNode::Type::String,
                    this->mParent
            );

            i = NextClosing;

        }

        mNeedsParse = false;
        return 0;

    }

    LabAssetNode&  LabAssetNode::operator[](const std::string& Key) {
        if(mNodes.count(Key) < 1)   { return LabAssetNode::InvalidLabAsset(); }
        if(mNodes[Key]->mNeedsParse) { return mNodes[Key]->Parse() >= 0 ? *mNodes[Key] : LabAssetNode::InvalidLabAsset(); }
        return *mNodes[Key];
    }






}

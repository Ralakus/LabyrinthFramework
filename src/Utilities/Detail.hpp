#include <string>

namespace Labyrinth::Detail {

    template<class T>
	inline T FromString(const std::string& Value) {
		//static_assert(false, "Type conversion not supported");
		return T(Value);
	}

	template<>
	inline int FromString<int>(const std::string& Value) { return std::stoi(Value); }

	template<>
	inline long FromString<long>(const std::string& Value) { return std::stol(Value); }

	template<>
	inline long long FromString<long long>(const std::string& Value) { return std::stoll(Value); }

	template<>
	inline unsigned int FromString<unsigned int>(const std::string& Value) { return std::stoul(Value); }

	template<>
	inline unsigned long FromString<unsigned long>(const std::string& Value) { return std::stoul(Value); }

	template<>
	inline unsigned long long FromString<unsigned long long>(const std::string& Value) { return std::stoull(Value); }

	template<>
	inline float FromString<float>(const std::string& Value) { return std::stof(Value); }

	template<>
	inline double FromString<double>(const std::string& Value) { return std::stod(Value); }

	template<>
	inline long double FromString<long double>(const std::string& Value) { return std::stold(Value); }

	template<>
	inline std::string FromString<std::string>(const std::string& Value) { return Value; }

	template<>
	inline bool FromString<bool>(const std::string& Value) {
		if (Value == "true" || Value == "TRUE" || Value == "True") {
			return true;
		}
		else {
			return false;
		}
	}

}
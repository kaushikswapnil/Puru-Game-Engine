#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>

//#####################
//SmartEnum - Documentation
//#####################
//To create:
//----------
//	Use SMARTENUM(<enum name>, <element1>, <element2>, ...);
//	You can also specify type by using SMARTENUM_T(<enum name>, <type>, <element1>, ..); 
//	SMARTENUM(Color, Red, Green, Blue); //same as enum Color { Red, Green, Blue };
//	SMARTENUM(Color, Red = 2, Green, Blue = 4);
//----------
//To assign to a variable;
//----------
//	Color color = Color::Red;
//----------
//To use with ostream:
//----------
//	std::cout << color; //Should print Red
//----------
//To use with switch:
//----------
//	switch (Color)
//	{
//		case Color::Red:
//		case 4:
//	}

//Defines a const unsigned int _variadicArgumentsCount wherever you declare this. 
#define MACRO_VARIADIC_ARGUMENT_COUNT(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

//This macro should help us to apply a macro/function to all pass arguments. #TODO Find a better way to split and parse arguments
#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP##count

#define APPLY(MACRO, ...) IDENTITY(MACRO(__VA_ARGS__))

//This expands __VA_ARGS__ eagerly. This is because in MSVC preprocessor, __VA_ARGS__ is taken to be a combined token, and is replaced after the sub macro is expanded, causing MSVC to sometimes read __VA_ARGS__ as a combination of all tokens.
#define IDENTITY(x) x

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, count, ...) count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))

#define STRINGIZE_SINGLE(expression) #expression,
#define STRINGIZE_MULTI_ARGUMENTS(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))

struct _ArgumentToIntConverter //We use this struct to convert the enum values to integer values
{
	_ArgumentToIntConverter(const int& value) : _m_Value(value) { }
	_ArgumentToIntConverter& operator=(const int& value) { return *this; }  //This negates any Enum value initializations in the macro argument. Think SMART_ENUM(TestEnum, Red = 1, Blue, Green)

	operator int() const { return _m_Value; }  //This is a type cast operator. Think (int)_ArgumentToIntConverter. Lets this be converted back to an integer
	int _m_Value;
};

#define PREFIX__ArgumentToIntConverter_SINGLE(expression) (_ArgumentToIntConverter)expression, //Keeping the comma
#define PREFIX__ArgumentToIntConverter_MULTI_ARGUMENTS(...) IDENTITY(MAP(PREFIX__ArgumentToIntConverter_SINGLE, __VA_ARGS__))

#define SMARTENUM(_Enumname, ...) \
	struct _Enumname \
	{ \
		enum _Enumeration \
		{ \
			__VA_ARGS__ \
		}; \
		\
		_Enumname(const _Enumeration& value) : m_Value(value) {} \
		_Enumname(const _Enumname& other) : m_Value(other.m_Value) {} \
		const _Enumname& operator=(const _Enumeration& value) { m_Value = value; } \
		operator _Enumeration() { return static_cast<_Enumeration>(m_Value); } \
        friend std::ostream &operator<<(std::ostream& outputStream, const _Enumname& smartEnum) \
        { \
            outputStream << smartEnum.ToString(); \
            return outputStream; \
        } \
		\
		const std::string& ToString() const \
        { \
            const int* const values = GetValues(); \
			size_t index = 0; \
			while (index < m_Count) \
			{ \
				if (values[index] == m_Value) \
                    break; \
				++index; \
			} \
            \
            /*ASSERT HERE: Could not find index of enum value. Something is wrong. #TODO Have your own debug and assert system.*/\
            return GetNames()[index]; \
        } \
		private: \
		_Enumeration m_Value; \
		\
        static constexpr size_t m_Count = MACRO_VARIADIC_ARGUMENT_COUNT(__VA_ARGS__); \
		\
		static const int* const GetValues() \
		{ \
			static const int values[] = { IDENTITY(PREFIX__ArgumentToIntConverter_MULTI_ARGUMENTS(__VA_ARGS__)) }; \
			return values; \
		} \
        \
		static constexpr bool IsUnwantedCharacterInName(const char c) \
		{ \
			switch(c) \
			{ \
				case ' ': \
				case '=': \
					return true; \
					break; \
				default: \
					return isdigit(c); \
					break; \
			} \
			\
			return false; \
		} \
		static const std::string* const GetNames() \
		{ \
			static std::string names[] = { IDENTITY(STRINGIZE_MULTI_ARGUMENTS(__VA_ARGS__)) }; \
			static bool initialized = false; \
			\
			if(!initialized) \
			{ \
				for (std::string& name : names) \
				{ \
					name.erase(std::remove_if(name.begin(), name.end(), &IsUnwantedCharacterInName), name.end()); \
				} \
				\
				initialized = true; \
			} \
			return names; \
		} \
        \
	}
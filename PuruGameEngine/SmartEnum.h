#pragma once

//This macro should help us to apply a macro/function to all pass arguments
#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP##count

#define APPLY(MACRO, ...) IDENTITY(MACRO(__VA_ARGS__))

//This expands __VA_ARGS__ eagerly. This is because in MSVC preprocess, __VA_ARGS__ is taken to be a combined token, and is replaced after the sub macro is expanded, causing MSVC to sometimes read __VA_ARGS__ as a combination of all tokens.
#define IDENTITY(x) x

#define MAP1(m, x) m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, count, ...) count
#define COUNT(...) IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 4, 3, 2, 1))

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

#define SMART_ENUM(_EnumName, ...) \
	struct _EnumName \
	{ \
		enum _Enumeration \
		{ \
			__VA_ARGS__ \
		}; \
		\
		_EnumName(const _Enumeration& value) : m_Value(value) {} \
		_EnumName(const _EnumName& other) : m_Value(other.m_Value) {} \
		const _EnumName& operator=(const _Enumeration& value) { m_Value = value; } \
		operator _Enumeration() { return static_cast<_Enumeration>(m_Value); } \
		\
		private: \
		_Enumeration m_Value; \
		\
		static const size_t _count = IDENTITY(COUNT(__VA_ARGS__));
\
static const int* const _GetValues() \
{ \
static const int _values[] = { IDENTITY(PREFIX__ArgumentToIntConverter_MULTI_ARGUMENTS(__VA_ARGS__)) }; \
return _values; \
} \
\
static const char* const _GetNames() \
{ \
static const char* rawNames = { IDENTITY(STRINGIZE_MULTI_ARGUMENTS(__VA_ARGS__)) }; \
\
static char* processedNames[_count]; \ //Creating an array 
static bool initialized = false; \
\
if (!initialized) \
{ \
for (size_t index = 0; index < _count; ++index) \
{ \
//remove offending characters
size_t length = std::strcspn(rawNames[index], "=\t\n\r"); \ //strcspn returns the first index where any of the characters given matches
\
processedNames[index] = new char[length + 1]; \
\
std::strncpy(processedNames[index], rawNames[index], length); \//copy good characters
\
(processedNames[index])[length] = "\0"; \
} \
} \
\
return processedNames; \
} \
};


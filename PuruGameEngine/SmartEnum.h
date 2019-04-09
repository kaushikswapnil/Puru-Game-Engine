#pragma once

#define STRINGIZE_SINGLE(expression) #expression
#define STRINGIZE_MULTI_ARGUMENTS(...) APPLY_FUNC_TO_ALL(STRINGIZE_SINGLE, __VA_ARGS__)

#define SMART_ENUM(_EnumName, ...) \
	( \
		struct _EnumName \
		{ \
			enum _Enumeration \
			{ \
				__VA_ARG__ \
			}; \
			\
			_EnumName(const _Enumeration& value) : m_Value(value) {} \
			_EnumName(const _EnumName& other) : m_Value(other.m_Value) {} \
			const _EnumName& operator=(const _Enumeration& value) { }
			operator _Enumeration() { return static_cast<_Enumeration>(value); }
			\
			private: \
			_Enumeration m_Value; \
			\
			//We use this struct to convert the enum values to integer values
			struct _ArgumentToIntConverter \
			{ \
				_ArgumentToIntConverter(const int& value) : m_Value(value) {} \
				_ArgumentToIntConverter& operator=(const int& value) \
				{
					m_Value = value; \
				} \
				\
				operator int() const { return m_Value; } \ //This is a type cast operator. Think (int)_ArgumentToIntConverter. Lets this be converted back to an integer
				const _ArgumentToIntConverter& operator=(const int& dummy) { return *this; } \ //This negates any Enum value initializations in the macro argument. Think SMART_ENUM(TestEnum, Red = 1, Blue, Green)
				int m_Value; \
			}; \
			\
			static const int* const GetValues() \
			{ \
				static const int _values[] = { PREFIX__ArgumentToIntConverter_MULTI_ARGUMENTS_(__VA_ARG__) }; \
				return values; \
			} \
			\
			static const char* const GetNames() \
			{ \
				static const char* _rawNames = { STRINGIZE_MULTI_ARGUMENTS(__VA_ARG__) }; \

				static char* _processedNames[_COUNT];
				static bool _initialized = false;

				if (!_initialized)
				{

				}

				return _processedNames; \
			}
		}; \
	) \
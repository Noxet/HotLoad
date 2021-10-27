#pragma once

/*
 * It is difficult to export a template class to a DLL.
 * Instead, we define everything in the header here, and we can just import it normally
 */

#include <array>
#include <stdexcept>
#include <unordered_map>
#include <string>

#include "Windows.h"


namespace HotLoad
{
	template <typename E, size_t numSymbols>
	class HotLoadModule
	{
	public:
		HotLoadModule()
		{
			/*
			 * This can't be on the stack since it grows dynamically, hence
			 * the size of the class is unknown and we can not export is to DLL.
			 * https://stackoverflow.com/a/6869033
			 */
			m_symbols = new std::unordered_map<std::string, void*>();
		}


		virtual ~HotLoadModule()
		{
			delete m_symbols;
		}


		static void loadLibrary() { getInstance().load(); }
		static void reloadLibrary() { getInstance().reload(); }

	protected:
		/*
		 * Returns a singleton, we only want to have one instance of this class in memory.
		 * This class does not return an instance of HotLoadModule (itself) since it is supposed
		 * to be inherited by a subclass which should set the instance to itself.
		 */
		static E& getInstance()
		{
			static E instance;
			return instance;
		}


		// Returns the path to the library on disk
		virtual const char* getPath() const = 0;

		/*
		 * Returns a reference to an array of size numSymbols of C-strings,
		 * used when loading/reloading the library to lookup the addresses of all exported symbols
		 */
		virtual std::array<const char*, numSymbols>& getSymbolNames() const = 0;

		/*
		 * The type R is the return value.
		 * "typename... Args" denotes a parameter pack, i.e., variadic num of argument with varying types
		 * ex. If we call execute with: execute<string, int, double>("test", 1337, 42.0); it would be turned in to:
		 *
		 * char execute(string str, int arg1, double arg2)
		 * {
		 *		...
		 *		return reinterpret_cast<string(*)(int, double)>(symbol->second)(arg1, arg2);
		 *		...
		 * }
		 */
		template <typename R, typename... Args>
		R execute(const char* name, Args ... args)
		{
			// Find the function address
			const auto symbol = m_symbols->find(name);
			if (symbol != m_symbols->end())
			{
				// Cast it to the correct signature
				return reinterpret_cast<R(*)(Args ...)>(symbol->second)(args...);
			}

			throw std::runtime_error(std::string("Function not found: ") + name);
		}


		template <typename T>
		T* getVar(const char* name)
		{
			// Find the symbol address
			const auto symbol = m_symbols->find(name);
			if (symbol != m_symbols->end())
			{
				return static_cast<T*>(symbol->second);
			}

			return nullptr;
		}


	private:
		void load()
		{
			/*
			 * Loads the DLL into memory.
			 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
			 */
			m_libHandle = LoadLibrary(getPath());
			loadSymbols();
		}


		void reload()
		{
			close();
			m_symbols->clear();
			load();
		}


		void close()
		{
			/*
			 * Frees the DLL module.
			 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary
			 */
			FreeLibrary(static_cast<HMODULE>(m_libHandle));
		}


		void loadSymbols()
		{
			// Find all symbols and store it in a map
			for (const char* symbol : getSymbolNames())
			{
				/*
				 * Gets the address of an exported function or variable.
				 * https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
				 */
				(*m_symbols)[symbol] = GetProcAddress(static_cast<HMODULE>(m_libHandle), symbol);
			}
		}


		void* m_libHandle = nullptr;
		std::unordered_map<std::string, void*>* m_symbols;
	};
}

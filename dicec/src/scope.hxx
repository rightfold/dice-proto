#pragma once

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <functional>
#include <utility>

namespace dc
{
    template<typename T>
    class scope
    {
    public:
        scope();

        template<typename U>
        scope(scope<U> const& parent);

        T const& find(llvm::StringRef name) const;
        void declare(llvm::StringRef name, T element);

    private:
        std::function<T const&(llvm::StringRef)> parent;
        llvm::StringMap<T> symbols;
    };

    template<typename T>
    scope<T>::scope() = default;

    template<typename T>
    template<typename U>
    scope<T>::scope(scope<U> const& parent)
        : parent([&] (auto&& name) { return parent.find(name); })
    {
    }

    template<typename T>
    T const& scope<T>::find(llvm::StringRef name) const
    {
        auto it = symbols.find(name);
        if (it == symbols.end())
        {
            if (parent)
            {
                return parent(name);
            }
            else
            {
                // TODO: This should return a useful exception.
                throw "find: no such symbol";
            }
        }
        return it->getValue();
    }

    template<typename T>
    void scope<T>::declare(llvm::StringRef name, T element)
    {
        if (symbols.count(name))
        {
            // TODO: This should return a useful exception.
            throw "declare: duplicate symbol";
        }
        symbols.try_emplace(name, std::move(element));
    }
}

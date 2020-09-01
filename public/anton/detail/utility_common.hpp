#pragma once

#include <anton/type_traits.hpp>
#include <anton/types.hpp>

namespace anton {
    template<typename T>
    constexpr void swap(T& a, T& b) {
        T tmp(move(a));
        a = move(b);
        b = move(tmp);
    }

    template<typename T, typename U, typename = void>
    struct Is_Swappable_With: False_Type {};

    // Note: swap left unprefixed to allow ADL of user-defined swap specializations
    template<typename T, typename U>
    struct Is_Swappable_With<T, U, void_sink<decltype(swap(declval<U&>(), declval<T&>())), decltype(swap(declval<T&>(), declval<U&>()))>>: True_Type {};

    template<typename T, typename U>
    constexpr bool is_swappable_with = Is_Swappable_With<T, U>::value;

    template<typename T>
    struct Is_Swappable: Is_Swappable_With<T, T> {};

    template<typename T>
    constexpr bool is_swappable = Is_Swappable<T>::value;

    // Integer_Sequence
    //
    template<typename T, T... Integers>
    struct Integer_Sequence {
        using type = Integer_Sequence<T, Integers...>;
    };

    template<typename T, T... Integers>
    using integer_sequence = Integer_Sequence<T, Integers...>;

    namespace detail {
        template<typename T, T, T, typename>
        struct Make_Integer_Sequence;

        template<typename T, T N, T X, T... Integers>
        struct Make_Integer_Sequence<T, N, X, Integer_Sequence<T, Integers...>> {
            using type = typename Make_Integer_Sequence<T, N, X + 1, Integer_Sequence<T, Integers..., X>>::type;
        };

        template<typename T, T N, T... Integers>
        struct Make_Integer_Sequence<T, N, N, Integer_Sequence<T, Integers...>> {
            using type = Integer_Sequence<T, Integers...>;
        };
    } // namespace detail

    // Make_Integer_Sequence
    // Creates an Integer_Sequence which starts at 0 and ends at N - 1.
    //
    template<typename T, T N>
    struct Make_Integer_Sequence {
        using type = typename detail::Make_Integer_Sequence<T, N, 0, Integer_Sequence<T>>::type;
    };

    template<typename T, T N>
    using make_integer_sequence = typename Make_Integer_Sequence<T, N>::type;
} // namespace anton

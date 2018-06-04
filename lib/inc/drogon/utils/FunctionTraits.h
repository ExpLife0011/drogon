#pragma once

#include <tuple>
#include<type_traits>
namespace drogon{
    class HttpRequest;
    class HttpResponse;
    namespace utility {

        template<typename> struct FunctionTraits;

        template <typename Function>
        struct FunctionTraits : public FunctionTraits<
                decltype(&std::remove_reference<Function>::type::operator())
        > { };

        template <
                typename    ClassType,
                typename    ReturnType,
                typename... Arguments
        >
        struct FunctionTraits<
                ReturnType(ClassType::*)(Arguments...) const
        > : FunctionTraits<ReturnType(*)(Arguments...)> { };

        /* support the non-const operator ()
         * this will work with user defined functors */
        template <
                typename    ClassType,
                typename    ReturnType,
                typename... Arguments
        >
        struct FunctionTraits<
                ReturnType(ClassType::*)(Arguments...)
        > : FunctionTraits<ReturnType(*)(Arguments...)> {
            static const bool isClassFunction=true;
        };


        template <
                typename    ClassType,
                typename    ReturnType,
                typename... Arguments
        >
        struct FunctionTraits<
                ReturnType(ClassType::*)(const HttpRequest& req,std::function<void (HttpResponse &)>callback,Arguments...)
        > : FunctionTraits<ReturnType(*)(Arguments...)> {
            static const bool isHTTPApiFunction=true;
            typedef ClassType class_type;
            };

        template <
                typename    ReturnType,
                typename... Arguments
        >
        struct FunctionTraits<
                ReturnType(*)(Arguments...)
        > {
            typedef ReturnType result_type;

            template <std::size_t Index>
            using argument = typename std::tuple_element<
                    Index,
                    std::tuple<Arguments...>
            >::type;

            static const std::size_t arity = sizeof...(Arguments);

            static const bool isHTTPApiFunction=false;
        };

    }
}

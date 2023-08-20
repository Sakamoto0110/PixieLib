#pragma once

#include <iostream>

namespace pxTests {
    /**
     * \brief
     * \tparam T The instance that member is of.
     */
    template<typename T>
    class MemberFunctionInvoker {
    public:
        template<typename TReturn, typename...Tx> class FunctionInvoker;
        using cT = const T;
        using rT = T&;
        using crT = const T&;
        using pT = T*;
        using v = void;
        template<typename return_type, typename...args_type>
        using sig_any = FunctionInvoker<return_type, args_type...>;

        template<typename...args_type>
        using sig_v_any = FunctionInvoker<v, args_type...>;

        using sig_v_crT = FunctionInvoker<v, crT>;
    public:
        /**
         * \brief
         * \tparam TReturn Type that will be returned ( can be void )
         * \tparam Tx Argument list
         */
        template<typename TReturn, typename...Tx>
        class FunctionInvoker {
        public:
            using _type_ = TReturn(T::*)(Tx...);

            template<_type_... Fx, typename...Ty>
            static void _invoke_(T& t, Ty...obj) {
                return ((t.*Fx)(obj...), ...);


            }
        };
    public:
        /**
         * \brief hahaha
         * \tparam any_fn_signature A FunctionInvoker template instance with the arguments of the desired function
         * \tparam fn One or more functions to be invoked
         * \tparam Tx Arguments types that will be supplied to the function
         * \param e Instance that contains the function
         * \param obj Arguments that will be supplied to the function
         * */
        template<class any_fn_signature, typename any_fn_signature::_type_...fn, typename...Tx >
        static void _call_any_fn_signature_(T& e, Tx... obj) {
            any_fn_signature::template _invoke_<fn...>(e, obj...);
        }
        using _T = T;
    };



   

    
    template<int TotalLoops = 1, int NumberOfPauses = 1, bool PauseBeforeStart = true, typename TxCallable>
    int Run(TxCallable fn) {
        static_assert(TotalLoops > 0, "TotalLoops MUST be a non-zero positive integer");
        static_assert(NumberOfPauses > 0, "NumberOfPauses MUST be a positive integer, including zero.");
        static_assert(NumberOfPauses <= TotalLoops, "NumberOfPauses Must be less then or equal TotalLoops.");

        constexpr int Slice = TotalLoops / NumberOfPauses;
        std::cout << "Test initializing.\n";
        std::cout << "Press C to clear the screen and start. Press any other key to start\n";
        char key = 0;
        if(PauseBeforeStart) {
            key = _getch();
            if (key == 'c') { system("cls"); }
        }

        for (int i = 0; i < NumberOfPauses; i++) {
            for (int j = 0; j < Slice; j++) {
                fn();
            }
            std::cout << "Section terminated. Press C to clear screen and continue. Press T to terminate. Press any other key to continue.\n";
            int ___ = 0;
        	key = _getch();
            if (key == 'c') { system("cls"); }
            if (key == 't') { std::cout << "Test aborted.\n"; return 1; }

        }
        std::cout << "Test terminated.\n";

        return 0;
    }
}

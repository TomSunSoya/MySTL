#ifndef MYSTL_FUNCTIONAL_H
#define MYSTL_FUNCTIONAL_H

#include <utility>
#include <exception>

namespace MySTL {
    template<typename T>
    struct Plus {
        T operator()(const T &a, const T &b) const {
            return a + b;
        }
    };

    template<typename T>
    struct Minus {
        T operator()(const T &a, const T &b) const {
            return a - b;
        }
    };

    template<typename T>
    struct Multiplies {
        T operator()(const T &a, const T &b) const {
            return a * b;
        }
    };

    template<typename T>
    struct Divides {
        T operator()(const T &a, const T &b) const {
            return a / b;
        }
    };

    template<typename T>
    struct Less {
        bool operator()(const T &a, const T &b) const {
            return a < b;
        }
    };

    template<typename T>
    struct Greater {
        bool operator()(const T &a, const T &b) const {
            return a > b;
        }
    };

    template<typename T>
    struct EqualTo {
        bool operator()(const T &a, const T &b) const {
            return a == b;
        }
    };


    template<typename T>
    struct LogicalAnd {
        bool operator()(const T &a, const T &b) const {
            return a && b;
        }
    };

    template<typename T>
    struct LogicalOr {
        bool operator()(const T &a, const T &b) const {
            return a || b;
        }
    };

    template<typename T>
    struct LogicalNot {
        bool operator()(const T &a) const {
            return !a;
        }
    };

    template<typename Func, typename Arg>
    auto Bind(Func func, Arg arg) {
        return [func, arg](auto &&... otherArgs) {
            return func(arg, std::forward<decltype(otherArgs)>(otherArgs)...);
        };
    }

    template<typename Predicate>
    auto NotFn(Predicate pred) {
        return [pred](auto &&... args) {
            return !pred(std::forward<decltype(args)>(args)...);
        };
    }

    class bad_function_call : public std::exception {
    public:
        // return error message
        [[nodiscard]] const char *what() const noexcept override {
            return "bad_function_call: Attempt to call an empty function object";
        }
    };

    template<typename Signature>
    class Function;

    template<typename Ret, typename... Args>
    class Function<Ret(Args...)> {
    public:
        Function() : callable(nullptr) {}

        template<class Func>
        explicit Function(Func f) : callable(new Callable<Func>(f)) {}

        Function(const Function &other) : callable(other.callable ? other.callable->clone() : nullptr) {}

        Function(Function &&other) noexcept: callable(other.callable) {
            other.callable = nullptr;
        }

        ~Function() {
            delete callable;
        }

        Function &operator=(const Function &other) {
            if (this != &other) {
                delete callable;
                callable = other.callable ? other.callable->clone() : nullptr;
            }
            return *this;
        }

        Function &operator=(Function &&other) noexcept {
            if (this != &other) {
                delete callable;
                callable = other.callable;
                other.callable = nullptr;
            }
            return *this;
        }

        Ret operator()(Args &&... args) {
            if (!callable)
                throw bad_function_call();
            return callable->invoke(std::forward<Args>(args)...);
        }

        explicit operator bool() const {
            return callable != nullptr;
        }

    private:
        struct CallableBase {
            virtual ~CallableBase() = default;

            virtual Ret invoke(Args &&... args) = 0;

            virtual CallableBase *clone() = 0;
        };

        template<class Func>
        struct Callable : public CallableBase {
            Func func;

            explicit Callable(Func f) : func(f) {}

            Ret invoke(Args &&... args) override {
                return func(std::forward<Args>(args)...);
            }

            CallableBase *clone() override {
                return new Callable(func);
            }
        };

        CallableBase *callable;
    };

    template<typename F1, typename F2>
    auto Compose(F1 f1, F2 f2) {
        return [f1, f2](auto &&... args) {
            return f1(f2(std::forward<decltype(args)>(args)...));
        };
    }


}


#endif //MYSTL_FUNCTIONAL_H

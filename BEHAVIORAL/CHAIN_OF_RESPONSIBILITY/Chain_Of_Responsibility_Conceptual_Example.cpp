#include <iostream>
#include <vector>

// The Handler interface declares a method for building the chain of handlers.
// It also declares a method for executing a request.
class Handler
{
public:
    virtual ~Handler() = default;
    virtual Handler* setNext(Handler* const handler) = 0;
    virtual std::string handle(const std::string& request) const = 0;
};

// The default chaining behavior can be implemented inside a base handler class.
class HandlerBase : public Handler
{
    const Handler* m_nextHandler;

public:
    explicit HandlerBase()
        : m_nextHandler(nullptr)
    { }
    Handler* setNext(Handler* const handler) override
    {
       m_nextHandler = handler;
       // Returning a handler from here will let us link handlers in a convenient way like this: monkey->setNext(squirrel)->setNext(dog);
       return handler;
    }
    std::string handle(const std::string& request) const override
    {
        return m_nextHandler ? m_nextHandler->handle(request) : std::string();
    }
};

// All Concrete Handlers either handle a request or pass it to the next handler in the chain.
class MonkeyHandler : public HandlerBase
{
public:
    std::string handle(const std::string& request) const override
    {
        return request == "Banana" ? "Monkey: I'll eat the " + request + ".\n" : HandlerBase::handle(request);
    }
};

class SquirrelHandler : public HandlerBase
{
public:
    std::string handle(const std::string& request) const override
    {
        return request == "Nut" ? "Squirrel: I'll eat the " + request + ".\n" : HandlerBase::handle(request);
    }
};

class DogHandler : public HandlerBase
{
public:
    std::string handle(const std::string& request) const override
    {
        return request == "MeatBall" ? "Dog: I'll eat the " + request + ".\n" : HandlerBase::handle(request);
    }
};

// The client code is usually suited to work with a single handler.
// In most cases, it is not even aware that the handler is part of a chain.
void clientCode(const Handler& handler)
{
    const std::vector<std::string> foods{"Nut", "Banana", "Cup of coffee"};
    for (const std::string& food : foods)
    {
        std::cout << "Client: Who wants a " << food << "?\n";
        const std::string result = handler.handle(food);
        std::cout << '\t' << (result.empty() ? food + " was left untouched.\n" : result);
    }
}

// The other part of the client code constructs the actual chain.
int main()
{
    MonkeyHandler* const monkey = new MonkeyHandler;
    SquirrelHandler* const squirrel = new SquirrelHandler;
    DogHandler* const dog = new DogHandler;
    monkey->setNext(squirrel)->setNext(dog);

    // The client should be able to send a request to any handler, not just the first one in the chain.
    std::cout << "Chain: Monkey -> Squirrel -> Dog\n\n";
    clientCode(*monkey);

    std::cout << "\nSubchain: Squirrel -> Dog\n\n";
    clientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;
}

#include <algorithm>
#include <iostream>
#include <memory>

// The Strategy interface declares operations common to all supported versions of some algorithm.
// The Context uses this interface to call the algorithm defined by Concrete Strategies.
class Strategy
{
public:
    virtual ~Strategy() = default;
    virtual std::string doAlgorithm(std::string data) const = 0;
};

// Concrete Strategies implement the algorithm while following the base Strategy
// interface. The interface makes them interchangeable in the Context.
class ConcreteStrategyA : public Strategy
{
private:
    std::string doAlgorithm(std::string data) const override
    {
        std::sort(std::begin(data), std::end(data));
        return data;
    }
};

class ConcreteStrategyB : public Strategy
{
private:
    std::string doAlgorithm(std::string data) const override
    {
        std::sort(std::begin(data), std::end(data), std::greater<std::string::value_type>());
        return data;
    }
};

// The Context defines the interface of interest to clients.
class Context
{
    // The Context maintains a reference to one of the Strategy objects. The Context does not
    // know the concrete class of a strategy. It should work with all strategies via the Strategy interface.
private:
    std::unique_ptr<Strategy> m_strategy;
    // Usually, the Context accepts a strategy through the constructor,
    // but also provides a setter to change it at runtime.
public:
    explicit Context(std::unique_ptr<Strategy> strategy)
        : m_strategy(std::move(strategy))
    { }
    // Usually, the Context allows replacing a Strategy object at runtime.
    void setStrategy(std::unique_ptr<Strategy> strategy) { m_strategy = std::move(strategy); }
    // The Context delegates some work to the Strategy object instead of
    // implementing multiple versions of the algorithm on its own.
    void doSomeBusinessLogic() const
    {
        if (m_strategy)
        {
            std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n"
                      << m_strategy->doAlgorithm("aecbd") << '\n';
        }
        else
        {
            std::cout << "Context: Strategy isn't set\n";
        }
    }
};

// The client code picks a concrete strategy and passes it to the context. The client
// should be aware of the differences between strategies in order to make the right choice.
int main()
{
    Context context(std::make_unique<ConcreteStrategyA>());
    std::cout << "Client: Strategy is set to normal sorting.\n";
    context.doSomeBusinessLogic();

    std::cout << "\nClient: Strategy is set to reverse sorting.\n";
    context.setStrategy(std::make_unique<ConcreteStrategyB>());
    context.doSomeBusinessLogic();
}

#include <iostream>
#include <string>
// The Implementation defines the interface for all implementation classes. It
// doesn't have to match the Abstraction's interface. In fact, the two
// interfaces can be entirely different. Typically the Implementation interface
// provides only primitive operations, while the Abstraction defines higher-
// level operations based on those primitives.
class Implementation
{
public:
    virtual ~Implementation() = default;
    virtual std::string operationImplementation() const = 0;
};

// Each Concrete Implementation corresponds to a specific platform and
// implements the Implementation interface using that platform's API.
class ConcreteImplementationA : public Implementation
{
public:
    std::string operationImplementation() const override
    {
        return "ConcreteImplementationA: Here's the result on the platform A.\n";
    }
};
class ConcreteImplementationB : public Implementation
{
public:
    std::string operationImplementation() const override
    {
        return "ConcreteImplementationB: Here's the result on the platform B.\n";
    }
};

// The Abstraction defines the interface for the "control" part of the two class
// hierarchies. It maintains a reference to an object of the Implementation
// hierarchy and delegates all of the real work to this object.
class Abstraction
{
protected:
    const Implementation* const m_implementation;
public:
    Abstraction(const Implementation* const implementation)
        : m_implementation(implementation)
    { }
    virtual ~Abstraction() = default;
    virtual std::string operation() const
    {
        return "Abstraction: Base operation with:\n" + m_implementation->operationImplementation();
    }
};

// You can extend the Abstraction without changing the Implementation classes.
class ExtendedAbstraction : public Abstraction
{
public:
    ExtendedAbstraction(const Implementation* const implementation)
        : Abstraction(implementation)
    { }
    std::string operation() const override
    {
        return "ExtendedAbstraction: Extended operation with:\n" + m_implementation->operationImplementation();
    }
};

// Except for the initialization phase, where an Abstraction object gets linked
// with a specific Implementation object, the client code should only depend on
// the Abstraction class. This way the client code can support any abstraction-
// implementation combination.
void clientCode(const Abstraction& abstraction)
{
    std::cout << abstraction.operation();
}

// The client code should be able to work with any pre-configured abstraction-implementation combination.
int main()
{
    const Implementation* implementation = new ConcreteImplementationA;
    const Abstraction* abstraction = new Abstraction(implementation);
    clientCode(*abstraction);
    delete implementation;
    delete abstraction;

    std::cout << '\n';

    implementation = new ConcreteImplementationB;
    abstraction = new ExtendedAbstraction(implementation);
    clientCode(*abstraction);
    delete implementation;
    delete abstraction;
}
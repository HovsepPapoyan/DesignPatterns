#include <string>
#include <iostream>

// The Subsystem can accept requests either from the facade or client directly. In any case,
// to the Subsystem, the Facade is yet another client, and it's not a part of the Subsystem.
class Subsystem1
{
public:
    std::string operation1() const { return "Subsystem1: Ready!\n"; }
    std::string operationN() const { return "Subsystem1: Go!\n"; }
};

// Some facades can work with multiple subsystems at the same time.
class Subsystem2
{
public:
    std::string operation1() const { return "Subsystem2: Get ready!\n"; }
    std::string operationZ() const { return "Subsystem2: Fire!\n"; }
};

// The Facade class provides a simple interface to the complex logic of one or several subsystems. The Facade
// delegates the client requests to the appropriate objects within the subsystem. The Facade is also responsible
// for managing their lifecycle. All of this shields the client from the undesired complexity of the subsystem.
class Facade
{
protected:
    const Subsystem1* const m_subsystem1;
    const Subsystem2* const m_subsystem2;
    // Depending on your application's needs, you can provide the Facade with
    // existing subsystem objects or force the Facade to create them on its own.
public:
    // In this case we will delegate the memory ownership to Facade Class
    explicit Facade(const Subsystem1* const subsystem1 = nullptr, const Subsystem2* const subsystem2 = nullptr)
        : m_subsystem1(subsystem1 ? : new Subsystem1)
        , m_subsystem2(subsystem2 ? : new Subsystem2)
    { }
    ~Facade()
    {
        delete m_subsystem1;
        delete m_subsystem2;
    }
    // The Facade's methods are convenient shortcuts to the sophisticated functionality of
    // the subsystems. However, clients get only to a fraction of a subsystem's capabilities.
    std::string operation() const
    {
        std::string result = "Facade initializes subsystems:\n";
        result += m_subsystem1->operation1();
        result += m_subsystem2->operation1();
        result += "Facade orders subsystems to perform the action:\n";
        result += m_subsystem1->operationN();
        result += m_subsystem2->operationZ();
        return result;
    }
};

// The client code works with complex subsystems through a simple interface provided by the Facade. When a facade manages the lifecycle of the
// subsystem, the client might not even know about the existence of the subsystem. This approach lets you keep the complexity under control.
void clientCode(const Facade* const facade)
{
    std::cout << facade->operation();
}

// The client code may have some of the subsystem's objects already created. In this case, it might be worthwhile
// to initialize the Facade with these objects instead of letting the Facade create new instances.
int main()
{
    const Subsystem1* const subsystem1 = new Subsystem1;
    const Subsystem2* const subsystem2 = new Subsystem2;
    const Facade* const facade = new Facade(subsystem1, subsystem2);
    clientCode(facade);
    delete facade;
}

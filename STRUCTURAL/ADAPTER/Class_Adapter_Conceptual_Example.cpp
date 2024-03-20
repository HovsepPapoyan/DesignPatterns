#include <iostream>
#include <string>

// The Adaptee contains some useful behavior, but its interface is incompatible with the
// existing client code. The Adaptee needs some adaptation before the client code can use it.
class Adaptee
{
public:
    std::string specificRequest() const { return "Adaptee: std::string specificRequest() const"; }
};

// The Target defines the domain-specific interface used by the client code.
class Target
{
public:
    virtual ~Target() = default;
    virtual std::string request() const { return "Target: virtual std::string request() const"; }
};

// The Adapter makes the Adaptee's interface compatible with the Target's interface using multiple inheritance.
class Adapter : public Target, public Adaptee
{
public:
    std::string request() const override { return specificRequest(); }
};

// The client code supports all classes that follow the Target interface.
void clientCode(const Target* const target)
{
    std::cout << "void clientCode(const Target* const target)\n" << target->request() << '\n';
}

int main()
{
    const Target* const target = new Target;
    clientCode(target);

    const Adapter* const adapter = new Adapter;
    clientCode(adapter);

    delete target;
    delete adapter;
}

#include <array>
#include <iostream>

class ConcreteComponentA;
class ConcreteComponentB;

// The Visitor Interface declares a set of visiting methods that correspond to
// component classes. The signature of a visiting method allows the visitor to
// identify the exact class of the component that it's dealing with.
class Visitor
{
public:
    virtual void visitConcreteComponentA(const ConcreteComponentA* const element) const = 0;
    virtual void visitConcreteComponentB(const ConcreteComponentB* const element) const = 0;
};

// The Component interface declares an `accept` method that should take the base visitor interface as an argument.
class Component
{
public:
    virtual ~Component() = default;
    virtual void accept(const Visitor* const visitor) const = 0;
};

// Each Concrete Component must implement the `accept` method in such a way that
// it calls the visitor's method corresponding to the component's class.
class ConcreteComponentA : public Component
{
public:
    // Note that we're calling `visitConcreteComponentA`, which matches the current class name.
    // This way we let the visitor know the class of the component it works with.
    void accept(const Visitor* const visitor) const override { visitor->visitConcreteComponentA(this); }
    // Concrete Components may have special methods that don't exist in their base class or interface.
    // The Visitor is still able to use these methods since it's aware of the component's concrete class.
    std::string exclusiveMethodOfConcreteComponentA() const { return "A"; }
};

class ConcreteComponentB : public Component
{
public:
    // Same here: visitConcreteComponentB => ConcreteComponentB
    void accept(const Visitor* const visitor) const override { visitor->visitConcreteComponentB(this); }
    std::string specialMethodOfConcreteComponentB() const { return "B"; }
};

// Concrete Visitors implement several versions of the same algorithm, which can work with all concrete component classes.
// You can experience the biggest benefit of the Visitor pattern when using it with a complex object structure, such as a
// Composite tree. In this case, it might be helpful to store some intermediate state of the algorithm while executing
// visitor's methods over various objects of the structure.
class ConcreteVisitor1 : public Visitor
{
public:
    void visitConcreteComponentA(const ConcreteComponentA* const element) const override
    {
        std::cout << element->exclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
    }
    void visitConcreteComponentB(const ConcreteComponentB* const element) const override
    {
        std::cout << element->specialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
    }
};

class ConcreteVisitor2 : public Visitor
{
public:
    void visitConcreteComponentA(const ConcreteComponentA* const element) const override
    {
        std::cout << element->exclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
    }
    void visitConcreteComponentB(const ConcreteComponentB* const element) const override
    {
        std::cout << element->specialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
    }
};

// The client code can run visitor operations over any set of elements without figuring out their concrete
// classes. The accept operation directs a call to the appropriate operation in the visitor object.
void clientCode(const std::array<const Component*, 2>& components, const Visitor* const visitor)
{
    for (const Component* const comp : components)
    {
        comp->accept(visitor);
    }
}

int main()
{
    const std::array<const Component*, 2> components = { new ConcreteComponentA, new ConcreteComponentB };
    std::cout << "The client code works with all visitors via the base Visitor interface:\n";
    const ConcreteVisitor1* const visitor1 = new ConcreteVisitor1;
    clientCode(components, visitor1);

    std::cout << "\nIt allows the same client code to work with different types of visitors:\n";
    const ConcreteVisitor2* const visitor2 = new ConcreteVisitor2;
    clientCode(components, visitor2);

    for (const Component* const comp : components)
    {
        delete comp;
    }
    delete visitor1;
    delete visitor2;
}
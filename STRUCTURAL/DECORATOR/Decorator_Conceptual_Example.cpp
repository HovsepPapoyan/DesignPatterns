#include <iostream>
#include <string>

// The base Component interface defines operations that can be altered by decorators.
class Component
{
public:
    virtual ~Component() = default;
    virtual std::string operation() const = 0;
};

// Concrete Components provide default implementations of the operations.
// There might be several variations of these classes.
class ConcreteComponent : public Component
{
public:
    std::string operation() const override { return "ConcreteComponent"; }
};

// The base Decorator class follows the same interface as the other components.
// The primary purpose of this class is to define the wrapping interface for all
// concrete decorators. The default implementation of the wrapping code might
// include a field for storing a wrapped component and the means to initialize it.
class Decorator : public Component
{
protected:
    const Component& m_component;
public:
    explicit Decorator(const Component& component)
        : m_component(component)
    { }
    // The Decorator delegates all work to the wrapped component.
    std::string operation() const override { return m_component.operation(); }
};

// Concrete Decorators call the wrapped object and alter its result in some way.
class ConcreteDecoratorA : public Decorator
{
// Decorators may call parent implementation of the operation, instead of calling the
// wrapped object directly. This approach simplifies extension of decorator classes.
public:
    explicit ConcreteDecoratorA(const Component& component)
        : Decorator(component)
    { }
    std::string operation() const override { return "ConcreteDecoratorA(" + Decorator::operation() + ")"; }
};

// Decorators can execute their behavior either before or after the call to a wrapped object.
class ConcreteDecoratorB : public Decorator
{
public:
    explicit ConcreteDecoratorB(const Component& component)
        : Decorator(component)
    { }
    std::string operation() const override { return "ConcreteDecoratorB(" + Decorator::operation() + ")"; }
};

// The client code works with all objects using the Component interface. This
// way it can stay independent of the concrete classes of components it works with.
void clientCode(const Component& component)
{
    std::cout << "RESULT: " << component.operation();
}

int main()
{
    const ConcreteComponent concreteComponent;
    clientCode(concreteComponent);

    std::cout << '\n';

    const ConcreteDecoratorA concreteDecoratorA(concreteComponent);
    const ConcreteDecoratorB concreteDecoratorB(concreteDecoratorA);
    clientCode(concreteDecoratorB);
}

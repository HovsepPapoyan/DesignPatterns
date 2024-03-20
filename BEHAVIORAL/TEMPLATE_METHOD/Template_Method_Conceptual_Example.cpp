#include <iostream>

// The Abstract Class defines a template method that contains a skeleton of some algorithm, composed of
// calls to (usually) abstract primitive operations. Concrete subclasses should implement these operations,
// but leave the template method itself intact.
class AbstractClass
{
public:
    virtual ~AbstractClass() = default;
    // The template method defines the skeleton of an algorithm.
    void templateMethod() const
    {
        baseOperation1();
        requiredOperations1();
        baseOperation2();
        hook1();
        requiredOperation2();
        baseOperation3();
        hook2();
    }

protected:
    // These operations already have implementations.
    void baseOperation1() const { std::cout << "AbstractClass says: I am doing the bulk of the work\n"; }
    void baseOperation2() const { std::cout << "AbstractClass says: But I let subclasses override some operations\n"; }
    void baseOperation3() const { std::cout << "AbstractClass says: But I am doing the bulk of the work anyway\n"; }

    // These operations have to be implemented in subclasses.
    virtual void requiredOperations1() const = 0;
    virtual void requiredOperation2() const = 0;

    // These are "hooks." Subclasses may override them, but it's not mandatory since the hooks already have default
    // (but empty) implementation. Hooks provide additional extension points in some crucial places of the algorithm.
    virtual void hook1() const {}
    virtual void hook2() const {}
};

// Concrete classes have to implement all abstract operations of the base class.
// They can also override some operations with a default implementation.
class ConcreteClass1 : public AbstractClass
{
protected:
    void requiredOperations1() const override { std::cout << "ConcreteClass1 says: Implemented Operation1\n"; }
    void requiredOperation2() const override { std::cout << "ConcreteClass1 says: Implemented Operation2\n"; }
};

// Usually, concrete classes override only a fraction of base class' operations.
class ConcreteClass2 : public AbstractClass
{
protected:
    void requiredOperations1() const override { std::cout << "ConcreteClass2 says: Implemented Operation1\n"; }
    void requiredOperation2() const override { std::cout << "ConcreteClass2 says: Implemented Operation2\n"; }
    void hook1() const override { std::cout << "ConcreteClass2 says: Overridden Hook1\n"; }
};

// The client code calls the template method to execute the algorithm. Client code does not have to know the
// concrete class of an object it works with, as long as it works with objects through the interface of their base class.
void clientCode(const AbstractClass* const c)
{
    c->templateMethod();
}

int main()
{
    std::cout << "Same client code can work with different subclasses:\n";
    const ConcreteClass1* const concreteClass1 = new ConcreteClass1;
    clientCode(concreteClass1);
    std::cout << "\nSame client code can work with different subclasses:\n";
    const ConcreteClass2* const concreteClass2 = new ConcreteClass2;
    clientCode(concreteClass2);
    delete concreteClass1;
    delete concreteClass2;
}

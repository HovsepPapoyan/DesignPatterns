#include <iostream>
#include <string>

// Each distinct product of a product family should have a base interface.
// All variants of the product must implement this interface.
class AbstractProductA
{
public:
    virtual ~AbstractProductA() = default;
    virtual std::string usefulFunctionA() const = 0;
};
class ConcreteProductA1 : public AbstractProductA
{
public:
    std::string usefulFunctionA() const override { return "ConcreteProductA1."; }
};
class ConcreteProductA2 : public AbstractProductA
{
public:
    std::string usefulFunctionA() const override { return "ConcreteProductA2."; }
};

// Here's the the base interface of another product. All products can interact with each other,
// but proper interaction is possible only between products of the same concrete variant.
class AbstractProductB
{
public:
    virtual ~AbstractProductB() = default;
    virtual std::string usefulFunctionB() const = 0;
};
// Concrete Products are created by corresponding Concrete Factories.
class ConcreteProductB1 : public AbstractProductB
{
public:
    std::string usefulFunctionB() const override { return "ConcreteProductB1."; }
};
class ConcreteProductB2 : public AbstractProductB
{
public:
    std::string usefulFunctionB() const override { return "ConcreteProductB2."; }
};

// The Abstract Factory interface declares a set of methods that return different abstract products.
// These products are called a family and are related by a high-level theme or concept. Products of
// one family are usually able to collaborate among themselves. A family of products may have several
// variants, but the products of one variant are incompatible with products of another.
class AbstractFactory
{
public:
    virtual AbstractProductA* createProductA() const = 0;
    virtual AbstractProductB* createProductB() const = 0;
};
// Concrete Factories produce a family of products that belong to a single variant. The factory guarantees
// that resulting products are compatible. Note that signatures of the Concrete Factory's methods return
// an abstract product, while inside the method a concrete product is instantiated.
class ConcreteFactory1 : public AbstractFactory
{
public:
    AbstractProductA* createProductA() const override { return new ConcreteProductA1(); }
    AbstractProductB* createProductB() const override { return new ConcreteProductB1(); }
};
// Each Concrete Factory has a corresponding product variant.
class ConcreteFactory2 : public AbstractFactory
{
public:
    AbstractProductA* createProductA() const override { return new ConcreteProductA2(); }
    AbstractProductB* createProductB() const override { return new ConcreteProductB2(); }
};

// The client code works with factories and products only through abstract types: AbstractFactory and AbstractProduct.
// This lets you pass any factory or product subclass to the client code without breaking it.
void clientCode(const AbstractFactory& factory)
{
    const AbstractProductA* const abstractProductA = factory.createProductA();
    const AbstractProductB* const abstractProductB = factory.createProductB();
    std::cout << abstractProductA->usefulFunctionA() << '\n';
    std::cout << abstractProductB->usefulFunctionB() << '\n';
    delete abstractProductA;
    delete abstractProductB;
}

int main()
{
    std::cout << "Client: Testing client code with the first factory type:\n";
    const ConcreteFactory1* const f1 = new ConcreteFactory1();
    clientCode(*f1);
    delete f1;
    std::cout << "\nClient: Testing client code with the second factory type:\n";
    const ConcreteFactory2* const f2 = new ConcreteFactory2();
    clientCode(*f2);
    delete f2;
}
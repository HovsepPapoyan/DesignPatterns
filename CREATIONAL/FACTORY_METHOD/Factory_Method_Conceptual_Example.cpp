#include <iostream>
#include <string>

// The Product interface declares the operations that all concrete products must implement.
class Product
{
public:
    virtual ~Product() = default;
    virtual std::string operation() const = 0;
};

// Concrete Products provide various implementations of the Product interface.
class ConcreteProduct1 : public Product
{
public:
    std::string operation() const override { return "{Result of the ConcreteProduct1}"; }
};

class ConcreteProduct2 : public Product
{
public:
    std::string operation() const override { return "{Result of the ConcreteProduct2}"; }
};

// The Creator class declares the factory method that is supposed to return an object of a Product class.
// The Creator's subclasses usually provide the implementation of this method.
class Creator
{
// Note that the Creator may also provide some default implementation of the factory method.
public:
    virtual ~Creator() = default;
    virtual Product* factoryMethod() const = 0;

    std::string someOperation() const
    {
        const Product* const product = factoryMethod();
        const std::string result = "Creator: The same creator's code has just worked with " + product->operation();
        delete product;
        return result;
    }
};

// Concrete Creators override the factory method in order to change the resulting product's type.
class ConcreteCreator1 : public Creator
{
// Note that the signature of the method still uses the abstract product type, even though the concrete product
// is actually returned from the method. This way the Creator can stay independent of concrete product classes.
public:
    Product* factoryMethod() const override { return new ConcreteProduct1(); }
};

class ConcreteCreator2 : public Creator
{
public:
    Product* factoryMethod() const override { return new ConcreteProduct2(); }
};

// The client code works with an instance of a concrete creator, albeit through its base interface. As long as
// the client keeps working with the creator via the base interface, you can pass it any creator's subclass.
void clientCode(const Creator& creator)
{
    std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
              << creator.someOperation() << std::endl;
}

// The Application picks a creator's type depending on the configuration or environment.
int main()
{
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    const Creator* const creator1 = new ConcreteCreator1();
    clientCode(*creator1);
    std::cout << "\nApp: Launched with the ConcreteCreator2.\n";
    const Creator* const creator2 = new ConcreteCreator2();
    clientCode(*creator2);

    delete creator1;
    delete creator2;
}
#include <iostream>
#include <string>
#include <vector>

// It makes sense to use the Builder pattern only when your products are quite complex and require extensive configuration.
// Unlike in other creational patterns, different concrete builders can produce unrelated products. In other words,
// results of various builders may not always follow the same interface.
class Product1
{
public:
    std::vector<std::string> m_parts;
    void listParts() const
    {
        std::cout << "Product parts: ";
        for (const std::string& part : m_parts)
        {
            std::cout << part << ", ";
        }
        std::cout << "\n\n";
    }
};

// The Builder interface specifies methods for creating the different parts of the Product objects.
class Builder
{
public:
    virtual ~Builder() = default;
    virtual void producePartA() const = 0;
    virtual void producePartB() const = 0;
    virtual void producePartC() const = 0;
};

// The Concrete Builder classes follow the Builder interface and provide specific implementations of
// the building steps. Your program may have several variations of Builders, implemented differently.
class ConcreteBuilder1 : public Builder
{
private:
    Product1* m_product;

public:
    // A fresh builder instance should contain a blank product object, which is used in further assembly.
    ConcreteBuilder1() { reset(); }
    ~ConcreteBuilder1() { delete m_product; }
    void reset() { m_product = new Product1(); }
    // All production steps work with the same product instance.
    void producePartA() const override { m_product->m_parts.emplace_back("PartA1"); }
    void producePartB() const override { m_product->m_parts.emplace_back("PartB1"); }
    void producePartC() const override { m_product->m_parts.emplace_back("PartC1"); }
    Product1* getProduct()
    {
        Product1* result = m_product;
        reset();
        return result;
    }
};

// The Director is only responsible for executing the building steps in a particular sequence.
// It is helpful when producing products according to a specific order or configuration. Strictly
// speaking, the Director class is optional, since the client can control builders directly.
class Director
{
private:
    const Builder* m_builder;

    // The Director works with any builder instance that the client code passes to it.
    // This way, the client code may alter the final type of the newly assembled product.
public:
    void setBuilder(const Builder* const builder) { m_builder = builder; }

    // The Director can construct several product variations using the same building steps.
    void buildMinimalViableProduct() const { m_builder->producePartA(); }
    void buildFullFeaturedProduct() const
    {
        m_builder->producePartA();
        m_builder->producePartB();
        m_builder->producePartC();
    }
};

// The client code creates a builder object, passes it to the director and then initiates
// the construction process. The end result is retrieved from the builder object.
void clientCode(Director& director)
{
    ConcreteBuilder1* const builder = new ConcreteBuilder1();
    director.setBuilder(builder);
    std::cout << "Standard basic product:\n"; 
    director.buildMinimalViableProduct();
    const Product1* p = builder->getProduct();
    p->listParts();
    delete p;

    std::cout << "Standard full featured product:\n"; 
    director.buildFullFeaturedProduct();
    p = builder->getProduct();
    p->listParts();
    delete p;

    // Usage without a Director class.
    std::cout << "Custom product:\n";
    builder->producePartA();
    builder->producePartC();
    p = builder->getProduct();
    p->listParts();
    delete p;

    delete builder;
}

int main()
{
    Director* const director = new Director();
    clientCode(*director);
    delete director;
}
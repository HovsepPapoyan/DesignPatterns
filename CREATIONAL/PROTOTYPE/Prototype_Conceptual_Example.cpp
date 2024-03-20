#include <iostream>
#include <string>
#include <unordered_map>

// Prototype Design Pattern
// Intent: Lets you copy existing objects without making your code dependent on their classes.
enum Type
{
    PROTOTYPE_1 = 0,
    PROTOTYPE_2 = 1
};

// The example class that has cloning ability. We'll see how
// the values of field with different types will be cloned.
class Prototype
{
protected:
    std::string m_prototypeName;
    float m_prototypeField;

public:
    explicit Prototype(std::string prototypeName, const float prototypeField)
        : m_prototypeName(std::move(prototypeName))
        , m_prototypeField(prototypeField)
    { }
    virtual ~Prototype() = default;
    virtual Prototype* clone() const = 0;
    virtual void method(const float prototypeField)
    {
        m_prototypeField = prototypeField;
        std::cout << "call method from " << m_prototypeName << " with field: " << m_prototypeField << '\n';
    }
};

// ConcretePrototype1 is a sub-class of Prototype and implement the clone method. In this example all data members
// of Prototype class are in the stack. If you have pointers in your properties for ex: std::string* m_name, you
// will need to implement the copy-constructor to make sure you have a deep copy from the clone method.
class ConcretePrototype1 : public Prototype
{
public:
    ConcretePrototype1(std::string prototypeName, const float concretePrototypeField)
        : Prototype(std::move(prototypeName), concretePrototypeField)
    { }

    // Notice that clone method return a pointer to a new ConcretePrototype1 replica, so, the client
    // (who call the clone method) has the responsability to free that memory. You may prefer to use std::unique_ptr here.
    Prototype* clone() const override { return new ConcretePrototype1(*this); }
};

class ConcretePrototype2 : public Prototype
{
public:
    ConcretePrototype2(std::string prototypeName, const float concretePrototypeField)
        : Prototype(std::move(prototypeName), concretePrototypeField)
    { }

    // Notice that clone method return a pointer to a new ConcretePrototype2 replica, so, the client
    // (who call the clone method) has the responsability to free that memory. You may prefer to use std::unique_ptr here.
    Prototype* clone() const override { return new ConcretePrototype2(*this); }
};

// In PrototypeFactory you have two concrete prototypes, one for each concrete prototype class,
// so each time you want to create a bullet, you can use the existing ones and clone those.
class PrototypeFactory
{
private:
    std::unordered_map<Type, Prototype*> m_prototypes;

public:
    explicit PrototypeFactory()
    {
        m_prototypes[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1 ", 50.f);
        m_prototypes[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2 ", 60.f);
    }

    ~PrototypeFactory()
    {
        delete m_prototypes[Type::PROTOTYPE_1];
        delete m_prototypes[Type::PROTOTYPE_2];
    }

    // Notice here that you just need to specify the type of the prototype you
    // want and the method will create from the object with this type.
    Prototype* createPrototype(const Type type) { return m_prototypes[type]->clone(); }
};

void client(PrototypeFactory& prototypeFactory)
{
    std::cout << "Let's create a Prototype 1\n";
    Prototype* prototype = prototypeFactory.createPrototype(Type::PROTOTYPE_1);
    prototype->method(90);
    delete prototype;

    std::cout << "\nLet's create a Prototype 2\n";
    prototype = prototypeFactory.createPrototype(Type::PROTOTYPE_2);
    prototype->method(10);
    delete prototype;
}

int main()
{
    PrototypeFactory* const prototypeFactory = new PrototypeFactory();
    client(*prototypeFactory);
    delete prototypeFactory;
}

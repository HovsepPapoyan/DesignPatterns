#include <iostream>
#include <string_view>

// The Mediator interface declares a method used by components to notify the mediator about various
// events. The Mediator may react to these events and pass the execution to other components.
class BaseComponent;
class Mediator
{
public:
    virtual void notify(const BaseComponent* const sender, const std::string_view event) const = 0;
};

// The BaseComponent provides the basic functionality of storing a mediator's instance inside component objects.
class BaseComponent
{
protected:
    const Mediator* m_mediator;

public:
    explicit BaseComponent(const Mediator* const mediator = nullptr)
        : m_mediator(mediator)
    { }
    void setMediator(const Mediator* const mediator)
    {
        m_mediator = mediator;
    }
};

// Concrete Components implement various functionality. They don't depend on other components.
// They also don't depend on any concrete mediator classes.
class Component1 : public BaseComponent
{
public:
    void doA() const
    {
        std::cout << "Component1 does A.\n";
        m_mediator->notify(this, "A");
    }
    void doB() const
    {
        std::cout << "Component1 does B.\n";
        m_mediator->notify(this, "B");
    }
};

class Component2 : public BaseComponent
{
public:
    void doC() const
    {
        std::cout << "Component2 does C.\n";
        m_mediator->notify(this, "C");
    }
    void doD() const
    {
        std::cout << "Component2 does D.\n";
        m_mediator->notify(this, "D");
    }
};

// ConcreteMediator implement cooperative behavior by coordinating several components.
class ConcreteMediator : public Mediator
{
private:
    Component1* const m_component1;
    Component2* const m_component2;

public:
    explicit ConcreteMediator(Component1* const c1, Component2* const c2)
        : m_component1(c1)
        , m_component2(c2)
    {
        m_component1->setMediator(this);
        m_component2->setMediator(this);
    }
    void notify(const BaseComponent* const sender, const std::string_view event) const override
    {
        if (event == "A")
        {
            std::cout << "Mediator reacts on A and triggers following operations:\n";
            m_component2->doC();
        }
        if (event == "D")
        {
            std::cout << "Mediator reacts on D and triggers following operations:\n";
            m_component1->doB();
            m_component2->doC();
        }
    }
};

int main()
{
    Component1* const c1 = new Component1;
    Component2* const c2 = new Component2;
    const ConcreteMediator* const mediator = new ConcreteMediator(c1, c2);

    std::cout << "Client triggers operation A.\n";
    c1->doA();
    std::cout << "\nClient triggers operation D.\n";
    c2->doD();

    delete c1;
    delete c2;
    delete mediator;
}

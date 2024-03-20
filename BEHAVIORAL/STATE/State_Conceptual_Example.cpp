#include <iostream>

class Context;
// The base State class declares methods that all Concrete State should implement
// and also provides a backreference to the Context object, associated with the State.
// This backreference can be used by States to transition the Context to another State.
class State
{
protected:
    Context* m_context;

public:
    virtual ~State() = default;
    void setContext(Context* const context) { m_context = context; }
    virtual void handle1() = 0;
    virtual void handle2() = 0;
};

// The Context defines the interface of interest to clients. It also maintains a reference
// to an instance of a State subclass, which represents the current state of the Context.
class Context
{
private:
    // A reference to the current state of the Context.
    State* m_state;

public:
    explicit Context(State* const state)
        : m_state(nullptr)
    {
        transitionTo(state);
    }
    ~Context() { delete m_state; }

    // The Context allows changing the State object at runtime.
    void transitionTo(State* const state)
    {
        std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
        delete m_state;
        m_state = state;
        m_state->setContext(this);
    }

    // The Context delegates part of its behavior to the current State object.
    void request1() { m_state->handle1(); }
    void request2() { m_state->handle2(); }
};

// Concrete States implement various behaviors, associated with a state of the Context.
class ConcreteStateA : public State
{
public:
    void handle1() override;
    void handle2() override { std::cout << "ConcreteStateA handles request2.\n"; }
};

class ConcreteStateB : public State
{
public:
    void handle1() override { std::cout << "ConcreteStateB handles request1.\n"; }
    void handle2() override
    {
        std::cout << "ConcreteStateB handles request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        m_context->transitionTo(new ConcreteStateA);
    }
};

void ConcreteStateA::handle1()
{
    std::cout << "ConcreteStateA handles request1.\n";
    std::cout << "ConcreteStateA wants to change the state of the context.\n";
    m_context->transitionTo(new ConcreteStateB);
}

int main()
{
    Context* const context = new Context(new ConcreteStateA);
    std::cout << '\n';
    context->request1();
    std::cout << '\n';
    context->request2();
    delete context;
}

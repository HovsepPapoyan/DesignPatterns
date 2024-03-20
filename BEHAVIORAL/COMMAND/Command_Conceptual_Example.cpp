#include <iostream>

// The Command interface declares a method for executing a command.
class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() const = 0;
};

// Some commands can implement simple operations on their own.
class SimpleCommand : public Command
{
private:
    std::string m_payload;

public:
    explicit SimpleCommand(std::string payload)
        : m_payload(std::move(payload))
    { }
    void execute() const override
    {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" << m_payload << ").\n";
    }
};

// The Receiver classes contain some important business logic. They know how to perform all kinds of
// operations, associated with carrying out a request. In fact, any class may serve as a Receiver.
class Receiver
{
public:
    void doSomething(const std::string& a) const
    {
        std::cout << "Receiver: Working on (" << a << ").\n";
    }
    void doSomethingElse(const std::string& b) const
    {
        std::cout << "Receiver: Also working on (" << b << ").\n";
    }
};

// However, some commands can delegate more complex operations to other objects, called "receivers."
class ComplexCommand : public Command
{
private:
    const Receiver* const m_receiver;
    // Context data, required for launching the receiver's methods.
    std::string m_a;
    std::string m_b;

public:
    // Complex commands can accept one or several receiver objects along with any context data via the constructor.
    explicit ComplexCommand(const Receiver* const receiver, std::string a, std::string b)
        : m_receiver(receiver)
        , m_a(std::move(a))
        , m_b(std::move(b))
    { }
    // Commands can delegate to any methods of a receiver.
    void execute() const override
    {
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        m_receiver->doSomething(m_a);
        m_receiver->doSomethingElse(m_b);
    }
};

// The Invoker is associated with one or several commands. It sends a request to the command.
class Invoker
{
private:
    const Command* m_onStart;
    const Command* m_onFinish;

public:
    ~Invoker()
    {
        delete m_onStart;
        delete m_onFinish;
    }
    void setOnStart(const Command* const command) { m_onStart = command; }
    void setOnFinish(const Command* const command) { m_onFinish = command; }
    
    // The Invoker does not depend on concrete command or receiver classes. The
    // Invoker passes a request to a receiver indirectly, by executing a command.
    void doSomethingImportant() const
    {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (m_onStart)
        {
            m_onStart->execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (m_onFinish)
        {
            m_onFinish->execute();
        }
    }
};

// The client code can parameterize an invoker with any commands.
int main()
{
    Invoker* const invoker = new Invoker;
    invoker->setOnStart(new const SimpleCommand("Say Hi!"));
    const Receiver* const receiver = new Receiver;
    invoker->setOnFinish(new const ComplexCommand(receiver, "Send email", "Save report"));
    invoker->doSomethingImportant();

    delete invoker;
    delete receiver;
}

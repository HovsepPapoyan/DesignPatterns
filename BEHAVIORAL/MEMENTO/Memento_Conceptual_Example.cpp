#include <iostream>
#include <memory>
#include <vector>

class Memento
{
private:
    std::string m_metadata;

public:
    virtual ~Memento() = default;
    explicit Memento(std::string metadata)
      : m_metadata (std::move(metadata))
    { }
    std::string getMetadata() const { return m_metadata; }
};

// The Originator holds some important state that may change over time. It also defines a method
// for saving the state inside a memento and another method for restoring the state from it.
class Originator
{
private:
    std::string m_state;

public:
    explicit Originator(std::string state)
      : m_state(std::move(state))
    {
        std::cout << "Originator: My initial state is: " << m_state << '\n';
    }

private:
    struct ConcreteMemento : public Memento
    {
        std::string m_state;
        explicit ConcreteMemento(std::string metadata, std::string state)
          : Memento(std::move(metadata))
          , m_state(state)
        { }
    };

public:
    void doSomething()
    {
        std::cout << "Originator: I'm doing something important.\n";
        m_state = "random state";
        std::cout << "Originator: and my state has changed to: " << m_state << '\n';
    }

    // Saves the current state inside a memento.
    std::unique_ptr<Memento> save() const { return std::make_unique<ConcreteMemento>("some metadata", m_state); }

    // Restores the Originator's state from a memento object.
    void restore(const std::unique_ptr<Memento>& memento)
    {
        const ConcreteMemento* const concreteMemento = dynamic_cast<const ConcreteMemento* const>(memento.get());
        if (concreteMemento)
        {
            m_state = concreteMemento->m_state;
            std::cout << "Originator: My state has changed to: " << m_state << "\n";
        }
        else
        {
            std::cout << "Originator: My state not changed, because I got not correct memento\n";           
        }
    }
};

// The Caretaker doesn't depend on the Concrete Memento class. Therefore, it doesn't have access to the
// originator's state, stored inside the memento. It works with all mementos via the base Memento interface.
class Caretaker
{
private:
    std::vector<std::unique_ptr<Memento>> m_mementos;
    const std::unique_ptr<Originator>& m_originator;

public:
    Caretaker(const std::unique_ptr<Originator>& originator)
      : m_originator(originator)
    { }
    void backup()
    {
        std::cout << "\nCaretaker: Saving Originator's state...\n";
        m_mementos.push_back(m_originator->save());
        std::cout << "Caretaker: Memento's metadata: " << m_mementos.back()->getMetadata() << '\n';
    }
    void undo()
    {
        if (m_mementos.empty())
        {
            return;
        }
        std::cout << "Caretaker: Restoring state\n";
        m_originator->restore(m_mementos.back());
        m_mementos.pop_back();
    }
};

// Client code
int main()
{
    const std::unique_ptr<Originator> originator = std::make_unique<Originator>("initial state");
    const std::unique_ptr<Caretaker> caretaker = std::make_unique<Caretaker>(originator);
    caretaker->backup();
    originator->doSomething();
    caretaker->undo();
}
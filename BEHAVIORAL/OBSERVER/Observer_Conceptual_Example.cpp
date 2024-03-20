// Observer Design Pattern
// Intent: Lets you define a subscription mechanism to notify multiple objects about any events that happen
// to the object they're observing. Note that there's a lot of different terms with similar meaning associated
// with this pattern. Just remember that the Subject is also called the Publisher and the Observer is often
// called the Subscriber and vice versa. Also the verbs "observe", "listen" or "track" usually mean the same thing.

#include <iostream>
#include <list>

class IObserver // ISubscriber
{
public:
    explicit IObserver() noexcept {}
    virtual ~IObserver() = default;
    IObserver(const IObserver&) = delete;
    IObserver& operator=(const IObserver&) = delete;

    virtual void update(std::string messageFromSubject) = 0;
};

class ISubject // IPublisher
{
public:
    explicit ISubject() noexcept {}
    virtual ~ISubject() = default;
    ISubject(const ISubject&) = delete;
    ISubject& operator=(const ISubject&) = delete;

    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notify() = 0;
};

// The Subject owns some important state and notifies observers when the state changes.
class Subject : public ISubject
{
public:
    explicit Subject() noexcept {}
    ~Subject() override { std::cout << "Goodbye, I was the Subject.\n"; }

    // The subscription management methods.
    void attach(IObserver* observer) override { m_observers.push_back(observer); }
    void detach(IObserver* observer) override { m_observers.remove(observer); }

    void howManyObservers() const { std::cout << "There are " << m_observers.size() << " observers in the list.\n"; }
    void notify() override
    {
        howManyObservers();
        for (IObserver* const observer : m_observers)
        {
            observer->update(m_message);
        }
    }
    void createMessage(std::string message = "Empty")
    {
        m_message = std::move(message);
        notify();
    }
    // Usually, the subscription logic is only a fraction of what a Subject can really do.
    // Subjects commonly hold some important business logic, that triggers a notification
    // method whenever something important is about to happen (or after it).
    void someBusinessLogic()
    {
        m_message = "change message";
        notify();
        std::cout << "I'm about to do something important\n";
    }

private:
    std::list<IObserver*> m_observers;
    std::string m_message;
};

class Observer : public IObserver
{
public:
    explicit Observer(Subject& subject)
       : m_subject(subject)
    {
        m_subject.attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++staticNumber << "\".\n";
        m_number = staticNumber;
    }
    ~Observer() override { std::cout << "Goodbye, I was the Observer \"" << m_number << "\".\n"; }
    void printInfo() const { std::cout << "Observer \"" << m_number << "\": a new message is available --> " << m_messageFromSubject << "\n"; }
    void update(std::string messageFromSubject) override
    {
        m_messageFromSubject = std::move(messageFromSubject);
        printInfo();
    }
    void removeMeFromTheList()
    {
        m_subject.detach(this);
        std::cout << "Observer \"" << m_number << "\" removed from the list.\n";
    }

private:
    std::string m_messageFromSubject;
    Subject& m_subject;
    int m_number;
    static int staticNumber;
};

int Observer::staticNumber = 0;

int main()
{
    Subject* const subject = new Subject;
    Observer* const observer1 = new Observer(*subject);

    Observer* const observer2 = new Observer(*subject);
    Observer* const observer3 = new Observer(*subject);

    subject->createMessage("Hello World! :D");
    observer3->removeMeFromTheList();

    subject->createMessage("The weather is hot today! :p");
    Observer* const observer4 = new Observer(*subject);

    observer2->removeMeFromTheList();
    Observer* const observer5 = new Observer(*subject);

    subject->createMessage("My new car is great! ;)");
    observer5->removeMeFromTheList();
    observer4->removeMeFromTheList();
    observer1->removeMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}

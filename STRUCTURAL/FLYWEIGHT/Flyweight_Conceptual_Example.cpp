#include <string>
#include <unordered_map>
#include <iostream>
#include <tuple>
#include <vector>

class Context
{
private:
    std::string m_owner;
    std::string m_plates;

public:
    explicit Context(std::string owner, std::string plates)
        : m_owner(std::move(owner))
        , m_plates(std::move(plates))
    { }
    friend std::ostream& operator<<(std::ostream& os, const Context& c)
    {
        return os << "[ " << c.m_owner << " , " << c.m_plates << " ]";
    }
};

// The Flyweight stores a common portion of the state (also called intrinsic state)
// that belongs to multiple real business entities. The Flyweight accepts the rest
// of the state (extrinsic state, unique for each entity) via its method parameters.
class Flyweight
{
private:
    std::string m_brand;
    std::string m_model;
    std::string m_color;

public:
    explicit Flyweight(std::string brand, std::string model, std::string color)
        : m_brand(std::move(brand))
        , m_model(std::move(model))
        , m_color(std::move(color))
    { }
    void operation(const Context& context) const
    {
        std::cout << "Flyweight: Displaying shared (" << *this << ") and unique (" << context << ") states.\n";
    }
    friend std::ostream& operator<<(std::ostream& os, const Flyweight& f)
    {
        return os << "[ " << f.m_brand << " , " << f.m_model << " , " << f.m_color << " ]";
    }
};

// The Flyweight Factory creates and manages the Flyweight objects. It ensures that flyweights
// are shared correctly. When the client requests a flyweight, the factory either returns an
// existing instance or creates a new one, if it doesn't exist yet.
class FlyweightFactory
{
private:
    std::unordered_map<std::string, Flyweight> m_flyweights;
    // Returns a Flyweight's string hash for a given state.
    std::string getKey(const std::string& brand, const std::string& model, const std::string& color) const
    {
        return brand + '_' + model + '_' + color;
    }

public:
    explicit FlyweightFactory(std::vector<std::tuple<std::string, std::string, std::string>> sharedStates)
    {
        for (auto& sharedState : sharedStates)
        {
            std::string key = getKey(std::get<0>(sharedState), std::get<1>(sharedState), std::get<2>(sharedState));
            m_flyweights.emplace(std::move(key),
                                 Flyweight(std::move(std::get<0>(sharedState)), std::move(std::get<1>(sharedState)), std::move(std::get<2>(sharedState))));
        }
    }
    void listFlyweights() const
    {
        std::cout << "\nFlyweightFactory: I have " << m_flyweights.size() << " flyweights:\n";
        for (const auto& pair : m_flyweights)
        {
            std::cout << pair.first << "\n";
        }
    }
    // Returns an existing Flyweight with a given state or creates a new one.
    Flyweight getFlyweight(const std::string& brand, const std::string& model, const std::string& color)
    {
        std::string key = getKey(brand, model, color);
        auto it = m_flyweights.find(key);
        if (it == m_flyweights.end())
        {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
            it = m_flyweights.emplace(std::move(key), Flyweight(brand, model, color)).first;
        }
        else
        {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }
        return it->second;
    }
};

void addCarToDatabase(FlyweightFactory& flyweightFactory, const Context& context, const std::string& brand, const std::string& model, const std::string& color)
{
    std::cout << "\nClient: Adding a car to database.\n";
    const Flyweight& flyweight = flyweightFactory.getFlyweight(brand, model, color);
    // The client code either stores or calculates extrinsic state and passes it to the flyweight's methods.
    flyweight.operation(context);
}

// The client code usually creates a bunch of pre-populated flyweights in the initialization stage of the application.
int main()
{
    FlyweightFactory* const factory = new FlyweightFactory(
        {
            {"Chevrolet", "Camaro2018", "pink"},
            {"Mercedes Benz", "C300", "black"},
            {"Mercedes Benz", "C500", "red"},
            {"BMW", "M5", "red"},
            {"BMW", "X6", "white"}
        }
    );
    factory->listFlyweights();
    Context context("CL234IR", "James Doe");
    addCarToDatabase(*factory, context, "BMW", "M5", "red");
    addCarToDatabase(*factory, context, "BMW", "X1", "red");
    factory->listFlyweights();
    delete factory;
}

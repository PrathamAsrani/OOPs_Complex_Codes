#include <bits/stdc++.h>
using namespace std;

class Driver;
class IndianParliament;
class MemberOfParliament;
class Commisoner;

static vector<Driver> drivers;
static vector<MemberOfParliament> members;

class Driver
{
private:
    string name, driving_MP_car;
    unsigned int age, drivingExperience;
    double salary;

public:
    Driver()
    {
    }
    Driver(string name, unsigned int age, double salary = 10000)
    {
        this->name = name;
        this->age = age;
        this->salary = salary;
        this->drivingExperience = rand() % 30;
        this->driving_MP_car = "NA";
    }
    void showDriverDetails()
    {
        if (this->name == "")
        {
            cout << "Not allocated the driver\n";
            return;
        }
        cout << "Name: " << name << "\n"
             << "Age: " << age << "\n"
             << "Salary: " << salary << "\n"
             << "Driving Experience: " << drivingExperience << "\n"
             << "Serving to MP: " << driving_MP_car << "\n\n";
    }

    friend class MemberOfParliament;
};

class IndianParliament
{
protected:
    virtual MemberOfParliament *getConstituency() = 0;
    virtual Driver getDriver() = 0;
    virtual bool exceedsSpendingLimit() = 0;
};

class MemberOfParliament : public IndianParliament
{
private:
    string name, type;
    double limit, spent;
    Driver driver;
    MemberOfParliament *wonMP;

    void setDriver()
    {
        Driver d;
        if (type == "prime minister")
        {
            int mxExp = 0, idx = 0, i = 0;
            for (auto &driver : drivers)
            {
                if (driver.drivingExperience > mxExp)
                {
                    mxExp = driver.drivingExperience;
                    d = driver;
                    idx = i;
                }
                i++;
            }
            drivers[idx].driving_MP_car = this->name;
            this->driver = d;
        }
        else if (type == "minister" || type == "member of parliament")
        {
            for (auto &driver : drivers)
            {
                if (driver.driving_MP_car == "NA")
                {
                    driver.driving_MP_car = this->name;
                    d = driver;
                    break;
                }
            }
            d.driving_MP_car = this->name;
            this->driver = d;
        }
        else
        {
            cerr << "Error_code 101: Driver not be allocated to the member of parliaments\n";
            exit(1);
        }
    }

    void showMP_Details()
    {
        cout << "\n************************************************************************************************************\nMP DETAILS:\n************************************************************************************************************\n";
        cout << "Name: " << this->name << "\n"
             << "Type: " << this->type << "\n"
             << "Limit: " << to_string(this->limit) << "\n"
             << "Spent: " << to_string(this->spent) << "\n"
             << "Driver Details: \n";
        this->driver.showDriverDetails();
        cout << "\n";
    }

    MemberOfParliament()
    {
        name = "not defined";
        type = "member of parliament";
        limit = spent = 0;
    }

    void GivePermission(bool &permission)
    {
        permission = ((rand() % 100) & 1);
    }

public:
    MemberOfParliament(string name, string type)
        : name(name), type(type), spent(0)
    {
        if (type != "prime minister" && type != "minister" && type != "member of parliament")
        {
            cerr << "Error_code 102: Not a member of parliaments\n";
            exit(1);
        }

        if (type == "prime minister")
            limit = 10000000;
        else if (type == "minister")
            limit = 1000000;
        else
            limit = 100000;

        spent = rand() % 10000000;

        this->setDriver();
        this->showMP_Details();

        wonMP = new MemberOfParliament();
    }

    // override
    Driver getDriver()
    {
        return driver;
    }

    bool exceedsSpendingLimit()
    {
        return this->spent > this->limit;
    }

    MemberOfParliament *getConstituency()
    {
        return wonMP;
    }

    string getType()
    {
        return this->type;
    }

    friend class Commisoner;
};

class Commisoner
{
private:
    string name;
    MemberOfParliament arrested_MP, PM;
    bool PM_permission;

public:
    Commisoner(string name) : name(name), PM_permission(false)
    {
        arrested_MP.showMP_Details();
    }
    void setArrestedMinister(MemberOfParliament arrested_MP)
    {
        if (arrested_MP.getType() == "prime minister")
        {
            cout << "Prime minister cannot be arrested\n";
            return;
        }
        else if (arrested_MP.getType() == "minister")
        {
            this->getPM_permission();
            if (PM_permission)
            {
                this->arrested_MP = arrested_MP;
                cout << "\nDetails of arrested minister: \n";
                arrested_MP.showMP_Details();
            }
            else
            {
                cout << "Prime minister not allowed can't arrest the minister\n";
            }
        }
        else
        {
            this->arrested_MP = arrested_MP;
            cout << "\nDetails of arrested minister: \n";
            arrested_MP.showMP_Details();
        }
    }
    void showArrestedMinisterDetails()
    {
        arrested_MP.showMP_Details();
    }
    void getPM_permission()
    {
        PM.GivePermission(PM_permission);
    }
    void setPM(MemberOfParliament PM){
        this->PM = PM;
    }
};

int main()
{
    int N;
    cout << "Enter number of MP: ";
    cin >> N;

    for (int i = 0; i < N; i++)
    {
        drivers.push_back(Driver("Driver " + to_string(i + 1), rand() % 61 + 18, rand() % 50000));
        drivers[i].showDriverDetails();
    }

    Commisoner commisoner("Mudit Goel");

    for (int i = 0; i < N; i++)
    {
        if (i == 0){
            members.push_back(MemberOfParliament("Pratham Asrani", "prime minister"));
            commisoner.setPM(members[i]);
        }
        else if (i & 1)
            members.push_back(MemberOfParliament("Member" + to_string(i), "member of parliament"));
        else
            members.push_back(MemberOfParliament("Member" + to_string(i), "minister"));
    }

    /*
    int idx = rand()%N;
    while(members[idx].getType() == "prime minister") idx = rand()%N;
    commisoner.setArrestedMinister(members[idx]);
    commisoner.showArrestedMinisterDetails();
    */

    for (int i = 0; i < N; i++)
    {
        if (members[i].exceedsSpendingLimit() == true)
        {
            commisoner.setArrestedMinister(members[i]);
        }
    }

    return 0;
}
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

/*
 * Создайте структуру Person с 3 полями: фамилия, имя, отчество.
 * Поле отчество должно быть опционального типа, т.к. не у всех людей есть отчество.
 * Перегрузите оператор вывода данных для этой структуры. Также перегрузите операторы < и == (используйте tie).
 * */
struct Person {
    std::string FirstName;
    std::optional<std::string> Patronymic;
    std::string SecondName;
    friend std::ostream & operator<<(std::ostream & stream, const struct Person & person);
    friend bool operator==(struct Person const & person1,struct Person const & person2);
    //friend bool operator<(struct Person const & person1, struct Person const & person2);
    bool operator<(struct Person const & person2) const {
        if (SecondName < person2.SecondName) // сначала Фамилии
            return true;
// Uncomenting two strings below does SYSSEGV fault
//    else
        if (FirstName < person2.FirstName)
            return true;
//    else
        // Человек без отчества всегда меньше человека с отчеством
        if (Patronymic.has_value() && ! person2.Patronymic.has_value())
            return false;
        else if(! Patronymic.has_value() && person2.Patronymic.has_value())
            return true;
        else if (! person2.Patronymic.has_value() && ! Patronymic.has_value()) {
            if (SecondName < person2.SecondName) // сначала Фамилии
                return true;
            else if (FirstName < person2.FirstName)
                return true;
            else
                return false;
        }
        else
            return tie(SecondName,FirstName,Patronymic.value())
                   < tie(person2.SecondName,person2.FirstName,person2.Patronymic.value());

    };
};

/*
bool operator<(struct Person const & person1, struct Person const & person2) {
    if (person1.SecondName < person2.SecondName) // сначала Фамилии
        return true;
// Uncomenting two strings below does SYSSEGV fault
//    else
    if (person1.FirstName < person2.FirstName)
        return true;
//    else
    // Человек без отчества всегда меньше человека с отчеством
    if (person1.Patronymic.has_value() && ! person2.Patronymic.has_value())
        return false;
    else if(! person1.Patronymic.has_value() && person2.Patronymic.has_value())
        return true;
    else if (! person2.Patronymic.has_value() && ! person1.Patronymic.has_value()) {
        if (person1.SecondName < person2.SecondName) // сначала Фамилии
            return true;
        else if (person1.FirstName < person2.FirstName)
            return true;
        else
            return false;
    }
    else
        return tie(person1.SecondName,person1.FirstName,person1.Patronymic.value())
             < tie(person2.SecondName,person2.FirstName,person2.Patronymic.value());
}
*/

bool operator==(const struct Person & person1,const struct Person & person2) {
    if (
            person1.Patronymic.has_value() && !person2.Patronymic.has_value()
            ||
            person2.Patronymic.has_value() && !person1.Patronymic.has_value()
        )
        return false;
    else if (! person2.Patronymic.has_value() && ! person1.Patronymic.has_value()) {
        if (person1.FirstName == person2.FirstName && person1.SecondName == person2.SecondName)
            return true;
        else
            return false;
    }
    else
        return std::tie(person1.SecondName,person1.FirstName,person1.Patronymic.value())
            == std::tie(person2.SecondName,person2.FirstName,person2.Patronymic.value());
}

std::ostream & operator<<(std::ostream & stream, const struct Person & person) {
     std::string toPrint;
     /* toPrint = person.Patronymic.has_value() ? person.Patronymic.value() : ""; */
     //stream << person.FirstName << " " << person.Patronymic.value_or("") << " " << person.SecondName ;
     stream << std::setw(12) << person.SecondName << std::setw(10) << person.FirstName
            << std::setw(18) << person.Patronymic.value_or("");
     return stream;
}

/* Создайте структуру PhoneNumber с 4 полями:
 * · код страны (целое число)
 * · код города (целое число)
 * · номер (строка)
 * · добавочный номер (целое число, опциональный тип)
 * Для этой структуры перегрузите оператор вывода.
 * Необходимо, чтобы номер телефона выводился в формате: +7(911)1234567 12, где
 * 7 – это номер страны,
 * 911 – номер города,
 * 1234567 – номер,
 * 12 – добавочный номер.
 * Если добавочного номера нет, то его выводить не надо.
 * Также перегрузите операторы < и == (используйте tie)
 * */
struct PhoneNumber {
    int CountryCode;
    int TownCode;
    std::string Number;
    std::optional<int> LocalNumber;
    friend std::ostream & operator<<(std::ostream & stream, const struct PhoneNumber & phoneNumber);
    friend bool operator==(const struct PhoneNumber & phoneNumber1, const struct PhoneNumber & phoneNumber2);
    friend bool operator<(const struct PhoneNumber & phoneNumber1, const struct PhoneNumber & phoneNumber2);
};

bool operator==(const struct PhoneNumber & phoneNumber1, const struct PhoneNumber & phoneNumber2) {
    if( phoneNumber1.LocalNumber.has_value() && ! phoneNumber2.LocalNumber.has_value()
       ||
       ! phoneNumber1.LocalNumber.has_value() && phoneNumber2.LocalNumber.has_value()
    )
        return false;
    else if (!phoneNumber1.LocalNumber.has_value() && ! phoneNumber2.LocalNumber.has_value())
    {
        return std::make_tuple(phoneNumber1.CountryCode, phoneNumber1.TownCode, phoneNumber1.Number)
        == std::make_tuple(phoneNumber2.CountryCode, phoneNumber2.TownCode, phoneNumber2.Number);
    }
    else
        return std::tie(phoneNumber1.CountryCode,
                        phoneNumber1.TownCode,
                        phoneNumber1.Number,
                        phoneNumber1.LocalNumber.value()) ==
               std::tie(phoneNumber2.CountryCode,
                        phoneNumber2.TownCode,
                        phoneNumber2.Number,
                        phoneNumber2.LocalNumber.value());
}

bool operator<(const struct PhoneNumber & phoneNumber1, const struct PhoneNumber & phoneNumber2) {
    // Тут если у человека есть локальный номер, то его суммарный номер считается наибольшим.
    // По-хорошему надо всё приводить к строке, но тогда ни к чему будет std::tie.
    if( phoneNumber1.LocalNumber.has_value() && ! phoneNumber2.LocalNumber.has_value() )
        return false;
    else if (! phoneNumber1.LocalNumber.has_value() && phoneNumber2.LocalNumber.has_value() )
        return true;
    else if (!phoneNumber1.LocalNumber.has_value() && ! phoneNumber2.LocalNumber.has_value())
    {
        return std::make_tuple(phoneNumber1.CountryCode, phoneNumber1.TownCode, phoneNumber1.Number)
               < std::make_tuple(phoneNumber2.CountryCode, phoneNumber2.TownCode, phoneNumber2.Number);
    }
    else
        return std::tie(phoneNumber1.CountryCode,
                        phoneNumber1.TownCode,
                        phoneNumber1.Number,
                        phoneNumber1.LocalNumber.value()) <
               std::tie(phoneNumber2.CountryCode,
                        phoneNumber2.TownCode,
                        phoneNumber2.Number,
                        phoneNumber2.LocalNumber.value());
}

std::ostream & operator<<(std::ostream & stream, const struct PhoneNumber & phoneNumber) {
    std::string toPrint;
    stream << "+" << phoneNumber.CountryCode << "(" << phoneNumber.TownCode <<  ")"
        << phoneNumber.Number ;
    if (phoneNumber.LocalNumber)
        stream << " " << phoneNumber.LocalNumber.value();
    return stream;
}

/*
 * Создайте класс PhoneBook, который будет в контейнере хранить пары: Человек – Номер телефона.
 * Конструктор этого класса должен принимать параметр типа ifstream – поток данных, полученных из файла.
 * В теле конструктора происходит считывание данных из файла и заполнение контейнера.
 * Класс PhoneBook должен содержать перегруженный оператор вывода, для вывода всех данных из
 * контейнера в консоль.
 *
 * В классе PhoneBook реализуйте метод SortByName, который должен сортировать элементы контейнера
 * по фамилии людей в алфавитном порядке.
 * Если фамилии будут одинаковыми, то сортировка должна выполняться по именам,
 * если имена будут одинаковы, то сортировка производится по отчествам.
 * Используйте алгоритмическую функцию sort.
 *
 * Реализуйте метод SortByPhone, который должен сортировать элементы контейнера по номерам
 * телефонов.
 * Используйте алгоритмическую функцию sort.
 * Реализуйте метод GetPhoneNumber, который принимает фамилию человека, а возвращает кортеж
 * из строки и PhoneNumber. Строка должна быть пустой, если найден ровно один человек с заданной
 * фамилией в списке. Если не найден ни один человек с заданной фамилией, то в строке должна быть
 * запись «not found», если было найдено больше одного человека, то в строке должно быть «found more than 1».
 * Реализуйте метод ChangePhoneNumber, который принимает человека и новый номер телефона и, если
 * находит заданного человека в контейнере, то меняет его номер телефона на новый, иначе ничего не делает.
 * */


typedef std::pair<Person,PhoneNumber> PersonPhoneTuple;

class PhoneBook {
    private:
        std::vector<PersonPhoneTuple> phoneBook;
    public:
        friend std::ostream & operator<<(std::ostream & out, const PhoneBook & phoneBook);
        void SortByPhone() {
            std::sort(phoneBook.begin(),phoneBook.end(),
                      [](std::pair<const Person, const PhoneNumber> const & one,
                         std::pair<const Person, const PhoneNumber> const & two) {
                          return one.second < two.second;});
        }
        void SortByName() {
            std::sort(phoneBook.begin(),phoneBook.end(),
                [](std::pair<Person, PhoneNumber> const & one,
                   std::pair<Person, PhoneNumber> const & two) -> bool {
                        return (one.first < two.first);});
        }
        PhoneBook(std::ifstream & phb_file) {
            //Ilin Petr Artemovich 7 17 4559767 -
            //Solovev Ivan Vladimirovich 7 273 5699819 5543
            //Makeev Marat - 77 4521 8880876 999
            Person person;
            PhoneNumber phone;
            std::string PatronymicCandidate,LocalNumberCandidate;
            while (! phb_file.eof() ) {
                phb_file >> person.SecondName
                         >> person.FirstName
                         >> PatronymicCandidate
                         >> phone.CountryCode
                         >> phone.TownCode
                         >> phone.Number
                         >> LocalNumberCandidate;
                if (PatronymicCandidate == "-")
                    person.Patronymic = std::nullopt;
                else
                    person.Patronymic = PatronymicCandidate;
                if (LocalNumberCandidate == "-")
                    phone.LocalNumber = std::nullopt;
                else
                    phone.LocalNumber = std::stoi(LocalNumberCandidate);
                if(person.FirstName != "") // last line is single \n initializes w/ "" all vars.
                   phoneBook.push_back(std::make_pair(person,phone));
            }
            phb_file.close();
        };

};

std::ostream & operator<<(std::ostream & out, const PhoneBook &phoneBook) {
    Person person;
    PhoneNumber phone;
    for(int index=0;index<phoneBook.phoneBook.size();index++) {
        std::tie(person, phone) = phoneBook.phoneBook.at(index);
        out << person  << std::setw(10) << " " << phone << std::endl;
    }
    return out;
}


int main() {
     /*
     Person person1 {"Ilin","Petr","Artemovich"};
     Person person2 {"Zaitsev", "Zakhar", "Artemovich"};
     std::cout << "\n=============\n" << ( person1 < person2 ) << "=============\n" ;
     */
     std::ifstream file;
     try {
        file.open("../PhoneBook.txt",std::_S_in); // путь к файлу PhoneBook.txt
     }
     catch (...) {
         std::cout << "Unable to open file.";
         return 1;
     }
     PhoneBook book(file);
     std::cout << book;

     std::cout << "------SortByPhone-------" << std::endl;
     book.SortByPhone();
     std::cout << book;

     std::cout << "------SortByName--------" << std::endl;
     book.SortByName();
     std::cout << book;


    return 0;
}

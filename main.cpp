#include "Beverages.h"
#include "Condiments.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
Функциональный объект, создающий лимонную добавку
*/
struct MakeLemon
{
	explicit MakeLemon(unsigned quantity)
		:m_quantity(quantity)
	{}

	auto operator()(IBeveragePtr && beverage)const
	{
		return make_unique<CLemon>(std::move(beverage), m_quantity); 
	}
private:
	unsigned m_quantity;
};

/*
Функция, возвращающая функцию, создающую коричную добавку
*/
function<IBeveragePtr(IBeveragePtr &&)> MakeCinnamon()
{
	return [] (IBeveragePtr && b) {
		return make_unique<CCinnamon>(std::move(b));
	};
}

/*
Возвращает функцию, декорирующую напиток определенной добавкой

Параметры шаблона: 
	Condiment - класс добавки, конструктор которого в качестве первого аргумента
				принимает IBeveragePtr&& оборачиваемого напитка
	Args - список типов прочих параметров конструктора (возможно, пустой)
*/

template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	// Возвращаем функцию, декорирующую напиток, переданный ей в качестве аргумента
	// Дополнительные аргументы декоратора, захваченные лямбда-функцией, передаются
	// конструктору декоратора через make_unique
	return [=](auto && b) {
		// Функции make_unique передаем b вместе со списком аргументов внешней функции
		return make_unique<Condiment>(std::forward<decltype(b)>(b), args...);
	};
}

/*
Перегруженная версия оператора <<, которая предоставляет нам синтаксический сахар
для декорирования компонента

Позволяет создать цепочку оборачивающих напиток декораторов следующим образом:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);

Функциональные объекты MakeCondiment* запоминают аргументы, необходимые для создания
дополнения, и возвращают фабричную функцию, принимающую оборачиваемый напиток, которая
при своем вызове создаст нужный объект Condiment, передав ему запомненные аргументы.
Использование:
	auto beverage = 
		make_unique<CConcreteBeverage>(a, b, c)
		<< MakeCondimentA(d, e, f)
		<< MakeCondimentB(g, h);
или даже так:
	auto beverage = 
		make_unique<CConcreteBeverage>
		<< MakeCondiment<CondimentA>(d, e, f)
		<< MakeCondiment<CondimentB>(g, h);
В последнем случае нет необходимости писать вручную реализации MakeCondimentA и MakeCondimentB, т.к.
необходимую реализацию сгенерирует компилятор

Классический способ оборачивания выглядел бы так:
	auto baseBeverage = make_unique<CConcretedBeverage>(a, b, c);
	auto wrappedWithCondimentA = make_unique<CCondimentA>(std::move(baseBeverage), d, e, f);
	auto beverage = make_unique<CCondimentB>(std::move(wrappedWithCondimentA), g, h);
либо так:
	auto beverage = make_unique<CCondimentB>(
						make_unique<CCondimentA>(
							make_unique<CConcreteBeverage>(a, b, c), // Напиток
							d, e, f	// доп. параметы CondimentA
						),
						g, h		// доп. параметры CondimentB
					);

unique_ptr<CLemon> operator << (IBeveragePtr && lhs, const MakeLemon & factory)
{
	return factory(std::move(lhs));
}
unique_ptr<CCinnamon> operator << (IBeveragePtr && lhs, const MakeCinnamon & factory)
{
	return factory(std::move(lhs));
}
*/
template <typename Component, typename Decorator>
auto operator << (Component && component, const Decorator & decorate)
{
	return decorate(std::forward<Component>(component));
}


bool MakeBeverageChoice(unique_ptr<IBeverage> &beverage, int beverageChoice)
{

    switch (beverageChoice)
    {
        case 1:
            beverage = make_unique<CCoffee>();
            return true;
        case 2:
            cout << "Choose Cappuccino portion (1 - Standard, 2 - Double): ";
            int cappuccinoChoice;
            cin >> cappuccinoChoice;
            if (cappuccinoChoice > 4 or cappuccinoChoice < 1)
            {
                cout << "Invalid choice, go away from my cafe!";
                return false;
            }
            beverage = make_unique<CCappuccino>(cappuccinoChoice != 1);
            return true;
        case 3:
            cout << "Choose Latte portion (1 - Standard, 2 - Double): ";
            int latteChoice;
            cin >> latteChoice;
            if (latteChoice > 4 or latteChoice < 1)
            {
                cout << "Invalid choice, go away from my cafe!";
                return false;
            }
            beverage = make_unique<CLatte>(latteChoice != 1);
            return true;
        case 4:
        {
            cout << "Choose tea type (1 - Black, 2 - White, 3 - Blue, 4 - Cyan): ";
            int teaChoice;
            cin >> teaChoice;
            if (teaChoice > 4 or teaChoice < 1)
            {
                cout << "Invalid choice, go away from my cafe!";
                return false;
            }
            beverage = make_unique<CTea>(static_cast<TeaType>(teaChoice - 1));
            return true;
        }
        case 5:
        {
            cout << "Choose milkshake size (1 - Small, 2 - Medium, 3 - Large): ";
            int sizeChoice;
            cin >> sizeChoice;
            if (sizeChoice > 3 or sizeChoice < 1)
            {
                cout << "Invalid choice, go away from my cafe!";
                return false;
            }
            beverage = make_unique<CMilkshake>(static_cast<MilkshakeSize>(sizeChoice - 1));
            return true;
        }
        default:
            cout << "Invalid choice, go away from my cafe!" << endl;
            return false;
    }
}

bool MakeCondimentChoice(unique_ptr<IBeverage> &beverage, int condimentChoice)
{
    switch (condimentChoice)
    {
        case 1:
            beverage = std::move(beverage) << MakeCondiment<CLemon>(2);
            return true;
        case 2:
            beverage = std::move(beverage) << MakeCondiment<CCinnamon>();
            return true;
        case 3:
            cout << "Choose Ice Cubes Type (1 - Water, 2 - Dry): ";
            int cubeChoice;
            cin >> cubeChoice;
            if (cubeChoice > 2 or cubeChoice < 1)
            {
                cout << "Invalid choice, try again)";
                return false;
            }
            beverage = std::move(beverage) << MakeCondiment<CIceCubes>
                    (2, cubeChoice == 1 ? IceCubeType::Water : IceCubeType::Dry);
            return true;
        case 4:
            beverage = std::move(beverage) << MakeCondiment<CChocolateCrumbs>(5);
            return true;
        case 5:
            beverage = std::move(beverage) << MakeCondiment<CCoconutFlakes>(5);
            return true;
        case 6:
            cout << "Choose Syrup Type (1 - Maple, 2 - Chocolate): ";
            int syrupChoice;
            cin >> syrupChoice;
            if (syrupChoice > 2 or syrupChoice < 1)
            {
                cout << "Invalid choice, try again)";
                return false;
            }
            beverage = std::move(beverage) << MakeCondiment<CSyrup>
                    (syrupChoice == 1 ? SyrupType::Maple : SyrupType::Chocolate);
            return true;
        case 7:
            beverage = std::move(beverage) << MakeCondiment<CCream>();
            return true;
        case 8:
            cout << "Choose Liqueur Type (1 - Nutty, 2 - Chocolate): ";
            int liqueurChoice;
            cin >> liqueurChoice;
            if (liqueurChoice > 2 or liqueurChoice < 1)
            {
                cout << "Invalid choice, try again)";
                return false;
            }
            beverage = std::move(beverage) << MakeCondiment<CLiqueur>
                    (liqueurChoice == 1 ? LiqueurType::Nutty : LiqueurType::Chocolate);
            return true;
        case 0:
            cout << "Checkout!" << endl;
            cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
            return false;
        default:
            cout << "Invalid choice, try again." << endl;
            return true;
    }
}

void DialogWithUser()
{
    cout << "Welcome to the beverage ordering system!" << endl;

    unique_ptr<IBeverage> beverage;

    cout << "Choose your base beverage:\n";
    cout << "1 - Coffee\n2 - Cappuccino\n3 - Latte\n4 - Tea\n5 - Milkshake\n";
    int beverageChoice;
    cin >> beverageChoice;

    if (MakeBeverageChoice(beverage, beverageChoice))
    {
        bool choosingFlag = true;
        while (choosingFlag)
        {
            cout << "Choose your condiment:\n";
            cout << "1 - Lemon\n2 - Cinnamon\n3 - Ice Cubes\n4 - Chocolate Crumbs\n";
            cout << "5 - Coconut Flakes\n6 - Syrup\n7 - Cream\n8 - Liqueur\n";
            cout << "0 - Checkout\n";
            int condimentChoice;
            cin >> condimentChoice;

            choosingFlag = MakeCondimentChoice(beverage, condimentChoice);
        }
    }
}


int main()
{
	DialogWithUser();
	cout << endl;
//	{
//		// Наливаем чашечку латте
//		auto latte = make_unique<CLatte>();
//		// добавляем корицы
//		auto cinnamon = make_unique<CCinnamon>(std::move(latte));
//		// добавляем пару долек лимона
//		auto lemon = make_unique<CLemon>(std::move(cinnamon), 2);
//		// добавляем пару кубиков льда
//		auto iceCubes = make_unique<CIceCubes>(std::move(lemon), 2, IceCubeType::Dry);
//		// добавляем 2 грамма шоколадной крошки
//		auto beverage = make_unique<CChocolateCrumbs>(std::move(iceCubes), 2);
//
//		// Выписываем счет покупателю
//		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
//	}
//
//	{
//		auto beverage =
//			make_unique<CChocolateCrumbs>(			// Внешний слой: шоколадная крошка
//				make_unique<CIceCubes>(				// | под нею - кубики льда
//					make_unique<CLemon>(			// | | еще ниже лимон
//						make_unique<CCinnamon>(		// | | | слоем ниже - корица
//							make_unique<CLatte>()),	// | | |   в самом сердце - Латте
//						2),							// | | 2 дольки лимона
//					2, IceCubeType::Dry),			// | 2 кубика сухого льда
//				2);									// 2 грамма шоколадной крошки
//
//		// Выписываем счет покупателю
//		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
//	}
//
//	// Подробнее рассмотрим работу MakeCondiment и оператора <<
//	{
//		// lemon - функция, добавляющая "2 дольки лимона" к любому напитку
//		auto lemon2 = MakeCondiment<CLemon>(2);
//		// iceCubes - функция, добавляющая "3 кусочка льда" к любому напитку
//		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);
//		
//		auto tea = make_unique<CTea>();
//
//		// декорируем чай двумя дольками лимона и тремя кусочками льда
//		auto lemonIceTea = iceCubes3(lemon2(std::move(tea)));	
//		/* Предыдущая строка выполняет те же действия, что и следующий код:
//		auto lemonIceTea = 
//			make_unique<CIceCubes>(
//				make_unique<CLemon>(
//					std::move(tea), 
//					2), 
//				2, IceCubeType::Water);
//		*/
//		
//		auto oneMoreLemonIceTea =
//			make_unique<CTea>()	// Берем чай
//			<< MakeCondiment<CLemon>(2)	// добавляем пару долек лимона
//			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // и 3 кубика льда
//		/*
//		Предыдущая конструкция делает то же самое, что и следующая:
//		auto oneMoreLemonIceTea =
//			MakeCondiment<CIceCubes>(3, IceCubeType::Water)(
//				MakeCondiment<CLemon>(2)(make_unique<CTea>())
//				);
//		*/
//	}
//
//	// Аналог предыдущего решения с добавкой синтаксического сахара
//	// обеспечиваемого операторами << и функцией MakeCondiment
//	{
//		auto beverage = 
//			make_unique<CLatte>()							// Наливаем чашечку латте,
//			<< MakeCondiment<CCinnamon>()					// оборачиваем корицей,
//			<< MakeCondiment<CLemon>(2)						// добавляем пару долек лимона
//			<< MakeCondiment<CIceCubes>(2, IceCubeType::Dry)// брасаем пару кубиков сухого льда
//			<< MakeCondiment<CChocolateCrumbs>(2);			// посыпаем шоколадной крошкой
//
//		// Выписываем счет покупателю
//		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
//	}
//
//	{
//		auto beverage = 
//			make_unique<CMilkshake>()					// Наливаем молочный коктейль
//			<< MakeCondiment<CSyrup>(SyrupType::Maple)	// заливаем кленовым сиропом
//			<< MakeCondiment<CCoconutFlakes>(8);		// посыпаем кокосовой стружкой
//
//		// Выписываем счет покупателю
//		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
//	}
}

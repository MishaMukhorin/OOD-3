#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
	CBeverage(const std::string & description)
		:m_description(description)
	{}

	std::string GetDescription()const override final
	{
		return m_description;
	}
private:
	std::string m_description;
};

// Кофе
class CCoffee : public CBeverage
{
public:
	CCoffee(const std::string& description = "Coffee")
		:CBeverage(description) 
	{}

	double GetCost() const override 
	{
		return 60; 
	}
};

// Капуччино
class CCappuccino : public CCoffee
{
public:
    CCappuccino(bool isDoublePortion = false)
            : CCoffee(isDoublePortion ? "Double Cappuccino" : "Standard Cappuccino"), m_isDoublePortion(isDoublePortion)
            {}

    double GetCost() const override
    {
        return m_isDoublePortion ? 120 : 80;
    }
private:
    bool m_isDoublePortion;
};

// Латте
class CLatte : public CCoffee
{
public:
    CLatte(bool isDoublePortion = false)
            : CCoffee(isDoublePortion ? "Double Latte" : "Standard Latte"), m_isDoublePortion(isDoublePortion)
            {}

    double GetCost() const override
    {
        return m_isDoublePortion ? 130 : 90;
    }
private:
    bool m_isDoublePortion;
};


enum class TeaType {
    Black,
    White,
    Blue,
    Cyan
};

std::string ToString(TeaType size)
{
    switch (size)
    {
        case TeaType::Black: return "Black";
        case TeaType::White: return "White";
        case TeaType::Blue:  return "Blue";
        case TeaType::Cyan:  return "Cyan";
        default:             return "Unknown Tea Type";
    }
}

// Чай
class CTea : public CBeverage
{
public:
    explicit CTea(TeaType type = TeaType::Black)
            : CBeverage(ToString(type) + " Tea"), m_type(type)
            {}

    double GetCost() const override
    {
        return 30;
    }
private:
    TeaType m_type;
};

enum class MilkshakeSize {
    Small,
    Medium,
    Large
};

std::string ToString(MilkshakeSize size)
{
    switch (size)
    {
        case MilkshakeSize::Small:  return "Small";
        case MilkshakeSize::Medium: return "Medium";
        case MilkshakeSize::Large:  return "Large";
        default:                    return "Unknown Size";
    }
}

// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
    explicit CMilkshake(MilkshakeSize size = MilkshakeSize::Small)
            : CBeverage(ToString(size) + " Milkshake"), m_size(size)
            {}

    double GetCost() const override
    {
        switch (m_size)
        {
            case MilkshakeSize::Small:  return 50;
            case MilkshakeSize::Medium: return 60;
            case MilkshakeSize::Large:  return 80;
        }
    }
private:
    MilkshakeSize m_size;
};

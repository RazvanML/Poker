#include "stdafx.h"
#include "Card.h"


namespace pk {
    CardUtils::CardUtils() {
        // card value
        for (int i = 2; i != 11; i++)
            card2value[std::to_string(i)] = i - 2;
        card2value["J"] = Card::valueofJ;
        card2value["Q"] = Card::valueofQ;
        card2value["K"] = Card::valueofK;
        card2value["A"] = Card::valueofA;

        for (auto i : card2value)
            value2card[i.second] = i.first;

        // card color - ascii codes.
        card2color["\3"] = 0;
        card2color["\4"] = 1;
        card2color["\5"] = 2;
        card2color["\6"] = 3;

        for (auto i : card2color)
            color2card[i.second] = i.first;
        // additional color aliases - hard to type the ascii chars of symbols
        card2color["H"] = 0;
        card2color["D"] = 1;
        card2color["T"] = 1;
        card2color["C"] = 2;
        card2color["S"] = 3;

    };


    const CardUtils Card::utils;


    std::ostream& operator<<(std::ostream& os, const Card& c)   {
        os << c.toDisplay() ;
        return os;
    }



};
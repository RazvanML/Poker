#pragma once

namespace pk {

    class CardUtils {
    private:
        std::map<std::string, int> card2value;
        std::map<int, std::string> value2card;

        std::map<std::string, int> card2color;
        std::map<int, std::string> color2card;
        CardUtils();
        friend class Card;
    };

    class Card
    {
    public:
        const static char HeartASCII = 3;
        const static char DiamondASCII = 4;
        const static char ClubsASCII = 5;
        const static char SpadeASCII = 6;

        const static int valueofJ = 9;
        const static int valueofQ = 10;
        const static int valueofK = 11;
        const static int valueofA = 12;
    private:
        const static CardUtils utils;

        /*
         0-51 value
         ordinal / 4 + 2 is the number of the card 11=j, 12=q, 13=k 14=a
         ordinal % 4 is the color 0=heart 1=diamond/tiles 2=clubs 3=spade
         // see https://en.wikipedia.org/wiki/Suit_(cards)
        */
        int ordinal;
        
    public:
        Card(int ordinal) {
            this->ordinal = ordinal;
        }

        // default constructor - an invalid card
        Card() {
            ordinal = -99;
        }

        // convert from text form to internal
        // this is mainly for testing, user does not input cards
        Card(std::string s) {
            if (s.length() < 2)
                throw std::exception("Card string too short");
            std::string num = s.substr(0, s.length() - 1);
            std::string col = s.substr(s.length() - 1, 1);
            auto colval = utils.card2color.find(col);
            if (colval == utils.card2color.end())
                throw std::exception("Invalid color");
            auto numval = utils.card2value.find(num);
            if (numval == utils.card2value.end())
                throw std::exception("Invalid card number");
            ordinal = numval->second * 4 + colval->second;
        }

        std::string toDisplay() const  {
            return utils.value2card.find(getNumber())->second 
                + utils.color2card.find(getColor())->second;
        }

        int getNumber() const {
            return ordinal / 4;
        }
        int getColor() const {
            return ordinal % 4;
        }

        int getOrdinal() const {
            return ordinal;
        }
// comparison
// equal / non equal test for identity
//  > and co. tests for "better card"
        bool operator==(const Card c) const {
            return ordinal == c.ordinal;
        }
        bool operator!=(const Card c) const {
            return ordinal != c.ordinal;
        }
        bool operator>(const Card c) const {
            return getNumber() > c.getNumber();
        }
        bool operator>=(const Card c) const {
            return getNumber() >= c.getNumber();
        }
        bool operator<(const Card c) const {
            return getNumber() < c.getNumber();
        }
        bool operator<=(const Card c) const {
            return getNumber() <= c.getNumber();
        }

    };

    std::ostream& operator<<(std::ostream& os, const Card& dt);
 

}
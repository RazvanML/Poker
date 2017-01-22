#pragma once
namespace pk {

    // immutable class consisting of a player decision.
    // fold / call / raise, allIn 
    class PlayerDecision
    {
    public:
        typedef enum {
            fold = 0,
            call,
            raise,
            allIn
        }
        DecisionType;
    private:
        DecisionType decision;
        int amount;
    public:
        PlayerDecision(DecisionType decision) {
            this->decision = decision;
            if (decision == raise)
                throw new std::exception("Missing raise amount!");
        }
        PlayerDecision(DecisionType decision, int amount) {
            this->decision = decision;
            this->amount = amount;
            if (decision == fold && amount != 0)
                throw new std::exception("Cannot fold with amount!");
            if (decision == call && amount == 0)
                throw new std::exception("Call has to be with zero amount!");
        };

        DecisionType getDecision() const {
            return decision;
        }

        int getAmount() const {
            return amount;
        }
    };

}
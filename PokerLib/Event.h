#pragma once

namespace pk {
    class Event;
    class Player;
    class Card;

    template<class T1,class T2> struct SimplePair {
        SimplePair(T1 t1, T2 t2) {
            first = t1;
            second = t2;
        }
        T1 first;
        T2 second;
    };

    class EventListener {
    protected:
        virtual void receiveEvent(const Event &e) const = 0;
        friend class EventCollector;
    };


    class EventCollector {
    private:
        typedef std::pair<EventListener*, const Player*> listnerPlayer;
        std::list< listnerPlayer > listeners;
    public:
        void registerListener(EventListener* listener, const Player *p) {
            listeners.push_back(listnerPlayer(listener, p));
        }
        void ingestEvent(const Event& e);
        virtual ~EventCollector() {
            for (listnerPlayer lp : listeners)
                delete lp.first;
        }
    };

    class Event
    {
    public:
        enum Activity {
            join,
            getCards,
            communityCards,
            check,
            fold,
            bet,
            showCards,
            win
        };
        // a can be join, fold
        Event(Activity a, const Player* p) {
            initpub(a, p);
            if (a != join && a != fold && a!=check)
                throw new std::exception("Expecting join/fold/check");
        }
        // bet, win
        Event(Activity a, const Player* p, int amtchips) {
            initpub(a, p);
            amt = amtchips;
            if (a != bet && a != win)
                throw new std::exception("Expecting bet win");
        }
        Event(Activity a, const Player* p, const std::vector<Card>& c) {
            initpub(a, p);
            cards.insert(cards.end(), c.begin(), c.end());

            if (p != NULL) {
                priv = (a != getCards);
                if (a != showCards && a != getCards)
                    throw new std::exception("Expecting show cards/get cards");
            }
            else {
                priv = false;
                if (a != communityCards)
                    throw new std::exception("community cards if no player mentioned");
            }
        }

        friend std::ostream& operator<<(std::ostream &os, const Event& evt);

        const Player* getPlayer() const {
            return who;
        }

        const Activity getActivity() const {
            return what;
        }

    private:
        void initpub(Activity a, const Player* p) {
            what = a;
            who = p;
            priv = false;
        }

        // what happened
        Activity what;
        bool priv;
        const Player* who;
        std::vector<pk::Card> cards;
        int amt;
    };

    // every class generating events should inherit this
    class EventGenerator {
    private:
        EventCollector* theEventCollector = NULL;
    public:
        void setEventCollector(EventCollector &evt) {
            theEventCollector = &evt;
        }

        void generateEvent(const Event &e) {
            if (theEventCollector == NULL)
                return;
            // this is only to identify who doesn't have an event collector.
            if (theEventCollector == NULL)
                throw std::exception("Failed to tell who is the event collector");
            theEventCollector->ingestEvent(e);
        }
    };

}